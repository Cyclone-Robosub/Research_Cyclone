#include <base_surveyor.h>
#include <do_iso_surveyor.h>
#include <do_surveyor.h>
#include <orp_iso_surveyor.h>
#include <orp_surveyor.h>
#include <ph_iso_surveyor.h>
#include <ph_surveyor.h>
#include <rtd_surveyor.h>

#include <SD.h>
#include <SPI.h>
//Arduino Code
#include <TSYS01.h>
#include <Wire.h>
#include <floatToString.h>
#include <MS5837.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADS1X15.h>  //include for the ADS1115 ADC



#ifdef USE_PULSE_OUT
#include "ph_iso_surveyor.h"
Surveyor_pH_Isolated pH = Surveyor_pH_Isolated(A0);
#else
#include "ph_surveyor.h"
Surveyor_pH pH = Surveyor_pH(A0);
#endif



MS5837 depthReader = MS5837();
TSYS01 tempReader = TSYS01();

#define SDcardPIN 10
#define NULL_SENSOR_VALUE -320000

File researchFile;
String pathtoResearchFile;
bool isSDcardReady = false;

unsigned long startTime;

void ReadAllSensors() {
  startTime = millis() / 1000;
  depthReader.read();
  tempReader.read();

  String tempStringC;
  if (tempReader.temperature() < -1000) {
    // Serial.println("No Got it");
    tempStringC = String(NULL_SENSOR_VALUE);
  } else {
    // Serial.println("Got it");
    tempStringC = String(tempReader.temperature(), 5);
  }

  String pHString = String(pH.read_ph(), 5);
  String depthString = String(depthReader.depth(), 5);
  String altString = String(depthReader.altitude(), 5);
  String pressureString = String(depthReader.pressure(), 5);
  String startTimeString = String(startTime, 6);
  if (!isSDcardReady) {
    Serial.println(startTimeString + "," + depthString + ", " + pressureString + ", " + altString + ", " + tempStringC + ", " + pHString);
  } else {
    researchFile.println(startTimeString + "," + depthString + ", " + pressureString + ", " + altString + ", " + tempStringC + ", " + pHString);
  }
}
void startupSensors() {
  //start depth sensor and configure
  depthReader.setModel(MS5837::MS5837_30BA);
  depthReader.setFluidDensity(997);
  depthReader.init();
  //print confirmation
  if (depthReader.init() && tempReader.init()) {
    Serial.println("All sensors are ready.");
  } else {
    Serial.println("failed from arduino side.");
    while (!depthReader.init()) {
      Serial.println("Depth sensor.");
      delay(100);
    }
    while (!tempReader.init()) {
      Serial.println("temp sensor.");
      delay(100);
    }
    Serial.println("All sensors are ready.");
  }
  if (!pH.begin()) {
    Serial.println("PH sensor problem.");
    delay(100);
  }
}

void SetResearchFileName() {
  int count = 0;
  pathtoResearchFile = "Research_" + String(count) + ".txt";
  //We need to stop once all the previous files are gone through.
  while (SD.exists(pathtoResearchFile)) {
    count++;
    pathtoResearchFile = "Research_" + String(count) + ".txt";
  }
}
void OpenResearchFile() {
  researchFile = SD.open(pathtoResearchFile, FILE_WRITE);
  if (researchFile) {
    isSDcardReady = true;
  } else {
    isSDcardReady = false;
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  startupSensors();
  pinMode(SDcardPIN, OUTPUT);
  if (!SD.begin(SDcardPIN)) {
    Serial.println("Failure to connect to SD card");
    return;
  }
  SetResearchFileName();
}
void loop() {
  OpenResearchFile();
  //benchmark needed.
  for (int i = 0; i < 8; i++) {
    ReadAllSensors();
  }
  researchFile.close();
}
