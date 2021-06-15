// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.


// wood sensor

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>//added duane
//                BS  E  D4 D5  D6 D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);//added duane

#define DHTPIN 2     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;


#include "HX711.h"


// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 3;


HX711 scale;



void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);//added duane
  // Initialize device.
  dht.begin();
  //Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  /*Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  */
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
 /* Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  */
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
//}  Removed to compile
//Working orig do not change
//{    Removed to compile


//  Serial.begin(38400);   Dup
 // Serial.println("HX711 Demo");

  Serial.println("Initializing the scale");

  // Initialize library with data output pin, clock input pin and gain factor.
  // Channel selection is made by passing the appropriate gain:
  // - With a gain factor of 64 or 128, channel A is selected
  // - With a gain factor of 32, channel B is selected
  // By omitting the gain factor parameter, the library
  // default "128" (Channel A) is used here.
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());      // print a raw reading from the ADC

 // Serial.print("read average: \t\t");
 // Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  //Serial.print("get value: \t\t");
 // Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

 // Serial.print("get units: \t\t");
  //Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
  // by the SCALE parameter (not set yet)

  scale.set_scale(10801.55);                      // this value is obtained by calibrating the scale with known weights; see the README for details

  // scale.tare(151);               // reset the scale to 0

 // Serial.print("get tare: \t\t");
 // Serial.println(scale.get_offset());

  scale.set_offset(144347.5);    //set offset of scale instead of tare


 // Serial.print("get tare: \t\t");
  //Serial.println(scale.get_offset());


  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
 //  by the SCALE parameter set with set_scale

  Serial.println("Readings:");
}

void loop()  {
  Serial.print("weight  :\t");// changed to weight
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average weight:\t");
  Serial.println(scale.get_units(10), 1);
  delay (5000);

  scale.power_down();              // put the ADC in sleep mode
  delay(500);
  scale.power_up();

  {
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.print(F("°C   "));// removed new line
    // Display Temperature in F
   lcd.setCursor(0, 1);
   lcd.print(event.temperature);

  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.print(F("%   "));// removed newline
lcd.setCursor(6, 1);
    // lcd.print(event.relative_humidity);
  
      
   {
  lcd.setCursor(0, 0);
    lcd.print("Temp Humid Weigh");
    

   
    lcd.setCursor(0, 1);
   // lcd.print(event.temperature);
    lcd.setCursor(6, 1);
    lcd.print(event.relative_humidity);
    lcd.setCursor(12, 1);
    lcd.print(scale.get_units(10), 1);
   }

  } 
{
  lcd.setCursor(0, 0);
    lcd.print("Temp Humid Weigh");
    

   
    lcd.setCursor(0, 1);
   // lcd.print(event.temperature);
    lcd.setCursor(6, 1);
    lcd.print(event.relative_humidity);
    lcd.setCursor(12, 1);
    lcd.print(scale.get_units(10), 1);
   }
  }
  }
