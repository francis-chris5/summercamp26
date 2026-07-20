
#include <DHT.h>
#include <Wire.h>
#include <U8x8lib.h>

DHT dht(2, DHT11);
int light = A6;
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);


void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(light, INPUT);
  u8x8.begin();
}//end setup()

void loop() {
  byte deg = 167;
  float humidity = dht.readHumidity();
  float temp_F = dht.readTemperature(true); //true gives fahrenheit
  float temp_C = dht.readTemperature(false);

  String temp = "T: ";
  temp += (int)temp_F;
  temp += " F, ";
  temp += (int)temp_C;
  temp += " C";

  String air = "H: ";
  air += (int)humidity;
  air += "%, ";
  air += getAbsHum(humidity, temp_C);
  air += " g/m3";

  String lit = "L: ";
  lit += getLumens(analogRead(light));
  lit += " lx";

  
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  u8x8.drawString(0, 0, temp.c_str());
  u8x8.drawString(0, 2, air.c_str());
  u8x8.drawString(0, 4, lit.c_str());

  delay(3333);
  u8x8.clearDisplay();
  delay(333);
}//end loop()





int getLumens(int value){
  float Vout = float(value) * (5 / float(1023)); //analog to voltage
  float res = (10000 * (5 - Vout)) / Vout; //voltage to resistance, # is non-photo part of voltage divider
  int lux = 500 / (res/1000); //500 lumen per meter per milliOhm
  return lux;
}


int getAbsHum(float hum, float tem){
  float absolute = ( 6.11 * exp((17.67 * tem)/(tem + 243.5)) * hum * 2.1674 ) / (273.5 + tem);
  return (int)absolute;
}
