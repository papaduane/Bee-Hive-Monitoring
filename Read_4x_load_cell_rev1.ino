//-------------------------------------------------------------------------------------
// HX711_ADC.h
// Arduino master library for HX711 24-Bit Analog-to-Digital Converter for Weigh Scales
// Olav Kallhovd sept2017
// Tested with      : HX711 asian module on channel A and YZC-133 3kg load cell
// Tested with MCU  : Arduino Nano
//-------------------------------------------------------------------------------------
// This is an example sketch on how to use this library for two ore more HX711 modules
// Settling time (number of samples) and data filtering can be adjusted in the config.h file

#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#include <Wire.h>               // Includes the Wire library for I2C communication
#include <LiquidCrystal_I2C.h>  // Includes the LiquidCrystal_I2C library

#endif

//pins:
const int HX711_dout_1 = 4; //mcu > HX711 no 1 dout pin
const int HX711_sck_1 = 5; //mcu > HX711 no 1 sck pin
const int HX711_dout_2 = 6; //mcu > HX711 no 2 dout pin
const int HX711_sck_2 = 7; //mcu > HX711 no 2 sck pin
const int HX711_dout_3 = 8; //mcu > HX711 no 3 dout pin
const int HX711_sck_3 = 9; //mcu > HX711 no 3 sck pin
const int HX711_dout_4 = 10; //mcu > HX711 no 4 dout pin
const int HX711_sck_4 = 11; //mcu > HX711 no 4 sck pin

//HX711 constructor (dout pin, sck pin)
HX711_ADC LoadCell_1(HX711_dout_1, HX711_sck_1); //HX711 1
HX711_ADC LoadCell_2(HX711_dout_2, HX711_sck_2); //HX711 2
HX711_ADC LoadCell_3(HX711_dout_3, HX711_sck_3); //HX711 3
HX711_ADC LoadCell_4(HX711_dout_4, HX711_sck_4); //HX711 4


const int calVal_eepromAdress_1 = 0; // eeprom adress for calibration value load cell 1 (4 bytes)
const int calVal_eepromAdress_2 = 4; // eeprom adress for calibration value load cell 2 (4 bytes)
unsigned long t = 0;

#define lcdAddress 0x3f         // LCD I2C Address

int count = 0;                  // Variable to hold current count value

LiquidCrystal_I2C lcd( lcdAddress );   // Create instance of LiquidCrystal_I2C and set the LCD I2C address

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");
   


  float calibrationValue_1; // calibration value load cell 1
  float calibrationValue_2; // calibration value load cell 2
  float calibrationValue_3; // calibration value load cell 3
  float calibrationValue_4; // calibration value load cell 4

  float offsetValue_1; // offset value load cell 1
  float offsetValue_2; // offset value load cell 2
  float offsetValue_3; // offset value load cell 3
  float offsetValue_4; // offset value load cell 4

  calibrationValue_1 = 9600.31; // uncomment this if you want to set this value in the sketch
  calibrationValue_2 = 9294; // uncomment this if you want to set this value in the sketch
  calibrationValue_3 = 8586.75; // uncomment this if you want to set this value in the sketch
  calibrationValue_4 = 9446.01; // uncomment this if you want to set this value in the sketch

  offsetValue_1 = 8491529; // offset value load cell 1
  offsetValue_2 = 8400255; // offset value load cell 2
  offsetValue_3 = 8772830; // offset value load cell 3
  offsetValue_4 = 8719916; // offset value load cell 4



 LoadCell_1.setTareOffset(0); // user set offset value (float)
  LoadCell_2.setTareOffset(0); // user set offset value (float)
  LoadCell_3.setTareOffset(0); // user set offset value (float)
  LoadCell_4.setTareOffset(0); // user set offset value (float)
 
    
