#include <base_surveyor.h>
#include <do_iso_surveyor.h>
#include <do_surveyor.h>
#include <orp_iso_surveyor.h>
#include <orp_surveyor.h>
#include <ph_iso_surveyor.h>
#include <ph_surveyor.h>
#include <rtd_surveyor.h>


//Arduino Code
#include <TSYS01.h>
#include <Wire.h>
#include <floatToString.h>
#include <MS5837.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADS1X15.h> //include for the ADS1115 ADC



#ifdef USE_PULSE_OUT
  #include "ph_iso_surveyor.h"       
  Surveyor_pH_Isolated pH = Surveyor_pH_Isolated(A0);         
#else
  #include "ph_surveyor.h"             
  Surveyor_pH pH = Surveyor_pH(A0);   
#endif


//I2C Multiplexer functions are not needed right now because only the ADC is in use. -KJH 08/01/25

MS5837 depthReader = MS5837();
TSYS01 tempReader = TSYS01();


#define NULL_SENSOR_VALUE -320000
#define FAULTY_SENSOR_VALUE -40404


void ReadAllSensors()
{
    depthReader.read();
    tempReader.read();

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
  //String pHString = String(pH.read_ph(), 5); //commented out while library issue is unresolved - KJH 08/01/25
  //String depthString = String(depthReader.depth(), 5);
  String pHString = String(pH.read_ph(), 5);
  String depthString = String(depthReader.depth(), 5);
  String altString = String(depthReader.altitude(), 5);
  String pressureString = String(depthReader.pressure(), 5);

  Serial.println(depthString + ", " + pressureString + ", " + altString +
                 ", " + tempStringC + ", " + pHString);
}
void startupSensors()
{
  //start depth sensor and configure
  depthReader.setModel(MS5837::MS5837_30BA);
  depthReader.setFluidDensity(997);
  depthReader.init();
  //print confirmation
  if (depthReader.init() && tempReader.init())
  {
    Serial.println("All sensors are ready.");
  }
  else
  {
    Serial.println("failed from arduino side.");
    while(!depthReader.init()){
      Serial.println("Depth sensor.");
      delay(100);
    }while(!tempReader.init()){
      Serial.println("temp sensor.");
      delay(100);
    }
    Serial.println("All sensors are ready.");
  }
   if(!pH.begin()){
      Serial.println("PH sensor problem.");
      delay(100);
    }

}



void setup()
{
  Serial.begin(9600);
  Wire.begin();
  startupSensors();

}
void loop()
{
  ReadAllSensors();
  
}
