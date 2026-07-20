
#include <DHT.h>
#include <Wire.h>
#include <U8x8lib.h>
#include <Adafruit_BMP085.h>
#include <Encoder.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_Sensor.h>

DHT dht(7, DHT11);
int light = A1;
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
Adafruit_BMP085 barometer;
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
int gas = A0;

int rotary_clk = 2;
int rotary_dt = 3;
Encoder knob(2, 3);
long lastPosition = -999;
int encoderCount = 0;
int lastEncoder = 0;


void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(light, INPUT);
  u8x8.begin();
  barometer.begin();
  mag.begin();
  pinMode(gas, INPUT);
}//end setup()

void loop() {
  long currentPosition = knob.read();
  long newPosition = currentPosition / 4;
  if(newPosition != lastPosition){
    Serial.println(newPosition);
    lastPosition = newPosition;
    u8x8.clearDisplay();
  }

  if(abs(lastPosition) % 6 == 0){
    getLumens(analogRead(light));
  }
  else if(abs(lastPosition) % 6 == 1){
    getHumidity();
  }
  else if(abs(lastPosition) % 6 == 2){
    getTemperature();
  }
  else if(abs(lastPosition) % 6 == 3){
    getPressure();
  }
  else if(abs(lastPosition) % 6 == 4){
    getMagnetic();
  }
  else if(abs(lastPosition) % 6 == 5){
    getGasses();
  }


  // delay(3333);
  // u8x8.clearDisplay();
}//end loop()




void getLumens(int value){
  float Vout = float(value) * (5 / float(1023)); //analog to voltage
  float res = (10000 * (5 - Vout)) / Vout; //voltage to resistance, # is non-photo part of voltage divider
  int lux = 500 / (res/1000); //500 lumen per meter per milliOhm
  String label = "Lumens: ";
  String lit = String(lux) + " lx";
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  u8x8.drawString(0, 0, label.c_str());
  u8x8.drawString(0, 3, lit.c_str());
}//end getLumens()

void getHumidity(){
  float humidity = dht.readHumidity();
  // float temp_C = dht.readTemperature(false); //true gives farenheit
  // float absolute = ( 6.11 * exp((17.67 * temp_c)/(temp_c + 243.5)) * humidty * 2.1674 ) / (273.5 + temp_c);
  String label = "Humidity";
  String hum = String(humidity) + "%";
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  u8x8.drawString(0, 0, label.c_str());
  u8x8.drawString(0, 3, hum.c_str());
}//end getHumidity()


void getTemperature(){
  float temp_F = dht.readTemperature(true); //true gives fahrenheit
  float temp_C = dht.readTemperature(false);
  String label = "Temperature: ";
  String far = String(temp_F) + " F";
  String cel = String(temp_C) + " C";
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  u8x8.drawString(0, 0, label.c_str());
  u8x8.drawString(0, 3, far.c_str());
  u8x8.drawString(0, 5, cel.c_str());
}//end getTemperature()


void getPressure(){
  int32_t reading = barometer.readPressure();
  int pressure = reading * 0.0002953;
  String label = "Pressure";
  String pres = String(pressure) + " inHg";
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  u8x8.drawString(0, 0, label.c_str());
  u8x8.drawString(0, 3, pres.c_str());
}//end getPressure()


void getMagnetic(){
  float gain = 0.92;
  sensors_event_t event;
  mag.getEvent(&event);
  String label = "Magnetic Field";
  String x = "X: ";
  x += (event.magnetic.x * gain);
  x += " mG";
  String y = "Y: ";
  y += (event.magnetic.y * gain);
  y += " mG";
  String z = "Z: ";
  z += (event.magnetic.z * gain);
  z += " mG";
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  u8x8.drawString(0, 0, label.c_str());
  u8x8.drawString(0, 3, x.c_str());
  u8x8.drawString(0, 5, y.c_str());
  u8x8.drawString(0, 7, z.c_str());
  /*
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  if(heading < 0){
    heading += 2*PI;
  }
  if(heading > 2*PI){
    heading -= 2*PI;
  }
  */
}//end getMagnetic()


void getGasses(){
  float voltage = 0;
  for(int i=0; i < 50; i++){
    voltage += analogRead(gas) * (5.0/1023);
    delay(10);
  }
  voltage = voltage / 50;
  float ratio = ((5 - voltage) / voltage) / (4.0 / 3.6);
  float a = 605.18;
  float b = -3.93;
  float ppmCO = a * pow(ratio, b);
  String label = "Carbon Monoxide";
  String co = String(ppmCO);
  co += " ppm";
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  u8x8.drawString(0, 0, label.c_str());
  u8x8.drawString(0, 3, co.c_str());
}//end getGgasses()





















