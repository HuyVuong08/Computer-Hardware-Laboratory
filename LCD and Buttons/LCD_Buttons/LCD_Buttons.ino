//Nhớ cài thư viện LiquidCrystal_I2C.h vô arduino thì mới chạy đc nha
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);



/*
 *MBut là địa chỉ chân của nút ViewMode
 *SSBut là địa chỉ chân nút Start/Stop
 *SDBut và SUBut là địa chỉ chân nút giảm tốc và tăng tốc băng chuyền 
 *RBut là địa chỉ chân nút Reset
 *CBut là địa chỉ chân nút calibate
 *SpeedPin là địa chỉ của chân điều khiển tốc độ băng chuyền
 */

void setup ()
{
    void LCD_Button_setup ();
    Serial.begin (9600);
}

void loop ()
{
    int tmp = analogRead(A0);
    Serial.println (tmp);
    delay(1000);
}
