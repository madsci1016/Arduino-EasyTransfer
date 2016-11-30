#include <VirtualWire.h>
#include <EasyTransferVirtualWire.h>


//create object
EasyTransferVirtualWire ET; 

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  //Struct can'e be bigger then 26 bytes for VirtualWire version
  int16_t blinks;
  int16_t pause;
};

//give a name to the group of data
SEND_DATA_STRUCTURE mydata;

void setup(){
  //start the library, pass in the data details
  ET.begin(details(mydata));
  
  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);	 // Bits per sec
  
  pinMode(13, OUTPUT);
  
  randomSeed(analogRead(0));
  
}

void loop(){
  //this is how you access the variables. [name of the group].[variable name]
  mydata.blinks = random(5);
  mydata.pause = random(5);
  //send the data
  ET.sendData();
  
  //Just for fun, we will blink it out too
   for(int i = mydata.blinks; i>0; i--){
      digitalWrite(13, HIGH);
      delay(mydata.pause * 100);
      digitalWrite(13, LOW);
      delay(mydata.pause * 100);
    }
  
  delay(5000);
}

