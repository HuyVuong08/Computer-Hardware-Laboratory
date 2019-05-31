#include "HX711.h"

#include <math.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN  = 14;

HX711 scale;


extern int          newly_data;
extern int          n_apples;


void    LoadCell_setup() {
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}


/**
 *  Calibrate LoadCell
 */
void    Calibrate() {
    if (!isCalibrated() || prev_state == St_LCD_Button) {
        switch (sub_state) {
            case st_calib_noload: {
                // print a message to LCD
                printCalib_doing();


                // calculate offset as no load
                scale.set_scale();
                scale.tare();

                // require engineer to put already known weight load onto the load cell
                Serial.println("Please put the smartphone onto the sensor.");
                Serial.println("The process will start within 10 seconds");


                sub_state = st_wait;
                setTimer(10);
                startTimer();
                break;
            }
            case st_wait: {
                if (isTimeOut()) {
                    sub_state = st_calib;
                }
                break;
            }
            case st_calib: {
                Serial.println("Start.");
                Serial.print("Please type the weight of the object: ");
                int weight = Serial.readStringUntil('\n').toInt();
                Serial.println(weight);

                for (int i = 0; i < 15; i++) {
                    float tmp = scale.get_units(10);
                    scale.set_scale(tmp / weight);
                }

                // write to EEPROM
                
                // Testing
                Serial.print("Scale before set: "); Serial.println(scale.get_scale());                
                // End Testing


                eeWriteCalibrate(scale.get_scale());

                // notify onto screen
                Serial.print("Calibrating complete. The weight: ");
                Serial.println(scale.get_units(5));

                // modify program flow
                if (prev_state == St_LCD_Button)
                    state = St_Wait;
                else  // prev_state == St_Greeting
                    state = St_LCD_Button;

                // print a message to LCD
                printCalib_done();
                break;
            }
        }
    }
    else {   //isCalibrated() == true

        // Testing
        Serial.print("Get from EEPROM: "); Serial.println(eeGetCalibrate());
        // End Testing



        scale.set_scale(eeGetCalibrate());

        prev_state = state;
        state = St_ConnectionCheck;

        // print a message to LCD
        printCalib_done();


        // modify program flow
        if (prev_state == St_LCD_Button)
            state = St_Wait;
        else  // prev_state == St_Greeting
            state = St_LCD_Button;
    }

}

/**
 *  Read data from LoadCell
 */ 
void    ReadSensor() {
    switch (sub_state) {
        case st_readSensor: {
            if (scale.is_ready()) {
                long reading = scale.get_units(5);
                Serial.print("HX711 reading: ");
                Serial.println((int)reading);

                newly_data = (int)reading;
                n_apples++;

                // modify program flow
                prev_state = state;
                state = St_ConnectionCheck;
            } else {
                Serial.println("HX711 not found. Try again...");
                
                sub_state = st_wait; 
                setTimer(1);
                startTimer();
            }
            break;
        }
        case st_wait: {
            if (isTimeOut())
                sub_state = st_readSensor;
            break;
        }
    }
       
}
