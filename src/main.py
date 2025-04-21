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
from rclpy.executors import MultiThreadedExecutor
import os
from std_msgs.msg import String
import io
import string


ser = serial.Serial('/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_3433132363535180A231-if00', 9600, timeout=0.9)

#Class to start the Publisher node -> 'research_node' The queue is 10
class MinimalPublisher(Node):

    def __init__(self, node_name):
        super().__init__(node_name)
        self.rpublisher_ = self.create_publisher(String, 'researchSensorsData', 10)
        self.depthpublisher = self.create_publisher(String, 'depthPressureSensorData', self.getData, 10)
    def getData(self):
        while True:
            line = ser.readline().decode('utf-8').rstrip()
            self.publish_line(line)
        
    #publishing the actual line of the sensor data from Arduino with ROS
    def publish_line(self, line):
        msg = String()
        msg.data = line
        # need to parse it into a variable -> depth
        self.rpublisher_.publish(msg)
        data_string = line
        string1, string2, string3, = data_string.split(",")
        self.depthpublisher.publish(f"{string2},{string3},")
class ManiCommandSubscriber(Node):
    def __init__(self, node_name):
        super().__init__(node_name)
        self.subscription = self.create_subscription(
            String,
            'maniControlData',
            self.command_Callback,
            10
        )
    def command_Callback(self, msg):
        ser.write(msg.data.encode())
        


#This compile command will build, use the ROS source library files, and then
#execute. Make sure that this bash script is inside a thread, because the bash
#script command will not continue while the c++ files are running/bash script
#ends.
def compileFunction():
    # Build the package
    subprocess.run(["./startupcpp.sh"], shell=True, executable='/bin/bash')

#start the c++ files before we start the ROS node and publish the data
def main():
        t1 = threading.Thread(target = compileFunction)
        t1.start()
        i = 0
        while True:
            try:
                ser.reset_input_buffer()    
                line = ser.readline().decode('utf-8').rstrip()
                break
            except:
             ser = serial.Serial('/dev/ttyACM' + str(i), 9600, timeout=0.2)
             i += 1
             if i > 11:
                 print("Complete Failure for port connecting to arduino.")
                 return
        reading_string = line
        string1 = reading_string.split(",")
        while line != "All sensors are ready." or string1 != "Reading":
            print("Not Connecting\n")
            time.sleep(3)
            print(line)
            line = ser.readline().decode('utf-8').strip()
            reading_string = line
            string1 = reading_string.split(",")
        stringcmdlol = "start."
        ser.write(stringcmdlol.encode())
        rclpy.init()
        node1 = MinimalPublisher('research_python_node')
        node2 = ManiCommandSubscriber('maniControl')
        executor = MultiThreadedExecutor()
        executor.add_node(node1)
        executor.add_node(node2)
        executor.spin()
#The ROS Node will keep running and this function will read off the 
#Arduino data
            #minimal_publisher.publish_line(f"YAY Time : {time.time()}")                

main()
