//Here's what you need to know. Put 5V to 5V, probably on a bredboard. It's easier if you have them all connected. 
//Connect the I2C connector, which is the one for the temp sensor. Plug in 5V, ground, and then SCL and SDA to their pins, on the digital side.
//Plug in the A on the pH board to the A0 pin.
//Take the 9V battery and attach the node, and plug into the arduino.

//Done with the wiring! Very easy


#include <Wire.h>
#include "TSYS01.h"
#ifdef USE_PULSE_OUT
  #include "ph_iso_surveyor.h"       
  Surveyor_pH_Isolated pH = Surveyor_pH_Isolated(A0);         
#else
  #include "ph_surveyor.h"             
  Surveyor_pH pH = Surveyor_pH(A0);   
#endif
                
TSYS01 sensor;

uint8_t user_bytes_received = 0;                
const uint8_t bufferlen = 32;                   
char user_data[bufferlen];                     

void parse_cmd(char* string) {                   
  strupr(string);                                
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

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");                            
  delay(200);

  Wire.begin();

  while (!sensor.init()) {
        Serial.println("TSYS01 device failed to initialize!");
        delay(2000);
  }
  Serial.println(F("Use commands \"CAL,7\", \"CAL,4\", and \"CAL,10\" to calibrate the circuit to those respective values"));
  Serial.println(F("Use command \"CAL,CLEAR\" to clear the calibration"));
  if (pH.begin()) {                                     
    Serial.println("Loaded EEPROM");
  }
}

void loop() {
  if (Serial.available() > 0) {                                                      
    user_bytes_received = Serial.readBytesUntil(13, user_data, sizeof(user_data));   
  }

  if (user_bytes_received) {                                                      
    parse_cmd(user_data);                                                          
    user_bytes_received = 0;                                                        
    memset(user_data, 0, sizeof(user_data)); //This is for the pH sensor                                         
  }
  
  sensor.read();
  Serial.print("Temperature:  ");
  Serial.print(sensor.temperature()); 
  Serial.println(" C");  //Temp sensor
  
  Serial.print("pH:           ");
  Serial.println(pH.read_ph()); //pH Sensor                                                 
  delay(500);
  
}
