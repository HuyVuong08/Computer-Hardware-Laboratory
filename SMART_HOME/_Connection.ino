#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


const char *ssid             = "AndroidAP";    
const char *password         = "nguyen12185";

const char *ThingSpeakServer = "api.thingspeak.com";
const int  ID                = 797869;                            // ThingSpeak Channel ID
String     writeAPIKey       = "VG5AXCGU18N6LBIQ";                // ThingSpeak API


WiFiClient client;


// extern variable
extern float  Humidity;

// this variable is for controlling sending data stored in EEPROM
// this variable reveals that how many numbers in EEPROM are sent
int     posInMem    = 0;

/**
 *  Send to ThingSpeak: send newly weighted data and the ones stored in EEPROM if exist
 */ 
void Send()
{
    switch (sub_state) 
    {
        case st_send_new: 
        {
            if (isDataInEEPR()) 
            {
                sub_state = st_send_inMem;

                Serial.print("Speed = "); Serial.println((int)Speed);
                setTimer((int)Speed);
                startTimer();
                sub_state = st_wait;
            }
            else
                sub_state = st_send_done;
            break;
        }
        case st_send_inMem: 
        {
            int tmp = getSavedData();

            if (tmp != -1)
            {
                if (client.connect(ThingSpeakServer,80)) 
                {
                    Serial.println("Send data from EEPROM...");
                    sendToThingSpeak(tmp);

                    client.stop();

                    Serial.print("Speed = "); Serial.println((int)Speed);
                    setTimer((int)Speed);
                    startTimer();
                    sub_state = st_wait;
                }
                else 
                {
                    Serial.println("Data is still in EEPROM but the connection has terminated.");
                }
            }
            else 
            {
                sub_state = st_send_done;
            }
            break;
        }
        case st_wait: 
        {

            if (isTimeOut())
                sub_state = st_send_inMem;
            break;
        }
        case st_send_done: 
        {
            // send Humidity data
            sendToThingSpeak(Humidity);
            client.stop();
            
            Serial.print("Speed = "); Serial.println((int)Speed);
            
            setTimer((int)Speed);
            startTimer();
            state = St_Wait;
        }
    }    
}


void    Connection_setup() 
{    
    WiFi.mode(WIFI_OFF);
    delay(1000);
    WiFi.begin(ssid, password);
}

void    ConnectionCheck() 
{
    if (prev_state == St_Unlock) 
    {
        state = St_ReadSensor;
        sub_state = st_readSensor;
    }
    else 
    { // prev_state == St_LCDButton
        if (!(WiFi.status() == WL_CONNECTED && client.connect(ThingSpeakServer,80)))
            state = St_SaveMem;        
        else 
        {
            // connection establised successfully
            Serial.println(" * Conenction: OK");
        
            state = St_Send;
            sub_state = st_send_new;
        }
    }
}


// ******************** private functions *********************
/**
 * 
 */
void    sendToThingSpeak(int data) 
{
    String postStr = writeAPIKey;
    postStr +="&field1=";
    postStr += String(data);
    postStr += "\r\n\r\n";
        
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Sent to server: "); Serial.println(data);
}