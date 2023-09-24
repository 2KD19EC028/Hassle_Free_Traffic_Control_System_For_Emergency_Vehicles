/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-rfid-nfc
 */

#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
//#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

#define SS_PIN  5  // ESP32 pin GIOP5 
#define RST_PIN 27 // ESP32 pin GIOP27 
#define GREEN_LED 4
#define YELLOW_LED 33
#define RED_LED 25
#define BUZZER 26

#define RXD2 16
#define TXD2 17

char ssid[] = "Pixel";   // your network SSID (name) 
char pass[] = "12345678";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "12NTZ8JI2N3XUSBN";//"XPUEVS4VV1NMDJII";

int number = 0;

MFRC522 rfid(SS_PIN, RST_PIN);
unsigned int card_ID_0_buffer[4]; 
bool card_detected = 0;
uint8_t count = 0,count1 =0;
void setup() {
  Serial.begin(9600); 
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode (GREEN_LED, OUTPUT);
  pinMode (YELLOW_LED, OUTPUT);
  pinMode (RED_LED, OUTPUT);  
  pinMode (BUZZER, OUTPUT);
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
}

void loop() {
   // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      Serial.print("RFID/NFC Tag Type: ");
      Serial.println(rfid.PICC_GetTypeName(piccType));

      // print UID in Serial Monitor in the hex format
      Serial.print("UID:");
      for (int i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(rfid.uid.uidByte[i], HEX);
       card_ID_0_buffer[i] = rfid.uid.uidByte[i];
      }

      if ((card_ID_0_buffer[0] == 0x80) && (card_ID_0_buffer[1]  == 0xFA) && (card_ID_0_buffer[2]  == 0xE6) && (card_ID_0_buffer[3] == 0x26))      
      {
        memset(card_ID_0_buffer, 0, sizeof(card_ID_0_buffer));
         Serial2.write("b");
        digitalWrite (RED_LED, LOW);
        digitalWrite (YELLOW_LED, HIGH);
         digitalWrite (GREEN_LED, LOW);
        digitalWrite (BUZZER, HIGH);
        delay(2000);
        digitalWrite (YELLOW_LED, LOW); 
        Serial.println("Turn on Green light");
        digitalWrite (GREEN_LED, HIGH);
        card_detected = 1;        
      }

      if ((card_ID_0_buffer[0] == 0xE8) && (card_ID_0_buffer[1]  == 0x8E) && (card_ID_0_buffer[2]  == 0xC7) && (card_ID_0_buffer[3] == 0x0D))      
      {
        memset(card_ID_0_buffer, 0, sizeof(card_ID_0_buffer));
         Serial2.write("b");
         digitalWrite (RED_LED, LOW);
        digitalWrite (YELLOW_LED, HIGH);
        digitalWrite (GREEN_LED, LOW);
        digitalWrite (BUZZER, HIGH);
        delay(2000);
        digitalWrite (YELLOW_LED, LOW);        
        Serial.println("Turn on Green light");
        digitalWrite (GREEN_LED, HIGH);
        card_detected = 1;
      }      
      
      if(card_detected)
      {
          card_detected = 0;        
           number = 1;
          int x = ThingSpeak.writeField(myChannelNumber, 1, number, myWriteAPIKey);
          if(x == 200)
          {
            Serial.println("Channel update successful.");
          }
          else
          {
            Serial.println("Problem updating channel. HTTP error code " + String(x));
          }
           delay(10000);
          digitalWrite (GREEN_LED, LOW);
          digitalWrite (BUZZER, LOW);      
           Serial.println("Turn on Red light"); 
           digitalWrite (RED_LED, HIGH);    
           count1 = 1;

      }
      Serial.println();

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
 
  if(!count)
  {        
      int x = ThingSpeak.writeField(myChannelNumber, 1, 0, myWriteAPIKey);
      if(x == 200)
          {
            Serial.println("Channel update successful.");
          }
          else
          {
            Serial.println("Problem updating channel. HTTP error code " + String(x));
          }   
          count++;   
  }   

   if(count1)
   {        
          int z = ThingSpeak.writeField(myChannelNumber, 1, 0, myWriteAPIKey);
          if(z == 200)
          {
            Serial.println("Channel update successful.");
            count1 = 0;;            
          }
          else
          {
            Serial.println("Problem updating channel. HTTP error code " + String(z));
          }
        }

#if 0
    if(card_detected == 0)
    {
       Serial2.write("a");
        digitalWrite (YELLOW_LED, LOW);
        digitalWrite (GREEN_LED, LOW);        
        digitalWrite (RED_LED, HIGH);
        delay(5000);
        digitalWrite (RED_LED, LOW);
        digitalWrite (YELLOW_LED, HIGH);
        delay(5000);
        digitalWrite (YELLOW_LED, LOW);
        digitalWrite (GREEN_LED, HIGH);
        delay(5000);                
        
    }
#endif
                
}
