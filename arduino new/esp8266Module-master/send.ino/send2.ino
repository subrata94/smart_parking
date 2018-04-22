#include <SoftwareSerial.h>
String ssid ="subrata";
String password="hjBS25683@094";
SoftwareSerial esp(6, 7);// RX, TX
String server = "192.168.0.103"; // www.example.com
String uri = "/send.php";// our example is /esppost.php
String Data;
OneWire ds(2);
String temp ,hum;

void setup() {
	esp.begin(9600);
	Serial.begin(9600);
	reset();
	connectWifi();
}
//reset the esp8266 module

void reset() {
	esp.println("AT+RST");
	delay(1000);
	if(esp.find("OK") ) Serial.println("Module Reset");
}

//connect to your wifi network

void connectWifi() {
	String cmd = "AT+CWJAP=\"" +ssid+"\",\"" + password + "\"";
	esp.println(cmd);
	delay(4000);
	if(esp.find("OK")) 
		Serial.println("Connected!");
	else {
		connectWifi();
		Serial.println("Cannot connect to wifi"); 
	}
}

void httppost () {
	esp.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.
	if( esp.find("OK")) {
		Serial.println("TCP connection ready");
	} 
	delay(1000);
	
	String postRequest = "POST " + uri + " HTTP/1.0\r\n" +
	"Host: " + server + "\r\n" +
	"Accept: *" + "/" + "*\r\n" +
	"Content-Length: " + Data.length() + "\r\n" +
	"Content-Type: application/x-www-form-urlencoded\r\n" +
	"\r\n" + Data;
	
	String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
	esp.print(sendCmd);
	esp.println(postRequest.length() );
	delay(500);
	if(esp.find(">")) { 
		Serial.println("Sending.."); esp.print(postRequest);
		if( esp.find("SEND OK")) { Serial.println("Packet sent");
			while (esp.available()) {
				String tmpResp = esp.readString();
				Serial.println(tmpResp);
			}
			// close the connection
			esp.println("AT+CIPCLOSE");
		}

	}
}

void temp_check(){
  
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
  Data = "cel="+celsius+"&fer="+fahrenheit;
  Serial.print("Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
}

void loop() {
  temp_check();
  httppost();
  delay(1000);
}
