
#include <Arduino.h>
#include <Stream.h>
#include "PocketBotProtocol.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

/** Start of message from AndroidAccessory */
#define START_BYTE 255
/** Command Contract */
#define NO_FACE -1

class PocketBot {
  
 public:
   bool read(Stream &stream, PocketBotMessage& pbMessage);
   bool read(byte& input, PocketBotMessage& pbMessage);
   
  private:
   bool messageStarted = false;
   byte bytesLeft = 0;
   byte msg[PocketBotMessage_size];
   byte pos = 0;
   bool decodeBytes(PocketBotMessage& pbMessage);
   
};

