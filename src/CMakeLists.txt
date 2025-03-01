cmake_minimum_required(VERSION 3.5)
project(research_cyclone)
set(CMAKE_CXX_STANDARD 20)
add_executable(researchcnode
        GetSensorData.cpp
        main.cpp
        Setup.hpp
        Setup.cpp
)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)


ament_target_dependencies(researchcnode rclcpp std_msgs)
install(TARGETS
researchcnode
  DESTINATION lib/${PROJECT_NAME})


ament_package()



