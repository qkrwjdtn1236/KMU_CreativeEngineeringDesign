#define PIN_LED 13
unsigned int count, toggle;

void setup() {
  pinMode(PIN_LED,OUTPUT);
  Serial.begin(115200);
  while(!Serial){
    Serial.println("시리얼 준비중");
  }
  Serial.println("작동 준비 완료");
  count = toggle = 0;
  digitalWrite(PIN_LED,toggle); // toggle = 0 즉, 불이 끈 상태.
}

void loop() {
  Serial.println(++count);
  toggle = toggle_state(toggle);
  digitalWrite(PIN_LED,toggle);
  delay(1000);
}

int toggle_state(int toggle){
  return !toggle;
}
