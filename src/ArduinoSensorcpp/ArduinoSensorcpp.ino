//Arduino Code
#include <TSYS01.h>
#include <Wire.h>
#include <floatToString.h>
#include <MS5837.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADS1X15.h> //include for the ADS1115 ADC


/* not needed for competition
#ifdef USE_PULSE_OUT
  #include "ph_iso_surveyor.h"       
  Surveyor_pH_Isolated pH = Surveyor_pH_Isolated(A0);         
#else
  #include "ph_surveyor.h"             
  Surveyor_pH pH = Surveyor_pH(A0);   
#endif
*/

//I2C Multiplexer functions are not needed right now because only the ADC is in use. -KJH 08/01/25

MS5837 depthReader = MS5837();
TSYS01 tempReader = TSYS01();
Adafruit_ADS1115 ads;

#define TCADDR 0x70
#define NULL_SENSOR_VALUE -320000
#define FAULTY_SENSOR_VALUE -40404
#define CHANNEL_ANALOG_VOLTAGE 1 //ADS1115 channel for the voltage analog out
#define CHANNEL_ANALOG_CURRENT 2 //ADS1115 channel for the current analog out
#define ADS1115ADR 0x48

void ReadAllSensors()
{
    tcaselect(1);
    Wire.beginTransmission(118);
    depthReader.read();

    tcaselect(7);
    Wire.beginTransmission(119);
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
  String pHString = String(NULL_SENSOR_VALUE, 5);
  String depthString = String(NULL_SENSOR_VALUE, 5);
  String altString = String(depthReader.altitude(), 5);
  String pressureString = String(depthReader.pressure(), 5);

  tcaselect(1);
  String packVoltageString = String(measureBatteryVoltage(), 5);
  String packCurrentString = String(measureBatteryCurrent(), 5);

  Serial.begin(9600);
  Wire.begin();
  Serial.println(depthString + ", " + pressureString + ", " + altString +
                 ", " + tempStringC + ", " + pHString + ", " + packVoltageString + ", " + packCurrentString);
}

void tcaselect(uint8_t portOnTCA) {
  if (portOnTCA > 7) return;
 
  Wire.beginTransmission(TCADDR);
  Wire.write(1 << portOnTCA);
  Wire.endTransmission();  
}

float measureBatteryVoltage(){
  float adcVoltage = ads.computeVolts(ads.readADC_SingleEnded(CHANNEL_ANALOG_VOLTAGE));
  float packVoltage = adcVoltage*60/3.3;
  return packVoltage;
}

float measureBatteryCurrent(){
  float adcVoltage = ads.computeVolts(ads.readADC_SingleEnded(CHANNEL_ANALOG_CURRENT));
  float packCurrent = adcVoltage*120/3.3;
  return packCurrent;
}

void startupSensors()
{
  //start depth sensor and configure
  tcaselect(1);
  Wire.beginTransmission(118);
  depthReader.setModel(MS5837::MS5837_30BA);
  depthReader.setFluidDensity(997);
  depthReader.init();

   if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }

  //start tempt reader and configure
  tcaselect(7);
  Wire.beginTransmission(119);
  tempReader.init();

  //start adc
  ads.begin(ADS1115ADR,&Wire); 

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

}



void setup()
{
  Serial.begin(9600);
  Wire.begin();
  startupSensors();

  Serial.println("start working");
  String givenString = Serial.readStringUntil('.');
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
