cmake_minimum_required(VERSION 3.8)
project(floatbot_mpc)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(pluginlib REQUIRED)
find_package(acados_solver_base REQUIRED)
find_package(acados_solver_plugins REQUIRED)

# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

# configure executables
add_executable(floatbot_mpc_controller src/floatbot_mpc_controller.cpp)
ament_target_dependencies(floatbot_mpc_controller
  rclcpp
  pluginlib
  acados_solver_base
  acados_solver_plugins
)

# configure cmake targets
install(TARGETS floatbot_mpc_controller
  DESTINATION lib/${PROJECT_NAME}
)

# configure python targets
ament_python_install_package(${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()