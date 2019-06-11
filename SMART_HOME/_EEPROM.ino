/**
 *    This files contains functions used to communicate with EEPROM
 */ 

#include        <EEPROM.h>

/**
 *   The format of EEPROM is as follows:
 *      0th -> 3rd byte     : contains calibrated value
 *      4th byte            : position of head of queue of data
 *      5th byte            : position of data of queue of data
 *      from 6th byte       : store data
 * 
 * => no. data stored = (tail - head) / 4
 */

const int   EEPROM_CalibAddr    = 0;                     // first position of Calibration index
const int   EEPROM_Head         = 4;                     // head byte
const int   EEPROM_Tail         = 5;                     // tail byte
const int   EEPROM_StartData    = 6;                     // the position of the first byte of data


//extern variables
extern float  Humidity;

void        EEPROM_setup()
{
    EEPROM.begin(400);
}
/**
 *  Write calibrated index to EEPROM
 *  @params val - calibrated index
 */ 
void        eeWriteCalibrate(float val) {
    int pos = EEPROM_CalibAddr;        
    byte* p = (byte*) &val;

    EEPROM.write(pos, *p);
    EEPROM.write(pos + 1, *(p + 1));
    EEPROM.write(pos + 2, *(p + 2));
    EEPROM.write(pos + 3, *(p + 3));
    EEPROM.commit();
}

/**
 *  Get calibrated index saved in EEPROM
 *  @return int - calibrated index
 */
float       eeGetCalibrate() {
    int pos = EEPROM_CalibAddr;
    float val = 0;
    byte* p = (byte*) &val;
    *p          = EEPROM.read(pos);
    *(p + 1)    = EEPROM.read(pos + 1);
    *(p + 2)    = EEPROM.read(pos + 2);
    *(p + 3)    = EEPROM.read(pos + 3);

    return val;
}

/**
 *  Check whether the LoadCel has been calibrated by checking the 0th byte
 *  Usually the scale value is negative number => 0th is always different from 0
 */ 
bool        isCalibrated() {
    return EEPROM.read(EEPROM_CalibAddr) != 0;
}


/**
 *  Check whether weighted data are stored in EEPROM by checking 4th byte
 */ 
bool        isDataInEEPR() {
    Serial.print("isDataInEEPR: "); Serial.println(getNSavedData());
    Serial.print("at head: "); Serial.println(EEPROM.read(getHead()));
    Serial.print("at tail: "); Serial.println(EEPROM.read(getTail()));
    return getNSavedData() != 0;
}


/**
 *  Write int array containing weighted data into EEPROM
 *  @params values - int array
 *          data_size - no. elements in array
 */ 
void        SaveMem() {
    // write newly data from LoadCell
    int pos = getTail();                        // get position of byte to write

    byte* p = (byte*) &Humidity;

    EEPROM.write(pos, *p);
    EEPROM.write(pos + 1, *(p + 1));
    EEPROM.write(pos + 2, *(p + 2));
    EEPROM.write(pos + 3, *(p + 3));

    // update tail postion
    EEPROM.write(EEPROM_Tail, getTail() + 4);

    EEPROM.commit();

    Serial.print("no.data in savemem: "); Serial.println(getNSavedData());

    state = St_Wait;
    setTimer((int)Speed);
    startTimer();
}


/**
 *  Get number of weighted data stored in EEPROM
 *  @return int - number of weighted data
 */
int         getNSavedData() {
    return (getTail() - getHead()) / 4;
}


/**
 *  Get weighted values stored in EEPROM
 *  @return -1: no data in EEPROM
 *          positive number: number from EEPROM
 */
int        getSavedData() {
    if (!isDataInEEPR())
        return -1;
    

    int pos = getHead();
    int tmp = 0;
    byte *p = (byte *) &tmp;
    *p          = EEPROM.read(pos);
    *(p + 1)    = EEPROM.read(pos + 1);
    *(p + 2)    = EEPROM.read(pos + 2);
    *(p + 3)    = EEPROM.read(pos + 3);

    byte head = getHead() + 4, tail = getTail();

    if (head == tail)
        head = tail = EEPROM_StartData;
    EEPROM.write(EEPROM_Head, head);
    EEPROM.write(EEPROM_Tail, tail);
    EEPROM.commit();

    Serial.print("no.data in savemem: "); Serial.println(getNSavedData());

    return tmp;
}


byte        getHead() {
    return EEPROM.read(EEPROM_Head);
}

byte        getTail() {
    return EEPROM.read(EEPROM_Tail);
}