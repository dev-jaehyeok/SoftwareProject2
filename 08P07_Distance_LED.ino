#define PIN_LED  9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24°C (m/s)
#define INTERVAL 25        // sampling interval (msec)
#define PULSE_DURATION 10  // ultra-sound Pulse Duration (usec)
#define _DIST_MIN 100.0    // minimum measurable distance (mm)
#define _DIST_MAX 300.0    // maximum measurable distance (mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // convert duration to distance (mm)

unsigned long last_sampling_time = 0;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  Serial.begin(57600);
}

void loop() {
  
  if (millis() < last_sampling_time + INTERVAL) return;
  last_sampling_time = millis();

  float distance = USS_measure(PIN_TRIG, PIN_ECHO);

  if (distance < _DIST_MIN || distance > _DIST_MAX || distance == 0) {
    analogWrite(PIN_LED, 255); 
  } else {
    int brightness = calcBrightness(distance);
    analogWrite(PIN_LED, brightness);
  }

  Serial.print("Distance(mm): ");
  Serial.print(distance);
  Serial.print(" | LED PWM: ");
  Serial.println(calcBrightness(distance));
}

int calcBrightness(float d) {
  float brightness;

  if (d <= 200.0)
    brightness = 255.0 * (1.0 - (d - 100.0) / 100.0);  
  else
    brightness = 255.0 * ((d - 200.0) / 100.0);       

  if (brightness < 0) brightness = 0;
  if (brightness > 255) brightness = 255;

  return (int)brightness;
}

float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  float duration = pulseIn(ECHO, HIGH, TIMEOUT);
  float dist = duration * SCALE; 
  return dist;
}
