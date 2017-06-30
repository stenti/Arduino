// named constant for the pin the sensor is connected to
const int axis1 = A1;
const int axis2 = A0;

void setup() {
  // open a serial connection to display values
  Serial.begin(9600);
}

void loop() {
  // read the value on AnalogIn pin 1
  // and store it in a variable
  int axis1Val = analogRead(axis1);
    int axis2Val = analogRead(axis2);

  Serial.print("axis1 Value: ");
  Serial.println(axis2Val);
//   Serial.print("axis2 Value: ");
//  Serial.println(axis2Val);
  
  delay(1);
}
