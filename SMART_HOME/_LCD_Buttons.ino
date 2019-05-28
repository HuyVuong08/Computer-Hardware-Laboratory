//Nhớ cài thư viện LiquidCrystal_I2C.h vô arduino thì mới chạy đc nha
#include <LiquidCrystal_I2C.h>

#include "General_Resources.hpp"

LiquidCrystal_I2C lcd(0x27, 16, 2);



/*
 *MBut là địa chỉ chân của nút ViewMode
 *SSBut là địa chỉ chân nút Start/Stop
 *SDBut và SUBut là địa chỉ chân nút giảm tốc và tăng tốc băng chuyền 
 *RBut là địa chỉ chân nút Reset
 *CBut là địa chỉ chân nút calibate
 *SpeedPin là địa chỉ của chân điều khiển tốc độ băng chuyền
 */

typedef enum {Bt_NoPress, Bt_1, Bt_2, Bt_3, Bt_4} Button;


bool    isStarted   = false;                   // Start/stop value có giá trị 1(stop) hoặc 2(start)
int     ViewMode    = 0;                       // Có 2 loại ViewMode (1 và 2)


extern int newly_data;                          // Biến Weight lưu khối lượng của trái vừa được cân
extern int n_apples;
extern enum SubState sub_state;

Button whichButPressed ();

void LCD_Button()
{
    switch (whichButPressed())
    {
        case Bt_4:
            Serial.println("Up Arrow pressed");
            SpeedDown();
            break;
        case Bt_3:
            Serial.println("Backward Button pressed");
            Reset();
            break;
        case Bt_2:
            Serial.println("Down Arrow pressed");
            processView(ViewMode);
            break;
        case Bt_1:
            Serial.println("Enter Button pressed");
            
            if (isStarted) {
                prev_state = state;
                state = St_Calibrate;
                sub_state = st_calib_noload;
                
                return;
            }

        default:
            break;
    }

    if (isStarted)
        state = St_Wait;
}



// for testing only
// void LCD_Button() {
//     if (prev_state == St_ReadSensor) {
//         prev_state = state;
//         state = St_ConnectionCheck;
//     }
//     else
//     {
//         state = St_Wait;
//     }
    
// }
// end testing



// Setup ban đầu cho LCD, xuất ra màn hình "Welcome!" 
// Lưu ý địa chỉ bus ic là 0x27 nếu thay đổi địa chỉ ic thì vào LCD_and_Buttons.h sửa lại
void LCD_Button_setup ()
{    
    lcd.begin(16,2);
    lcd.init();
    lcd.backlight();
}

void Greeting() {
    lcd.clear();
    lcd.setCursor(2, 0);            
    lcd.print("SMART_HOME");
    lcd.setCursor(4, 1);            
    lcd.print("WELCOME!");

    setTimer(1);
    startTimer();

    prev_state = St_Greeting;
    state = St_Wait;
}

void Home ()
{
    lcd.clear();
    lcd.setCursor(0,1); 
    lcd.print("HUM: ");
    lcd.setCursor(5,1); 
    lcd.print(Hunmidity);

    lcd.setCursor(7,1); 
    lcd.print("TEM: ");
    lcd.setCursor(12,1); 
    lcd.print(Temperature);
}

void LogIn ()
{
    lcd.clear();
    lcd.setCursor(4,0); 
    lcd.print("LOG IN");
    lcd.setCursor(4,1); 
    lcd.print("SUCCESS");
}

void LogOut ()
{
    lcd.clear();
    lcd.setCursor(4,0); 
    lcd.print("LOCKED");
}

void Register ()
{
    lcd.clear();
    lcd.setCursor(3,0); 
    lcd.print("REGISTER");
    lcd.setCursor(3,1); 
    lcd.print("NEW CARD");
}

void Send_SMS ()
{
    lcd.clear();
    lcd.setCursor(1,0); 
    lcd.print("SEND SMS TO");
    lcd.setCursor(2,1); 
    lcd.print(Phone);
}

void Menu_1_SendSMS ()
{
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("SEND SMS");
    lcd.setCursor(13,1); 
    lcd.print("RG");
}

void Menu_2_Register ()
{
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("REGISTER");

    lcd.setCursor(13,0); 
    lcd.print("SS");

    lcd.setCursor(13,1); 
    lcd.print("LO");
}

void Menu_3_LogOut ()
{
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("LOG OUT");
    lcd.setCursor(13,0); 
    lcd.print("RG");
}

void choice (int a)
{
    if (a == 1)
        ViewMode1 ();
    else
        ViewMode2 ();
}

