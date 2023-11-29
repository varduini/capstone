/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
Servo myServo;

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int SERVOPIN=D16;
float servoAngle (int waterflow);

// Run the application and system concurrently in separate threads
//SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
//SerialLogHandler logHandler(LOG_LEVEL_INFO);

// setup() runs once, when the device is first turned on
void setup() {
myServo.attach(SERVOPIN);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  //servoAngle ();
  myServo.write(angle);
  delay (2000);

}

float servoAngle (int waterflow) {
float angle;
int i, x; 

// create int to run through aray
int waterFlow [i][x];

//if value > 0 - eliminates false days

// for loop

angle = map(waterflow, 0, 5000, 40, 125);
return angle;


}