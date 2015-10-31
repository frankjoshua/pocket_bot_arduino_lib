#include <Usb.h> //https://github.com/felis/USB_Host_Shield
#include <AndroidAccessory.h> //https://github.com/bblanchon/ArduinoJson
#include <PocketBot.h>

/*
* These parameters must match what the app expects
*/
AndroidAccessory acc("Tesseract Mobile LLC",
		     "PocketBot",
		     "",
		     "1.0",
		     "http://pocketbot.io",
		     "1");

/** This will be used to decode messages from the Android device */
PocketBot pocketBot;
/* Allocate space for the decoded message. */
PocketBotMessage message = PocketBotMessage_init_zero;

void setup(void){
  Serial.begin(115200);
  
  acc.powerOn();
}

byte in[1];
void loop(void){

  if(acc.isConnected()){ 
    while(acc.read(in, sizeof(in), 1) > 0){
      if(pocketBot.read(in[0], message)){
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
      }
    }
  }
}

