#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "Depth.cpp"
#include "Temperature.cpp"
#include "phSensor.cpp"
void setup() {
    Serial.begin(9600);
    Serial.println("Arduino is ready");
}
void loop() {
    if (Serial.available() > 0) {
        String data = Serial.readStringUntil('\n');
        if(data == "start"){
          startSensors();
          while(data != "stop"){
          	startReading();
          }
        }
}
void startReading() {
    if (Serial.available() > 0) {
        String wantedData = Serial.readStringUntil('\n');
        depthReader.read();
        tempReader.read();
        if(wantedData == "all"){
          Serial.println(depthReader.depth()+","+tempReader.temperature()+","+depthReader.altitude());
        }
        if(wantedData == "depth"){
          Serial.println(depthReader.depth());
        }
        if(wantedData == "temp"){
          Serial.println(tempReader.temperature());
        }
        if(wantedData == "ph"){

        }
        if(wantedData == "altitude"){
          Serial.println(depthReader.altitude());
        }

    }

    // Close the file when done
    outFile.close();
}

void startSensors() {
  	MS5837 depthReader = new MS5837();
    TSYS01 tempReader = new TSYS01();
    depthReader.setModel(MS5837::MS5837_30BA);
    depthReader.setFluidDensity(997);
    tempReader.init();


    return 0;
}
