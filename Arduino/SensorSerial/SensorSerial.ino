/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SoftwareSerial.h>

SoftwareSerial ESP8266(0, 1); // RX, TX
const int numSensors = 3;
char sensorStatus[numSensors] = {0};
int temperaturePin  = A0;
int tempID          =  0;
int lightPin        = A1;
int lightID         =  1;
int soilMoisturePin = A2;
int soilMoistureID  =  2;

int sensorValue;

bool packResponse = false;
short responseSize = 0;
short incomingSize = 0;
char addr = 0;
char cmd = 0;
char sensorID = 0;

char temp1, temp2;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  ESP8266.begin(115200);
  //ESP8266.println("Hello, world?\n");
}

void loop() { // run over and over
  if (ESP8266.available()) {
    if (ESP8266.read() == 0xFE)
    {
      if(getMessage())
        sendResponseUSB();
        Serial.println("Message Complete!");
    }
    else
    {
      ESP8266.flush();
      ESP8266.println("Not enough data\n");
    }
  }
}

bool getMessage()
{
    while(!ESP8266.available()) {}
    temp1 = ESP8266.read();
    while(!ESP8266.available()) {}
    temp2 = ESP8266.read();
  incomingSize = ((temp1 << 8) | (temp2) - 2) >> 1;
  responseSize = (incomingSize*4) + 2;
    while(!ESP8266.available()) {}
  ESP8266.read();
    while(!ESP8266.available()) {}
  ESP8266.read();
    while(!ESP8266.available()) {}
  addr = ESP8266.read();
    while(!ESP8266.available()) {}
  cmd = ESP8266.read();
  
  if (cmd != 'R')
    return false;
    
  for(int i = 0; i < incomingSize; i++)
  {
      while(!ESP8266.available()) {}
    ESP8266.read();
      while(!ESP8266.available()) {}
    sensorID = ESP8266.read();
    sensorStatus[sensorID] = 1;
  }
  return true;
}

void sendResponse()
{
  ESP8266.write(0xFE);
  ESP8266.write(responseSize >> 8);
  ESP8266.write((byte)responseSize);
  ESP8266.write((byte)0x0);
  ESP8266.write((byte)0x0);
  ESP8266.write('V'); // data response
  
  for(short i = 0; i < numSensors; i++)
  {
    if(sensorStatus[i] == 1)
    {
      ESP8266.write(i >> 8);
      ESP8266.write((byte)i);
      sensorValue = getSensorValue(i);
      ESP8266.write((sensorValue >> 8) & 0xFF);
      ESP8266.write((sensorValue) & 0xFF);
    }
  }
}

void sendResponseUSB()
{
  Serial.write(0xFE);
  Serial.write(responseSize >> 8);
  Serial.write((byte)responseSize);
  Serial.write((byte)0x0);
  Serial.write((byte)0x0);
  Serial.write('V'); // data response
  
  for(short i = 0; i < numSensors; i++)
  {
    if(sensorStatus[i] == 1)
    {
      Serial.write(i >> 8);
      Serial.write((byte)i);
      sensorValue = getSensorValue(i);
      Serial.write((sensorValue >> 8) & 0xFF);
      Serial.write((sensorValue) & 0xFF);
    }
  }
}

int getSensorValue(int i) {
  switch(i)
  {
    case 0:
      return getTemperature();
    case 1:
      return getLightValue();
    case 2:
      return getSoilMoisture();
    default:
      break;
  }
  return 0;
}

int getTemperature() {
  return map(analogRead(temperaturePin), 0, 1023, 0, 255);
}

int getLightValue() {
  return map(analogRead(lightPin), 0, 1023, 0, 255);
}

int getSoilMoisture() {
  return map(analogRead(soilMoisturePin), 0, 1023, 0, 255);
}


