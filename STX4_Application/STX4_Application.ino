/*

 Designer Systems DS-STX4 Shield Arduino Demonstrator
 
 Requires Arduino Uno or MEGA or NANO boards
 
 DS-STX4.S [A0 & A1 jumpers ON] connected to I2C interface (ANALOG IN 4 [SDA] & 5 [SCL])
 Connected to 5V 1:16 ratio unipolar stepper motor
 Demonstrates speed control and global enable
 
 STX4_Application.ino	Date: 14/7/14	Version: 1.00 

 */

#include <Wire.h>

#define STX4 0x6C		// STX4 I2C Address

// Setup code
void setup()
{

  Wire.begin();			// Start I2C comms

}

// Main code
void loop(){
    
  // Motor 1, 5mS per step, 500 half-steps anti-clockwise
  WriteRegister(1, 9); // Operate & HalfStep & Anti-clockwise
  WriteRegister(2, 0x01);
  WriteRegister(3, 0xF4); // 0x1F4 = 500
  WriteRegister(4, 20); // (5mS / 0.25mS) = 20
  WriteRegister(17, 0x00); // Global update
  
  // Wait 5 seconds
  delay (5000);
   
  // Motor 1, 3mS per step, 1000 normal steps clockwise
  WriteRegister(1, 3); // Operate & Normal Step & Clockwise
  WriteRegister(2, 0x03);
  WriteRegister(3, 0xE8); // 0x3E8 = 1000
  WriteRegister(4, 12); // (3mS / 0.25mS) = 12
  WriteRegister(17, 0x00); // Global update
  
  // Wait 5 seconds
  delay (5000);
  
  // Motor 1, 10mS per step, 200 normal steps clockwise
  WriteRegister(1, 3); // Operate & Normal Step & Clockwise
  WriteRegister(2, 0x00);
  WriteRegister(3, 200); // 200
  WriteRegister(4, 40); // (10mS / 0.25mS) = 40
  WriteRegister(17, 0x00); // Global update
  
  // Wait 5 seconds
  delay (5000);
}

/****************************************************************************
  Function:
    void WriteRegister(byte Register, byte Value)

  Description:
    This routine writes a byte value to an I2C register

  Precondition:
    Wire.Begin() has been called

  Parameters:
    Register = Register address
    Value = Byte value to write to register address

  Returns:
    None
    
  Remarks:
    None
  ***************************************************************************/
void WriteRegister(byte Register, byte Value) {
  Wire.beginTransmission(STX4);
  Wire.write(Register);				      
  Wire.write(Value);				      
  Wire.endTransmission();
}


