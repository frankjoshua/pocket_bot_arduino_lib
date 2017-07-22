#include <PocketBot.h> // https://github.com/frankjoshua/PocketBot
#include <Wire.h>

/* This will be used to decode messages from the Android device */
PocketBot pocketBot;
/* Allocate space for the decoded message. */
PocketBotMessage message = PocketBotMessage_init_zero;

/* 
* Motor Drivers
* Set your motor driver to 1 to activate it, set any unused to 0
*/
#define ADAFRUIT 0
#define SABERTOOTH 1
/* End Motor drivers */

#if ADAFRUIT
  #include <Adafruit_MotorShield.h> // https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
  // Create the motor shield object with the default I2C address
  Adafruit_MotorShield AFMS = Adafruit_MotorShield();
  Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
  Adafruit_DCMotor *rightMotor = AFMS.getMotor(4);
#endif

#if SABERTOOTH
  #include <SabertoothSimplified.h>
  #include <AltSoftSerial.h>
  //Can't use serial because PocketBot is using it
  //Unlike SoftSerial, AltSoftSerial doesn't interfere with Hardware Serial
  AltSoftSerial SWSerial; //https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
  SabertoothSimplified ST(SWSerial); // Use SWSerial as the serial port.
  #define LEFT 2
  #define RIGHT 1
#endif


/* 1 to 127 Lower this number to slow down the robot */
const int maxSpeed = 127;
/* 1 to 127 Lower this number if the robot is turning too sharp */
const int maxTurnSpeed = 60;

//Track time between messages
long mLastCommTime = 0;

int leftPower = 0;
int rightPower = 0;

void setup() {
  //Start motor shield
  #if ADAFRUIT
    AFMS.begin();  // create with the default frequency 1.6KHz
  #endif
  #if SABERTOOTH
    SWSerial.begin(9600);
  #endif
  //Talk to arduino through USB Serial at 115200 baud
  Serial.begin(115200);
  
  pinMode(13, OUTPUT);
}

bool flip;
void loop() {
  //Check for PocketBot message
  if(pocketBot.read(Serial, message)){
    /* This code will only be called if a complete message is received*/
    mLastCommTime = millis();
    //Toggle LED 13
    flip = !flip;
    if(flip){
      digitalWrite(13, HIGH);
    } else {
      digitalWrite(13, LOW);
    }
    //Translate the Joystick X, Y (-1.0 <-> 1.0) to motor controler (0 <-> 255, FORWARD, BACKWARD) 
    //Speed
    int throttle = mapfloat(message.control.joy1.Y, -1, 1, -maxSpeed, maxSpeed);
    //Direction
    int dir = mapfloat(message.control.joy1.X, -1, 1, -maxTurnSpeed, maxTurnSpeed);
    //Left and right power
    int powerL = 0;
    int powerR = 0;
    powerL = constrain(throttle + dir, -maxSpeed, maxSpeed);
    powerR = constrain(throttle - dir, -maxSpeed, maxSpeed);
    //Send power values to the motors
    leftPower = powerL;
    rightPower = powerR; 
  }
  
  //If too much time has passed kill the motors
  if(millis() - mLastCommTime > 200){
    mLastCommTime += 10;
    //Drop power by 1%
    leftPower = leftPower * .99;
    rightPower = rightPower * .99; 
  }
  
  driveMotors(leftPower, rightPower);
}

/*
* Translates values of left and right power to what the motor driver understands
* TODO: Change the motor speed slowly to avoid jerking the robot
*/
int mLastLeft = 0;
int mLastRight = 0;
void driveMotors(int leftPower, int rightPower){
  //If no change just return
  if(mLastLeft == leftPower && mLastRight == rightPower){
     return;
  }
  
  //Save the last input values
  mLastLeft = leftPower;
  mLastRight = rightPower;
  
  #if ADAFRUIT
    //Set motors to FORWARD or BACKWARD
    if(leftPower < 0){
      leftMotor->run(BACKWARD);
    } else {
      leftMotor->run(FORWARD);
    }
    if(rightPower < 0){
      rightMotor->run(BACKWARD);
    } else {
      rightMotor->run(FORWARD);
    }
    //Adjust power from -127:127 to 1:255
    leftPower = abs(leftPower) * 2 + 1;
    rightPower = abs(rightPower) * 2 + 1;
    //Set motor speeds
    rightMotor->setSpeed(rightPower);
    leftMotor->setSpeed(leftPower);
  #endif
  #if SABERTOOTH
    ST.motor(RIGHT, rightPower);
    ST.motor(LEFT, leftPower);
  #endif
}

/*
* Helper function to map Floats, based on Arduino map()
*/
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
