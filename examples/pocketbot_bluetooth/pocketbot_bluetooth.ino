#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <MemoryFree.h>

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"


#include <PocketBot.h>


#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              6
#define BLUEFRUIT_SPI_SCK              13
#define BLUEFRUIT_SPI_MISO             12
#define BLUEFRUIT_SPI_MOSI             11
/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
//Use with UNO
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Use with Mega
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

PocketBot pocketBot;

long mLastTime = 0;
long mLastLocalTime = 0;

void setup(void){
  Serial.begin(115200);
  
  initBluetooth();

  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  
  pocketBot.begin(&ble);
}

void initBluetooth(){
     /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(true) )
  {
    Serial.println(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
    while(1);
  }
  Serial.println( F("OK!") );
  /* Disable command echo from Bluefruit */
  ble.echo(false);
  ble.verbose(false);  // debug info is a little annoying after this point!
  /* Wait for connection */
  int count = 0;
  while (! ble.isConnected() && count < 5) {
      Serial.print(".");
      delay(500);
      //count ++;
  }
  
  // Set module to DATA mode
  ble.setMode(BLUEFRUIT_MODE_DATA);
  Serial.println("Connected");
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

  if(pocketBot.read()){
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
  
  //Debuging
//  Serial.print(millis());
//  Serial.print(" ");
//  Serial.print("freeMemory()=");
//  Serial.println(freeMemory());
  //pocketBot.printRawTo(Serial);

}
