#define PIN_REQUEST 7
#define PIN_DELIVERED 6
#define PIN_LED 10

int count = 0;
int start = 0;
int dur;

void setup() {
  pinMode(PIN_DELIVERED, INPUT);
  pinMode(PIN_REQUEST, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  
//start = millis();
//digitalWrite(PIN_PELLET,HIGH);
//digitalWrite(PIN_LED,HIGH);
//count++;
//delay(10000);
//digitalWrite(PIN_PELLET,LOW);
//digitalWrite(PIN_LED,LOW);
//dur = millis() - start;
//delay (20000 - dur);


dur = millis() - start;
if (dur >= 30000){
  digitalWrite(PIN_REQUEST,HIGH);
  digitalWrite(PIN_LED,HIGH);
  start = millis();
}

if (digitalRead(PIN_DELIVERED) == HIGH) {
  digitalWrite(PIN_REQUEST,LOW);
}
if (dur < 30000 && dur > 2000){
        digitalWrite(PIN_LED,LOW);

}

}
