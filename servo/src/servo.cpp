#include "Particle.h"
#include "Adafruit_PWMServoDriver.h"
#include "math.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const int SERVOMIN = 150; // this is the 'minimum' pulse length count (out of 4096)
const int SERVOMAX = 600; // this is the 'maximum' pulse length count (out of 4096)

int i,bri;
float t;
int whichServo = 1;
int pulselen;

void angleTest();
void wave();
void setServoPulse(uint8_t n, double pulse);
void serveUp(int servonum, int angle);

SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
}

void loop() {
  for(i=10;i<=180;i=i+20) {
    serveUp(whichServo,i);
    delay(2000);
  }

  //angleTest();
  //wave();
}

void angleTest() {
  for(i=150;i<600;i=i+10) {
    Serial.printf("angle = %i\n",i);
    pwm.setPWM(14,0,i);
    pwm.setPWM(15,0,i);
    delay(1000);
  }
}

void wave() {
  t = millis()/1000.0;
  bri = 225*sin(2*M_PI*0.05*t)+375;
  Serial.printf("bri = %i\n",bri);
  pwm.setPWM(14,0,bri);
  pwm.setPWM(15,0,bri); 
}

void serveUp(int servonum, int angle) {

  int pulselen;
  int servoAngle;
  static int lastAngle = 0;

  servoAngle = map(angle,0,180,SERVOMIN,SERVOMAX);
  Serial.printf("Servo Number = %i to Angle = %i\n",servonum,angle);
  for (pulselen = lastAngle; pulselen < servoAngle; pulselen++) {
    pwm.setPWM(servonum, 0, pulselen);
  }
  lastAngle = servoAngle;
}


// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  float pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}