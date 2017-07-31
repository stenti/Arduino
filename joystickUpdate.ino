#include <TimeLib.h>
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
boolean isConnected = false;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
char receivedChar;

boolean inGoal = false;
boolean reaching = false;
boolean zero = false;
const int LED_rthreshold = 50;
const int LED_gthreshold = 300;
const int LED_LRthreshold = 100;
IRsend irsend;

void setup() {
  // open a serial connection to display values
  Serial.begin(38400);
  pinMode(PIN_DETECT, INPUT);
  pinMode(PIN_STATUS, OUTPUT);
  irsend.enableIROut(38);
  irsend.mark(0);

  setTime(12,11,11,26,06,2017);
}

void loop() {

// set baseline and normalise joystick output
  while (count<8) {
    a1Baseline = (a1Baseline + analogRead(axis1));
    a2Baseline = (a2Baseline + analogRead(axis2));
    count++;
  }
a1Baseline=a1Baseline>>3; //bit shift 3 = divide by 8
a2Baseline=a2Baseline>>3; //bit shift 3 = divide by 8
  
  if (stringComplete == true) {
    if (inputString.startsWith("TIME")) {
      setTime(inputString.substring(14,15).toInt(),inputString.substring(16,17).toInt(),inputString.substring(18,19).toInt(),inputString.substring(12,13).toInt(),inputString.substring(10,11).toInt(),inputString.substring(6,9).toInt());
      inputString = "";
    }
  }

  
  // read the value on AnalogIn pin 1
  // and store it in a variable
  float axis1Val = analogRead(axis1);
  float axis2Val = analogRead(axis2);


  float a1 = (axis1Val - a1Baseline);//*3.3;
  float a2 = (axis2Val - a2Baseline);//*3.3;

//        digitalClockDisplay();
        Serial.print(millis());
        Serial.print(',');
  Serial.print(a1);
  Serial.print(',');
  Serial.println(a2);

  if (a1 > LED_rthreshold) {
    if (!reaching) {
//        digitalClockDisplay();
        Serial.print(millis());
        Serial.print(',');
        Serial.println("REACH");
        reaching = true;
    }
  }else {
    reaching = false;
  }
  if (a1 < 10  && a1 > -10  && a2 < 10  && a2 > -10) {
    if (!zero) {
//        digitalClockDisplay();
        Serial.print(millis());
        Serial.print(',');
        Serial.println("ZERO");
        zero = true;
    }
  }else {
    zero = false;
  }
  if (a1 > LED_gthreshold && a2 < LED_LRthreshold && a2 > -LED_LRthreshold) {
    digitalWrite(PIN_STATUS, HIGH);
    if (!inGoal) {
//        digitalClockDisplay();
        Serial.print(millis());
        Serial.print(',');
        Serial.println("GOAL");
        inGoal = true;
    }
  }else{
    digitalWrite(PIN_STATUS, LOW);
    inGoal = false;
  }

  IR_STATUS = digitalRead(PIN_DETECT);
  if (IR_STATUS != isBroken){
    isBroken = IR_STATUS;
//        digitalClockDisplay();
        Serial.print(millis());
        Serial.print(',');
    Serial.print(isBroken);
    Serial.println("BEAMBREAK");
  }

//    digitalWrite(PIN_STATUS, !IR_STATUS);
  
//    delay(1/1000);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(year());
  twoDigit(month()); 
  twoDigit(day());
  twoDigit(hour());
  twoDigit(minute());
  twoDigit(second());
  Serial.print(" ");
}

void twoDigit(int t) {
  if (t < 10) {
    Serial.print('0');
  }
  Serial.print(t);
}
//
//void serialEvent() {
//  while (Serial.available()) {
//    // get the new byte:
//    char inChar = (char)Serial.read();
//    // add it to the inputString:
//    inputString += inChar;
//    if (inChar == 'X'){
//      stringComplete = true;
//    }
//  }
//}
