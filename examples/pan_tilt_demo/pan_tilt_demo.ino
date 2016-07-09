/*
* Example of connecting PocketBot running on Android
* to an Arduino using USB Serial via an OTG Cable
* Tested with Adruino IDE 1.6.9
* http://pocketbot.io
*/
#include <PocketBot.h> // https://github.com/frankjoshua/PocketBot
#include <Servo.h>

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
#define  SERVO_MAX_TILT 170
#define  SERVO_MIN_TILT 145
#define  SPEED          1

Servo servos[2];
int servoPos[2];
int servoDest[2];

void setup(void){
  Serial.begin(115200);
  initServos();
}

void loop(void){

  if(pocketBot.read(Serial, message)){
    //Track face with servos
    if(message.face.X > 1.1){
      servoDest[PAN] += SPEED;
    } else if(message.face.X < .9){
      servoDest[PAN] -= SPEED;
    }
    if(message.face.Y > .85){
      servoDest[TILT] -= SPEED;
    } else if(message.face.Y < .65){
      servoDest[TILT] += SPEED;
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
  if(servoDest[TILT] > SERVO_MAX_TILT){
      servoDest[TILT] = SERVO_MAX_TILT;
  } else if(servoDest[TILT] < SERVO_MIN_TILT){
      servoDest[TILT] = SERVO_MIN_TILT;
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
  servoDest[PAN] = center;
  servoDest[TILT] = center;
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
  }
  for(int i = SERVO_MIN; i <= SERVO_MAX; i++){
    setServo(servo, i);
    delay(25);
  }
  for(int j = SERVO_MAX; j >= SERVO_MIN; j--){
    setServo(servo, j);
    delay(25);
  }
  for(int k = SERVO_MIN; k <= center; k++){
    setServo(servo, k);
    delay(25);
  }
}

void moveServo(int servo, int dest){
   int pos = servoPos[servo] + dest;
   setServo(servo, pos);
}
