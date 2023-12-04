/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "waterflow.h"
#include <neopixel.h> 
#include <Colors.h>
#include "IoTTimer.h"
#include "math.h"
#include "birddata.h"
#include "Adafruit_PWMServoDriver.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int SERVOPIN=D16;
int servoAngle();
void birdLights (int birdData[31][12], int *startPixel, int *endPixel);
void pixelFill(int startPixel, int endPixel, int hexColor, int month);
void serveUp(int servonum, int angle);
void serveUpNew(int servonum, int angle);
int angle;
//int pixelOn;
int month, day; 
//const unsigned int UPDATE = 2000;
//unsigned int lastAngle;
const int PIXELCOUNT = 24;
int endPixel, startPixel;
const int SERVOMIN = 150; // this is the 'minimum' pulse length count (out of 4096)
const int SERVOMAX = 600; // this is the 'maximum' pulse length count (out of 4096)
int i;
int sAngle;

//Servo myServo;
Adafruit_NeoPixel pixel (PIXELCOUNT, SPI1, WS2812B);
IoTTimer dayTimer;
Adafruit_PWMServoDriver pwm=Adafruit_PWMServoDriver();


// Run the application and system concurrently in separate threads
//SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
//SerialLogHandler logHandler(LOG_LEVEL_INFO);

// setup() runs once, when the device is first turned on
void setup() {
//myServo.attach(SERVOPIN);

  Serial.begin(9600);

  pixel.begin ();
  pixel.show (); // initialize all off
  pixel.setBrightness (24);

  dayTimer.startTimer(1000);

  pwm.begin();
  pwm.setPWMFreq(60);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

  if (dayTimer.isTimerReady()) {
    day++;
    dayTimer.startTimer(1000);
      if (day==30) {
        day=1;
        month++; 
      }
  
angle=servoAngle();
//   // for(i=10;i<=180;i=i+20) {
for (i=0; i<=5; i++) {
serveUpNew(i, angle); //0-180 angle
//   //     delay(2000);
// serveUp(1,angle); //0-180 angle
//   //  delay(2000);
}

  if (angle!=-1){
  //delay (2000);
      //myServo.write(angle);   // how to do multiple servos?
      birdLights (birdData, &startPixel, &endPixel);//function to determine startPixel, endPixel
      pixel.clear(); 
      pixelFill(startPixel, endPixel,teal, month); // how to have variations? 
      
      //pixel.setPixelColor ();
     // pixel.show (); 
    }
  else {
   pixel.clear(); 
  }
  }
}

int servoAngle () {

    angle=map(waterFlow[day][month], 0, 5000, 125, 40); //inverted for servo orientation
    
    if (waterFlow[day][month]<0){
      Serial.printf ("fake date\n");
      return -1;
    }
    else {
      Serial.printf ("Waterflow is %i, angle is %i\n", waterFlow[day][month], angle);
      return angle;
    }
}

void birdLights (int birdData[31][12], int *startPixel, int *endPixel) {
int x;

*startPixel= 0;
*endPixel=map((int)log2(birdData[day][month]), 13, 25, 0, 23);

if (birdData[day][month]==0) { 
  x=random(0,3);
  *endPixel=x;
}

// *startPixel=PIXELCOUNT;
  // *endPixel= PIXELCOUNT-randNumber;
  Serial.printf ("birds are %i\n", birdData[day][month]);
Serial.printf ("Start Pixel is %i, end pixel is %i\n",*startPixel, *endPixel);

}

void pixelFill(int startPixel, int endPixel, int hexColor, int month) {
  //int randNumber;
  int i;



for (i=startPixel; i<=endPixel; i++) {
 if (month<6) {
  pixel.setPixelColor (i, hexColor);
 }
 else {
  pixel.setPixelColor (PIXELCOUNT-i, hexColor);
  
 }
Serial.printf ("I am filling %i to %i\n",startPixel, endPixel);
  pixel.show();

}
}


void serveUpNew(int servonum, int angle) {

  int sAngle;

  sAngle = map(angle,0,180,SERVOMIN,SERVOMAX);
  Serial.printf("Servo Number = %i to Angle = %i\n",servonum,angle);
  //pwm.setPWM(servonum, 0, 165);
  delay(500);
  pwm.setPWM(servonum, 0, sAngle);
}

// void serveUp(int servonum, int angle) {

//   int pulselen;
//   int servoAngle;
//   static int lastAngle = 0;

//   servoAngle = map(angle,0,180,SERVOMIN,SERVOMAX);
//   Serial.printf("Servo Number = %i to Angle = %i\n",servonum,angle);
//   for (pulselen = lastAngle; pulselen < servoAngle; pulselen++) {
//     pwm.setPWM(servonum, 0, pulselen);
//   }
//   lastAngle = servoAngle;
// }