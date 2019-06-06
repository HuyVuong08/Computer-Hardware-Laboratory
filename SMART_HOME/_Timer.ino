#include "General_Resources.hpp"

bool    isTimerOn = false;
unsigned int times = 0;
long int timePrev = 0, timeCur = 0;

bool    getTimerState() 
{
    return isTimerOn;
}

void    setTimer(unsigned int t) 
{
    times = t * 1000;
}

void    setTimerms(unsigned int t) 
{
    times = t;
}

void    startTimer() 
{
    if (!isTimerOn) 
    {
        isTimerOn = true;
        timePrev = millis();
    }
}

bool    isTimeOut() 
{
    if (millis() - timePrev >= times) 
    {
        isTimerOn = false;
        return true;
    }
    else
    {
        return false;
    }
}

/**
 *  Very important state, 
 *  an alternative solution for delay()
 */ 
void    Wait() 
{
    if (isTimeOut()) 
    {
        if (prev_state == St_Greeting) 
        {
            state = St_Unlock;
        }
        else 
        {
            state = St_ReadSensor;
            sub_state = st_readSensor;
        }
    }
    else
        state = St_LCD_Button;    
} 