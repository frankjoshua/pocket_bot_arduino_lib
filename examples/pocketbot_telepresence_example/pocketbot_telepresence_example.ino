#include <PocketBot.h> // https://github.com/frankjoshua/PocketBot
#include <Wire.h>
#include <Adafruit_MotorShield.h> // https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library

/* This will be used to decode messages from the Android device */
PocketBot pocketBot;
/* Allocate space for the decoded message. */
PocketBotMessage message = PocketBotMessage_init_zero;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(4);

/* 1 to 255 Lower this number to slow down the robot */
const int maxSpeed = 255;
/* 1 to 255 Lower this number if the robot is turning too sharp */
const int maxTurnSpeed = 255;

    
void setup() {
  //Start motor shield
  AFMS.begin();  // create with the default frequency 1.6KHz
  //Talk to arduino through USB Serial at 115200 baud
  Serial.begin(115200);
}

void loop() {
  //Check for PocketBot message
  if(pocketBot.read(Serial, message)){
    /* This code will only be called if a complete message is received*/
    //Translate the Joystick X, Y (-1.0 <-> 1.0) to motor controler (0 <-> 255, FORWARD, BACKWARD) 
    //Speed
    int throttle = mapfloat(message.control.joyY, -1, 1, -maxSpeed, maxSpeed);
    //Direction
    int dir = mapfloat(message.control.joyX, -1, 1, -maxTurnSpeed, maxTurnSpeed);
    //Left and right power
    int powerL = 0;
    int powerR = 0;
    if(throttle < 0){
      powerL = constrain(throttle + dir, -maxSpeed, maxSpeed);
      powerR = constrain(throttle - dir, -maxSpeed, maxSpeed);
    } else {
      powerL = constrain(throttle + dir, -maxSpeed, maxSpeed);
      powerR = constrain(throttle - dir, -maxSpeed, maxSpeed);
    }
    //Send power values to the motors
    driveMotors(powerL, powerR);
    
  }
}

/*
* Translates values of left and right power to what the motor driver understands
*/
void driveMotors(int leftPower, int rightPower){
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
    //Set motor speeds
    rightMotor->setSpeed(abs(leftPower));
    leftMotor->setSpeed(abs(rightPower));
}

/*
* Helper function to map Floats, based on Arduino map()
*/
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}