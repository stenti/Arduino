/*


/****************************************************************************/
/*   Arduino Uno and DS_STX4 Shield                                         */
/*                                                                          */
/*   DS-STX4.S [A0 & A1 jumpers ON]                                         */
/*   Connected to I2C interface (ANALOG IN 4 [SDA] & 5 [SCL]                */      
/*                                                                          */
/*  Left servo connected to digital pin 10                                  */
/*  Right servo connected to digital pin 9                                  */
/*                                                                          */
/*  Ian Horsfield 24/5/16                                                   */
/****************************************************************************/

/*============== HEADERS ===================================================*/

#include <Wire.h>
#include <Servo.h>

/*============== CONSTANT DEFINITIONS ======================================*/

#define STX4 0x6C                                 // STX4 I2C Address

#define DISABLE               0b0011110
#define ENABLE                0b0011111
#define CLOCKWISE             0b0011111
#define ANTICLOCK             0b0011101
#define STEP_ROTATION         0b0011011
#define CONSTANT_ROTATION     0b0011111
#define FULL                  0b0010111
#define HALF                  0b0011111
#define HOLD_OFF              0b0001111
#define HOLD_ON               0b0011111

#define ONE_REVOLUTION        6000
#define UPDATE                0x00     // Global update
#define UP                    130
#define DOWN                  70
#define MIDDLE                88
#define POSITION_ONE          73
#define POSITION_TWO          103


/*============== FUNCTION PROTOTYPES =======================================*/

void setup();
void WriteRegister(byte Register, byte Value);
void ReadRegister(byte Register, byte Value);

/*============== Global Variables ==========================================*/

unsigned int  steps = ONE_REVOLUTION;             // Steps per revolution
byte steps_MSB = steps >> 8;
byte steps_LSB = steps & 0xff;

Servo servoRotate;                                // Define left servo
Servo servoLift;                                  // Define right servo

const byte interruptPin = 2;
const byte requestPill = 11;
const byte ackPill = 12;
const byte LED13 = 13;

const byte step_time=5;                        // in milliseconds
const byte quarter_milli=step_time/0.25;

volatile byte pillState = false;


/*============== Main =====================================================*/

void loop(){  

  while(pillState) {
    while(!digitalRead(requestPill));
    pillState = false; 
  }


//    pillState = false; 
//    
//    while(!digitalRead(requestPill));

    while(!pillState){
      WriteRegister(5, ENABLE & CLOCKWISE & STEP_ROTATION & FULL & HOLD_OFF);
      WriteRegister(6, steps_MSB);
      WriteRegister(7, steps_LSB); 
      WriteRegister(8, quarter_milli); // (i.e. 5mS / 0.25mS) = 20
      WriteRegister(17, UPDATE); // Global update
      delay((step_time*ONE_REVOLUTION)+1900);  //  
    }
    
    digitalWrite(ackPill,1);
    while(digitalRead(requestPill));
    digitalWrite(ackPill,0);
}

/*============= FUNCTIONS =================================================*/

void setup(){  

  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(requestPill, INPUT);
  pinMode(ackPill, OUTPUT);
  pinMode(LED13, OUTPUT);
  digitalWrite(LED13,0);
  digitalWrite(ackPill,0);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), pilldetected, FALLING);
  
  Wire.begin();                           // Start I2C comms

//  servoLift.attach(10);                   // Set lift servo to digital pin 10
//  servoRotate.attach(9);                   // Set right servo to digital pin 9
//  servoLift.write(DOWN);
//  servoRotate.write(POSITION_ONE);
  
  WriteRegister(5, ENABLE & CLOCKWISE & CONSTANT_ROTATION & FULL & HOLD_OFF);
  WriteRegister(8, quarter_milli); // (i.e. 5mS / 0.25mS) = 20
  WriteRegister(17, UPDATE); // Global update
  while(!pillState);
  WriteRegister(5, DISABLE & CLOCKWISE & CONSTANT_ROTATION & FULL & HOLD_OFF);
  WriteRegister(17, UPDATE); // Global update


  WriteRegister(5, ENABLE & CLOCKWISE & STEP_ROTATION & FULL & HOLD_OFF);
  WriteRegister(6, 1);
  WriteRegister(7, 0); 
  WriteRegister(8, quarter_milli); // (i.e. 5mS / 0.25mS) = 20
  WriteRegister(17, UPDATE); // Global update

  delay((step_time*ONE_REVOLUTION)+1100);  //
}

/***************************************************************************/

void WriteRegister(byte Register, byte Value) {
  
  Wire.beginTransmission(STX4);
  Wire.write(Register);				      
  Wire.write(Value);				      
  Wire.endTransmission();
}

void ReadRegister(byte Register, byte Value){
  
char c;

  Wire.beginTransmission(STX4);
  Wire.write(Register);                      
  Wire.endTransmission();
  Wire.requestFrom(STX4, Value);   
}

void pilldetected(){
  pillState = true;
  digitalWrite(LED13,pillState);
}






