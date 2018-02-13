#include <FlexCAN.h>
#include <kinetis_flexcan.h>

#ifndef __MK66FX1M0__
#error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif


//These baudrates are used on some newer Freightliners
#define BAUDRATE0 250000
#define BAUDRATE1 500000

//Create a counter to keep track of message traffic
uint32_t RXCount0 = 0;
uint32_t RXCount1 = 0;

//Keep track of the LED state so we can toggle it.
boolean LEDstate = 0;

//Declare the CAN message variable that includes
//ID, Data Length Code, and Data Buffer
static CAN_message_t rxmsg;

//A generic CAN Frame print function for the Serial terminal
void printFrame(CAN_message_t rxmsg, uint8_t channel, uint32_t RXCount)
{
  char CANdataDisplay[50];
  sprintf(CANdataDisplay, "%d %12lu %12lu %08X %d %d", channel, RXCount, micros(), rxmsg.id, rxmsg.ext, rxmsg.len);
  Serial.print(CANdataDisplay);
  for (uint8_t i = 0; i < rxmsg.len; i++) {
    char CANBytes[4];
    sprintf(CANBytes, " %02X", rxmsg.buf[i]);
    Serial.print(CANBytes);
  }
  Serial.println();
}


void setup() {
  // put your setup code here, to run once:

 //enable the LED Pin
  pinMode(LED_BUILTIN, OUTPUT);

  //Start the CAN message on the first CAN controller (works on both the Teensy 3.2 and Teensy 3.6)
  Can0.begin(BAUDRATE0);
  Can1.begin(BAUDRATE1);

  Can0.startStats();
  Can1.startStats();

  pinMode(37,OUTPUT);
  pinMode(38,OUTPUT);
  pinMode(39,OUTPUT);
  digitalWrite(37,HIGH);
  digitalWrite(38,HIGH);
  digitalWrite(39,HIGH);
  
  //The default filters exclude the extended IDs, so we have to set up CAN filters to allow those to pass.
  CAN_filter_t allPassFilter;
  allPassFilter.id=0;
  allPassFilter.ext=1;
  allPassFilter.rtr=0;
  for (uint8_t filterNum = 4; filterNum < 16;filterNum++){
    Can0.setFilter(allPassFilter,filterNum); 
    Can1.setFilter(allPassFilter,filterNum); 
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Can0.available()) {
    Can0.read(rxmsg);
    printFrame(rxmsg,0,RXCount0++);
  }
  if (Can1.available()) {
    Can1.read(rxmsg);
    printFrame(rxmsg,1,RXCount1++);
  }
}
