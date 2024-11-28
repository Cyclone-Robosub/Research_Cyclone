#!/usr/bin/env python
import time
import serial
import subprocess
from pathlib import Path
from datetime import datetime
from Depth import DepthSens
import io
import string
from Temperature import TempSensor

    

def main():
    start_input = input("Type y to start recording")
    if start_input.upper() == "Y":
        if __name__ == '__main__':
            ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1) #Need to change the file path as needed.
            ser.reset_input_buffer()
            ser.write("start\n")
            subprocess.run(["g++", "StartandSend.cpp", "GetSensorData.cpp","-o","runprogram"])
            run_path = subprocess.run(["./runprogram"], capture_output=True, text=True)
            if run_path.stdout == "failed":
                print("Failed to start recording")
            else:
                commandControl = "start"
                while commandControl != "stop":
                    ser.write(run_path.stdout)
                    commandControl = input()







main()