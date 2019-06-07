#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11

#define dht_dpin D4
DHT dht(dht_dpin, DHTTYPE); 

extern float Humidity;
extern float Temperature;

void DHT11_Setup()
{ 
  dht.begin();
}

void On_DHT11() 
{
    switch (sub_state)
    {
        case st_readSensor:
    
            if (dht.read())
            {
                Humidity = dht.readHumidity();
                Temperature = dht.readTemperature();         

                prev_state = state;
                state = St_ConnectionCheck;
            }
            else 
            {
                Serial.println("DHT11 not found. Try again...");
                
                sub_state = st_wait; 
                setTimer(1);
                startTimer();
            }
            break;

        case st_wait: 
            if (isTimeOut())
                sub_state = st_readSensor;
            break;
    }
}

