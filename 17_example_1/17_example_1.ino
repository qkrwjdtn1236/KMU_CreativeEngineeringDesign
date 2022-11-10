#include <Servo.h>

// Arduino pin assignment

#define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
// Add IR Sensor Definition Here !!!
#define PIN_IR A0
#define PIN_SERVO 10
#define PIN_LED 9

#define _DUTY_MIN 550  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 925 // servo neutral position (90 degree)
#define _DUTY_MAX 2400 // servo full counter-clockwise position (180 degree)




#define LOOP_INTERVAL 50   // Loop Interval (unit: msec)

#define _DIST_MIN 100
#define _DIST_MAX 250

#define _EMA_ALPHA 0.1

int servoMotorMS(float readmm);
Servo myservo;
unsigned long last_loop_time;   // unit: msec

void setup()
{
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(57600);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_value, duty;
  float dist_prev, dist_raw,dist_ema,dist;
  

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // Remove Next line !!!
  //a_value = analogRead(PIN_POTENTIOMETER);
  
  // Read IR Sensor value !!!
  a_value = analogRead(PIN_IR);
  // Convert IR sensor value into distance !!!
  //dist = (6762.0/(a_value-9)-4.0)*10.0 - 60.0;
  
  dist_raw = map(a_value,0,1023,_DUTY_MAX,_DUTY_MIN);
  // we need distance range filter here !!!


  // 거리 측정하는 식이 이상하여 범위필터 적용이 불가능
  // 거리 측정이 안되오니 LED도 사용 의미가 없음.
//  if (dist_raw < _DIST_MIN) {
//    dist_raw = dist_prev;           // cut lower than minimum
//    digitalWrite(PIN_LED, 1);       // LED OFF
//  } else if (dist_raw > _DIST_MAX) {
//    dist_raw = dist_prev;           // Cut higher than maximum
//    digitalWrite(PIN_LED, 1);       // LED OFF
//  } else {    // In desired Range
//    digitalWrite(PIN_LED, 0);       // LED ON      
//    dist_prev = dist_raw;
//  }
  
  
  // we need EMA filter here !!!
  // 거리 공식 적용을 못하여 정확한 ema 필터 적용이 불가능함.
//  dist_ema = (_EMA_ALPHA * dist_raw) + (1-_EMA_ALPHA) * dist_prev;
  // map distance into duty

  myservo.writeMicroseconds(dist_raw);

  Serial.print("Min:"); Serial.print(_DIST_MIN);
  Serial.print(",IR:"); Serial.print(a_value);
  Serial.print(",dist:"); Serial.print(dist);
  Serial.print(",ema:"); Serial.print(dist_ema);
  Serial.print(",servo:"); Serial.print(duty);
  Serial.print(",MAX"); Serial.print(_DIST_MAX);
  Serial.println(""); 
}

// dummy code
//int servoMotorMS(float readmm){
////    float result;
////    if(readmm < _DIST_MIN)
////    return _DUTY_MIN;
////    
////  if(readmm > _DIST_MAX)
////    return _DUTY_MAX;
////    
//////  float chooseCase = case1<case2 ? case2 : case1; 
////
////  result = ((readmm+_DIST_MIN)/(_DIST_MAX+_DIST_MIN))*(float)_DIST_MAX;
////  
////  return (int)result;
//
//  return (int)map(readmm,0,1023,_DUTY_MIN,_DUTY_MAX);
//}
