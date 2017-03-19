#include <FlexCAN.h>

#define BAUDRATE 250000

//Create a counter to keep track of message traffic
uint32_t RXCount = 0;

//Keep track of the LED state so we can toggle it.
boolean LEDstate = 0;

void printFrame(CAN_message_t rxmsg, int mailbox)
{
    char CANdataDisplay[50];
    sprintf(CANdataDisplay, "%12lu %12lu %2i %08X %d %d", RXCount, micros(), mailbox, rxmsg.id, rxmsg.ext, rxmsg.len);
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
   void gotFrame(CAN_message_t &frame, int mailbox); //overrides the parent version so we can actually do something
};

void CANClass::gotFrame(CAN_message_t &frame, int mailbox)
{
    RXCount++;
    LEDstate = !LEDstate;
    digitalWrite(LED_BUILTIN,LEDstate);
    printFrame(frame, mailbox);
}

CANClass CANClass0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  
  //Start the CAN message on the first CAN controller (works on both the Teensy 3.2 and Teensy 3.6)
  Can0.begin(BAUDRATE);
  Can0.attachObj(&CANClass0);
  CANClass0.attachGeneralHandler();
    
  //The default filters exclude the extended IDs, so we have to set up CAN filters to allow those to pass.
  CAN_filter_t allPassFilter;
  allPassFilter.ext=1;
  for (uint8_t filterNum = 4; filterNum < 16;filterNum++){ //only use half the available filters for the extended IDs
    Can0.setFilter(allPassFilter,filterNum); 
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //Do nothing
  delay(1);
}
