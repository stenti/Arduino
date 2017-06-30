int LEDpin = 7;
int IRpin = 2;
int IRstat;

void setup() {
  pinMode(LEDpin, OUTPUT);
  pinMode(IRpin, INPUT);
}

void loop() {
    IRstat = digitalRead(IRpin);
      if (IRstat == HIGH) {              // check if IRbeam UNinterupted
          digitalWrite(LEDpin,LOW);
      }
      if (IRstat == LOW) {               // check if IRbeam interupted
        digitalWrite(LEDpin,HIGH);
      }
  delay(1/1000);
}

