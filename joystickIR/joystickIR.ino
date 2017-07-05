#include <IRremote.h>

#define PIN_IR 3
#define PIN_DETECT 2
#define PIN_STATUS 11

const int axis1 = A1;
const int axis2 = A0;
float a1Baseline;
float a2Baseline;
int count = 0;

int isBroken = 0;
int IR_STATUS;
IRsend irsend;

boolean onLED = false;
boolean zero = false;
int counter = 0;

const int zthreshold = 15;
const int gthreshold = 300;
const int LRthreshold = 100;


void setup() {
  // open a serial connection to display values
  Serial.begin(38400);
  pinMode(PIN_DETECT, INPUT);
  pinMode(PIN_STATUS, OUTPUT);
  irsend.enableIROut(38);
  irsend.mark(0);
}

void loop() {

// set baseline and normalise joystick output
  while (count<100) {
    a1Baseline = (a1Baseline + analogRead(axis1))/2;
    a2Baseline = (a2Baseline + analogRead(axis2))/2;
    count++;
  }
  
  // read the value on AnalogIn pins
  // and store it in a variable
  float axis1Val = analogRead(axis1);
  float axis2Val = analogRead(axis2);


  float a1 = (axis1Val - a1Baseline);//*3.3;
  float a2 = (axis2Val - a2Baseline);//*3.3;

  Serial.print(millis());
  Serial.print(',');
  Serial.print(a1);
  Serial.print(',');
  Serial.println(a2);
  
  if (a1 < zthreshold && a1 > -zthreshold  && a2 < zthreshold  && a2 > -zthreshold) {
    zero = true;
    }
  
  if (a1 > gthreshold && a2 < LRthreshold && a2 > -LRthreshold) {
    if (zero){
      onLED = true;
      //SEND TTL PULSE TO DISPENSER (set pin to high)
      zero = false;
    }

  }else{
    digitalWrite(PIN_STATUS, LOW);
    onLED = false;
  }

  if (onLED && counter < 20) {
    digitalWrite(PIN_STATUS, HIGH);
    counter++;
  }else{
    onLED = false;
    counter = 0;
  }

  IR_STATUS = digitalRead(PIN_DETECT);
  if (IR_STATUS != isBroken){
    isBroken = IR_STATUS;
    Serial.print(millis());
    Serial.print(',');
    Serial.print(isBroken);
    Serial.println("BEAMBREAK");
  }
}
