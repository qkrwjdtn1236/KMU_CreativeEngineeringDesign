#define LED 9

int brightness = 120;
int fadeAmount = 10;    

// the setup routine runs once when you press reset:
void setup() {
  pinMode(LED, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  analogWrite(LED, brightness);

  brightness = brightness + fadeAmount;

  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  delay(30);
}

void set_period(int period){
  
}
void set_duty(int duty){
  
}
