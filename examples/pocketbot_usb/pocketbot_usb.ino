#include <Arduino.h>
#include <ArduinoJson.h>
#include <Usb.h> //https://github.com/felis/USB_Host_Shield
#include <AndroidAccessory.h>
#include <MemoryFree.h>
#include <PocketBot.h>

AndroidAccessory acc("Google, Inc.",
		     "DemoKit",
		     "DemoKit Arduino Board",
		     "1.0",
		     "http://www.android.com",
		     "0000000012345678");

PocketBot pocketBot;

long mLastTime = 0;
long mLastLocalTime = 0;

void setup(void){
  Serial.begin(115200);

  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  
  acc.powerOn();
  pocketBot.begin();
}

void loop(void){

  
//  if (Serial.available())
//  {
//    n = Serial.readBytes(inputs, BUFSIZE);
//    inputs[n] = 0;
//    // Send characters to Bluefruit
//    Serial.print("Sending: ");
//    Serial.print(inputs);
//
//    // Send input data to host via Bluefruit
//    ble.print(inputs);
//  }

  if(acc.isConnected()){
    byte in[1];
    while(acc.read(in, sizeof(in), 1) > 0){
      char input = (char) in[0];
      Serial.print(input);
      if(pocketBot.read(input)){
        JsonObject& root = pocketBot.getJson();
        if(!root.success()){
         Serial.println("*************** JSON ERROR **************"); 
         return;
        }
        
        Serial.println("JSON");
        root.printTo(Serial);
        Serial.println("");
        
        for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it){
          Serial.print(it->key);
          Serial.print(" = ");
          Serial.println(it->value.asString());
        }
        
        long tMills = root["time"];
        Serial.print(tMills);
        Serial.print("-");
        Serial.print(mLastTime);
        long mills = millis();
        int timeDiff = tMills - mLastTime;
        int localTimeDiff = mills - mLastLocalTime;
        mLastTime = tMills;
        mLastLocalTime = mills;
        Serial.print("Tansmision time = ");
        Serial.println(timeDiff);
        Serial.print("Local time = ");
        Serial.println(localTimeDiff);
        int lag = localTimeDiff - timeDiff;
        Serial.print("lag = ");
        Serial.println(lag);
      }
    }
  }
  
  //Debuging
//  Serial.print(millis());
//  Serial.print(" ");
//  Serial.print("freeMemory()=");
//  Serial.println(freeMemory());
  //pocketBot.printRawTo(Serial);

}
