#include <adk.h> //https://github.com/felis/USB_Host_Shield_2.0
#include <usbhub.h> //https://github.com/felis/USB_Host_Shield_2.0
#include <SPI.h>
#include <PocketBot.h> //https://github.com/frankjoshua/PocketBot
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <RunningMedian.h>

//Init USB connection for android device
USB Usb;
//These two lines give support for using a powered USB hub
USBHub hub0(&Usb);
USBHub hub1(&Usb);

/*
* These parameters must match what the app expects
*/
ADK adk(&Usb, "Tesseract Mobile LLC",
		     "PocketBot",
		     "",
		     "1.0",
		     "http://pocketbot.io",
		     "1");

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Select which 'port' M1, M2, M3 or M4
Adafruit_DCMotor *rightMotor = AFMS.getMotor(3);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4);

RunningMedian avg(5);

/** This will be used to decode messages from the Android device */
PocketBot pocketBot;
/* Allocate space for the decoded message. */
PocketBotMessage message = PocketBotMessage_init_zero;

long mLastUpdate = 0;
long mLastSensorRead = 0;
boolean mRoamingMode = true;
const int SonarPin = A0;
boolean mProximityAlert = false;

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
  
void setup(void){
  Serial.begin(115200);

  AFMS.begin();
  rightMotor->run(RELEASE);
  leftMotor->run(RELEASE);
  
 //Connect to Android device
  if(Usb.Init() == -1) {
    Serial.println(F("OSCOKIRQ failed to assert"));
    while(1); //halt
  }
  Serial.println(F("Waiting for Android device..."));
  while(adk.isReady() == false){
     Usb.Task();
  }
  Serial.println("");
  Serial.println("Connected");
  
  // set pin for sonar
  pinMode(SonarPin, INPUT);
}

void loop(void){
  uint8_t rcode;
  byte msg[1] = { 0x00 };
  uint16_t len = sizeof(msg);
  Usb.Task();
  if(adk.isReady()){ 
    len = sizeof(msg);
    rcode = adk.RcvData(&len, msg);
    while(!rcode && len > 0){
      Usb.Task();
      if(pocketBot.read(msg[0], message)){
     
        mProximityAlert = message.sensor.proximity;
        int faceId = message.face.faceId;
        if(faceId > 0){
          mRoamingMode = false;
          //Get face location
          float x = message.face.faceX;
          float z = message.face.faceZ;
          //Navigate towards face
          if(x > 1.1){
            int s = mapfloat(x, 1, 2, 30, 70);
            Serial.println(s);
            rightMotor->run(FORWARD);
            rightMotor->setSpeed(s);
            leftMotor->run(BACKWARD);
            leftMotor->setSpeed(s);
          } else if(x < .9){
            int s = mapfloat(x, 0.0, 1, 70, 30);
            Serial.println(s);
            rightMotor->run(BACKWARD);
            rightMotor->setSpeed(s);
            leftMotor->run(FORWARD);
            leftMotor->setSpeed(s);
          } 
          else if (z > .3 && z < .45){
            move(BACKWARD, 100);
          } else if ( z > .5) {
            move(FORWARD, 100);
          }
          else {
            rightMotor->setSpeed(0);
            leftMotor->setSpeed(0);
          }
        } else {
          mRoamingMode = true;
        }
        mLastUpdate = millis();
      }
      rcode = adk.RcvData(&len, msg);
    }
  }

  if(mRoamingMode){
    if(mProximityAlert){
      //backup and turn  
      rightMotor->run(FORWARD);
      rightMotor->setSpeed(255);
      leftMotor->run(FORWARD);
      leftMotor->setSpeed(200); 
    } else if(millis() - mLastSensorRead > 100){
      mLastSensorRead = millis();
      //Read from sonar sensor
      //Add to running median
      //int pulse = analogRead(SonarPin);
      avg.add(analogRead(SonarPin));
      int mDistance = avg.getMedian() * 0.497;
      Serial.println(mDistance);
      //If no obsticals move forward
      if(mDistance > 24){
        //Forward
         move(BACKWARD, 100);
      } else if(mDistance > 12){
        //slight turn  
        rightMotor->run(BACKWARD);
        rightMotor->setSpeed(75);
        leftMotor->run(BACKWARD);
        leftMotor->setSpeed(100);     
      } else {
        //turn
        rightMotor->run(FORWARD);
        rightMotor->setSpeed(150);
        leftMotor->run(BACKWARD);
        leftMotor->setSpeed(150);
      }
    }
  } 
  
  if(millis() - mLastUpdate > 400){
     mLastUpdate = millis();
     mRoamingMode = true;
  }
  
  
}

void move(int dir, int speed){
  rightMotor->run(dir);
  rightMotor->setSpeed(speed);
  leftMotor->run(dir);
  leftMotor->setSpeed(speed);
}
