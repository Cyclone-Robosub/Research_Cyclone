#!/usr/bin/env python
import time
import serial
import subprocess
from pathlib import Path
from datetime import datetime
import io
import string



def main():
    start_input = input("Type y to start sensors.")
    if start_input.upper() == "Y":
        if __name__ == '__main__':
            ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1) #Need to change the file path as needed.
            ser.reset_input_buffer()
            subprocess.run(["g++ ","main.cpp " "StartandSend.cpp ",
                            "GetSensorData.cpp ","-o ","runprogram"])
            run_path = subprocess.run(["./runprogram"], capture_output=True, text=True)
            if run_path.stdout == "failed":
                print("Failed to start recording")
            else:
                ser.write("start.\n")
                line = ser.readLine().decode('utf-8').rstrip()
                print(line)








main()