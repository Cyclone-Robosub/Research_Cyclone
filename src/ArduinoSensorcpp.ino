//Arduino Code
#include <TSYS01.h>
#include <Wire.h>
#include <floatToString.h>
#include <MS5837.h>

#ifdef USE_PULSE_OUT
  #include "ph_iso_surveyor.h"       
  Surveyor_pH_Isolated pH = Surveyor_pH_Isolated(A0);         
#else
  #include "ph_surveyor.h"             
  Surveyor_pH pH = Surveyor_pH(A0);   
#endif
MS5837 depthReader = MS5837();
TSYS01 tempReader = TSYS01();
#define NULL_SENSOR_VALUE -320000
#define FAULTY_SENSOR_VALUE -40404
void ReadAllSensors()
{
  if (depthReader.init())
  {
    depthReader.read();
  }
  if (tempReader.init())
  {
    tempReader.read();
  }
  String tempStringC;
  if (tempReader.temperature() < -1000)
  {
    // Serial.println("No Got it");
    tempStringC = FAULTY_SENSOR_VALUE;
  }
  else
  {
    // Serial.println("Got it");
    tempStringC = String(tempReader.temperature(), 5);
  }
  String depthString = String(depthReader.depth(), 5);
  String altString = String(depthReader.altitude(), 5);
  String pressureString = String(depthReader.pressure(), 5);

  Serial.println("Reading: " + depthString + "," + pressureString + "," altString +
                 "," + tempStringC + "," + pH.read_ph());
}

void startupSensors()
{
  depthReader.setModel(MS5837::MS5837_30BA);
  depthReader.setFluidDensity(997);
  if (depthReader.init())
  {
    Serial.println("All sensors are ready.");
  }
  else
  {
    Serial.println("failed from arduino side.");
    while(!depthReader.init()){
      Serial.println("failed from arduino side.");
      delay(1000);
    }
    Serial.println("All sensors are ready.");
  }
  if (pH.begin()) {                                     
    Serial.println("Loaded EEPROM");
  }

  return 0;
}
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  startupSensors();
  String givenString = Serial.readString();
  Serial.println(givenString);
  while (givenString != "start") {
    Serial.println("All sensors are ready.");
    givenString = Serial.readStringUntil('.');
    delay(10);
    // debugging: Serial.println(givenString);
  }
  // loop();
}
void loop()
{
  ReadAllSensors();
  
}
