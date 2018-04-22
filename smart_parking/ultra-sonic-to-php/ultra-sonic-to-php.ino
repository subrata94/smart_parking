#include <SoftwareSerial.h>

const int trigPin = 4;
const int echoPin = 5;

const String ssid = "ryo";
const String password = "15111994";

float data[50][2]={0};
float distance[2]={0};
int count = 0;
String D;
SoftwareSerial esp8266Module(2, 3);
String server = "192.168.43.212"; //this ip is for api.thinhspeak.com

void setup()
{
  Serial.begin(9600);
  esp8266Module.begin(9600);
  reset();
  delay(2000);
  connectWifi();
}

void reset() {
  esp8266Module.println("AT+RST");
  delay(1000);
  if (esp8266Module.find((char*)"OK") ) 
    Serial.println("Module Reset");
  else{ 
    Serial.println("Module not Reset");
    reset();
  }
}

//connect to your wifi network
void connectWifi() {
  //esp8266Module.println("AT+CWMODE=3");
  String cmd = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";
  //esp8266Module.println("AT+CIFSR");
  esp8266Module.println(cmd);
  delay(5000);
  if (esp8266Module.find((char*)"OK")){
    Serial.println("Connected!");
    delay(1000);
  }
  else {
    Serial.println("Cannot connect to wifi");
    Serial.println("Trying again...");
    connectWifi();
  }
  
}

//to send the data

void httpGet () {
  esp8266Module.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");   //start a TCP connection.
  if ( esp8266Module.find((char*)"OK")) {
    Serial.println("TCP connection ready");
  }
  
  //String getRequest = "GET "+uri; 
  String getRequest = "GET /send.php?"+D;
       
  Serial.println(getRequest);
  
  String sendCmd = "AT+CIPSEND=";    //determine the number of caracters to be sent.
  esp8266Module.print(sendCmd);
  int i = getRequest.length()+2;  //for uploading to localhost the dataline should be incremented by 2 as done here
  esp8266Module.println(i);
  delay(100);
  if (esp8266Module.find((char*)">")) {
    delay(1000);
    Serial.println("Sending..");
    esp8266Module.println(getRequest);
    delay(3000);
    if ( esp8266Module.find((char*)"SEND OK")) {
      Serial.println("Packet sent");
      while (esp8266Module.available()) {
        String tmpResp = esp8266Module.readString();
        Serial.println(tmpResp);
      }
      delay(1000);
      // close the connection
      //esp8266Module.println("AT+CIPCLOSE=4");
    }else{
      Serial.println("Packet not sent");
      //esp8266Module.println("AT+CIPCLOSE=4");
    }
  }
}

void calculateDistance()
{
  float duration;
  float inches, cm;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  data[count][0] = inches;
  data[count][1] = cm;
  //D = "cm=" + String(cm) + "&inch=" + String(inches);
  delay(100); 
  count++;
}

float microsecondsToInches(long microseconds)
{
  float inch = microseconds/74.0/2.0; 
  return inch;
}

float microsecondsToCentimeters(long microseconds)
{
  float cm = microseconds/29.0/2.0; 
  return cm;
}

void average(){
  float inch=0, cm=0;
  for(int i=0;i<50;i++){
    inch = inch + data[i][0];
    cm = cm + data[i][1];
    data[i][0]=data[i][0]=0;
  }
  inch = inch / 50.0;
  cm = cm / 50.0;
  distance[0]=inch;
  distance[1]=cm;
  D = "cm=" + String(cm) + "&inch=" + String(inch);
}


void loop()
{
  calculateDistance();
  if(count > 50)
  {
    average();
    distance[0]=distance[1]=0;
    count = 0;
    delay(2000);
    httpGet();
  }
  //delay(1000);
  
}


