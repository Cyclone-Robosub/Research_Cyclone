#!/bin/bash
cd src
source /opt/ros/jazzy/setup.bash
source install/setup.bash
export ROS_DOMAIN_ID=4
chmod +x startupcpp.sh
python3 main.py
