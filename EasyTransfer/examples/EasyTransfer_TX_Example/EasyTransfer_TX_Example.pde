#include <EasyTransfer.h>

//create object
EasyTransfer ET; 

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t blinks;
  int16_t pause;
};

//give a name to the group of data
SEND_DATA_STRUCTURE mydata;

void setup(){
  Serial.begin(9600);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ET.begin(details(mydata), &Serial);
  
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
