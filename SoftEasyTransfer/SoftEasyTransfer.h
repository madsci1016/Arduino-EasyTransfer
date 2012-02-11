/******************************************************************
*  EasyTransfer Arduino Library 
*		details and example sketch: 
*			http://www.billporter.info/easytransfer-arduino-library/
*
*		Brought to you by:
*              Bill Porter
*              www.billporter.info
*
*		See Readme for other info and version history
*	
*  
*This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
<http://www.gnu.org/licenses/>
*
*This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
*To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or
*send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
******************************************************************/
#ifndef SoftEasyTransfer_h
#define SoftEasyTransfer_h


//make it a little prettier on the front end. 
#define details(name) (byte*)&name,sizeof(name)

//Not neccessary, but just in case. 
#if ARDUINO > 22
#include "Arduino.h"
#include <SoftwareSerial.h>
#else
#include "WProgram.h"
#include <NewSoftSerial.h>
#endif
//#include "HardwareSerial.h"


#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

class SoftEasyTransfer {
public:
//void begin(uint8_t *, uint8_t, HardwareSerial *theSerial);
#if ARDUINO > 22
void begin(uint8_t *, uint8_t, SoftwareSerial *theSerial);
#else
void begin(uint8_t *, uint8_t, NewSoftSerial *theSerial);
#endif
void sendData();
boolean receiveData();
private:
//HardwareSerial *_serial;

#if ARDUINO > 22
SoftwareSerial *_serial;
#else
NewSoftSerial *_serial;
#endif


uint8_t * address;  //address of struct
uint8_t size;       //size of struct
uint8_t * rx_buffer; //address for temporary storage and parsing buffer
uint8_t rx_array_inx;  //index for RX parsing buffer
uint8_t rx_len;		//RX packet length according to the packet
uint8_t calc_CS;	   //calculated Chacksum
};



#endif