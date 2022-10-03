// Arduino pin assignment
#define PIN_LED 9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

#define mid 200

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 30      // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300.0   // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance

unsigned long last_sampling_time;   // unit: msec

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 
  
  // initialize serial port
  Serial.begin(57600);
}

void loop() {
  float distance;

  // wait until next sampling time. 
  // millis() returns the number of milliseconds since the program started.
  //    Will overflow after 50 days.
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  distance = USS_measure(PIN_TRIG, PIN_ECHO); // read distance

  if (distance < _DIST_MIN) {
    //analogWrite(PIN_LED,255);
    distance = _DIST_MIN - 10.0;    // Set Lower Value
    //digitalWrite(PIN_LED, 1);       // LED OFF
  } else if (distance > _DIST_MAX) {
    //analogWrite(PIN_LED,255);
    distance = _DIST_MAX + 10.0;    // Set Higher Value
    //digitalWrite(PIN_LED, 1);       // LED OFF
  } else {    // In desired Range
    //digitalWrite(PIN_LED, 0);       // LED ON      
  }

//  int bright;
//  if(distance < mid){
//    bright = map(distance,_DIST_MIN-1,mid,0,255);
//  }else if(distance >=mid){
//    bright = map(distance,mid,_DIST_MAX+1,255,0);
//  }
  int bright = ledController((int)distance);
  analogWrite(PIN_LED,bright);

  // output the distance to the serial port
  Serial.print("Min:");        Serial.print(_DIST_MIN);
  Serial.print(",distance:");  Serial.print(distance);
  Serial.print(",Max:");       Serial.print(_DIST_MAX);
  Serial.print(",bright:");       Serial.print(bright);
  Serial.println("");
  
  // do something here
  
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

int ledController(int distance){
  if(distance<100)
    return 255;

  if(distance>300)
    return 255;
    
  int a = 200-distance;
  int b = distance-200;
  
  int bright = (((a) > (b)) ? (a) : (b));
  return (int)255*(bright/100.0);
}
