/*
 * Write personal data of a MIFARE RFID card using a RFID-RC522 reader
 * Uses MFRC522 - Library to use ARDUINO RFID MODULE KIT 13.56 MHZ WITH TAGS SPI W AND R BY COOQROBOT. 
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
 *
 * Hardware required:
 * Arduino
 * PCD (Proximity Coupling Device): NXP MFRC522 Contactless Reader IC
 * PICC (Proximity Integrated Circuit Card): A card or tag using the ISO 14443A interface, eg Mifare or NTAG203.
 * The reader can be found on eBay for around 5 dollars. Search for "mf-rc522" on ebay.com. 
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         D0          // Configurable, see typical pin layout above
#define SS_PIN          D3          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

byte Eraser[18]     = "0000000000000000";
byte Password[18]   = "123456789";
byte PasswordBlock  = 1;


int NumberOfCard = 1;
bool LoggedIn;
bool PasswordMatched;


void setup ()
{
    RFID_Setup ();
}

void loop ()
{
  On_RFID_LogIn ();
}

void RFID_Setup ()
{
    Serial.begin(9600);        // Initialize serial communications with the PC
    SPI.begin();               // Init SPI bus
    mfrc522.PCD_Init();        // Init MFRC522 card
}

void On_RFID_Register ()
{
    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

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

    Serial.print(F("Card UID:"));    //Dump UID
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.print(F(" PICC type: "));   // Dump PICC type
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    byte len;    
    MFRC522::StatusCode status;

    /*
      Serial.setTimeout(20000L) ;     // wait until 20 seconds for input from serial
      // Ask personal data: Family name
      Serial.println(F("Type Family name, ending with #"));
      len = Serial.readBytesUntil('#', (char *) buffer, 30) ; // read family name from serial
      for (byte i = len; i < 30; i++) buffer[i] = ' ';     // pad with spaces
    */

    //block = 1;
    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, PasswordBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else Serial.println(F("PCD_Authenticate() success: "));

    //Card Verify
    status = mfrc522.MIFARE_Read(PasswordBlock, buffer, &len);
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else Serial.println(F("MIFARE_Read() success: "));
    
    
    PasswordMatched = true;

    for (uint8_t i = 0; i < 16; i++) 
    {
        if (buffer[i] != Password[i])
        {
            PasswordMatched = false;
            break;
        }
    }

    if (PasswordMatched)
    {
        Serial.println ("Authenticated Card");
    }
    else
    {   // Write block
        status = mfrc522.MIFARE_Write(PasswordBlock, Password, 16);
        if (status != MFRC522::STATUS_OK) 
        {
            Serial.print(F("MIFARE_Write() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return;
        }
        else Serial.println(F("MIFARE_Write() success: "));
        NumberOfCard ++;
        Serial.println(F("\n**End Writing**\n"));
    }

    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}

void On_RFID_LogIn () 
{
    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

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

    //-------------------------------------------

    mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

    //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex

    //---------------------------------------- GET Password

    //some variables we need
    byte len;
    MFRC522::StatusCode status;


    byte buffer[18];
    //block = 1;

    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("Authentication failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else Serial.println(F("PCD_Authenticate() success: "));

    status = mfrc522.MIFARE_Read(PasswordBlock, buffer, &len);
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else Serial.println(F("MIFARE_Read() success: "));
    
    
    PasswordMatched = true;

    for (uint8_t i = 0; i < 16; i++) 
    {
        if (buffer[i] != Password[i])
        {
            PasswordMatched = false;
            break;
        }
    }

    if (PasswordMatched)
        LoggedIn = !LoggedIn;

    Serial.println (LoggedIn);

    Serial.println(F("\n**End Reading**\n"));

    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}

void On_RFID_CardDelete () 
{
    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

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

    //-------------------------------------------

    mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

    //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex

    //---------------------------------------- GET Password

    //some variables we need
    byte len;
    MFRC522::StatusCode status;


    byte buffer[18];
    //block = 1;

    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("Authentication failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else Serial.println(F("PCD_Authenticate() success: "));

    status = mfrc522.MIFARE_Read(PasswordBlock, buffer, &len);
    if (status != MFRC522::STATUS_OK) 
    {
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else Serial.println(F("MIFARE_Read() success: "));
    
    
    PasswordMatched = true;

    for (uint8_t i = 0; i < 16; i++) 
    {
        if (buffer[i] != Password[i])
        {
            PasswordMatched = false;
            break;
        }
    }

    if (PasswordMatched)
    {   
        if (NumberOfCard > 1)
        {
            // Write block
            status = mfrc522.MIFARE_Write(PasswordBlock, Eraser, 16);
            if (status != MFRC522::STATUS_OK) 
            {
                Serial.print(F("MIFARE_Write() failed: "));
                Serial.println(mfrc522.GetStatusCodeName(status));
                return;
            }
            else Serial.println(F("MIFARE_Write() success: "));
            NumberOfCard --;
            Serial.println(F("\n**End Erasing**\n"));
        }
        else 
            Serial.println ("Last Card");
    }
    else
    {
        Serial.println ("Not Authenticated Card");
    }


    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}