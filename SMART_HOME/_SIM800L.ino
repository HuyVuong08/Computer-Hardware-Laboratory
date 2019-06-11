#include <SoftwareSerial.h>

extern float Humidity;
extern enum State    state, prev_state;
extern enum SubState sub_state;

SoftwareSerial sim(D9, D10);

void SIM800L_Setup ()
{
    sim.begin(9600);
}

void On_SIM800L_SendSMS ()
{
    sim.print("AT+CMGF=1\r\n");
    delay(100);

    sim.print("AT+CMGS=\"084933792267\"\r\n");
    delay(100);

    String Humidity_string = String(Humidity);
    Serial.print(Humidity_string);
    sim.print(Humidity_string+"\r\n");

    delay(100);
    sim.write(26);
    delay(100);

    prev_state = St_SendSMS;
    state = St_Wait;
}
