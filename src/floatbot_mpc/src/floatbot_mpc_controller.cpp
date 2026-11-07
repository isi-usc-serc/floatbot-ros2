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

class FloatbotMpcController : public rclcpp::Node {

    public:
    FloatbotMpcController() : Node("floatbot_mpc_controller") {

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
            sampling_time);

        // Start the timer only after successful solver initialization.
        timer_ = create_wall_timer(
            timer_period,
            [this]() { controller_callback(); });
    }

private:
    void controller_callback()
    {
        // TODO: Update the measured initial state before each solve:
        // solver_->set_initial_state_values(current_state);
        //
        // Also update commanded states

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