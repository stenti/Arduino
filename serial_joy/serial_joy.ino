#include <IRremote.h>

#define PIN_DETECT_REACH 2
#define PIN_IR 3


#define PIN_REQUEST 6
#define PIN_DELIVERED 7

#define PIN_GREEN 10
#define DEBUG1 12
#define PIN_RED 11


const int axis1 = A1;
const int axis2 = A0;

int count = 0;
int a1Baseline;
int a2Baseline;

byte recieveData[10];
IRsend irsend;
int start;
int dur;

const int zero_threshold = 15;
const int reach_threshold = 30;
int goal_threshold = 100;
int LR_threshold = 50;
boolean perturb = false;
byte zero = false;
byte reach = false;
byte goal = false;
boolean active = false;
boolean perturbed = false;

int success = 0;
byte end_char = 0;

// Define union for downstream messages of type down_union_t
typedef union down_union_t{
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
   };

// declaring an instrance of type down_union_t
down_union_t downstream_message;

// Define union for upstream messages of type down_union_t
typedef union up_union_t{
      struct {
        byte srt;
        int x;
        int y;
        byte beamBreak_1;
        byte reach;
        byte goal;
        byte perturbed;
        byte fin;
      } packet;
  
      byte sendData[10];
  };
  
// declaring an instrance of type up_union_t
up_union_t upstream_message;


byte REACH_STATUS;

void setup() {
  Serial.begin(115200); //bits per second = 7200 bytes per second = 7 bytes per ms
  //Serial.begin(19200);
  pinMode(PIN_DETECT_REACH, INPUT);
  pinMode(PIN_DETECT_REWARD, INPUT);
  pinMode(PIN_DELIVERED, INPUT);
  pinMode(PIN_PELLET, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(PIN_RED, OUTPUT);
  irsend.enableIROut(38);
  irsend.mark(0);
  digitalWrite(PIN_GREEN,LOW);
}

void loop() {

///////////////////////////////////////
// Initialise joystick
///////////////////////////////////////

  // reset goal flag
    int reward = 0;

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

  start = micros();     // sample timer 
  digitalWrite(DEBUG1, !digitalRead(DEBUG1));     //toggle pin

//////////////////////////////////////////////
// Coms down
//////////////////////////////////////////////

  if (Serial.available() > 0 && !active) {
      for (int i = 0; i < 16; i++) {
          // read the incoming byte:
          downstream_message.recieveData[i] = Serial.read();
      }
      if(downstream_message.packet.srt == 0 && downstream_message.packet.fin == 0) {
          goal_threshold = downstream_message.packet.threshold;
          LR_threshold = downstream_message.packet.width;
          active = true;    // set active flag
      }
  }

  if (active) {

///////////////////////////////////////
// Get joystick data
///////////////////////////////////////
      
      // read the value on AnalogIn pins
      int axis1Val = analogRead(axis1);
      int axis2Val = analogRead(axis2);

      // compare to baseline
      int a1 = (axis1Val - a1Baseline);
      int a2 = (axis2Val - a2Baseline);


/////////////////////////////////////////
// Pull detection
/////////////////////////////////////////
      
      // detect joystick at zero location
      if (a1 < zero_threshold && a1 > -zero_threshold  && a2 < zero_threshold  && a2 > -zero_threshold) {
          zero = true;
      }
  
    // detect reach
       reach = a1 > reach_threshold;  
    
    // detect joystick in goal
      if (a1 > goal_threshold && a1 < -goal_threshold && a2 < LR_threshold && a2 > -LR_threshold) {
          if (zero){
              digitalWrite(PIN_GREEN, HIGH);  // turn reward LED on
              //SEND TTL PULSE TO DISPENSER (set pin to high)
              digitalWrite(PIN_REQUEST,HIGH);
              goal = true;
              zero = false;
              success++;
              LED_ON = millis();
          }
      } else {
          goal = false;
      }
  
      if (digitalRead(PIN_DELIVERED) == HIGH) {
          digitalWrite(PIN_REQUEST,LOW);
      }

      if ((millis()-LED_ON) > 2000){
          digitalWrite(PIN_LED,LOW);
      }
  
///////////////////////////////////////////
// Beam break detection
//////////////////////////////////////////
    
      REACH_STATUS = digitalRead(PIN_DETECT_REACH);
      digitalWrite(PIN_RED,REACH_STATUS);
  
////////////////////////////////////////////
// Detect if should end
///////////////////////////////////////////
  
      if (success >= downstream_message.packet.pellets || millis()>1800000) {
        active = false;
        end_char = 42;
      }
  
////////////////////////////////////////////////////////////////////////////
// Comms up
////////////////////////////////////////////////////////////////////////////
//      upstream_message.packet.srt = 0;
//      upstream_message.packet.x = a1;
//      upstream_message.packet.y = a2;
//      upstream_message.packet.beamBreak_1 = REACH_STATUS;
//      upstream_message.packet.reach = reach;
//      upstream_message.packet.goal = goal;
//      upstream_message.packet.perturbed = perturbed;
//      upstream_message.packet.fin = end_char;
  
////////////////////////////////////
//Test data (send downstream data back)
///////////////////////////////////
      upstream_message.packet.srt = byte(downstream_message.packet.srt);
      upstream_message.packet.x = downstream_message.packet.pellets;
      upstream_message.packet.y = downstream_message.packet.threshold;
      upstream_message.packet.beamBreak_1 = byte(downstream_message.packet.width);
      upstream_message.packet.beamBreak_2 = byte(downstream_message.packet.baseline);
      upstream_message.packet.goal = byte(downstream_message.packet.perturb);
      upstream_message.packet.perturbed = byte(downstream_message.packet.washout);
      upstream_message.packet.fin = byte(downstream_message.packet.fin);
  
      Serial.write(upstream_message.sendData,10); 
  }

  dur = micros() - start;
  delayMicroseconds(2000-dur);
}
