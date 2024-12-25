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
    start_input = input("Type y to start sensors.")
    if start_input.upper() == "Y":
        if __name__ == '__main__':
            ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1) #Need to change the file path as needed.
            ser.reset_input_buffer()
            subprocess.run(["g++","main.cpp" "StartandSend.cpp", "GetSensorData.cpp","-o","runprogram"])
            run_path = subprocess.run(["./runprogram"], capture_output=True, text=True)
            if run_path.stdout == "failed":
                print("Failed to start recording")
            else:
                commandControl = input()
                if commandControl == "All sensors are ready.":
                    print("Started sensors")
                    ser.write("start.")
                else:
                    print("failed to start sensors")
                







main()