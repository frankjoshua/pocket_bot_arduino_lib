# PocketBot
http://pocketbot.io

Arduino interface for Android PocketBot app

Example:

```
#include <PocketBot.h>

PocketBot pocketBot;
PocketBotMessage message = PocketBotMessage_init_zero;

setup(){
  Serial.begin(115200);
}

loop(){

  if(pocketBot.read(Serial, message)){
        Serial.print(F("FaceID = ")); Serial.println(message.face.faceId);
        Serial.print(F("FaceX = ")); Serial.println(message.face.faceX);
        Serial.print(F("FaceY = ")); Serial.println(message.face.faceY);
        Serial.print(F("FaceZ = ")); Serial.println(message.face.faceZ);
        Serial.print(F("JoyX = ")); Serial.println(message.control.joyX);
        Serial.print(F("JoyY = ")); Serial.println(message.control.joyY);
        Serial.print(F("JoyZ = ")); Serial.println(message.control.joyZ);
        Serial.print(F("Proximity = ")); Serial.println(message.sensor.proximity);
        Serial.print(F("Heading = ")); Serial.println(message.sensor.heading);
  }

}
```
