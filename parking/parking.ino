const int trigPin = 4;
const int echoPin = 5;

void setup() 
{
  Serial.begin(9600);
}

void loop()
{
  calculateDistance();
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
  delay(1000); 
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
