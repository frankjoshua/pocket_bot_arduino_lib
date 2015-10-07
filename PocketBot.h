/**
* Notes
*/

#include <Arduino.h>
#include <Stream.h>
#include <ArduinoJson.h>

/** Command Contract */
#define FACE_X face_x
#define FACE_Y face_y
#define FACE_Z face_z
#define HEADING heading

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
