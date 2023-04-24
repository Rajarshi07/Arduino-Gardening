/***************************************************************************************************************
 * IoT DHT Temp/Hum Station using NodeMCU ESP-12 Develop Kit V1.0
 *  DHT connected to NodeMCU pin D3
 *  DHT Data on local OLED Display
 *
 *  Developed by MJRovai 12 October 2017
 ********************************************************************************************************************************/

// #include <ACROBOTIC_SSD1306.h> // library for OLED: SCL ==> D1; SDA ==> D2
#include <SPI.h>
#include <Wire.h>

/* DHT22*/
#include "DHT.h"
#define DHTPIN D3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float hum = 0;
float temp = 0;


/* Soil Moister */
#define soilMoisterPin A0
#define soilMoisterVcc D4
int soilMoister = 0;


/* DS18B20 Temperature Sensor */
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 14 // DS18B20 on NodeMCU pin D5 corresponds to GPIO 014 on Arduino
float soilTemp;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();
  DS18B20.begin();
  digitalWrite (soilMoisterVcc, LOW);
}

void loop() {
  getDhtData();
  getSoilMoisterData();
  getSoilTempData();
  displayData();
  delay(2000);  // delay for getting DHT22 data
}


/***************************************************
 * Get DHT data
 **************************************************/
void getDhtData(void) {
  float tempIni = temp;
  float humIni = hum;
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  if (isnan(hum) || isnan(temp))  // Check if any reads failed and exit early (to try again).
  {
    Serial.println("Failed to read from DHT sensor!");
    temp = tempIni;
    hum = humIni;
    return;
  }
}



/***************************************************
 * Get Soil Moister Sensor data
 **************************************************/
void getSoilMoisterData(void)
{
  soilMoister = 0;
  digitalWrite (soilMoisterVcc, HIGH);
  delay (500);
  int N = 3;
  for(int i = 0; i < N; i++) // read sensor "N" times and get the average
  {
    soilMoister += analogRead(soilMoisterPin);   
    delay(150);
  }
  digitalWrite (soilMoisterVcc, LOW);
  soilMoister = soilMoister/N; 
  // Serial.println(soilMoister);
  // soilMoister = map(soilMoister, 380, 0, 0, 100); 
}


/***************************************************
 * Display data at Serial Monitora
 **************************************************/
void displayData(void)
{
  Serial.print(" Temperature: ");
  Serial.print(temp);
  Serial.print("oC   Humidity: ");
  Serial.print(hum);
  Serial.print("%   Moisture: ");
  Serial.print(soilMoister);
  Serial.print("   SoilTemp: ");
  Serial.print(soilTemp);
  Serial.println("oC");
}


/***************************************************
 * Get SoilTemp sensor data
 **************************************************/
void getSoilTempData()
{
  DS18B20.requestTemperatures(); 
  soilTemp = DS18B20.getTempCByIndex(0);

  int newData = ((soilTemp + 0.05) * 10); //fix soilTemp value to 1 decimal place.
  soilTemp = (newData / 10.0);
}
