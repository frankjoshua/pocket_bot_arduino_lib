# PocketBot
http://pocketbot.io

Arduino interface for Android PocketBot app

This is an arduino library that is meant to be used to receive messages from the PocketBot Android app.

Quick Start Example:

This example shows how to use a USB OTG cable to connect directly to an Arduino Uno.

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
