#include <SoftwareSerial.h>

SoftwareSerial wifiPort(2,3);  //tx rx 

void setup()  
{
  Serial.begin(9600);
  wifiPort.begin(9600); // or 115200 if your ESP can only communicate at that speed
}

void loop() 
{
  /*wifiPort.listen();
  if (wifiPort.available() ) {
    Serial.write(wifiPort.read());
  }
  if (Serial.available() ){
    wifiPort.write(Serial.read());
  }*/
}
