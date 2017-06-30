#include <IRremote.h>

#define PIN_IR 3
#define PIN_DETECT 2
#define PIN_STATUS 4

// named constant for the pin the sensor is connected to
const int axis1 = A1;
const int axis2 = A0;
float a1Baseline;
float a2Baseline;
int count = 0;

IRsend irsend;

void setup() {
  // open a serial connection to display values
  Serial.begin(9600);
  pinMode(PIN_DETECT, INPUT);
  pinMode(PIN_STATUS, OUTPUT);
  irsend.enableIROut(38);
  irsend.mark(0);
}

void loop() {

while (count<100) {
  a1Baseline = (a1Baseline + analogRead(axis1))/2;
  a2Baseline = (a2Baseline + analogRead(axis2))/2;
  count++;
}
  
  // read the value on AnalogIn pin 1
  // and store it in a variable
  float axis1Val = analogRead(axis1);
    float axis2Val = analogRead(axis2);


    float a1 = (axis1Val - a1Baseline)*3.3;
    float a2 = (axis2Val - a2Baseline)*3.3;

  Serial.print(a1);
    Serial.print(',');
    Serial.println(a2);

digitalWrite(PIN_STATUS, !digitalRead(PIN_DETECT));
  
  delay(1/1000);
}
