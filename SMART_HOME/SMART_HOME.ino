// there are 2 state machines concurrently existing. 
// The outer one loops among Big States
// the inner one loops inside each Big state

#include "General_Resources.hpp"

#include <Ticker.h>

Ticker sTick;

volatile int wd_count = 0;

void setup() 
{
    Serial.begin(57600);
  
    Serial.println("Start setting up");
    // set up components
    RFIF_Setup ();
    Serial.println("RFID set up done");    

    SIM800L_Setup ();
    Serial.println("SIM800L set up done");    

    EEPROM_setup();
    Serial.println("EEPROM set up done");  

    Connection_setup();
    Serial.println("Connection set up done");

    LCD_Button_setup();
    Serial.println("LCD and Buttons set up done");

    sTick.attach(1,ISRWatchdog);
    //Setting up watchdog ISR
    Serial.println("Watchdog set up done");
    Serial.println("All done");


    // init state
    state       = St_Greeting;
    sub_state   = st_calib_noload;
    
    // set up watchdog 1s
    // 5ph k can --> sleep
}


void loop() 
{
/*
    Serial.println(state);
    Serial.println(sub_state);
    Serial.println(prev_state);
*/
    
    wd_count = 0;
    //reset the watchdog timer
    

    switch (state) {
        case St_Greeting: 
        {
            Serial.println("on St_Greeting");
            On_Greeting();
            break;
        }
        case St_RFID_Unlock: 
        {
            Serial.println("on St_RFID_Unlock");
            On_RFID_LogInAndLogOut ();
            break;
        }
        case St_ConnectionCheck: 
        {
            //Serial.println("on St_ConCheck");
            ConnectionCheck();
            break;
        }
        case St_ReadSensor: 
        {
            // Serial.println("on ReadSensor");
            ReadSensor();
            break;
        }
        case St_Wait: 
        {
            // Serial.println("on Wait");
            Wait();
            break;
        }
        case St_LCD_Button: 
        {
            Serial.println("on LCD");
            LCD_Button();
            break;
        }
        case St_Send: 
        {
            // Serial.println("on Send");
            Send();
            break;
        }
        case St_SaveMem: 
        {
            // Serial.println("on SaveMem");
            SaveMem();
            break;
        }
        case St_SendSMS:
        {
            // Serial.println("on SaveMem");
            On_SIM800L_SendSMS ();
            break;
        }
    }

    delay(10);
}