#include "PocketBot.h"

bool PocketBot::read(Stream& stream, PocketBotMessage& pbMessage){
  
  while ( stream.available() ){
    byte in = stream.read();
    if(read(in, pbMessage)){
		return true;
	}
  }
  return false; 
}

bool PocketBot::read(byte& in, PocketBotMessage& pbMessage){
  
	//Check for start byte
    if(in == START_BYTE){
	  if(messageStarted == false){
		messageStarted = true;  
		//Reset the position pointer
	    pos = 0;
	  } else {
		messageStarted = false;
		//Decode Message
		return decodeBytes(pbMessage);
	  }
	} else {
	  //Read in the data
	  msg[pos] = in;
	  pos++;
	}
 
  return false; 
}

bool PocketBot::decodeBytes(PocketBotMessage& pbMessage){
    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer(msg, sizeof(msg));
    
    /* Decode the message. Return true if no problems */
    return pb_decode(&stream, PocketBotMessage_fields, &pbMessage);
}