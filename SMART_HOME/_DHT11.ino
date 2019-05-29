#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11

#define dht_dpin D8
DHT dht(dht_dpin, DHTTYPE); 

extern float Humidity;
extern float Temperature;

void DHT11_Setup()
{ 
  dht.begin();
  Serial.begin(9600);
  Serial.println("Humidity and temperature \n\n");
  Serial.println("==========================================");
  delay(700);
}

void On_DHT11() 
{
  Humidity = dht.readHumidity();
  Temperature = dht.readTemperature();         
  Serial.print("Current humidity = ");
  Serial.print(h);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(t); 
  Serial.println("C  ");
  delay(800);
}