const unsigned int _416ms = 416;
const unsigned char _37ms = 37;
unsigned long per_416ms = 0;
unsigned long per_37ms = 0;
int rcv;
int sensorValue;
float sensorFilter;
float alpa = 0.8;

char buf[15];
#include <SoftwareSerial.h>
SoftwareSerial SecoundSerial(10, 11);  // RX, TX

void setup() {
  Serial.begin(9600);
  SecoundSerial.begin(9600);  // Initialize software serial
}
void loop() {
  sensorValue = analogRead(A5);
  sensorFilter = alpa * sensorValue + (1.0 - alpa) * sensorFilter;
  delay(5);
  if (Serial.available() > 0) {
    rcv = Serial.read();
  }

  if (SecoundSerial.available() > 0) {
    rcv = SecoundSerial.read();
  }

  if (rcv == 82) {  //read "R"
    unsigned long currentTime = millis();
    if (currentTime - per_37ms > _37ms) {
      int myInt = (int)sensorFilter;
      sprintf(buf, "*%04d#", myInt);
      Serial.println(buf);
      SecoundSerial.println(buf);
      per_37ms = currentTime;
    }
  }
}
