#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11

#define dht_dpin D4
DHT dht(dht_dpin, DHTTYPE); 

void setup()
{ 
  dht.begin();
  Serial.begin(9600);
  Serial.println("Humidity and temperature \n\n");
  Serial.println("==========================================");
  delay(700);
}
void loop() {
    float Humidity = dht.readHumidity();
    float Temperature = dht.readTemperature();         
    Serial.print("Current humidity = ");
    Serial.print(Humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(Temperature); 
    Serial.println("C  ");
  delay(800);
}
