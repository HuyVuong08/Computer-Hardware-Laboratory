#include <SoftwareSerial.h>

////Create software serial object to communicate with SIM800L
//SoftwareSerial mySerial(D1, D2); //SIM800L Tx & Rx is connected to Arduino #3 & #2
//
//void updateSerial()
//{
//  delay(500);
//  while (Serial.available()) 
//  {
//    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
//  }
//  while(mySerial.available()) 
//  {
//    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
//  }
//}
//
//void setup()
//{
//  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
//  Serial.begin(9600);
//  
//  //Begin serial communication with Arduino and SIM800L
//  mySerial.begin(9600);
//
//  Serial.println("Initializing...");
//  delay(1000);
//
//  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
//  updateSerial();
//  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
//  updateSerial();
//  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
//  updateSerial();
//  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
//  updateSerial();
//
//  Serial.println("Initializing...");
//}
//
//void loop()
//{
//  updateSerial();
//}



#include <SoftwareSerial.h>
float num = 6.00;
SoftwareSerial sim(D1, D2);
void setup() {
  Serial.begin(115200);
    sim.begin(9600);
    sim.print("AT+CMGF=1\r\n");
    delay(100);
    
    sim.print("AT+CMGS=\"084933792267\"\r\n");
    delay(100);
    String s = String(num);
    Serial.print(s);
    sim.print(s+"\r\n");
    delay(100);
    sim.write(26);
    delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:

}



//#include <SoftwareSerial.h>
//SoftwareSerial sim(D1, D2);
//int _timeout;
//String _buffer;
//String number = "+84933792267"; //-> change with your number
//void setup() {
//    delay(7000); //delay for 7 seconds to make sure the modules get the signal
//    Serial.begin(9600);
//    _buffer.reserve(50);
//    Serial.println("Sistem Started...");
//    sim.begin(9600);
//    delay(1000);
//    Serial.println("Type s to send an SMS, r to receive an SMS, and c to make a call");
//
//    // Serial.begin (9600);
//    // delay (1000);
//    // Serial.println ("ATD84933792267;" );
//    // delay (10000);
//    // Serial.println ("AHT");
//}
//void loop() {
//    // if (Serial.available() > 0)
//    // switch (Serial.read())
//    // {
//    //     case 's':
//    //     SendMessage();
//    //     break;
//    //     case 'r':
//    //     RecieveMessage();
//    //     break;
//    //     case 'c':
//    //     callNumber();
//    //     break;
//    // }
//    // if (sim.available() > 0)
//    // Serial.write(sim.read());
//    callNumber();
//}
//void SendMessage()
//{
//    //Serial.println ("Sending Message");
//    sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
//    delay(1000);
//    //Serial.println ("Set SMS Number");
//    sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
//    delay(1000);
//    String SMS = "Hello, how are you?";
//    sim.println(SMS);
//    delay(100);
//    sim.println((char)26);// ASCII code of CTRL+Z
//    delay(1000);
//    _buffer = _readSerial();
//}
//void RecieveMessage()
//{
//    Serial.println ("SIM800L Read an SMS");
//    delay (1000);
//    sim.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
//    delay(1000);
//    Serial.write ("Unread Message done");
//}
//String _readSerial() {
//    _timeout = 0;
//    while  (!sim.available() && _timeout < 12000  )
//    {
//    delay(13);
//    _timeout++;
//    }
//    if (sim.available()) {
//    return sim.readString();
//    }
//}
//void callNumber() {
//    sim.println ("ATD84933792267;" );
////    Serial.print ("a");
////    sim.print (F("ATD"));
////    sim.print (number);
////    sim.print (F(";\r\n"));
////    _buffer = _readSerial();
////    Serial.println(_buffer);
//}
