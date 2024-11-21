#!/usr/bin/env python
import time
from pathlib import Path
from datetime import datetime
from Depth import DepthSens
import io
import string
from Temperature import TempSensor
import pHSensor

def start():
    date = datetime.today().strftime('%Y-%m-%d')
    data_file = date + ".txt"
    with open(data_file, 'w'):
        print(data_file)
        #Note: Every time a forced reading happens of a sensor is when it should write to a file with its time, not here. A scheduled reading should be on the same line when written (So the code should be here)
    depthSensor = DepthSens()
    temperatureSensor = TempSensor()
    while(True):
        depth = depthSensor.getDepth()
        temperatureF = temperatureSensor.getTempF()
        altitude = depthSensor.getAlt()


def main():
    startInput = input("Type y to start recording")
    if(startInput.upper() == "Y"):
        start()



main()