#!/usr/bin/env python
#Created by Tanishq Dwivedi
import time
import subprocess
import threading
import serial
from pathlib import Path
import datetime
from datetime import datetime
import rclpy
from rclpy.node import Node
import os
from std_msgs.msg import String
import io
import string
#Class to start the Publisher node -> 'research_node' The queue is 10
class MinimalPublisher(Node):

    def __init__(self):
        super().__init__('research_python_node')
        self.rpublisher_ = self.create_publisher(String, 'researchSensorsData', 10)
        self.depthpublisher = self.create_publisher(String, 'depthSensorData', 10)
        
    #publishing the actual line of the sensor data from Arduino with ROS
    def publish_line(self, line):
        msg = String()
        msg.data = line
        # need to parse it into a variable -> depth
        self.rpublisher_.publish(msg)
        self.depthpublisher.publish(msg)
        print(line)
#This compile command will build, use the ROS source library files, and then
#execute. Make sure that this bash script is inside a thread, because the bash
#script command will not continue while the c++ files are running/bash script
#ends.
def compileFunction():
    # Build the package
    subprocess.run(["./startupcpp.sh"], shell=True, executable='/bin/bash')
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=0.2)
#start the c++ files before we start the ROS node and publish the data
def main():
        t1 = threading.Thread(target = compileFunction)
        t1.start()
        ser.reset_input_buffer()
        line = ser.readline().decode('utf-8').rstrip()
        while line != "All sensors are ready.":
            print("Failure of sensors")
            time.sleep(3)
            line = ser.readline().decode('utf-8').strip()
            print(line)
        stringcmdlol = "start."
        ser.write(stringcmdlol.encode())
        rclpy.init()
        minimal_publisher = MinimalPublisher()
        rclpy.spin(getData(minimal_publisher))

#The ROS Node will keep running and this function will read off the 
#Arduino data
def getData(minimal_publisher):
    print("started reading data")
    while True:
            line = ser.readline().decode('utf-8').rstrip()
            minimal_publisher.publish_line(line)
            #minimal_publisher.publish_line(f"YAY Time : {time.time()}")                


main()