Button whichButPressed ()
{
    int tmp = analogRead(A0);

    if (tmp >= 415 && tmp <= 490)
        return Bt_4;
    if (tmp >= 570 && tmp <= 630)
        return Bt_3;
    if (tmp >= 840 && tmp <= 890)
        return Bt_2;
    if (tmp == 1024)
        return Bt_1;

    return Bt_NoPress;
}

void Menu_Down (int &a)
{
    if (a == 2)
    {
        a = 0;
    }
    a++;
    choice (a);
}

void Menu_Up (int &a)
{
    if (a == 0)
    {
        a = 2;
    }
    a--;
    choice (a);
}

void printSpeed (int Speed)
{
    lcd.clear();
    lcd.setCursor(3,0);

    switch (Speed)
    {
        case Lv1:
            lcd.print("SPEED: 1/4");
            lcd.setCursor(0,1); 
            lcd.print("####");  
            break;
        case Lv2:
            lcd.print("SPEED: 2/4");
            lcd.setCursor(0,1); 
            lcd.print("########");  
            break;
        case Lv3:
            lcd.print("SPEED: 3/4");
            lcd.setCursor(0,1); 
            lcd.print("############");
            break;
        case Lv4:
            lcd.print("SPEED: 4/4");
            lcd.setCursor(0,1); 
            lcd.print("################");
            break;
    
        default:
            break;
    }

    lcd.setCursor(3,1); 
    lcd.print('|');
    lcd.setCursor(7,1); 
    lcd.print('|');
    lcd.setCursor(11,1); 
    lcd.print('|');
    lcd.setCursor(15,1); 
    lcd.print('|'); 
}


void StartStop ()
{
    isStarted = !isStarted;
    if (isStarted == false)
    {
        lcd.clear();
        lcd.setCursor(4,0); 
        lcd.print("STOPPED");
        lcd.setCursor(3,1); 
        lcd.print('|');
        lcd.setCursor(7,1); 
        lcd.print('|');
        lcd.setCursor(11,1); 
        lcd.print('|');
        lcd.setCursor(15,1); 
        lcd.print('|');
    }
    else
    {
        printSpeed (Speed);
    }
}

void SpeedUp ()
{
    if (isStarted == false)        // check nếu động cơ chưa Start thì ko speed up
    {    
        lcd.clear();
        lcd.setCursor(1,0); 
        lcd.print("Engine Has Not");
        lcd.setCursor(2,1); 
        lcd.print("Started Yet!");
    }
    else
    {
        
        if (Speed == Lv4)
        {
            lcd.clear();
            lcd.setCursor(1,0); 
            lcd.print("Top-SPEED: 4/4");
            lcd.setCursor(0,1); 
            lcd.print("################");
            lcd.setCursor(3,1); 
            lcd.print('|');
            lcd.setCursor(7,1); 
            lcd.print('|');
            lcd.setCursor(11,1); 
            lcd.print('|');
            lcd.setCursor(15,1); 
            lcd.print('|');
        }
        else
        {
            if (Speed == Lv1)
            {
            Speed = Lv2;
            }
            else if (Speed == Lv2)
            {
            Speed = Lv3;
            }
            else if (Speed == Lv3)
            {
            Speed = Lv4;
            }
            printSpeed (Speed);        
        }
    }
}

void SpeedDown ()
{ 
    if (isStarted == false)        // check nếu động cơ chưa Start thì ko speed down
    {    
        lcd.clear();
        lcd.setCursor(1,0); 
        lcd.print("Engine Has Not");
        lcd.setCursor(2,1); 
        lcd.print("Started Yet!");
    }
    else
    {
        
        if (Speed == Lv1)
        {
            lcd.clear();
            lcd.setCursor(1,0); 
            lcd.print("Min-SPEED: 1/4");
            lcd.setCursor(0,1); 
            lcd.print("####");
            lcd.setCursor(3,1); 
            lcd.print('|');
            lcd.setCursor(7,1); 
            lcd.print('|');
            lcd.setCursor(11,1); 
            lcd.print('|');
            lcd.setCursor(15,1); 
            lcd.print('|');
        }
        else
        {
            if (Speed == Lv4)
            {
            Speed = Lv3;
            }
            else if (Speed == Lv3)
            {
            Speed = Lv2;
            }
            else if (Speed == Lv2)
            {
            Speed = Lv1;
            }
            printSpeed (Speed);        
        }
    }
}

void Reset ()
{
    n_apples = 0;

    lcd.clear();
    lcd.setCursor(6,0); 
    lcd.print("Reset");
    lcd.setCursor(0,1); 
    lcd.print("Count:");
    lcd.setCursor(7,1); 
    lcd.print(n_apples);
    lcd.setCursor(11,1); 
    lcd.print("Unit");
}
