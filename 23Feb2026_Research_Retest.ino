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
#include "TSYS01.h"
#include <MS5837.h>

#include <Wire.h>
#include <floatToString.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADS1X15.h>  //include for the ADS1115 ADC


#include "RTClib.h"
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


//#define USE_PULSE_OUT
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

TSYS01 sensor;
File researchFile;
String pathtoResearchFile;
bool isSDcardReady = false;


//pH Calibration stuffs
uint8_t user_bytes_received = 0;
const uint8_t bufferlen = 32;
char user_data[bufferlen];

void parse_cmd(char* string) {
  if (strcmp(string, "CAL,7") == 0) {
    pH.cal_mid();
    Serial.println("MID CALIBRATED");
  }
  else if (strcmp(string, "CAL,4") == 0) {
    pH.cal_low();
    Serial.println("LOW CALIBRATED");
  }
  else if (strcmp(string, "CAL,10") == 0) {
    pH.cal_high();
    Serial.println("HIGH CALIBRATED");
  }
  else if (strcmp(string, "CAL,CLEAR") == 0) {
    pH.cal_clear();
    Serial.println("CALIBRATION CLEARED");
  }
}
//End pH Callibration


void ReadAllSensors() {

  depthReader.read();
  delay(10);
  tempReader.read();
  delay(10);

  String tempStringC;
  if (tempReader.temperature() < -1000) {
    // Serial.println("No Got it");
    tempStringC = String(NULL_SENSOR_VALUE);
  } else {
    // Serial.println("Got it");
    tempStringC = String(tempReader.temperature(), 5);
  }

  String pHString = String(pH.read_ph(), 3);
  String depthString = String(depthReader.depth(), 5);
  String pressureString = String(depthReader.pressure(), 5);


  DateTime now = rtc.now();
  String hourStr = (now.hour() < 10 ? "0" : "") + String(now.hour(), DEC);
  String minuteStr = (now.minute() < 10 ? "0" : "") + String(now.minute(), DEC);
  String secondStr = (now.second() < 10 ? "0" : "") + String(now.second(), DEC);
  String formattedTime = hourStr + ":" + minuteStr + ":" + secondStr;

  if (isSDcardReady) {
    if (researchFile) {
      Serial.println("Time:   " + formattedTime);
      Serial.println("Temperature:  " + tempStringC + ", pH:   " + pHString);
      Serial.println("Depth:  " + depthString + ", Pressure:  " + pressureString);
      Serial.println("");
      researchFile.println(formattedTime + ", " + tempStringC + ", " + pHString + ", " + depthString + ", " + pressureString);
      researchFile.flush();
      delay (250);
    }
  }
}
void startupSensors() {
  //start depth sensor and configure
  depthReader.setModel(MS5837::MS5837_30BA);
  depthReader.setFluidDensity(997);
  depthReader.init();
  //print confirmation
  if (tempReader.init()) {
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
  pathtoResearchFile = "Research.txt";
  //We need to stop once all the previous files are gone through.
  Serial.println("Opening: Research.txt! 2 23 2026");
  //Serial.println("Opening: " + pathtoResearchFile);
}



void OpenResearchFile() {
  //researchFile = SD.open(pathtoResearchFile, FILE_WRITE);
  File researchFile = SD.open(pathtoResearchFile, FILE_WRITE);
  if (researchFile) {
    isSDcardReady = true;
  } else {
    isSDcardReady = false;
    //Serial.println("Failed to open file");
    //Leaving this out. Seems like it runs well, it just has issues where it fails to open and then opens the file at the same timestamp after.
  }
}

void setup() {

  Serial.begin(2000000);
  Serial.print("Arduino Initializing!");
  researchFile.close();

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //RTC
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(2025, 1, 1, 0, 0, 0));
  }


  //SD Card
  Wire.begin();
  startupSensors();
  pinMode(SDcardPIN, OUTPUT);
  Serial.print("Initializing SD card...");
  if (!SD.begin(SDcardPIN)) {
    Serial.println("Failure to connect to SD card");
    while (1);
  }
  Serial.println("card initialized.");
  SetResearchFileName();

  researchFile = SD.open("Research.txt", FILE_WRITE);
  //researchFile.println("HI THIS IS A NEW TEST RUN!! 2 23 26");
  Serial.println("HI THIS IS A NEW TEST RUN!! 2 23 26");
  delay (1000);
}

void loop() {
  if (Serial.available() > 0) {
    user_bytes_received = Serial.readBytesUntil(13, user_data, sizeof(user_data));
  }

  if (user_bytes_received) {
    parse_cmd(user_data);
    user_bytes_received = 0;
    memset(user_data, 0, sizeof(user_data));
  }

  OpenResearchFile();
  for (int i = 0; i < 1000; i++) {
    ReadAllSensors();
  }
  researchFile.close();


}
