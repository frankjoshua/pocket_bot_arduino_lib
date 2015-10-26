#include <Arduino.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson
#include <Usb.h> //https://github.com/felis/USB_Host_Shield
#include <AndroidAccessory.h>
#include <PocketBot.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <RunningMedian.h>

/*
* These parameters must match what the app expects
*/
AndroidAccessory acc("Tesseract Mobile LLC",
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

PocketBot pocketBot;

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
        
  acc.powerOn();
  pocketBot.begin();
  
  Serial.println(F("Waiting for Android device"));
  while(acc.isConnected() == false){
    Serial.print(".");
    delay(250); 
  }
  
  // set pin for sonar
  pinMode(SonarPin, INPUT);
}

void loop(void){

  if(acc.isConnected()){ 
    byte in[1];
    while(acc.read(in, sizeof(in), 1) > 0){
      char input = (char) in[0];
      Serial.print(input);
      //pocketBot.printRawTo(Serial);
      //Serial.println("");
      if(pocketBot.read(input)){
        JsonObject& root = pocketBot.getJson();
        if(!root.success()){
         Serial.println("");
         Serial.println(F("*************** JSON ERROR **************")); 
         return;
        }
        
        //Serial.println("JSON");
        for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it){
          Serial.print(it->key);
          //Serial.print(" = ");
          Serial.println(it->value.asString());
        }
        Serial.println("");
        mProximityAlert = root[F("proximity")];
        int faceId = root[F("face_id")];
        if(faceId > 0){
          mRoamingMode = false;
          //Get face location
          float x = root[F("face_x")];
          float z = root[F("face_z")];
          //Navigate towards face
          if(x > 1.1){
            int s = mapfloat(x, 1, 2, 50, 100);
            Serial.println(s);
            rightMotor->run(FORWARD);
            rightMotor->setSpeed(s);
            leftMotor->run(BACKWARD);
            leftMotor->setSpeed(s);
          } else if(x < .9){
            int s = mapfloat(x, 0.0, 1, 100, 50);
            Serial.println(s);
            rightMotor->run(BACKWARD);
            rightMotor->setSpeed(s);
            leftMotor->run(FORWARD);
            leftMotor->setSpeed(s);
          } else if (z > .3 && z < .45){
            move(BACKWARD, 100);
          } else if ( z > .5) {
            move(FORWARD, 100);
          }else {
            rightMotor->setSpeed(0);
            leftMotor->setSpeed(0);
          }
        } else {
          mRoamingMode = true;
        }
        mLastUpdate = millis();
      }
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
         move(BACKWARD, 255);
      } else if(mDistance > 12){
        //slight turn  
        rightMotor->run(BACKWARD);
        rightMotor->setSpeed(200);
        leftMotor->run(BACKWARD);
        leftMotor->setSpeed(255);     
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

