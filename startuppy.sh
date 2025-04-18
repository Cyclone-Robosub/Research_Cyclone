#Created By Tanishq Dwivedi
source /opt/ros/jazzy/setup.bash
source install/setup.bash
cd src
source /opt/ros/jazzy/setup.bash
source install/setup.bash

chmod +x startupcpp.sh

python3 main.py &
