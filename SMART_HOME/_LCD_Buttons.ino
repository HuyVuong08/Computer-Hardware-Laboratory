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
typedef enum {Menu_Off, Menu_LV1, Menu_Chosen} Menu;
typedef enum {Menu_Lv1_1, Menu_Lv1_2, Menu_Lv1_3} Menu_Lv1;


extern bool LoggedIn;
extern float Humidity;
extern float Temperature;
extern enum SubState sub_state;

Button whichButPressed ();

// Setup ban đầu cho LCD, xuất ra màn hình "Welcome!" 
void LCD_Button_setup ()
{   
    lcd.begin(16,2);
    lcd.init();
    lcd.backlight();
}

void LCD_Button()
{
    if (LoggedIn == false)
    {
        Locked ();
        state = St_Wait;
        return;
    }

    switch (whichButPressed())
    {
        case Bt_4:
            Serial.println("Up Arrow pressed");
            Up_Button ();
            break;

        case Bt_3:
            Serial.println("Backward Button pressed");
            Backward_Button ();
            break;

        case Bt_2:
            Serial.println("Down Arrow pressed");
            Down_Button ();
            break;

        case Bt_1:
            Serial.println("Enter Button pressed");
            Enter_Button();
            if (isStarted) {
                prev_state = state;
                state = St_Calibrate;
                sub_state = st_calib_noload;
                
                return;
            }

        default:
            break;
    }

    if (LoggedIn)
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

void Enter_Button ()
{
    switch (Menu)
    {
    case Menu_Off:
        Menu     = Menu_LV1;
        Menu_Lv1 = Menu_Lv1_1;
        Display_Menu_Lv1 ();
        break;

    case Menu_LV1: 
        Menu = Menu_Chosen;

    default:
        break;
    }
}

void Backward_Button ()
{
    switch (Menu)
    {
    case Menu_Chosen:
        Menu = Menu_LV1;
        Display_Menu_Lv1 ();
        break;

    case Menu_LV1:
        Menu = Menu_Off;

    default:
        break;
    }
}

void Down_Button ()
{
    if (Menu == Menu_Off || Menu == Menu_Chosen)
        return;

    switch (Menu_Lv1)
    {
    case Menu_Lv1_1:
        Menu_Lv1 == Menu_Lv1_2;
        break;
    
    case Menu_Lv1_2:
        Menu_Lv1 == Menu_Lv1_3;
        break;
    
    case Menu_Lv1_3:
        Menu_Lv1 == Menu_Lv1_1;

    default:
        break;
    }

    Display_Menu_Lv1 ();
}

void Up_Button ()
{
    if (Menu == Menu_Off || Menu == Menu_Chosen)
        return;

    switch (Menu_Lv1)
    {
    case Menu_Lv1_3:
        Menu_Lv1 == Menu_Lv1_2;
        break;
    
    case Menu_Lv1_2:
        Menu_Lv1 == Menu_Lv1_1;
        break;

    case Menu_Lv1_1:
        Menu_Lv1 == Menu_Lv1_3;

    default:
        break;
    }

    Display_Menu_Lv1 ();
}

void On_Greeting() {
    lcd.clear();
    lcd.setCursor(3, 0);            
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

void Locked ()
{
    lcd.clear();
    lcd.setCursor(5,0); 
    lcd.print("PLEASE");
    lcd.setCursor(5,1); 
    lcd.print("LOG IN");
}

void LogIn ()
{
    lcd.clear();
    lcd.setCursor(5,0); 
    lcd.print("LOG IN");
    lcd.setCursor(3,1); 
    lcd.print("SUCCESSFUL");
}

void Menu_Chosen_SendSMS ()
{
    lcd.clear();
    lcd.setCursor(2,0); 
    lcd.print("SEND SMS TO");
    lcd.setCursor(2,1); 
    lcd.print(Phone);
}

void Menu_Lv1_SendSMS ()
{
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("SEND SMS");
    lcd.setCursor(13,1); 
    lcd.print("RG");
}

void Menu_Chosen_Register ()
{
    lcd.clear();
    lcd.setCursor(4,0); 
    lcd.print("REGISTER");
    lcd.setCursor(4,1); 
    lcd.print("NEW CARD");
}

void Menu_Lv1_Register ()
{
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("REGISTER");

    lcd.setCursor(13,0); 
    lcd.print("SS");

    lcd.setCursor(13,1); 
    lcd.print("LO");
}

void Menu_Chosen_LogOut ()
{
    lcd.clear();
    lcd.setCursor(2,0); 
    lcd.print("LOGGED OUT");
    lcd.setCursor(1,0); 
    lcd.print("SYSTEM LOCKED");
}

void Menu_Lv1_LogOut ()
{
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("LOG OUT");
    lcd.setCursor(13,0); 
    lcd.print("RG");
}

void Display_Menu_Lv1 ()
{
    if (Menu != Menu_LV1)
        return;
    
    switch (Menu_Lv1)
    {
    case Menu_Lv1_1:
        Menu_Lv1_SendSMS ();
        break;
    
    case Menu_Lv1_2:
        Menu_Lv1_Register ();
        break;

    case Menu_Lv1_3:
        Menu_Lv1_LogOut ();

    default:
        break;
    }
}

void Display_Menu_Chosen ()
{
    if (Menu != Menu_Chosen)
        return;

    switch (Menu_Lv1)
    {
    case Menu_Lv1_1:
        Menu_Chosen_SendSMS ();
        break;
    
    case Menu_Lv1_2:
        Menu_Chosen_Register ();
        break;

    case Menu_Lv1_3:
        Menu_Chosen_LogOut ();

    default:
        break;
    }
}