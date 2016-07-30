#include <SoftwareSerial.h>
 
SoftwareSerial esp8266(2,3); // make RX Arduino line is pin 0, make TX Arduino line is pin 1.
                             // This means that you need to connect the TX line from the esp to the Arduino's pin 0
                             // and the RX line from the esp to the Arduino's pin 1
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600); // your esp's baud rate might be different
 pinMode(4, OUTPUT);
  digitalWrite(4,HIGH);
  //digitalWrite(4,LOW);
}
 
void loop()
{
  /*
  if(esp8266.available()) // check if the esp is sending a message 
  {
    while(esp8266.available())
    {
      // The esp has data so display its output to the serial window 
      char c = esp8266.read(); // read the next character.
      Serial.write(c);
    }  
  }
  */
  
  int countb = esp8266.available();
  if(countb>0) // check if the esp is sending a message 
  {
    for(int i=0; i<countb;i++)
    {
      // The esp has data so display its output to the serial window 
      char c = esp8266.read(); // read the next character.
      Serial.write(c);
      //Serial.flush();
    }  
          Serial.println();

  }
  
  if(Serial.available())
  {
    // the following delay is required because otherwise the arduino will read the first letter of the command but not the rest
    // In other words without the delay if you use AT+RST, for example, the Arduino will read the letter A send it, then read the rest and send it
    // but we want to send everything at the same time.
    delay(1000); 
    
    String command="";
    
    while(Serial.available()) // read the command character by character
    {
      // read one character
      command+=(char)Serial.read();
    }
    esp8266.println(command); // send the read character to the esp8266
  }
}
