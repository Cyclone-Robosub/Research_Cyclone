//            Libraries              //

#include <base_surveyor.h>
#include <do_iso_surveyor.h>
#include <do_surveyor.h>
#include <orp_iso_surveyor.h>
#include <orp_surveyor.h>
#include <ph_iso_surveyor.h>
#include <ph_surveyor.h>
#include <rtd_surveyor.h>
#include <string.h>
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


//         pH Calibration            //

uint8_t user_bytes_received = 0;
const uint8_t bufferlen = 32;
char user_data[bufferlen];

void parse_cmd(char* string) {
  string[strcspn(string, "\r\n")] = 0;
  if (strcmp(string, "CAL,7") == 0) {
    Serial.flush();
    pH.cal_mid();
    Serial.println("MID CALIBRATED");
  }
  else if (strcmp(string, "CAL,4") == 0) {
    Serial.flush();
    pH.cal_low();
    Serial.println("LOW CALIBRATED");
  }
  else if (strcmp(string, "CAL,10") == 0) {
    Serial.flush();
    pH.cal_high();
    Serial.println("HIGH CALIBRATED");
  }
  else if (strcmp(string, "CAL,CLEAR") == 0) {
    Serial.flush();
    pH.cal_clear();
    Serial.println("CALIBRATION CLEARED");
  }
}
//End pH Callibration


void ReadAllSensors() {  //******************************************************************
  depthReader.read();
  delay(10);
  tempReader.read();
  delay(10);

  String tempStringC;
  if (tempReader.temperature() < -1000) {
    tempStringC = String(NULL_SENSOR_VALUE);
  } else {
    // Serial.println("Got it");
    tempStringC = String(tempReader.temperature(), 5);
  }


  DateTime now = rtc.now();
  String hourStr = (now.hour() < 10 ? "0" : "") + String(now.hour(), DEC);
  String minuteStr = (now.minute() < 10 ? "0" : "") + String(now.minute(), DEC);
  String secondStr = (now.second() < 10 ? "0" : "") + String(now.second(), DEC);
  String formattedTime = hourStr + ":" + minuteStr + ":" + secondStr;
  String pHString = String(pH.read_ph(), 3);
  String depthString = String(depthReader.depth(), 3);
  String pressureString = String(depthReader.pressure(), 3);


  if (isSDcardReady) {
    if (researchFile) {
      //Serial.println("Time:   " + formattedTime);
      //Serial.println("Temperature:  " + tempStringC + ", pH:   " + pHString);
      //Serial.println("Depth:  " + depthString + ", Pressure:  " + pressureString);
      //Serial.println("");
      researchFile.println(formattedTime + ", " + tempStringC + ", " + pHString + ", " + depthString + ", " + pressureString);
      researchFile.flush();
      delay (5000); 
       
      //       This delay is to limit how many data points we receive: we don't need it to read more often than this         //
      
    }
  }
}
void startupSensors() {  //******************************************************************
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

void SetResearchFileName() {//**************************************************************
  pathtoResearchFile = "Research.txt";
  Serial.println("Opening: Research.txt!");
}



void OpenResearchFile() {  //***************************************************************
  researchFile = SD.open(pathtoResearchFile, FILE_WRITE);
  if (researchFile) {
    isSDcardReady = true;
  } else {
    isSDcardReady = false;
    //Serial.println("Failed to open file");
    //Leaving this out. Seems like it runs well, it just has issues where it fails to open and then opens the file at the same timestamp after.
  }
}

void setup() {  //**************************************************************************

  Serial.begin(115200);
  Serial.print("Arduino Initializing!!! ");
  researchFile.close();

  while (!Serial) {
    delay(10); // wait for serial port to connect. Needed for native USB port only
  }

  //RTC Setup
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
  //Serial.print("Initializing SD card... ");
  if (!SD.begin(SDcardPIN)) {
    Serial.println("Failure to connect to SD card");
    while (1);
  }
  Serial.println("card initialized. THIS IS A NEW TEST RUN!! 3 9 26");
  SetResearchFileName();

  researchFile = SD.open("Research.txt", FILE_WRITE);
  researchFile.println("");
  
  DateTime now = rtc.now();
  String yearStr = (now.year() < 10 ? "0" : "") + String(now.year(), DEC);
  String monthStr = (now.month() < 10 ? "0" : "") + String(now.month(), DEC);
  String dayStr = (now.day() < 10 ? "0" : "") + String(now.day(), DEC);
  String formattedDate = monthStr + "/" + dayStr + "/" + yearStr;
  researchFile.println(formattedDate);
  Serial.print(formattedDate);
  Serial.println("");
  Serial.println("");
  delay (1000);
}

void loop() { //**************************************************************************
  
  if (Serial.available() > 0) {
    user_bytes_received = Serial.readBytesUntil('\n', user_data, sizeof(user_data) - 1);
    user_data[user_bytes_received] = '\0';
  }
  if (user_bytes_received) {
    parse_cmd(user_data);
    user_bytes_received = 0;
    memset(user_data, 0, sizeof(user_data));
  }
  OpenResearchFile();
  ReadAllSensors();

}
