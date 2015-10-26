#include "PocketBot.h"

void PocketBot::begin(Stream *stream){
  begin();
  mStream = stream;
}

void PocketBot::begin(){
  mBracketOpenCount = 0;
}

bool PocketBot::read(){
  
  while ( mStream->available() ){
    char in = mStream->read();
    if(read(in)){
		return true;
	}
  }
  return false; 
}

bool PocketBot::read(char in){
  
    if (in == '{') {
		if(mBracketOpenCount == 0){
			mBegin = true;
			mResponse = "";
		}
	   mBracketOpenCount++;
    }
    
    if (mBegin) mResponse += (in);

    if (in == '}') {
		mBracketOpenCount--;
		if(mBracketOpenCount == 0){
			mBegin = false;
			return true;
		}
    }
 
  return false; 
}

JsonObject& PocketBot::getJson(){
  StaticJsonBuffer<250> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(mResponse);
  return root;
}

void PocketBot::printRawTo(Stream &stream){
	stream.print(mResponse);
}
