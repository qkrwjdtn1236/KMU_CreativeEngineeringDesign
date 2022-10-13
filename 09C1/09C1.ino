// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25       // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100     // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300     // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // coefficent to convert duration to distance

#define _EMA_ALPHA 0.5    // EMA weight of new sample (range: 0 to 1)
                          // Setting EMA to 1 effectively disables EMA filter.
#define N 20
// global variables
unsigned long last_sampling_time;   // unit: msec
float dist_prev = _DIST_MAX;        // Distance last-measured
float dist_ema;                     // EMA distance


float distanceLog[N] = {0,};


float sort(float arr[],int length);
float median(float arr[],int length);

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  pinMode(PIN_TRIG,OUTPUT);
  pinMode(PIN_ECHO,INPUT);
  digitalWrite(PIN_TRIG, LOW);

  // initialize serial port
  Serial.begin(57600);

  // initialize last sampling time
  last_sampling_time = 0;
}

void loop() {
  float dist_raw;
  
  // wait until next sampling time. 
  // millis() returns the number of milliseconds since the program started. 
  // Will overflow after 50 days.
  if (millis() < last_sampling_time + INTERVAL)
    return;

  // get a distance reading from the USS
  dist_raw = USS_measure(PIN_TRIG,PIN_ECHO);
  
  if (dist_raw < _DIST_MIN) { // 범위 최소값보다 거리가 짧게 나올때 처리 , 이전에 가지고 있는 숫자로 치환(dist_raw = dist_prev)
    dist_raw = dist_prev;    // Set Lower Value
    
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else if (dist_raw > _DIST_MAX) { // 범위 최소값보다 거리가 짧게 나올때 처리 , 이전에 가지고 있는 숫자로 치환(dist_prev)
    dist_raw = dist_prev;    // Set Lower Value
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else {    // In desired Range // 정상적인 거리 수치 이전의 거리 수치를 현재 측정한 dist_raw를 dist_prev로 치환
    digitalWrite(PIN_LED, 0);       // LED ON
    dist_prev = dist_raw;      
  }
  
  // Modify the below line to implement the EMA equation
  dist_ema = (_EMA_ALPHA * dist_raw) + ((1- _EMA_ALPHA) * dist_ema); // ema 측정

  for(int i = 1;i<N;i++){
    distanceLog[i-1] = distanceLog[i];
  }
  distanceLog[N] = dist_ema;
  int length = (int)sizeof(distanceLog)/sizeof(distanceLog[0]);

  for(int i = 0;i<length-1;i++){
    for(int j = 0;j<length-1-i;j++){
      if(distanceLog[j] > distanceLog[j+1]){
        int temp = distanceLog[j];
        distanceLog[j] = distanceLog[j+1];
        distanceLog[j+1] = temp;
      }
    }
  }
  dist_ema = distanceLog[length/2]; //N 개 중에서 가장 중앙값은 값 추출 후 dist_ema에 치환
  
  
  // output the distance to the serial port
  Serial.print("Min:");   Serial.print(_DIST_MIN);
  Serial.print(",raw:");  Serial.print(dist_raw);
  Serial.print(",ema:");  Serial.print(dist_ema);
  Serial.print(",Max:");  Serial.print(_DIST_MAX);
  Serial.println("");

  // update last sampling time
  last_sampling_time += INTERVAL;
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm

  // Pulse duration to distance conversion example (target distance = 17.3m)
  // - round trip distance: 34.6m
  // - expected pulse duration: 0.1 sec, or 100,000us
  // - pulseIn(ECHO, HIGH, timeout) * 0.001 * 0.5 * SND_VEL
  //        = 100,000 micro*sec * 0.001 milli/micro * 0.5 * 346 meter/sec
  //        = 100,000 * 0.001 * 0.5 * 346 * micro * sec * milli * meter
  //                                        ----------------------------
  //                                         micro * sec
  //        = 100 * 173 milli*meter = 17,300 mm = 17.3m
  // pulseIn() returns microseconds.
}

//float median(float arr[],int length){
//  return arr[length/2];
//}

//float sort(float arr[],int length){ // 버블정렬
//  for(int i = 0;i<length-1;i++){
//    for(int j = 0;j<length-1-i;j++){
//      if(arr[j] > arr[j+1]){
//        int temp = arr[j];
//        arr[j] = arr[j+1];
//        arr[j+1] = temp;
//      }
//    }
//  }
//  
//}
