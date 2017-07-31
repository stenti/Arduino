#include <IRremote.h>

#define PIN_IR 3
#define PIN_DETECT 2
#define PIN_STATUS 11

const int axis1 = A1;
const int axis2 = A0;
int a1Baseline;
int a2Baseline;
int count = 0;

int isBroken = 0;
int IR_STATUS;
IRsend irsend;

boolean onLED = false;
boolean zero = false;
int counter = 0;

const int zthreshold = 15;
const int gthreshold = 100;
const int LRthreshold = 50;


void setup() {
  // open a serial connection to display values
  Serial.begin(19200);
  pinMode(PIN_DETECT, INPUT);
  pinMode(PIN_STATUS, OUTPUT);
  irsend.enableIROut(38);
  irsend.mark(0);
}

void loop() {

// set baseline and normalise joystick output
  while (count<100) {
    a1Baseline = (a1Baseline + analogRead(axis1));
    a2Baseline = (a2Baseline + analogRead(axis2));
    count++;
  }
  a1Baseline = a1Baseline >> 3;
  a2Baseline = a2Baseline >> 3;
  
  // read the value on AnalogIn pins
  // and store it in a variable
  int axis1Val = analogRead(axis1);
  int axis2Val = analogRead(axis2);

  int a1 = (axis1Val - a1Baseline);//*3.3;
  int a2 = (axis2Val - a2Baseline);//*3.3;

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
  }

  if (onLED && counter < 1000) {
    digitalWrite(PIN_STATUS, HIGH);
    counter++;
  }else{
    onLED = false;
    counter = 0;
    digitalWrite(PIN_STATUS, LOW);
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
