#!/usr/bin/env python
import time
import serial
import subprocess
from pathlib import Path
from datetime import datetime
import io
import string



def main():
    if __name__ == '__main__':
        ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
        ser.reset_input_buffer()
        ser.write(b"start.\n")
        subprocess.run(["g++ ","main.cpp " "StartandSend.cpp ",
                        "GetSensorData.cpp ","-o ","runprogram"])
        run_path = subprocess.run(["./runprogram"], capture_output=True, text=True)
                








main()