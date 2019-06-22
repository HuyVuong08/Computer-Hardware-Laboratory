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
int prev = 0;
String Phone = "084933792267";
bool Start = false;


typedef enum {Bt_NoPress, Bt_Enter, Bt_Down, Bt_Backward, Bt_Up, Bt_Dublicate} Button;
enum Menu {Menu_Off, Menu_LV1, Menu_Chosen};
enum Menu_Lv1 {Menu_Lv1_1, Menu_Lv1_2, Menu_Lv1_3, Menu_Lv1_4, Menu_Lv1_5};


enum Menu     Menu = Menu_Off;
enum Menu_Lv1 Menu_Lv1 = Menu_Lv1_1;


extern int   NumberOfCards;
extern bool  LoggedIn;
extern bool  PasswordMatched;
extern float Humidity;
extern float Temperature;
extern enum  State    state, prev_state;
extern enum  SubState sub_state;


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
    switch (prev_state)
    {
    case St_Unlock:
        if (PasswordMatched == true && LoggedIn == true)
            LogIn_Success ();
        else if (PasswordMatched == true && LoggedIn == false)
            Menu_Chosen_LogOut ();
        else      
            LogIn_Fail ();
        Menu = Menu_Off;
        break;

    case St_Register:
        Register_Success ();
        Menu = Menu_Off;
        break;

    case St_SendSMS:
        SMS_Sent ();
        Menu = Menu_Off;
        break;

    default:
        break;
    }
    

    switch (whichButPressed())
    {
        case Bt_Up:
            Serial.println("Bt_Up Arrow pressed");
            Up_Button ();
            break;

        case Bt_Backward:
            Serial.println("Bt_Backward Button pressed");
            Backward_Button ();
            break;

        case Bt_Down:
            Serial.println("Bt_Down Arrow pressed");
            Down_Button ();
            break;

        case Bt_Enter:
            Serial.println("Bt_Enter Button pressed");
            Enter_Button();

        default:
            break;
    }

    if (Start == false)
        state = St_LCD_Button;
    else if (Menu == Menu_Off)
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

//*****************************************************************************************//

Button whichButPressed ()
{
    int tmp = analogRead(A0);
    if (prev > 40)
    {
        prev = tmp;
        return Bt_Dublicate;
    }
 
    if (tmp >= 70 && tmp <= 80)
    {
        prev = tmp;
        return Bt_Up;
    }
    if (tmp >= 100 && tmp <= 110)
    {        
        prev = tmp;
        return Bt_Backward;
    } 
    if (tmp >= 190 && tmp <= 200)
    {       
        prev = tmp;
        return Bt_Down;  
    }    
    if (tmp == 1024)
    {        
        prev = tmp;
        return Bt_Enter;
    }

    prev = tmp;
    return Bt_NoPress;
}

//*****************************************************************************************//

void Enter_Button ()
{
    if (Start == false)
    {
        Start = true;
        return;
    }    
    else if (LoggedIn == false)
    {
        Locked ();
        return;
    }   

    switch (Menu)
    {
        case Menu_Off:
        {
            Menu     = Menu_LV1;
            Menu_Lv1 = Menu_Lv1_1;
            Display_Menu_Lv1 ();
            break;
        }
        case Menu_LV1: 
        {
            Menu = Menu_Chosen;
            Display_Menu_Chosen ();
            break;
        }
        case Menu_Chosen: 
        {
            Menu = Menu_Off;
            Home ();
            break;
        }

        default:
            break;
    }
}

void Backward_Button ()
{
    if (Start == false)
    {
        Start = true;
        return;
    }

    if (LoggedIn == false)
    {
        Locked ();
        return;
    }   

    switch (Menu)
    {
        case Menu_Chosen:
            Menu = Menu_LV1;
            Display_Menu_Lv1 ();
            break;

        case Menu_LV1:
            Menu = Menu_Off;
            Home ();

        default:
            break;
    }
}

