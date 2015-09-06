#include "PocketBot.h"

void PocketBot::begin(Stream *stream){
  mStream = stream;
}

bool PocketBot::read(){
  
  while ( mStream->available() ){
    char in = mStream->read();
    if (in == '{') {
       mBegin = true;
       mResponse = "";
    }
    
    if (mBegin) mResponse += (in);

    if (in == '}') {
        mBegin = false;
        return true;
    }
  }
  return false; 
}

JsonObject& PocketBot::getJson(){
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(mResponse);
  if(root.success()){
    
  } 
  return root;
}
