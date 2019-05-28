#include "DHT.h"

const int DHTPIN = 2;

const int DHTTYPE = DHT11;

DHT = dht(DHTPIN, DHTTYPE);


void setup(){
  Serial.begin(9600);
  dht begin()
}
void loop(){

  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
  Serial.print("Temperature = ");
  Serial.println(t);
  Serial.print("Humidity = ");
  Serial.println(h);
  delay(1000);
}
