int led = 13;//pin 13 as OUTPUT LED pin
int resetPin = 12;
// the setup routine runs once when you press reset:
void setup() {  
  digitalWrite(resetPin, HIGH);
  delay(200);
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);    
  pinMode(resetPin, OUTPUT);     
  Serial.begin(9600);//initialize Serial Port
  Serial.println("reset");//print reset to know the program has been reset and 
  //the setup function happened
  delay(200);
}

void loop(){}
