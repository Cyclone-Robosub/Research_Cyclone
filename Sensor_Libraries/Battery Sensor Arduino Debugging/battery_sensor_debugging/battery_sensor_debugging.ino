/*
This script uses an Arduino UNO to communicate with the ADS1115 analog to digital converter.
Channel 2 of the ADC is connected to the analog output of a current sensor and channel 3 is connected 
to the analog output of a voltage sensor, both with a 3.3V full scale. */
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads; 

//constants
#define CHANNEL_ANALOG_VOLTAGE 1 //ADS1115 channel for the voltage analog out
#define CHANNEL_ANALOG_CURRENT 2 //ADS1115 channel for the current analog out
#define ADS1115ADR 0x48

//timing
long t = 0;

float countToVoltage(int16_t count){
  Serial.println(count);
  float maxCount = 65535;
  float vref = 5;
  return ((float) count/maxCount)*vref;

}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }




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



void loop() {
  // put your main code here, to run repeatedly:
  t = millis();

  Serial.print("Voltage = ");
  Serial.print(measureBatteryVoltage());
  Serial.print(", Current = ");
  Serial.println(measureBatteryCurrent());
  


  while(millis() - t < 50);
}
