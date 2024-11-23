#include <iostream>
#include "Libraries/TSYS01.h" 

class TempSensor {
private:
    tsys01 sensor;  
    double temperatureF;
    bool temperatureSuccess;

public:
    TempSensor(int bus = 1) {
        if (!sensor.init(bus)) {
            std::cerr << "Failed to initialize temperature sensor on I2C bus " << bus << std::endl;
        }
    }

    double getTempF() {
        if (sensor.init()) {  
            temperatureSuccess = sensor.read();  
            if (temperatureSuccess) {
                temperatureF = sensor.temperature(tsys01::UNITS_Farenheit);  // 
                return temperatureF;
            } else {
                std::cerr << "Error reading temperature" << std::endl;
                return -404; 
            }
        } else {
            std::cerr << "Sensor initialization failed" << std::endl;
            return -400;
        }
    }
};
