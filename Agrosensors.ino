#include <DHT.h>
#include <DHT_U.h>
#include<Wire.h>
#define Addr 0x4A
#include <Adafruit_Sensor.h>
#include "DHT.h"

#define DHTPIN 2     // pin DHT is connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
 Serial.begin(9600);
 Serial.println("LABEL,Time,Temperature (C), Humidity (%), Ambient Light Luminance (lux)");  
 dht.begin();
 
 Wire.begin();
 Wire.beginTransmission(Addr);
 Wire.write(0x02);
 Wire.write(0x40);
 Wire.endTransmission();
 
 delay(300);
}

void loop() {
  //Reading data from MAX44009
  unsigned int data[2];
 Wire.beginTransmission(Addr);
 Wire.write(0x03);
 Wire.endTransmission();
 
 Wire.requestFrom(Addr, 2);
 
 if (Wire.available() == 2)
 {
 data[0] = Wire.read();
 data[1] = Wire.read();
 }
 
 // Convert the data to lux
 int exponent = (data[0] & 0xF0) >> 4;
 int mantissa = ((data[0] & 0x0F) << 4) | (data[1] & 0x0F);
 float luminance = pow(2, exponent) * mantissa * 0.045;
 
 //Reading data from DHT22
 float h = dht.readHumidity();
 float t = dht.readTemperature();
 delay(4000);

 // check if returns are valid, if they are NaN (not a number) then something went wrong!
 if (isnan(t) || isnan(h)) {
   Serial.println("Failed to read from DHT");
 } else {
   Serial.print("DATA,TIME,"); Serial.print(t); Serial.print(","); Serial.print(h); Serial.print(","); Serial.println(luminance);
 }
}