#if defined(ESP8266) || defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266 and want to fetch the value from eeprom
#endif
  //EEPROM.get(calVal_eepromAdress_1, calibrationValue_1); // uncomment this if you want to fetch the value from eeprom
  //EEPROM.get(calVal_eepromAdress_2, calibrationValue_2); // uncomment this if you want to fetch the value from eeprom

   LoadCell_1.begin();
   LoadCell_2.begin();
   LoadCell_3.begin();
   LoadCell_4.begin();
  
  unsigned long stabilizingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilizing time
  boolean _tare = false; //set this to false if you don't want tare to be performed in the next step
  byte loadcell_1_rdy = 0;
  byte loadcell_2_rdy = 0;
  byte loadcell_3_rdy = 0;
  byte loadcell_4_rdy = 0;
  while ((loadcell_1_rdy + loadcell_2_rdy+ loadcell_3_rdy+ loadcell_4_rdy) < 4) { //run startup, stabilization and tare, both modules simultaniously
    if (!loadcell_1_rdy) loadcell_1_rdy = LoadCell_1.startMultiple(stabilizingtime, _tare);
    if (!loadcell_2_rdy) loadcell_2_rdy = LoadCell_2.startMultiple(stabilizingtime, _tare);
    if (!loadcell_3_rdy) loadcell_3_rdy = LoadCell_3.startMultiple(stabilizingtime, _tare);
    if (!loadcell_4_rdy) loadcell_4_rdy = LoadCell_4.startMultiple(stabilizingtime, _tare);
    
  }
  if (LoadCell_1.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.1 wiring and pin designations");
  }
  if (LoadCell_2.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.2 wiring and pin designations");
  }
  if (LoadCell_3.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.3 wiring and pin designations");
  }
  if (LoadCell_4.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.4 wiring and pin designations");
  }


 
   LoadCell_1.setCalFactor(calibrationValue_1); // user set calibration value (float)
   LoadCell_2.setCalFactor(calibrationValue_2); // user set calibration value (float)
   LoadCell_3.setCalFactor(calibrationValue_3); // user set calibration value (float)
   LoadCell_4.setCalFactor(calibrationValue_4); // user set calibration value (float)

  
  LoadCell_1.setTareOffset(offsetValue_1); // user set offset value (float)
  LoadCell_2.setTareOffset(offsetValue_2); // user set offset value (float)
  LoadCell_3.setTareOffset(offsetValue_3); // user set offset value (float)
  LoadCell_4.setTareOffset(offsetValue_4); // user set offset value (float)


  
  Serial.println("Startup is complete");

    
    lcd.begin(20,4);              // initialize the lcd as a 20 characters by 4 rows display
    lcd.setBacklight(HIGH);       // Turn on the LCD's backlight 
    lcd.home ();                  // set the cursor to the home position (first row, first column)

}
void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell_1.update()) newDataReady = true;
  LoadCell_2.update();
  LoadCell_3.update();
  LoadCell_4.update();


 
  //get smoothed value from data set
  if ((newDataReady)) {
    if (millis() > t + serialPrintInterval) {
      float a = LoadCell_1.getData();
      float b = LoadCell_2.getData();
      float c = LoadCell_3.getData();
      float d = LoadCell_4.getData(); 
      
      Serial.print("Load_cell 1 output val: ");
      Serial.print(a);
      Serial.print("    Load_cell 2 output val: ");
      Serial.print(b);
      Serial.print("    Load_cell 3 output val: ");
      Serial.print(c);
      Serial.print("    Load_cell 4 output val: ");
      Serial.println(d);
 
       lcd.setCursor ( 0, 0 );       // set cursor to first character of line 2
       lcd.print("Hive 1: ");
       lcd.print(a);     // print "hive number" to the LCD
 
       lcd.setCursor (0, 1 );       // set cursor to first character of line 2
       lcd.print("Hive 2: ");
       lcd.print(b);     // print "hive number" to the LCD

       lcd.setCursor (0, 2 );       // set cursor to first character of line 2
       lcd.print("Hive 3: ");
       lcd.print(c);     // print "hive number" to the LCD

       lcd.setCursor ( 0, 3 );       // set cursor to first character of line 2
       lcd.print("Hive 4: ");
       lcd.print(d);     // print "hive number" to the LCD
       
      newDataReady = 0;
      t = millis();
    }
  }

  // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') {
      LoadCell_1.tareNoDelay();
      LoadCell_2.tareNoDelay();
    }
  }

  //check if last tare operation is complete
  if (LoadCell_1.getTareStatus() == true) {
    Serial.println("Tare load cell 1 complete");
  }
  if (LoadCell_2.getTareStatus() == true) {
    Serial.println("Tare load cell 2 complete");
  }

}
