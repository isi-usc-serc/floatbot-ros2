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

class FloatbotMpcController : public rclcpp::Node {
    // Reads floatbot odometry data and uses an acados solver plugin to perform MPC control

    public:
    FloatbotMpcController() : Node("floatbot_mpc_controller") {

        // odometry subscriber setup
        const auto odom_topic = declare_parameter<std::string>(
            "odom_topic",
            "/odom/filtered"
        );

        odom_sub_ = this->create_subscription<nav_msgs::msg::Odometry>(
            odom_topic,
            rclcpp::SensorDataQoS(),
            [this](nav_msgs::msg::Odometry::ConstSharedPtr msg) { odom_callback(msg); }
        );

        // MPC controller setup
        const auto plugin_name = declare_parameter<std::string>(
            "solver_plugin",
            "acados_solver_plugins/FloatbotDirectAllocationMpc"
        );

        // Set MPC parameters
        const int horizon_steps = declare_parameter<int>("horizon_steps", 20);
        const double sampling_time = declare_parameter<double>("sampling_time", 0.01);

        if (horizon_steps <= 0) {
            throw std::invalid_argument("horizon_steps must be positive");
        }

        if (!std::isfinite(sampling_time) || sampling_time <= 0.0) {
            throw std::invalid_argument("sampling_time must be finite and positive");
        }

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

    void controller_callback() {
        // wait for odometry to arrive before starting controller
        if (!last_odom_) {
            RCLCPP_WARN_THROTTLE(
                get_logger(),
                *get_clock(),
                1000,
                "Waiting for odometry..."
            );
            return;
        }

        // get latest odom message
        const auto & pose = last_odom_->pose.pose;
        const auto & twist = last_odom_->twist.twist;

        // map odom message to expected state format
        std::vector<double> current_state = {
            pose.position.x, 
            pose.position.y, 
            pose.orientation.w, 
            pose.orientation.z,
            twist.linear.x,
            twist.linear.y,
            twist.angular.z
        };
        
        // Update the measured initial state before each solve:
        solver_->set_initial_state_values(current_state);
        
        // TO DO: Update commanded states

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

        // First control input in the optimized sequence.
        optimal_u_ = solver_->get_control_values(0);

        // TODO: Publish optimal_u_
    }

    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;
    nav_msgs::msg::Odometry::ConstSharedPtr last_odom_;
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