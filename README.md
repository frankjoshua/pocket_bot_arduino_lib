# PocketBot
Arduino interface for Android PocketBot app

Requires ArduinoJson library https://github.com/frankjoshua/ArduinoJson

Example:

```
#include <PocketBot.h>
#include <ArduinoJson.h>

PocketBot pocketBot;

setup(){
  
  Serial.begin(115200);
  
  pocketBot.begin(&Serial);
  
}

loop(){

  if(pocketBot.read()){
    JsonObject& root = pocketBot.getJson();
    const char* sensor = root["sensor"];
    long time          = root["time"];
    double latitude    = root["data"][0];
    double longitude   = root["data"][1];
  }

}
```
