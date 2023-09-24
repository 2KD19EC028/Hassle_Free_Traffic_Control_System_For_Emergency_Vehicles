/*
 ESP8266 Serial Communication
 
 The blue LED on the ESP-12 module is connected to GPIO2
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
*/

#define GREEN_LED 14
#define YELLOW_LED 12
#define RED_LED 13
#define GREEN_LED_1 5
#define RED_LED_1 4
#define YELLOW_LED_1 15
  uint8_t data = 0;
  long long int count = 0;
bool detected_flag = 0;   
void setup() {
  Serial.begin(9600);     // Initialize the Serial interface with baud rate of 9600
  pinMode (GREEN_LED, OUTPUT);
  pinMode (YELLOW_LED, OUTPUT);
  pinMode (RED_LED, OUTPUT); 
  pinMode (GREEN_LED_1, OUTPUT);
  pinMode (RED_LED_1, OUTPUT);  
  pinMode (YELLOW_LED_1, OUTPUT);  
  // Serial2.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  if(detected_flag == 1)
  {
    count = 0;
  }      
  else if(count == 800000)
  {
    count = 0;   
  }  
  else
  {
    count++;
  }
    
  if(Serial.available())    //Checks is there any data in buffer 
  {
    detected_flag = 1;
    Serial.print("We got:");
   // Serial.print(Serial.read());  //Read serial data byte and send back to serial monitor
     data = Serial.read(); 
     Serial.print(data);
  }

  if(data == 98)
  {      
     digitalWrite (GREEN_LED, LOW);
      digitalWrite (YELLOW_LED, HIGH);      
      digitalWrite (RED_LED, LOW);
      delay(6000);
        digitalWrite (GREEN_LED, HIGH);
      digitalWrite (YELLOW_LED, LOW);      
      digitalWrite (RED_LED, LOW);
      delay(6000);
        digitalWrite (GREEN_LED, LOW);
      digitalWrite (YELLOW_LED, LOW);      
      digitalWrite (RED_LED, HIGH);  
      detected_flag = 0; 
      data = 0;  
  }      

    if(count > 0 && count < 200000)
    {
      digitalWrite (GREEN_LED, LOW);
      digitalWrite (YELLOW_LED, LOW);      
      digitalWrite (RED_LED, HIGH); 
      digitalWrite (GREEN_LED_1, HIGH);
      digitalWrite (RED_LED_1, LOW); 
      digitalWrite (YELLOW_LED_1, LOW);    
    }
    else if(count > 200000 && count < 400000)
    {
            
      digitalWrite (GREEN_LED, LOW);
      digitalWrite (YELLOW_LED, HIGH);      
      digitalWrite (RED_LED, LOW);
      digitalWrite (RED_LED_1, HIGH);
      digitalWrite (GREEN_LED_1, LOW); 
        digitalWrite (YELLOW_LED_1, LOW);
    } 
    else if(count > 400000 && count < 800000)
    {
      digitalWrite (GREEN_LED, HIGH);
      digitalWrite (YELLOW_LED, LOW);      
      digitalWrite (RED_LED, LOW);  
      digitalWrite (YELLOW_LED_1, HIGH);
      digitalWrite (GREEN_LED_1, LOW); 
      digitalWrite (RED_LED_1, LOW);  
    }

}