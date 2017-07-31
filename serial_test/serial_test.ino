//#include <IRremote.h>

#define PIN_IR 3
#define PIN_DETECT_REACH 2
#define PIN_DETECT_REWARD 8
#define PIN_GREEN 11
#define PIN_RED 13
#define PIN_PELLET 6
#define PIN_DELIVERED 7

const int axis1 = A1;
const int axis2 = A0;

int count = 0;
int a1Baseline;
int a2Baseline;

byte recieveData[10];
//IRsend irsend;
int start;
int dur;

const int zthreshold = 15;
const int rthreshold = 30;
int gthreshold = 100;
int LRthreshold = 50;
int perturb = 0;
boolean zero = false;
int active = false;
int perturbed = 0;


int REACH_STATUS;
int REWARD_STATUS;

void setup() {
//  Serial.begin(57600); //bits per second = 7200 bytes per second = 7 bytes per ms
  Serial.begin(19200);
  pinMode(PIN_DETECT_REACH, INPUT);
  pinMode(PIN_DETECT_REWARD, INPUT);
    pinMode(PIN_DELIVERED, INPUT);
  pinMode(PIN_PELLET, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
    pinMode(12, OUTPUT);
  pinMode(PIN_RED, OUTPUT);
//  irsend.enableIROut(38);
//  irsend.mark(0);
  digitalWrite(PIN_GREEN,LOW);

}

void loop() {

// reset goal flag
  int goal = 0;

  // set baseline zero
  while (count<8) {
    a1Baseline = (a1Baseline + analogRead(axis1));
    a2Baseline = (a2Baseline + analogRead(axis2));
    count++;
  }
  if (count == 8) {
    // divide by 8 (take average)
    a1Baseline = a1Baseline >> 3;
    a2Baseline = a2Baseline >> 3;
    count++;
  }

  start = micros();
  digitalWrite(12, HIGH); 

    union {
      struct {
        int srt;
        int pellets;
        int threshold;
        int width;
        int baseline;
        int perturb;
        int washout;
        int fin;
      } packet;
  
      byte recieveData[16];
   } down;

  if (Serial.available() > 0) {
      for (int i = 0; i < 16; i++) {
        // read the incoming byte:
        recieveData[i] = Serial.read();
//        active = !active;
      }
  }

if (active == 0) {

  // read the value on AnalogIn pins
  int axis1Val = analogRead(axis1);
  int axis2Val = analogRead(axis2);

  // compare to baseline
  int a1 = (axis1Val - a1Baseline);
  int a2 = (axis2Val - a2Baseline);

//  Serial.print(millis());
//  Serial.print(',');
//  Serial.print(a1);
//  Serial.print(',');
//  Serial.println(a2);
  
    // detect joystick at zero location
  if (a1 < zthreshold && a1 > -zthreshold  && a2 < zthreshold  && a2 > -zthreshold) {
    zero = true;
    }

      
  // detect joystick in goal
  if (a1 > gthreshold && a1 < -gthreshold && a2 < LRthreshold && a2 > -LRthreshold) {
    if (zero){
      digitalWrite(PIN_GREEN, HIGH);  // turn reward LED on
      //SEND TTL PULSE TO DISPENSER (set pin to high)
      digitalWrite(PIN_PELLET,HIGH);
      goal = true;
      zero = false;
    }
  }

    REACH_STATUS = digitalRead(PIN_DETECT_REACH);
  digitalWrite(PIN_RED,REACH_STATUS);

    REWARD_STATUS = digitalRead(PIN_DETECT_REWARD);
  if (REWARD_STATUS == HIGH) { // beam broken (add: and pellet dispensed)
    //digitalWrite(PIN_GREEN,LOW); // turn reward LED off
  }




   union {
      struct {
        byte srt;
        int x;
        int y;
        byte beamBreak_1;
        byte beamBreak_2;
        byte goal;
        byte perturbed;
        byte fin;
      } packet;
  
      byte sendData[10];
  } up;

  up.packet.srt = 0;
  up.packet.x = a1;
  up.packet.y = a2;
  up.packet.beamBreak_1 = REACH_STATUS;
  up.packet.beamBreak_2 = REWARD_STATUS;
  up.packet.goal = goal;
  up.packet.perturbed = perturbed;
  up.packet.fin = 0;

  Serial.write(up.sendData,10); 
}
    dur = micros() - start;
  delayMicroseconds(2000-dur);
    digitalWrite(12, LOW); 
}

//
///*
//  SerialEvent occurs whenever a new data comes in the
// hardware serial RX.  This routine is run between each
// time loop() runs, so using delay inside loop can delay
// response.  Multiple bytes of data may be available.
// */
//void serialEvent() {
//  while (Serial.available()) {
//    // get the new byte:
//    char inChar = (char)Serial.read();
//    // add it to the inputString:
//    inputString += inChar;
//    // if the incoming character is a newline, set a flag
//    // so the main loop can do something about it:
//    if (inChar == '\n') {
//      stringComplete = true;
//    }
//  }
//}

