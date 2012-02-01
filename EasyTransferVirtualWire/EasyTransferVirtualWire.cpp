#include "EasyTransferVirtualWire.h"
#include <VirtualWire.h>

//so to make this easy, I'm just making my library be another data
// layer on top of Virtual wire. 


//Captures address and size of struct
void EasyTransferVirtualWire::begin(uint8_t * ptr, uint8_t length) { //HardwareSerial *theSerial){
address = ptr;
size = length;
//_serial = theSerial;
}

//Sends out struct in binary, with header, length info and checksum
void EasyTransferVirtualWire::sendData(){

	//temp storage place
	uint8_t temp_buffer[size+4];

	uint8_t CS = size;
	temp_buffer[0] = 0x06;
	temp_buffer[1] = 0x85;
	temp_buffer[2] = size;
	
	for(int i = 0; i<size; i++){
		CS^=*(address+i);
		temp_buffer[i+3] = (*(address+i));
	}
	temp_buffer[size+3] = CS;
	
	vw_send((uint8_t *)temp_buffer, size+4);
    vw_wait_tx(); // Wait until the whole message is gone

}

boolean EasyTransferVirtualWire::receiveData(){

	//temp storage places, from virtual wire example
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
	
	if (vw_get_message(buf, &buflen)) // Non-blocking
    {
		//if here, then a good message was received and is now sitting in buf
		
		if (buf[0] == 0x06 && buf[1] == 0x85 && buf[2] == size) {
			//found my headers, size matches
			
			calc_CS = buf[2]; // must be added after the header is received
			
			for(int i = 0; i<size; i++){
				calc_CS^=buf[i+3];
				}
			
			//compare CS
			if (calc_CS == buf[size+3]){
				//all good if here, warm up the photo copier
				memcpy(address,buf+3,size);
				calc_CS = 0;
				return true;
				}
			else {//bad CS
				calc_CS = 0;
				return false;
			}
		}	
			//need to extract what my library wants to see.
	
	}
	else
		return false;

}