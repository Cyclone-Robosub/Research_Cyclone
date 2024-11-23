#include <iostream>
#include <chrono>
#include <thread>
#include "Libraries/MS5837.h"  

class DepthSens {
private:
    ms5837 sensor;  

public:
    // Constructor with an optional FluidDensity parameter
    DepthSens(float FluidDensity = 1000.0) {
        // Initialize the sensor
        if (!sensor.init()) {
            std::cerr << "Failed to initialize depth sensor" << std::endl;
        }
        // Set the fluid density
        sensor.setFluidDensity(FluidDensity);
    }

    // Method to get the depth
    double getDepth() {
        if (sensor.read(ms5837::OSR_2048)) {  // Assuming `OSR_2048` is a constant in the C++ library
            return sensor.depth();
        } else {
            std::cerr << "Failed depth reading" << std::endl;
            return 0.0;  // Return a default or error value
        }
    }

    // Method to get the altitude
    double getAlt() {
        if (sensor.read()) {
            return sensor.altitude();
        } else {
            std::cerr << "Failed altitude reading" << std::endl;
            return 0.0;  // Return a default or error value
        }
    }

    // Method to update the sensor (force a read)
    void updateSensor() {
        sensor.read(ms5837::OSR_2048);
    }

    // Method to get the pressure
    std::pair<double, double> getPressure() {
        if (sensor.read(ms5837::OSR_2048)) {
            double pressure = sensor.pressure();
            double pressureAtm = sensor.pressure(ms5837::UNITS_atm);  // Assuming UNITS_atm is a constant in the C++ library
            return std::make_pair(pressure, pressureAtm);
        } else {
            std::cerr << "Failed pressure reading" << std::endl;
            return std::make_pair(0.0, 0.0);  // Return default values
        }
    }
};
