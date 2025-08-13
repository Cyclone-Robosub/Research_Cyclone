#!/bin/bash 
#Created by Tanishq Dwivedi

# Change directory to src
# Build the package
export ROS_DOMAIN_ID=4
source /opt/ros/jazzy/setup.bash
colcon build

# Source ROS setup files
source /opt/ros/jazzy/setup.bash
source install/setup.bash
export ROS_DOMAIN_ID=4
ros2 run research_cyclone researchcnode

