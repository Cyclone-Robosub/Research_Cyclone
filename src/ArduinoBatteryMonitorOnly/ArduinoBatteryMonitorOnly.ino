//Arduino Code
#include <Wire.h>
#include <floatToString.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADS1X15.h> //include for the ADS1115 ADC
#include <Servo.h>

Servo myServo;
const int servoPin = 9;

Adafruit_ADS1115 ads;

#define NULL_SENSOR_VALUE -320000
#define FAULTY_SENSOR_VALUE -40404
#define CHANNEL_ANALOG_VOLTAGE 1 //ADS1115 channel for the voltage analog out
#define CHANNEL_ANALOG_CURRENT 2 //ADS1115 channel for the current analog out
#define ADS1115ADR 0x48

//timing
long t1;

void ReadAllSensors()
{
    
  //placeholders so the Pi side doesn't need to change parsing
  String tempStringC = String(NULL_SENSOR_VALUE);
  String pHString = String(NULL_SENSOR_VALUE);
  String depthString = String(NULL_SENSOR_VALUE);
  String altString = String(NULL_SENSOR_VALUE);
  String pressureString = String(NULL_SENSOR_VALUE);

  //battery monitoring
  String packVoltageString = String(measureBatteryVoltage(), 5);
  String packCurrentString = String(measureBatteryCurrent(), 5);

 

  Serial.println(depthString + ", " + pressureString + ", " + altString +
                 ", " + tempStringC + ", " + pHString + ", " + packVoltageString + ", " + packCurrentString);
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
  //start adc
  ads.begin(ADS1115ADR, &Wire); 
  Serial.println("Startup Complete.");

}

void setupServo(){
  myServo.attach(servoPin); // Attach the servo to pin 9
  myServo.write(90); // Set initial position to 90 degrees
  Serial.println("Servo initialized at 90 degrees");
}

void updateServoUsingSerial(){
   if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n'); // Read input from serial monitor
        command.trim(); // Remove any whitespace

        if (command == "1") {
            myServo.write(50);
            Serial.println("Servo moved to state 1");
        } 
        else if (command == "2") {
            myServo.write(15);
            Serial.println("Servo moved to state 2");
        } 
        else if (command == "r") {
            myServo.write(80);
            Serial.println("Servo moved to rest state");
        } 
        else {
            Serial.println("Invalid command. Use open1, open2, or rest.");
        }
    }
}

void setup(){

  Serial.begin(9600);
  while(!Serial);

  Wire.begin();
  
  startupSensors();

  Serial.println("start working");

  //read input string on serial monitor
  String givenString = Serial.readStringUntil('.');
  Serial.println(givenString);

  setupServo();

  while (givenString != "start") {
    Serial.println("All sensors are ready.");
    givenString = Serial.readStringUntil('.');
    delay(10);
    // debugging: Serial.println(givenString);
  }
 
}

void loop()
{
  t1 = millis();
  
  ReadAllSensors();
  updateServoUsingSerial();

  while(millis()-t1 < 1000); //control loop frequency
  
}
