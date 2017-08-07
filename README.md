#PocketBot
http://pocketbot.io

You can get the Android app at https://play.google.com/store/apps/details?id=com.tesseractmobile.pocketbot

Arduino interface for Android PocketBot app

This is an arduino library that is meant to be used to receive messages from the PocketBot Android app.

Quick Start Example:

This example shows how to use a USB OTG cable to connect directly to an Arduino Uno. If everything is working correctly PocketBot should launch when the Arduino is plugged in.

![alt tag](http://pocketbot.io/wp-content/uploads/2015/11/pocketbot_otg.jpg)

```
#include <PocketBot.h> //https://github.com/frankjoshua/PocketBot

/** This will be used to decode messages from the Android device */
PocketBot pocketBot;
/* Allocate space for the decoded message. */
PocketBotMessage message = PocketBotMessage_init_zero;

void setup(void){
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop(void){
  
  if(pocketBot.read(Serial, message)){
   if(message.face.id != -1){
     //Turn on led is a face is seen
     digitalWrite(13, HIGH);
   } else {
     //Turn off LED if no face is seen
     digitalWrite(13, LOW);
   }
  }
    
}
```
####Protobuf Format
Example: to get the current compass heading in degrees  
int heading = message.sensor.heading;

Here is the Protobuf .proto file
```
message PocketBotMessage {
  required Face face = 1;
  required Control control = 2;
  required Sensor sensor = 3;
}

message Face {
  //-1 == no face, faceId increases when new face is detected
  required sint32 id = 1;
  //1.0 == center, 2.0 == left of robot, 0.0 == right of robot
  required float X = 2;
  //1.0 == center, 2.0 == below robot, 0.0 == above robot
  required float Y = 3;
  //1.0 == close, 0.0 == far, 0.5 == medium
  required float Z = 4;
}

message Sensor {
  //if something within 8cm == true
  required bool proximity = 1;
  //Compass heading in degrees 0-359
  required int32 heading = 2;
  //Current GPS position
  required Gps gps = 3;
}

message Control {
  //Right Joystick Control
  required Joystick joy1 = 1;
  //Left Joystick Control
  required Joystick joy2 = 2;
}

message Joystick {
  //-1.0 == Left, 1.0 == Right, 0.0 == Center 
  required float X = 1;
  //-1.0 == Down, 1.0 == Up, 0.0 == Center
  required float Y = 2;
  //Pressure of user touch 0 == Not touching, 1 == Full pressure
  required float Z = 3;
  //Button A
  required bool A = 4;
  //Button B
  required bool B = 5;
  //Direction in degrees 0 - 359, 0 == up
  required int32 heading = 6;
}

//Not currently enabled
message Gps {
  required float lat = 1;
  required float lon = 2;
}
```

