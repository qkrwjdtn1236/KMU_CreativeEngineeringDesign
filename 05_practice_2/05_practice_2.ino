#define LED 7
void setup()
{
  
    int i = 0;
    pinMode(LED, OUTPUT);
  
    digitalWrite(LED, LOW);
    delay(1000); 
  
    while(1){
      if (i>=5)
          break;

      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED, LOW);
      delay(100);
      i++;
    }
}

void loop()
{
  digitalWrite(LED,HIGH);
  delay(1000);
}
