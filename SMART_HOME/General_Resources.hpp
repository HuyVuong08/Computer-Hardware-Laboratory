#ifndef GENERAL_RESOURCES_HPP_
#define GENERAL_RESOURCES_HPP_

float Humidity;
float Temperature;

int  NumberOfCards   = 1;
bool LoggedIn        = false;
bool PasswordMatched = false;

// for states
enum State {St_Greeting, 
            St_Unlock, 
            St_Register, 
            St_Disprove, 
            St_ConnectionCheck, 
            St_ReadSensor, 
            St_Wait, 
            St_LCD_Button, 
            St_Send,  
            St_SaveMem, 
            St_SendSMS};

enum State   state, prev_state;

enum SubState { st_idle,
                st_wait,          
                st_readSensor,                               // for ReadSensor state
                st_send_new, st_send_inMem, st_send_done};   // for Send state

enum SubState sub_state;

// for Change level

enum SpeedLv {Lv1 = 15, Lv2 = 20, Lv3 = 25, Lv4 = 30};
enum SpeedLv Speed = Lv1;


#endif
