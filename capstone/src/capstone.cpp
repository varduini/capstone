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
Servo myServo;

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int SERVOPIN=D16;
float servoAngle();
float angle;
int month, day; 

// Run the application and system concurrently in separate threads
//SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
//SerialLogHandler logHandler(LOG_LEVEL_INFO);

// setup() runs once, when the device is first turned on
void setup() {
myServo.attach(SERVOPIN);

Serial.begin(9600);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  
  for (month=0; month<12; month++) {
    for (day=0; day<31; day++) {
  
  angle=servoAngle ();
    if (angle!=-1){
  myServo.write(angle);
  delay (2000);
    }
    else {}
  }
  }

}

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
