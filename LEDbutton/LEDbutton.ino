int ledPin = 2;                // LED is connected to pin 12
int switchPin = 12;              // switch is connected to pin 2
int val;                        // variable for reading the pin status


void setup() {
  pinMode(ledPin, OUTPUT);      // Set the LED pin as output
  pinMode(switchPin, INPUT);    // Set the switch pin as input
}


void loop(){
  val = digitalRead(switchPin);   // read input value and store it in val
  if (val == LOW) {               // check if the button is pressed
    digitalWrite(ledPin, HIGH);   // turn LED on
  }
  if (val == HIGH) {              // check if the button is not pressed
    digitalWrite(ledPin, LOW);    // turn LED off
  }
}