//*****************************************************************************************//


void Down_Button ()
{
    if (Start == false)
    {
        Start = true;
        return;
    }

    if (LoggedIn == false)
    {
        Locked ();
        return;
    }   

    if (Menu != Menu_LV1)
        return;

    switch (Menu_Lv1)
    {
        case Menu_Lv1_1:
            Menu_Lv1 = Menu_Lv1_2;
            break;
        
        case Menu_Lv1_2:
            Menu_Lv1 = Menu_Lv1_3;
            break;

        case Menu_Lv1_3:
            Menu_Lv1 = Menu_Lv1_4;
            break;

        case Menu_Lv1_4:
            Menu_Lv1 = Menu_Lv1_5;
            break;

        case Menu_Lv1_5:
            Menu_Lv1 = Menu_Lv1_1;
            break;

        default:
            break;
    }

    Display_Menu_Lv1 ();
}

void Up_Button ()
{
    if (Start == false)
    {
        Start = true;
        return;
    }

    if (LoggedIn == false)
    {
        Locked ();
        return;
    }   

    if (Menu == Menu_Off || Menu == Menu_Chosen)
        return;

    switch (Menu_Lv1)
    {
        case Menu_Lv1_5:
            Menu_Lv1 = Menu_Lv1_4;       
            break;

        case Menu_Lv1_4:
            Menu_Lv1 = Menu_Lv1_3;       
            break;

        case Menu_Lv1_3:
            Menu_Lv1 = Menu_Lv1_2;       
            break;
        
        case Menu_Lv1_2:
            Menu_Lv1 = Menu_Lv1_1;
            break;

        case Menu_Lv1_1:
            Menu_Lv1 = Menu_Lv1_5;
            break;

        default:
            break;
    }

    Display_Menu_Lv1 ();
}

//*****************************************************************************************//

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
    lcd.setCursor(0,0); 
    lcd.print("TEM: ");
    lcd.setCursor(5,0); 
    lcd.print(Temperature);
    lcd.setCursor(10,0); 
    lcd.print('C');

    lcd.setCursor(0,1); 
    lcd.print("HUM: ");
    lcd.setCursor(5,1); 
    lcd.print(Humidity);
    lcd.setCursor(10,1); 
    lcd.print('%');
}

//*****************************************************************************************//

void Locked ()
{
    lcd.clear();
    lcd.setCursor(4,0); 
    lcd.print("PLEASE");
    lcd.setCursor(4,1); 
    lcd.print("LOG IN");
}

void LogIn_Success ()
{
    lcd.clear();
    lcd.setCursor(5,0); 
    lcd.print("ACCESS");
    lcd.setCursor(5,1); 
    lcd.print("GRANTED");
}

void LogIn_Fail ()
{
    lcd.clear();
    lcd.setCursor(5,0); 
    lcd.print("REQUEST");
    lcd.setCursor(5,1); 
    lcd.print("DENIED");
}

//*****************************************************************************************//

void SMS_Sent ()
{
    lcd.clear();
    lcd.setCursor(2,0); 
    lcd.print("SMS SENT TO");
    lcd.setCursor(2,1); 
    lcd.print(Phone);
}

void Menu_Chosen_SendSMS ()
{
    lcd.clear();
    lcd.setCursor(3,0); 
    lcd.print("SENDING TO");
    lcd.setCursor(2,1); 
    lcd.print(Phone);
}

void Menu_Lv1_SendSMS ()
{
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("SEND SMS");

    lcd.setCursor(13,0); 
    lcd.print("LO");

    lcd.setCursor(13,1); 
    lcd.print("RE");
}

//*****************************************************************************************//

void Register_Authorized_Card ()
{
    lcd.clear();
    lcd.setCursor(3,0); 
    lcd.print("AUTHORIZED");
    lcd.setCursor(6,1); 
    lcd.print("CARD");
}

