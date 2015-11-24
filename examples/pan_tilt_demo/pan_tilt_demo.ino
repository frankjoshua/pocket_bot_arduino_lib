#include <adk.h> //https://github.com/felis/USB_Host_Shield_2.0
#include <usbhub.h> //https://github.com/felis/USB_Host_Shield_2.0
#include <SPI.h>
#include <PocketBot.h> //https://github.com/frankjoshua/PocketBot
#include <Servo.h>

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

/** This will be used to decode messages from the Android device */
PocketBot pocketBot;
/* Allocate space for the decoded message. */
PocketBotMessage message = PocketBotMessage_init_zero;

//Servo Control Pins
#define  SERVO1         6
#define  SERVO2         7
#define  PAN            0
#define  TILT           1
#define  SERVO_MAX      190
#define  SERVO_MIN      125
Servo servos[2];
int servoPos[2];
int servoDest[2];

void setup(void){
  Serial.begin(115200);
  initServos();
  
  //Connect to Android device
  if(Usb.Init() == -1) {
    Serial.println(F("OSCOKIRQ failed to assert"));
    while(1); //halt
  }
  Serial.println(F("Waiting for Android device..."));
  while(adk.isReady() == false){
     Usb.Task();
     delay(1);
  }
  Serial.println("");
  Serial.println(F("Connected"));
  
  
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
        Serial.println(millis());
        Serial.print(F("FaceID = ")); Serial.println(message.face.faceId);
        Serial.print(F("FaceX = ")); Serial.println(message.face.faceX);
        Serial.print(F("FaceY = ")); Serial.println(message.face.faceY);
        Serial.print(F("FaceZ = ")); Serial.println(message.face.faceZ);
        Serial.print(F("JoyX = ")); Serial.println(message.control.joyX);
        Serial.print(F("JoyY = ")); Serial.println(message.control.joyY);
        Serial.print(F("JoyZ = ")); Serial.println(message.control.joyZ);
        Serial.print(F("Proximity = ")); Serial.println(message.sensor.proximity);
        Serial.print(F("Heading = ")); Serial.println(message.sensor.heading);
        if(millis() % 2 == 0){
          //Track face with servos
          if(message.face.faceX > 1.1){
            servoDest[PAN]++;
          } else if(message.face.faceX < .9){
            servoDest[PAN]--;
          }
          if(message.face.faceY > .85){
            servoDest[TILT]--;
          } else if(message.face.faceY < .65){
            servoDest[TILT]++;
          }
        }
        //servoTask();
      }
      rcode = adk.RcvData(&len, msg);
    }
  }
  
  servoTask();
  
}

void servoTask(){
  boolean update = false;
  //Make sure Servos are in range
  if(servoDest[PAN] > SERVO_MAX){
      servoDest[PAN] = SERVO_MAX;
  } else if(servoDest[PAN] < SERVO_MIN){
      servoDest[PAN] = SERVO_MIN;
  }
  if(servoDest[TILT] > SERVO_MAX){
      servoDest[TILT] = SERVO_MAX;
  } else if(servoDest[TILT] < SERVO_MIN){
      servoDest[TILT] = SERVO_MIN;
  }
  //Update Servo positions
  if(servoDest[PAN] > servoPos[PAN]){
    servoPos[PAN]++;
    update = true;
  } else if(servoDest[PAN] < servoPos[PAN]){
    servoPos[PAN]--;
    update = true;
  }
  if(servoDest[TILT] > servoPos[TILT]){
    servoPos[TILT]++;
    update = true;
  } else if(servoDest[TILT] < servoPos[TILT]){
    servoPos[TILT]--;
    update = true;
  }
  
  if(update){
    updateServos(); 
    Serial.println("Moving Servos");
  }
}

void updateServos(){
  setServo(PAN, servoPos[PAN]);
  setServo(TILT, servoPos[TILT]);
}

void initServos(){  
  
  servos[PAN].attach(SERVO1);
  servos[TILT].attach(SERVO2);
  int center = SERVO_MIN + (SERVO_MAX - SERVO_MIN) / 2;
  setServo(PAN, center);
  setServo(TILT, center);
  //Test servo range
  testServoRange(PAN);
  testServoRange(TILT);
  setServo(PAN, center);
  servoDest[PAN] = center;
  setServo(TILT, center);
  servoDest[TILT] = center;
  updateServos();
}

void setServo(int servo, int pos){
   servos[servo].write(pos);
   servoPos[servo] = pos; 
}

void testServoRange(int servo){
  int center = SERVO_MIN + (SERVO_MAX - SERVO_MIN) / 2;
  for(int l = center; l >= SERVO_MIN; l--){
    setServo(servo, l);
    delay(25);
    Serial.println(l);
  }
  for(int i = SERVO_MIN; i <= SERVO_MAX; i++){
    setServo(servo, i);
    delay(25);
    Serial.println(i);
  }
  for(int j = SERVO_MAX; j >= SERVO_MIN; j--){
    setServo(servo, j);
    delay(25);
    Serial.println(j);
  }
  for(int k = SERVO_MIN; k <= center; k++){
    setServo(servo, k);
    delay(25);
    Serial.println(k);
  }
}

void moveServo(int servo, int dest){
   int pos = servoPos[servo] + dest;
   setServo(servo, pos);
}
