#include "EasyTransferI2C.h"




//Captures address and size of struct
void EasyTransferI2C::begin(uint8_t * ptr, uint8_t length, TwoWire *theSerial){
	address = ptr;
	size = length;
	_serial = theSerial;

	//dynamic creation of rx parsing buffer in RAM
	rx_buffer = (uint8_t*) malloc(size);
}

//Sends out struct in binary, with header, length info and checksum
void EasyTransferI2C::sendData(uint8_t i2c_address){
  uint8_t CS = size;
  _serial->beginTransmission(i2c_address);
#if ARDUINO >= 100
  _serial->write(0x06);
  _serial->write(0x85);
  _serial->write(size);
#else
  _serial->send(0x06);
  _serial->send(0x85);
  _serial->send(size);
#endif
  for(int i = 0; i<size; i++){
    CS^=*(address+i);
#if ARDUINO >= 100
    _serial->write(*(address+i));
#else
    _serial->send(*(address+i));
#endif
  }
#if ARDUINO >= 100
  _serial->write(CS);
#else
  _serial->send(CS);
#endif
  _serial->endTransmission();
}

boolean EasyTransferI2C::receiveData(){
  
  //start off by looking for the header bytes. If they were already found in a previous call, skip it.
  if(rx_len == 0){
  //this size check may be redundant due to the size check below, but for now I'll leave it the way it is.
    if(_serial->available() >= 3){
	//this will block until a 0x06 is found or buffer size becomes less then 3.
#if ARDUINO >= 100
      while(_serial->read() != 0x06) {
#else
      while(_serial->receive() != 0x06) {
#endif
		//This will trash any preamble junk in the serial buffer
		//but we need to make sure there is enough in the buffer to process while we trash the rest
		//if the buffer becomes too empty, we will escape and try again on the next call
		if(_serial->available() < 3)
			return false;
		}
#if ARDUINO >= 100
      if (_serial->read() == 0x85){
        rx_len = _serial->read();
#else
      if (_serial->receive() == 0x85){
        rx_len = _serial->receive();
#endif
		//make sure the binary structs on both Arduinos are the same size.
        if(rx_len != size){
          rx_len = 0;
          return false;
        }
      }
    }
  }
  
  //we get here if we already found the header bytes, the struct size matched what we know, and now we are byte aligned.
  if(rx_len != 0){
    while(_serial->available() && rx_array_inx <= rx_len){
#if ARDUINO >= 100
      rx_buffer[rx_array_inx++] = _serial->read();
#else
      rx_buffer[rx_array_inx++] = _serial->receive();
#endif
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
