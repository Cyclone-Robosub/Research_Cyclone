cmake_minimum_required(VERSION 3.5)
project(research_cyclone)
set(CMAKE_CXX_STANDARD 20)
add_executable(research_node
        GetSensorData.cpp
        main.cpp
        Setup.hpp
        Setup.cpp
)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)


ament_target_dependencies(research_node rclcpp std_msgs)
install(TARGETS
  research_node
  DESTINATION lib/${PROJECT_NAME})


ament_package()



