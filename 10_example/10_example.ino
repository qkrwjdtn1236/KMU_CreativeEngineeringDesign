#include <Servo.h>
#define PIN_SERVO 10

Servo myservo;

void setup() {
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(500);  
  delay(1000);
}

void loop() {
    // add code here.
}
