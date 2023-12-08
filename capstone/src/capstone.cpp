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
#include "credentials.h"
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

TCPClient TheClient; 
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY); 

Adafruit_MQTT_Subscribe waterButton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/waterbutton"); 

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
const int PIXELCOUNT = 23;
int endPixel, startPixel;
const int SERVOMIN = 150; // this is the 'minimum' pulse length count (out of 4096)
const int SERVOMAX = 600; // this is the 'maximum' pulse length count (out of 4096)
int i;
int sAngle;
float subValue;

//Servo myServo;
Adafruit_NeoPixel pixel (PIXELCOUNT, SPI1, WS2812B);
IoTTimer dayTimer;
Adafruit_PWMServoDriver pwm=Adafruit_PWMServoDriver();

void MQTT_connect();
bool MQTT_ping();

SerialLogHandler logHandler(LOG_LEVEL_INFO);

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

    WiFi.on();
  WiFi.connect();
  while(WiFi.connecting()) {
    Serial.printf(".");
  }
  Serial.printf("\n\n");

mqtt.subscribe(&waterButton);


}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

MQTT_connect();
  MQTT_ping();

  if (dayTimer.isTimerReady()) {
    day++;
    dayTimer.startTimer(1000);
      if (day==30) {
        day=1;
        month++; 
      }
  
angle=servoAngle();
//   // for(i=10;i<=180;i=i+20) {
for (i=0; i<6; i++) {
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

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(10000))) {
    if (subscription == &waterButton) {
      subValue = atof((char *)waterButton.lastread);
      if (subValue == 1){
      Serial.printf ("new submission!\n");
      pixel.clear(); 
      pixelFill(0, 23, magenta, month);
      delay (1000);
      }
        else  {}
    }
  }
}

void MQTT_connect() {
  int8_t ret;
 
  // Return if already connected.
  if (mqtt.connected()) {
    return;
  }
 
  Serial.print("Connecting to MQTT... ");
 
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.printf("Error Code %s\n",mqtt.connectErrorString(ret));
       Serial.printf("Retrying MQTT connection in 5 seconds...\n");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds and try again
  }
  Serial.printf("MQTT Connected!\n");
}

bool MQTT_ping() {
  static unsigned int last;
  bool pingStatus;

  if ((millis()-last)>120000) {
      Serial.printf("Pinging MQTT \n");
      pingStatus = mqtt.ping();
      if(!pingStatus) {
        Serial.printf("Disconnecting \n");
        mqtt.disconnect();
      }
      last = millis();
  }
  return pingStatus;
}

int servoAngle () {

    angle=map(waterFlow[day][month], 0, 5000, 10, 165); //inverted for servo orientation
    
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
  delay(250);
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