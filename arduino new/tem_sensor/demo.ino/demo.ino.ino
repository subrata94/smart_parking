#include <SoftwareSerial.h>

const byte rxPin = 0; // Wire this to Tx Pin of ESP8266
const byte txPin = 1; // Wire this to Rx Pin of ESP8266

// We'll use a software serial interface to connect to ESP8266
SoftwareSerial ESP8266 (rxPin, txPin);

void setup() {
  Serial.begin(115200);
  ESP8266.begin(115200); // Change this to the baudrate used by ESP8266
  delay(1000); // Let the module self-initialize
}

void loop() {
  Serial.println("Sending an AT command...");
  ESP8266.println("AT");
  delay(30);
  while (ESP8266.available()){
     String inData = ESP8266.readStringUntil('\n');
     Serial.println("Got reponse from ESP8266: " + inData);
  }  
}


/*#include <SoftwareSerial.h>

SoftwareSerial wifiPort(0,1); 

void setup()  
{
  Serial.begin(9600);
  while (!Serial) {
  }
  wifiPort.begin(9600); // or 115200 if your ESP can only communicate at that speed
}

void loop() 
{

  wifiPort.listen();
  if (wifiPort.available() ) {
    Serial.write(wifiPort.read());
  }
  if (Serial.available() ){
    wifiPort.write(Serial.read());
  }
  delay(10);
}*/
/*
#include <SoftwareSerial.h>

SoftwareSerial ESPModule(2, 3); //TX, RX

char message;
char data;

void setup() {
  Serial.begin(9600);
  ESPModule.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0) {
    message = Serial.read();
    ESPModule.write(message);
  }
  if(ESPModule.available()>0) {
    data = ESPModule.read();
    Serial.write(data);
  }
}*/
