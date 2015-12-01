#PocketBot
http://pocketbot.io

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
   if(message.face.faceId != -1){
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
int heading = message.Sensor.heading;

Here is the Protobuf .proto file
```
message PocketBotMessage {
  required Face face = 1;
  required Control control = 2;
  required Gps gps = 3;
  required Sensor sensor = 4;
}

message Face {
  required sint32 faceId = 1;
  required float faceX = 2;
  required float faceY = 3;
  required float faceZ = 4;
}

message Control {
  required float joyX = 1;
  required float joyY = 2;
  required float joyZ = 3;
  required bool buttonA = 4;
  required bool buttonB = 5;
  required int32 destHeading = 6;
}

message Gps {
  required float lat = 1;
  required float lon = 2;
}

message Sensor {
	required bool proximity = 1;
	required int32 heading = 2;
}
```

