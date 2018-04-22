#include <SoftwareSerial.h>
#include <OneWire.h>

String ssid = "subrata";
String password = "hjBS25683@094";

SoftwareSerial esp8266Module(2, 3);

//SoftwareSerial esp(2, 3);// TX, RX

String server = "192.168.0.102"; // www.example.com
String uri = "/send.php"; // our example is /esppost.php
String D;

//temparature sensor pin number
OneWire ds(8);

void setup() {
  Serial.begin(9600);
  esp8266Module.begin(9600);
  reset();
  connectWifi();
}//reset the esp8266 module

void reset() {
  esp8266Module.println("AT+RST");
  delay(5000);
  if (esp8266Module.find("OK") ) 
    Serial.println("Module Reset");
  else{ 
    Serial.println("Module not Reset");
    reset();
  }
}

//connect to your wifi network
void connectWifi() {
  String cmd = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";
  //esp8266Module.println("AT+CIFSR");
  esp8266Module.println(cmd);
  delay(10000);
  if (esp8266Module.find("OK"))
    Serial.println("Connected!");
  else {
    Serial.println("Cannot connect to wifi");
    Serial.println("Trying again...");
    connectWifi();
  }
}

void httppost () {
  esp8266Module.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.
  if ( esp8266Module.find("OK")) {
    Serial.println("TCP connection ready");
  }
  delay(3000);

  String postRequest = "POST " + uri + " HTTP/1.0\r\n" +
                       "Host: " + server + "\r\n" +
                       //"Accept: *" + "/" + "*\r\n" +
                       "Content-Length: " + D.length() + "\r\n" +
                       "Content-Type: application/x-www-form-urlencoded\r\n" +
                       "\r\n" + D;

  String sendCmd = "AT+CIPSEND=";    //determine the number of caracters to be sent.
  esp8266Module.print(sendCmd);
  esp8266Module.println(postRequest.length());
  delay(2000);
  if (esp8266Module.find(">")) {
    Serial.println("Sending..");
    esp8266Module.print(postRequest);
    delay(5000);
    
    if ( esp8266Module.find("SEND OK")) {
      Serial.println("Packet sent");
      while (esp8266Module.available()) {
        String tmpResp = esp8266Module.readString();
        Serial.println(tmpResp);
      }
      // close the connection
      esp8266Module.println("AT+CIPCLOSE");
    }else{
      Serial.println("Packet not sent");
    }
  }
}

void temp_check() {

  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;

  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }


  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return;
  }
  type_s = 0;


  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  D = "cel=" + String(celsius) + "&fer=" + String(fahrenheit);
  Serial.print("Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
  Serial.println(D);
}

void loop() {
  temp_check();
  httppost();
  delay(1000);
}
