#!/bin/bash 
#Created by Tanishq Dwivedi

# Change directory to src
# Build the package
source /opt/ros/jazzy/setup.bash
colcon build

# Source ROS setup files
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 run research_cyclone researchcnode

