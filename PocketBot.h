/**
* Notes
*/

#include <Arduino.h>
#include <Stream.h>
#include <ArduinoJson.h>

class PocketBot {
  
 public:
   void begin(Stream * stream);
   bool read();
   JsonObject &getJson();
   void printRawTo(Stream &stream);
   
  private:
   Stream *mStream;
   bool mBegin;
   String mResponse;
   int mBracketOpenCount;
   int mBracketCloseCount;
};