void Register_Success ()
{
    lcd.clear();
    lcd.setCursor(1,0); 
    lcd.print("AUTHORIZATION");
    lcd.setCursor(4,1); 
    lcd.print("COMPLETE");
}

void Menu_Chosen_Register ()
{
    lcd.clear();
    lcd.setCursor(4,0); 
    lcd.print("SCANNING");
    lcd.setCursor(4,1); 
    lcd.print("NEW CARD");
}

void Menu_Lv1_Register ()
{
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("REGISTER");

    lcd.setCursor(13,0); 
    lcd.print("SE");

    lcd.setCursor(13,1); 
    lcd.print("DI");
}

//*****************************************************************************************//

void Disprove_Password_Not_Matched ()
{
    lcd.clear();
    lcd.setCursor(2,0); 
    lcd.print("UNAUTHORIZED");
    lcd.setCursor(6,1); 
    lcd.print("CARD");
}

void Disprove_Insufficent_Cards ()
{
    lcd.clear();
    lcd.setCursor(3,0); 
    lcd.print("AUTHORIZED");
    lcd.setCursor(6,1); 
    lcd.print("CARD");
}

void Disprove_Success ()
{
    lcd.clear();
    lcd.setCursor(4,0); 
    lcd.print("DISPROVAL");
    lcd.setCursor(4,1); 
    lcd.print("COMPLETE");
}

void Menu_Chosen_Disprove ()
{
    lcd.clear();
    lcd.setCursor(2,0); 
    lcd.print("DISPROVING");
    lcd.setCursor(1,0); 
    lcd.print("CARD");
}

void Menu_Lv1_Disprove ()
{
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("DISPROVE");

    lcd.setCursor(13,0); 
    lcd.print("RE");

    lcd.setCursor(13,1); 
    lcd.print("NO");
}

//*****************************************************************************************//

void Menu_Chosen_NumberOfCards ()
{
    lcd.clear();
    lcd.setCursor(1,0); 
    lcd.print("AUTHORIZED");
    lcd.setCursor(1,1); 
    lcd.print("CARDS: ");

    lcd.setCursor(10,1);
    lcd.print(NumberOfCards);
}

void Menu_Lv1_NumberOfCards ()
{
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("N.O CARDS");

    lcd.setCursor(13,0); 
    lcd.print("DI");

    lcd.setCursor(13,1); 
    lcd.print("LO");
}

//*****************************************************************************************//

void Menu_Chosen_LogOut ()
{
    lcd.clear();
    lcd.setCursor(2,0); 
    lcd.print("LOGGED OUT");
    lcd.setCursor(1,1); 
    lcd.print("SYSTEM LOCKED");
}

void Menu_Lv1_LogOut ()
{
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("LOG OUT");

    lcd.setCursor(13,0); 
    lcd.print("RE");

    lcd.setCursor(13,1); 
    lcd.print("SE");
}

//*****************************************************************************************//

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
            Menu_Lv1_Disprove ();
            break;

        case Menu_Lv1_4:
            Menu_Lv1_NumberOfCards ();
            break;

        case Menu_Lv1_5:
            Menu_Lv1_LogOut ();
            break;

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
        {
            Menu_Chosen_SendSMS ();
            prev_state = St_LCD_Button;
            state = St_SendSMS;
            break;
        }
        case Menu_Lv1_2:
        {
            Menu_Chosen_Register ();
            prev_state = St_LCD_Button;
            state = St_Register;
            break;
        }
        case Menu_Lv1_3:
        {           
            Menu_Chosen_Disprove ();
            prev_state = St_LCD_Button;
            state = St_Disprove;
            break;
        }
        case Menu_Lv1_4:
        {           
            Menu_Chosen_NumberOfCards ();
            prev_state = St_LCD_Button;
            state = St_Wait;
            break;
        }
        case Menu_Lv1_5:
        {            
            Menu_Lv1_LogOut ();
            LoggedIn = false;
            state = St_Unlock;
            break;
        }
        default:
            break;
    }
}
