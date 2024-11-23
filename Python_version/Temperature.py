import Libraries.tsys01 as tsys01
sensor = tsys01.TSYS01() # Use default I2C bus 1
sensor = tsys01.TSYS01(0) # Specify I2C bus 0
class TempSensor(object):
    def __init_(self):
        self.tempeartureF
        self.temperatureSuccess
    def getTempF(self):
        if(sensor.init()):
            temperatureSuccess = sensor.read()
            if(temperatureSuccess):
                tempeartureF = sensor.temperature(tsys01.UNITS_Farenheit)
                return tempeartureF
            else:
                print("Error Temperature")
                return 5000
        else:
            print("Failed Failed")
