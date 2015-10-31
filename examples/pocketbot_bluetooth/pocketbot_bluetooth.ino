#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
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

/** This will be used to decode messages from the Android device */
PocketBot pocketBot;
/** Allocate space for the decoded message. */
PocketBotMessage message = PocketBotMessage_init_zero;


void setup(void){
  Serial.begin(115200);
  
  initBluetooth();
}

void loop(void){

  if(pocketBot.read(ble, message)){
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

void initBluetooth(){
     /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(true) ){
    Serial.println(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
    while(1);
  }
  Serial.println( F("OK!") );
  /* Disable command echo from Bluefruit */
  ble.echo(false);
  ble.verbose(false);  // debug info is a little annoying after this point!
  /* Wait for connection */
  while (! ble.isConnected()) {
      Serial.print(".");
      delay(500);
  }
  
  // Set module to DATA mode
  ble.setMode(BLUEFRUIT_MODE_DATA);
  Serial.println(F("Connected"));
}