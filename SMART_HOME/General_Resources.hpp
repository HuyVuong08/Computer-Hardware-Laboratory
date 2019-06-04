#ifndef GENERAL_RESOURCES_HPP_
#define GENERAL_RESOURCES_HPP_

float Humidity;
float Temperature;

bool LoggedIn = false;

// for states
enum State {St_Greeting, St_RFID_Unlock, St_ConnectionCheck, St_ReadSensor, St_Wait, St_LCD_Button,
            St_Send,  St_SaveMem};
enum State   state, prev_state;

enum SubState {st_calib_noload, st_calib, st_wait,          // for Calibrate state
               st_readSensor,                               // for ReadSensor state
               st_send_new, st_send_inMem, st_send_done};   // for Send state

enum SubState sub_state;

// for Change level

enum SpeedLv {Lv1 = 15, Lv2 = 20, Lv3 = 25, Lv4 = 30};
enum SpeedLv Speed = Lv1;


#endif
