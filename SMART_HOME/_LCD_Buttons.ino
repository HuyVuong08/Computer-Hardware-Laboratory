//Nhớ cài thư viện LiquidCrystal_I2C.h vô arduino thì mới chạy đc nha
#include <LiquidCrystal_I2C.h>

#include "General_Resources.hpp"

LiquidCrystal_I2C lcd(0x27, 16, 2);

/*
 *MBut là địa chỉ chân của nút Menu
 *SSBut là địa chỉ chân nút Start/Stop
 *SDBut và SUBut là địa chỉ chân nút giảm tốc và tăng tốc băng chuyền 
 *RBut là địa chỉ chân nút Reset
 *CBut là địa chỉ chân nút calibate
 *SpeedPin là địa chỉ của chân điều khiển tốc độ băng chuyền
 */

typedef enum {Bt_NoPress, Bt_1, Bt_2, Bt_3, Bt_4} Button;
typedef enum {Menu_Off, Menu_LV1} Menu;
typedef enum {Menu_Lv1_1, Menu_Lv1_2, Menu_Lv1_3} Menu_Lv1;


extern float Humidity;
extern float Temperature;
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
            processView(Menu);
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

void Enter_Button ()
{
    Menu     = Menu_LV1;
    Menu_Lv1 = Menu_Lv1_1;
    return;
}

void Backward_Button ()
{
    Menu = Menu_Off;
    return;
}

void Down_Button ()
{
    if (Menu_Lv1 == Me)
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
    lcd.print(Humidity);

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
        Menu1 ();
    else
        Menu2 ();
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

void Choose_Menu_Lv1 ()
{
    if (Menu != Menu_LV1)
        return;
    
    if (Menu_Lv1 == Menu_Lv1_1)
    
    else if (Menu_Lv1 == Menu_Lv1_2)
}
