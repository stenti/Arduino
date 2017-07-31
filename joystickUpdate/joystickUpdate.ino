#include <IRremote.h>

#define PIN_IR 3
#define PIN_DETECT_REACH 2
#define PIN_DETECT_REWARD 8
#define PIN_GREEN 11
#define PIN_RED 13
#define PIN_PELLET 6
#define PIN_DELIVERED 7

const int axis1 = A1;
const int axis2 = A0;

int a1Baseline;
int a2Baseline;
int count = 0;

int perturb = 0;
int isBroken_REACH = 0;
int isBroken_REWARD = 0;
int REACH_STATUS;
int REWARD_STATUS;
IRsend irsend;

boolean onLED = false;
boolean zero = false;
int counter = 0;

const int zthreshold = 15;
int gthreshold = 100;
int LRthreshold = 50;

int start;
int dur;

void setup() {
  // open a serial connection to display values
  Serial.begin(115200); //bits per second = 14400 bytes per second = 14 bytes per ms
  pinMode(PIN_DETECT_REACH, INPUT);
  pinMode(PIN_DETECT_REWARD, INPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_RED, OUTPUT);
  irsend.enableIROut(38);
  irsend.mark(0);
}

void loop() {

//// set baseline zero
//  while (count<8) {
//    a1Baseline = (a1Baseline + analogRead(axis1));
//    a2Baseline = (a2Baseline + analogRead(axis2));
//    count++;
//  }
//  if (count == 8) {
//    // divide by 8 (take average)
//    a1Baseline = a1Baseline >> 3;
//    a2Baseline = a2Baseline >> 3;
//    count++;
//  }
//
start = micros();


  union {
    struct {
      int srt;
      int threshold;
      int width;
      int perturb;
      int fin;
    } packet;

    byte recieveData[10];
  } down;

if (Serial.available() > 0) {
  digitalWrite(PIN_GREEN,HIGH);
  for (int i = 0; i < 10; i++) {
      // read the incoming byte:
      down.recieveData[i] = Serial.read();
  }

  gthreshold = down.packet.threshold;
  LRthreshold = down.packet.width; 
  perturb = down.packet.perturb;
}
digitalWrite(PIN_GREEN,LOW);

  // read the value on AnalogIn pins
  int axis1Val = analogRead(axis1);
  int axis2Val = analogRead(axis2);

  // compare to baseline
  int a1 = (axis1Val - a1Baseline);
  int a2 = (axis2Val - a2Baseline);

  Serial.print(millis());
  Serial.print(',');
  Serial.print(a1);
  Serial.print(',');
  Serial.println(a2);

  // detect joystick at zero location
  if (a1 < zthreshold && a1 > -zthreshold  && a2 < zthreshold  && a2 > -zthreshold) {
    zero = true;
    }
  // detect joystick in goal
  if (a1 > gthreshold && a1 < -gthreshold && a2 < LRthreshold && a2 > -LRthreshold) {
    if (zero){
//      onLED = true;
      digitalWrite(PIN_GREEN, HIGH);  // turn reward LED off
      //SEND TTL PULSE TO DISPENSER (set pin to high)
      digitalWrite(PIN_PELLET,HIGH);
      zero = false;
    }
  }

//  if (onLED && counter < 1000) {
//    digitalWrite(PIN_GREEN, HIGH);
//    counter++;
//  }else{
//    onLED = false;
//    counter = 0;
//    digitalWrite(PIN_GREEN, LOW);
//  }

  REACH_STATUS = digitalRead(PIN_DETECT_REACH);
  digitalWrite(PIN_RED,REACH_STATUS);
//  if (REACH_STATUS != isBroken_REACH){
//    isBroken_REACH = REACH_STATUS;
//    Serial.print(millis());
//    Serial.print(',');
//    Serial.print(isBroken_REACH);
//    Serial.println("BEAMBREAK");
//  }

  REWARD_STATUS = digitalRead(PIN_DETECT_REWARD);
  if (REWARD_STATUS == HIGH) { // beam broken (add: and pellet dispensed)
    digitalWrite(PIN_GREEN,LOW); // turn reward LED off
  }
//  if (REWARD_STATUS != isBroken_REWARD){
//    isBroken_REWARD = REWARD_STATUS;
//    Serial.print(millis());
//    Serial.print(',');
//    Serial.print(isBroken_REWARD);
//    Serial.println("BEAMBREAK");
//  }


  union {
    struct {
      int srt;
      int x;
      int y;
      int REACH_BB;
      int REWARD_BB;
//      int dT;
      int fin;
    } packet;

    byte sendData[12];
  } up;

up.packet.srt = 0;
up.packet.x = a1;
up.packet.y = a2;
up.packet.REACH_BB = REACH_STATUS;
up.packet.REWARD_BB = REWARD_STATUS;
//up.packet.
up.packet.fin = 0;

  Serial.write(up.sendData,12);

  dur = micros() - start;
  delayMicroseconds(1000-dur);
}
