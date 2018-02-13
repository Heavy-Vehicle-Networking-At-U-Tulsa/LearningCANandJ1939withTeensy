#include <FlexCAN.h>
#include <kinetis_flexcan.h>

/*
   Object Oriented CAN example for Teensy 3.6 with Dual CAN buses
   By Jeremy Daily. Based on the work by Collin Kidder, who based his
   work upon the work of Pawelsky and Teachop.

   The reception of frames in this example is done via callbacks
   to an object rather than polling. Frames are delivered as they come in.

   For high busloads, this sketch will lockup.
*/


#ifndef __MK66FX1M0__
#error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif


//These baudrates are used on some newer Freightliners
#define BAUDRATE0 250000
#define BAUDRATE1 666666

//Create a counter to keep track of message traffic
uint32_t RXCount0 = 0;
uint32_t RXCount1 = 0;

//Keep track of the LED state so we can toggle it.
boolean LEDstate = 0;

//A generic CAN Frame print function for the Serial terminal
void printFrame(CAN_message_t rxmsg, int mailbox, uint8_t channel, uint32_t RXCount)
{
  char CANdataDisplay[50];
  sprintf(CANdataDisplay, "%d %12lu %12lu %2i %08X %d %d", channel, RXCount, micros(), mailbox, rxmsg.id, rxmsg.ext, rxmsg.len);
  Serial.print(CANdataDisplay);
  for (uint8_t i = 0; i < rxmsg.len; i++) {
    char CANBytes[4];
    sprintf(CANBytes, " %02X", rxmsg.buf[i]);
    Serial.print(CANBytes);
  }
  Serial.println();
}

class CANClass : public CANListener
{
  public:
    bool frameHandler(CAN_message_t &frame, int mailbox, uint8_t controller); //overrides the parent version so we can actually do something
};

bool CANClass::frameHandler(CAN_message_t &frame, int mailbox, uint8_t channel)
{
  if (channel == 0) {
    RXCount0++;
    printFrame(frame, mailbox, channel, RXCount0);
  }
  else {
    RXCount1++;
    printFrame(frame, mailbox, channel, RXCount1);
  }

  LEDstate = !LEDstate;
  digitalWrite(LED_BUILTIN, LEDstate);

  return true;
}

//For this example, both CAN channels will use the same listener class
CANClass myCANClassInstance;

void setup() {
  // put your setup code here, to run once:

  //enable the LED Pin
  pinMode(LED_BUILTIN, OUTPUT);

  //Start the CAN message on the first CAN controller (works on both the Teensy 3.2 and Teensy 3.6)
  Can0.begin(BAUDRATE0);
  Can1.begin(BAUDRATE1);

  Can0.attachObj(&myCANClassInstance);
  Can1.attachObj(&myCANClassInstance);

  for (uint8_t filterNum = 4; filterNum < 16; filterNum++) {
    myCANClassInstance.attachMBHandler(filterNum);

  }

  //The default filters exclude the extended IDs, so we have to set up CAN filters to allow those to pass.
  CAN_filter_t allPassFilter;
  allPassFilter.ext = 1;
  for (uint8_t filterNum = 4; filterNum < 16; filterNum++) { //only use half the available filters for the extended IDs
    Can0.setFilter(allPassFilter, filterNum);
    Can1.setFilter(allPassFilter, filterNum);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //Do nothing
  delay(1);
}
