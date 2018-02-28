/*
* Example of connecting PocketBot running on Android
* to an Arduino using USB Serial via an OTG Cable
* Tested with Adruino IDE 1.6.9
* http://pocketbot.io
*/

//DC Motors with L293D

//Define Pins
//Motor A
const int ENABLE_A = 5;
const int MOTOR_A1 = 6;
const int MOTOR_A2 = 7;
 
//Motor B
const int ENABLE_B = 10;
const int MOTOR_B1 = 9;
const int MOTOR_B2 = 8;

#include <PocketBot.h> // https://github.com/frankjoshua/PocketBot

/** This will be used to decode messages from the Android device */
PocketBot pocketBot;
/* Allocate space for the decoded message. */
PocketBotMessage message = PocketBotMessage_init_zero;


void setup() {

  //configure pin modes
  pinMode (ENABLE_A, OUTPUT);
  pinMode (MOTOR_A1, OUTPUT);
  pinMode (MOTOR_A2, OUTPUT);  
  
  pinMode (ENABLE_B, OUTPUT);
  pinMode (MOTOR_B1, OUTPUT);
  pinMode (MOTOR_B2, OUTPUT);  

  //Turn on Serial to talk to phone over USB
  Serial.begin(115200);
  
}

void loop() {

  //Check for new message from Pocketbot app
  if(pocketBot.read(Serial, message)){
    //joy Y is speed from -1.0 to 1.0
    //joy X is turn rate from -1.0 to 10
    float leftSpeed = message.control.joy1.Y + message.control.joy1.X;
    float rightSpeed = message.control.joy1.Y - message.control.joy1.X;
    //Send commands to motors
    driveMotor(ENABLE_A, MOTOR_A1, MOTOR_A2, leftSpeed);  
    driveMotor(ENABLE_B, MOTOR_B1, MOTOR_B2, rightSpeed);     
  }
  
}

void driveMotor(int enablePin, int pinA, int pinB, float speed){
  if(speed < 0){
    //Set direction to reverse
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
  } else {
    //Set direction forward
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);  
  }
  //Set motor speed
  //PWM would be from 0 to 255 and speed is -1.0 to 1.0
  analogWrite(enablePin, 255 * abs(speed));
}

