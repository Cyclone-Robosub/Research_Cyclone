#include <TSYS01.h>
#include <Wire.h>
#include <floatToString.h>
#include <MS5837.h>
MS5837 depthReader = MS5837();
TSYS01 tempReader = TSYS01();
void startReading()
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
    tempStringC = String(depthReader.temperature(), 5);
  }
  else
  {
    // Serial.println("Got it");
    tempStringC = String(tempReader.temperature(), 5);
  }
  String depthString = String(depthReader.depth(), 5);
  String altString = String(depthReader.altitude(), 5);
  String pressureString = String(depthReader.pressure(), 5);

  Serial.println("Reading: " + depthString + "," + altString + "," + tempStringC + "," + pressureString);
}

void startSensors()
{
  depthReader.setModel(MS5837::MS5837_30BA);
  depthReader.setFluidDensity(997);
  if (depthReader.init() && tempReader.init())
  {
    Serial.println("All sensors are ready.");
  }
  else
  {
    Serial.println("failed.")
  }

  return 0;
}
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  startSensors();
  // loop();
}
void loop()
{
  String givenString = Serial.readStringUntil('.');
  if (givenString == "start")
  {
    while (givenString != "stop")
    {
      givenString = Serial.readStringUntil('.');
      delay(10);
      startReading();
      givenString = Serial.readStringUntil('.');
    }
  }
}
