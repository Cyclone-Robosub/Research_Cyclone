#!/usr/bin/python
#Created by Tanishq Dwivedi
import ms5837 as depthSensor
import datetime

# we need to make a sesnor object with the specific model 
sensor = depthSensor.MS5837()

if(sensor.init()):
    print("failed depth sesnor initlized")
class DepthSens:
    def __init_(self, FluidDensity=1000):
        sensor.setFluidDensity(FluidDensity)
    def getDepth(self):
        if(sensor.read(depthSensor.OSR_2048)):
            depth = sensor.depth()
        else:
            depth = 10000000
            print("failed depth reading")
        return depth
    def getAlt(self):
        if(sensor.read()):
            altitude = sensor.altitude()
        else:
            altitude = 1000000
            print("failed depth reading")
        return altitude
    def updateSensor(self):
        sensor.read(depthSensor.OSR_2048)
    def getPressure(self):
        if(sensor.read(depthSensor.OSR_2048)):
            return sensor.pressure(), sensor.pressure(depthSensor.UNITS_atm)
        else:
            return 86868686, 86868686
    
    
