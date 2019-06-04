#include <Sim800l.h>

#include <Bounce2.h>

#include <SoftwareSerial.h> //is necesary for the library!!

Sim800l Sim800l; //to declare the library

char* closedText;

char * openText;

char* number;

bool error; //to catch the response of sendSms

#define SWITCH_PIN D8 // Connect switch pin here



Bounce debouncer = Bounce();

int currentState = -1;

int lastState = -1;



void setup(){
    SIM800L_Setup ();
}



void loop(){
    On_SIM80L ();
}

void SIM800L_Setup ()
{
    Serial.begin(9600);

    Sim800l.begin(); // initializate the library.

    closedText="Door has been closed"; //text for the message.
    openText="Door has been opened"; //text for the message.

    number="0933792267"; //sms phone number

    pinMode(SWITCH_PIN, INPUT_PULLUP);

    debouncer.attach(SWITCH_PIN);
    debouncer.interval(100);

    //digitalWrite(BUTTON_PIN, HIGH);

}

void On_SIM80L ()
{
    debouncer.update();

    currentState = debouncer.read();

    if (currentState != lastState)
    {
        if(currentState == HIGH)
        {
            Serial.println("open");
            error=Sim800l.sendSms(number,openText);
        }
        else
        {
            error=Sim800l.sendSms(number,closedText);
            Serial.println("closed");
        }
        lastState = currentState;
    }
}
