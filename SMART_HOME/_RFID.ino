/*
 * Initial Author: ryand1011 (https://github.com/ryand1011)
 *
 * Reads data written by a program such as "rfid_write_personal_data.ino"
 *
 * See: https://github.com/miguelbalboa/rfid/tree/master/examples/rfid_write_personal_data
 *
 * Uses MIFARE RFID card using RFID-RC522 reader
 * Uses MFRC522 - Library
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         D0           // Configurable, see typical pin layout above
#define SS_PIN          D3          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

byte Buffer[18];
byte BufferSize    = 18;


byte Password[18]  = "12345";
byte Eraser[16]    = "000000000000000";
byte PasswordBlock = 2;


extern int  NumberOfCards;
extern bool LoggedIn;
extern bool PasswordMatched;
extern enum State state, prev_state;


//*****************************************************************************************//
void RFID_Setup () 
{
    SPI.begin ();             // Init SPI bus
    mfrc522.PCD_Init ();      // Init MFRC522 card
}

void On_RFID_Verify () 
{
    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

    //some variables we need
    MFRC522::StatusCode status;

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
        return;
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
        return;
    }

    Serial.println(F("**Card Detected:**"));

    mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

    //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex
    
    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("Authentication failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else Serial.println(F("PCD_Authenticate() success: "));

    //Read Password
    status = mfrc522.MIFARE_Read(PasswordBlock, Buffer, &BufferSize);
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else Serial.println(F("MIFARE_Read() success: "));

    Serial.print(F("Buffer: "));
    for (uint8_t i = 0; i < 16; i++) 
    {
        Serial.write(Buffer[i] );
    }

    Serial.println(F("\n**End Reading**"));

    mfrc522.PICC_HaltA();       // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD

    //Evaluate Password
    PasswordMatched = true;
    for (uint8_t i = 0; i < 16; i++) 
    {
        if (Buffer[i] != Password[i])
        {
            PasswordMatched = false;
            continue;
        }
    }
    LoggedIn = !LoggedIn;
    Serial.println(LoggedIn);

    prev_state = St_Unlock;
    state = St_LCD_Button;
}

void On_RFID_Register () 
{
    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

    //some variables we need
    MFRC522::StatusCode status;

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
        return;
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
        return;
    }

    Serial.println(F("**Card Detected:**"));

    mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

    //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex

    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else Serial.println(F("PCD_Authenticate() success: "));

    //Read Password
    status = mfrc522.MIFARE_Read(PasswordBlock, Buffer, &BufferSize);
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else Serial.println(F("MIFARE_Read() success: "));

    Serial.print(F("Buffer: "));
    for (uint8_t i = 0; i < 16; i++) 
    {
        Serial.write(Buffer[i] );
    }

    Serial.println(F("\n**End Reading**"));

    //Evaluate Password
    PasswordMatched = true;
    for (uint8_t i = 0; i < 16; i++) 
    {
        if (Buffer[i] != Password[i])
        {
            PasswordMatched = false;
            continue;
        }
    }

    if (PasswordMatched)
    {
        Serial.println ("Authenticated Card");
        Serial.println ("Card Aproval Canceled");
        Serial.print (F("NumberOfCard: "));
        Serial.print (NumberOfCards);
    }
    else
    {   // Write Password
        status = mfrc522.MIFARE_Write(PasswordBlock, Password, 16);
        if (status != MFRC522::STATUS_OK) 
        {
            Serial.print(F("MIFARE_Write() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return;
        }
        else Serial.println(F("MIFARE_Write() success: "));

        Serial.print(F("Password: "));
        for (uint8_t i = 0; i < 16; i++) 
        {
            Serial.write(Password[i] );
        }

        NumberOfCards ++;
        Serial.print (F("\nNumberOfCard: "));
        Serial.print (NumberOfCards);

        Serial.println(F("\n**End Writing**"));
    }  
    
    mfrc522.PICC_HaltA();       // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}

void On_RFID_Disprove () 
{
    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

    //some variables we need
    MFRC522::StatusCode status;

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
       return;
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
        return;
    }

    Serial.println(F("**Card Detected:**"));

    mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

    //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex

    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else Serial.println(F("PCD_Authenticate() success: "));

    //Read Password
    status = mfrc522.MIFARE_Read(PasswordBlock, Buffer, &BufferSize);
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else Serial.println(F("MIFARE_Read() success: "));

    Serial.print(F("Buffer: "));
    for (uint8_t i = 0; i < 16; i++) 
    {
        Serial.write(Buffer[i] );
    }

    Serial.println(F("\n**End Reading**"));

    //Evaluate Password
    PasswordMatched = true;
    for (uint8_t i = 0; i < 16; i++) 
    {
        if (Buffer[i] != Password[i])
        {
            PasswordMatched = false;
            continue;
        }
    }

    if (PasswordMatched)
    {
        if (NumberOfCards < 2)
        {
            Serial.println ("Insufficent Cards");
            Serial.println ("Card Disproval Canceled");
            Serial.print (F("NumberOfCard: "));
            Serial.print (NumberOfCards);
        }
        else
        {   // Erase Password
            status = mfrc522.MIFARE_Write(PasswordBlock, Eraser, 16);
            if (status != MFRC522::STATUS_OK) 
            {
                Serial.print(F("MIFARE_Write() failed: "));
                Serial.println(mfrc522.GetStatusCodeName(status));
                return;
            }
            else Serial.println(F("MIFARE_Write() success: "));

            Serial.print(F("Erased Password: "));
            for (uint8_t i = 0; i < 16; i++) 
            {
                Serial.write(Eraser[i] );
            }

            NumberOfCards --;
            Serial.print (F("\nNumberOfCard: "));
            Serial.print (NumberOfCards);

            Serial.println(F("\n**End Writing**"));
        }
    }  
    else
    {
        Serial.println ("Password Dose Not Matched");
        Serial.println ("Card Disproval Canceled");
    }

    mfrc522.PICC_HaltA();       // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}
//*****************************************************************************************//