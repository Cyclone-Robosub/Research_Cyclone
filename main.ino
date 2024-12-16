#include <TSYS01.h>
#include <Wire.h>
#include <floatToString.h>
#include <MS5837.h>
MS5837 depthReader = MS5837();
    TSYS01  tempReader = TSYS01();
void startReading() {
        String wantedData = "all";
        if(depthReader.init())
        {
      depthReader.read();
        }
    if(tempReader.init()){
      tempReader.read();
    }
    String tempStringC;
        if(tempReader.temperature() == 0)
        {
           tempStringC = String(depthReader.temperature(),5); 
        }else {
          tempStringC = String(tempReader.temperature(),5);
        }
        String depthString = String(depthReader.depth(),5);
        String altString = String(depthReader.altitude(),5);
        String pressureString = String(depthReader.pressure(), 5);
       
        if(wantedData == "all"){
         Serial.println(depthString+","+altString +","+ tempStringC +"," +pressureString);
        }
        if(wantedData == "depth"){
          Serial.println(depthReader.depth());
        }
        if(wantedData == "temp"){
        //  Serial.println(tempReader.temperature());
        }
        if(wantedData == "ph"){

        }
        if(wantedData == "altitude"){
          Serial.println(depthReader.altitude());
        }

    
}

void startSensors() {
    depthReader.setModel(MS5837::MS5837_30BA);
    depthReader.setFluidDensity(997);
    tempReader.init();
    
    return 0;
}
void setup() {
    Serial.begin(9600);
    Wire.begin();
    startSensors();
   // loop();
}
void loop() {
    String givenString = Serial.readStringUntil('.');
    if(givenString == "start"){
    while(givenString != "stop"){
      givenString = Serial.readStringUntil('.');
      //delay(100);
      startReading();
      givenString = Serial.readStringUntil('.');
    }
    }
}


