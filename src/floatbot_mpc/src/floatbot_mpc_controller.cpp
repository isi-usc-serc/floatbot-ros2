#include <chrono>
#include <cmath>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "pluginlib/class_loader.hpp"
#include "acados_solver_base/acados_solver.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "floatbot_interfaces/msg/thruster_command.hpp"

class FloatbotMpcController : public rclcpp::Node {
    // Reads floatbot odometry data and uses an acados solver plugin to perform MPC control

    public:
    FloatbotMpcController() : Node("floatbot_mpc_controller") {

        // odometry subscriber setup
        const auto odom_topic = declare_parameter<std::string>(
            "odom_topic",
            "odom/filtered"
        );

        odom_sub_ = this->create_subscription<nav_msgs::msg::Odometry>(
            odom_topic,
            rclcpp::SensorDataQoS(),
            [this](nav_msgs::msg::Odometry::ConstSharedPtr msg) { odom_callback(msg); }
        );

        // pose command subscriber setup
        const auto pose_cmd_topic = declare_parameter<std::string>(
            "pose_cmd_topic",
            "pose_cmd"
        );

        pose_cmd_sub_ = this->create_subscription<geometry_msgs::msg::PoseStamped>(
            pose_cmd_topic,
            rclcpp::QoS(1).reliable(),
            [this](geometry_msgs::msg::PoseStamped::ConstSharedPtr msg) { pose_cmd_callback(msg); }
        );

        // thruster command publisher setup
        const auto ctrl_topic = declare_parameter<std::string>(
            "ctrl_topic",
            "thrust_cmd"
        );

        thruster_ctrl_pub_ = this->create_publisher<floatbot_interfaces::msg::ThrusterCommand>(
            ctrl_topic,
            rclcpp::QoS(1).reliable()
        );

        // MPC plugin setup
        const auto plugin_name = declare_parameter<std::string>(
            "solver_plugin",
            "acados_solver_plugins/FloatbotDirectAllocationMpc"
        );

        // Set MPC parameters
        const int horizon_steps = declare_parameter<int>("horizon_steps", 20);
        const double sampling_time = declare_parameter<double>("sampling_time", 0.05);

        if (horizon_steps <= 0) {
            throw std::invalid_argument("horizon_steps must be positive");
        }

        if (!std::isfinite(sampling_time) || sampling_time <= 0.0) {
            throw std::invalid_argument("sampling_time must be finite and positive");
        }

        // run the controller at the same period as the MPC sampling time
        const auto timer_period = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(sampling_time));

        if (timer_period.count() <= 0) {
            throw std::invalid_argument("sampling_time is too small for the timer");
        }

        loader_ = std::make_unique<pluginlib::ClassLoader<acados::AcadosSolver>>(
            "acados_solver_base",
            "acados::AcadosSolver"
        );

        solver_ = loader_->createSharedInstance(plugin_name);

        const int status = solver_->init(static_cast<unsigned int>(horizon_steps), sampling_time);

        if (status != 0) {
            throw std::runtime_error(
                "Acados initialization failed with status " +
                std::to_string(status)
            );
        }

        RCLCPP_INFO(
            get_logger(),
            "Loaded solver '%s': N=%d, Ts=%.6f s",
            plugin_name.c_str(),
            horizon_steps,
            sampling_time
        );

        // Start the timer only after successful solver initialization.
        timer_ = create_wall_timer(
            timer_period,
            [this]() { controller_callback(); }
        );
    }

private:
    void odom_callback(nav_msgs::msg::Odometry::ConstSharedPtr msg) {
        last_odom_ = msg;
    }

    void pose_cmd_callback(geometry_msgs::msg::PoseStamped::ConstSharedPtr msg) {
        last_pose_cmd_ = msg;
    }

    void controller_callback() {
        // wait for odometry and commanded pose to arrive before starting controller
        if (!last_odom_ || !last_pose_cmd_) {
            RCLCPP_WARN_THROTTLE(
                get_logger(),
                *get_clock(),
                1000,
                "Waiting for odometry and a pose command..."
            );
            return;
        }

        // get latest odom and pose command messages
        const auto & pose_meas = last_odom_->pose.pose;
        const auto & twist_meas = last_odom_->twist.twist;
        const auto & pose_cmd = last_pose_cmd_->pose;

        // map odom message to expected state format {rx, ry, qw, qz, vx, vy, wz}
        std::vector<double> current_state = {
            pose_meas.position.x, 
            pose_meas.position.y, 
            pose_meas.orientation.w, 
            pose_meas.orientation.z,
            twist_meas.linear.x,
            twist_meas.linear.y,
            twist_meas.angular.z
        };

        // map pose command message to expected parameter format {rx, ry, qw, qz, vx, vy, wz}
        std::vector<double> current_params = {
            pose_cmd.position.x,
            pose_cmd.position.y,
            pose_cmd.orientation.w,
            pose_cmd.orientation.z,
            0,
            0,
            0
        };
        
        // Update the measured initial state and commanded state before each solve:
        solver_->set_initial_state_values(current_state);
        solver_->set_runtime_parameters(current_params);

        
        // TO DO: publish commanded states

        const int status = solver_->solve();

        if (status != 0) {
            RCLCPP_WARN_THROTTLE(
                get_logger(),
                *get_clock(),
                1000,
                "Acados solve failed with status %d",
                status
            );
            return;
        }

        // get first control input in the optimized sequence.
        optimal_u_ = solver_->get_control_values(0);

        // create blank thruster command message
        floatbot_interfaces::msg::ThrusterCommand msg;
        msg.header.stamp = now();

        // check if output of MPC is the right size for the thruster message
        if (optimal_u_.size() != msg.thrusts.size()) {
            RCLCPP_ERROR(
                get_logger(),
                "Expected %zu thruster commands, received %zu",
                msg.thrusts.size(),
                optimal_u_.size()
            );
            return;
        }

        // assemble thruster command message
        for (std::size_t i = 0; i < msg.thrusts.size(); ++i) {
            msg.thrusts[i] = static_cast<float>(optimal_u_[i]);
        }

        // publish control message
        thruster_ctrl_pub_->publish(msg);
    }

    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;
    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr pose_cmd_sub_;
    rclcpp::Publisher<floatbot_interfaces::msg::ThrusterCommand>::SharedPtr thruster_ctrl_pub_;
    nav_msgs::msg::Odometry::ConstSharedPtr last_odom_;
    geometry_msgs::msg::PoseStamped::ConstSharedPtr last_pose_cmd_;
    std::unique_ptr<pluginlib::ClassLoader<acados::AcadosSolver>> loader_;
    std::shared_ptr<acados::AcadosSolver> solver_;
    std::vector<double> optimal_u_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv) {
    rclcpp::init(argc, argv);

    int exit_code = 0;

    try {
        rclcpp::spin(std::make_shared<FloatbotMpcController>());
    } catch (const std::exception & ex) {
        RCLCPP_FATAL(
            rclcpp::get_logger("floatbot_mpc_controller"),
            "Floatbot MPC Controller failed: %s",
            ex.what());
        exit_code = 1;
    }

    rclcpp::shutdown();
    return exit_code;
}