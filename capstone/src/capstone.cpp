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


// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int SERVOPIN=D16;
float servoAngle();
float angle;
int month, day; 
const unsigned int UPDATE = 2000;
unsigned int lastAngle;
const int PIXELCOUNT = 30;


Servo myServo;
Adafruit_NeoPixel pixel ( PIXELCOUNT , SPI1 , WS2812B );
IoTTimer dayTimer;

// Run the application and system concurrently in separate threads
//SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
//SerialLogHandler logHandler(LOG_LEVEL_INFO);

// setup() runs once, when the device is first turned on
void setup() {
myServo.attach(SERVOPIN);

Serial.begin(9600);

pixel.begin ();
  pixel.show (); // initialize all off
  pixel.setBrightness (24);

dayTimer.startTimer(1000);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

  if (dayTimer.isTimerReady()) {
    day++;
    dayTimer.startTimer(1000);
      if (day==31) {
        day=1;
        month++; 
      }

angle=servoAngle ();
  if (angle!=-1){
  //delay (2000);
      myServo.write(angle);   // how to do multiple servos?
    }
  else {}
  }
    }
  // for (month=0; month<12; month++) {
  //   for (day=0; day<31; day++) {


float servoAngle () {
//float angle;
//int month, day; 


    angle=map(waterFlow[day][month], 0, 5000, 125, 40); //inverted for servo orientation
    
    if (waterFlow[day][month]<0){
      Serial.printf ("fake date\n");
      return -1;
    }
    else {
      Serial.printf ("Waterflow is %i, angle is %f\n", waterFlow[day][month], angle);
      return angle;
    }
 

}
//if value > 0 - eliminates false days
// for loop
