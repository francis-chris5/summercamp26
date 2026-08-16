#include <Arduino.h>

#include <U8g2lib.h>

#include <stdio.h>

#include <DHT.h>

#include <MQ135.h>


DHT dht(2, DHT11);


// Initialize for SH1106 with hardware I2C

U8G2_SH1106_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


int mq135AnalogPin = A0; // MQ-135 analog output to A0

float ppmCO2 = 0.0;

int light = A0;

#define D2 2       // Pin connected to the DHT sensor data line

#define DHTTYPE DHT11  // Change to DHT22 if you are using that sensor model

#define MAG_ADDR 0x2C


 int getLumens(int value){

   float Vout = float(value) * (5 / float(1023)); //analog to voltage

   float res = (10000 * (5 - Vout)) / Vout; //voltage to resistance, # is non-photo part of voltage divider

    int lux = 500 / (res/1000); //500 lumen per meter per milliOhm

   return lux;


 }

void setup(void) {

  u8g2.begin();

  pinMode(2, INPUT);


  dht.begin();

  // pinMode(light, INPUT);


   //Reading temperature in Celsius

    int sensorValue = analogRead(A0); // Read analog pin A0


}


void loop(void) {


int SensorValue = analogRead(mq135AnalogPin);


  // Formula to estimate PPM based on raw analog value (0 - 1023)

  if (SensorValue > 0) {

    ppmCO2 = (116.6020682 * pow((float(SensorValue) / (1024.0 - SensorValue)), 2.769034857))/10;

   } else

    {

    ppmCO2 = 0;

  }


  float tempC = dht.readTemperature();

    //Calculate temperature in Fahrenheit

    float tempF = dht.readTemperature() * 1.8 + 32; 


    //Reading humidity

    float h = dht.readHumidity();


  u8g2.firstPage();

  do {

    u8g2.setFont(u8g2_font_oskool_tr); // Set the font

    u8g2.setFlipMode(digitalRead(2));

    u8g2.drawStr(0, 8, "Lumens: ");

    u8g2.drawStr(48, 8, String(getLumens(analogRead(light))).c_str());

    u8g2.drawStr(0, 20, "TemperatureC: ");

    u8g2.drawStr(85, 20, String(tempC).c_str());

    u8g2.drawStr(0, 32, "TemperatureF: ");

    u8g2.drawStr(85, 32, String(tempF).c_str());

    u8g2.drawStr(0, 44, "Humidity%: ");

    u8g2.drawStr(65, 44, String(h).c_str());

    u8g2.drawStr(0, 56, "CO2 Count: ");

    u8g2.drawStr(65, 56, String(ppmCO2).c_str());

  } while (u8g2.nextPage());


}