#include "SoftEasyTransfer.h"




#if ARDUINO > 22
//Captures address and size of struct
void SoftEasyTransfer::begin(uint8_t * ptr, uint8_t length, SoftwareSerial *theSerial){
	address = ptr;
	size = length;
	_serial = theSerial;

	//dynamic creation of rx parsing buffer in RAM
	rx_buffer = (uint8_t*) malloc(size);
}

#else
//Captures address and size of struct
void SoftEasyTransfer::begin(uint8_t * ptr, uint8_t length, NewSoftSerial *theSerial){
	address = ptr;
	size = length;
	_serial = theSerial;
	
	//dynamic creation of rx parsing buffer in RAM
	rx_buffer = (uint8_t*) malloc(size);
}

#endif

#if ARDUINO > 22
//Sends out struct in binary, with header, length info and checksum
void SoftEasyTransfer::sendData(){
  uint8_t CS = size;
  _serial->write(0x06);
  _serial->write(0x85);
  _serial->write(size);
  for(int i = 0; i<size; i++){
    CS^=*(address+i);
    _serial->write(*(address+i));
  }
  _serial->write(CS);

}
#else
//Sends out struct in binary, with header, length info and checksum
void SoftEasyTransfer::sendData(){
  uint8_t CS = size;
  _serial->print(0x06, BYTE);
  _serial->print(0x85, BYTE);
  _serial->print(size, BYTE);
  for(int i = 0; i<size; i++){
    CS^=*(address+i);
    _serial->print(*(address+i), BYTE);
  }
  _serial->print(CS, BYTE);

}
#endif

boolean SoftEasyTransfer::receiveData(){
  
  //start off by looking for the header bytes. If they were already found in a previous call, skip it.
  if(rx_len == 0){
  //this size check may be redundant due to the size check below, but for now I'll leave it the way it is.
    if(_serial->available() >= 3){
	//this will block until a 0x06 is found or buffer size becomes less then 3.
      while(_serial->read() != 0x06) {
		//This will trash any preamble junk in the serial buffer
		//but we need to make sure there is enough in the buffer to process while we trash the rest
		//if the buffer becomes too empty, we will escape and try again on the next call
		if(_serial->available() < 3)
			return false;
		}
      if (_serial->read() == 0x85){
        rx_len = _serial->read();
		//make sure the binary structs on both Arduinos are the same size.
        if(rx_len != size){
          rx_len = 0;
          return false;
        }
      }
    }
  }
  
  
  
  if(rx_len != 0){
    while(_serial->available() && rx_array_inx <= rx_len){
      rx_buffer[rx_array_inx++] = _serial->read();
    }
    
    if(rx_len == (rx_array_inx-1)){
      //seem to have got whole message
      //last uint8_t is CS
      calc_CS = rx_len;
      for (int i = 0; i<rx_len; i++){
        calc_CS^=rx_buffer[i];
      } 
      
      if(calc_CS == rx_buffer[rx_array_inx-1]){//CS good
        memcpy(address,rx_buffer,size);
		rx_len = 0;
		rx_array_inx = 0;
		return true;
		}
        
	  else{
	  //failed checksum, need to clear this out anyway
		rx_len = 0;
		rx_array_inx = 0;
		return false;
	  }	
		
    }
  }
  
  return false;
}