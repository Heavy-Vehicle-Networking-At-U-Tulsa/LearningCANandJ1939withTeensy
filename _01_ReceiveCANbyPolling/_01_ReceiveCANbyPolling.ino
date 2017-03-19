#include <FlexCAN.h>

//Create a counter to keep track of message traffic
uint32_t RXCount = 0;

//Declare the CAN message variable that includes
//ID, Data Length Code, and Data Buffer
static CAN_message_t rxmsg;

void setup() {
  // put your setup code here, to run once:

  //Start the CAN message on the first CAN controller (works on both the Teensy 3.2 and Teensy 3.6)
  Can0.begin(250000);

  //The default filters exclude the extended IDs, so we have to set up CAN filters to allow those to pass.
  CAN_filter_t allPassFilter;
  allPassFilter.ext=1;
  for (uint8_t filterNum = 8; filterNum < 16;filterNum++){ //only use half the available filters for the extended IDs
    Can0.setFilter(allPassFilter,filterNum); 
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Can0.available()) {
    Can0.read(rxmsg);
    char CANdataDisplay[50];
    sprintf(CANdataDisplay, "%12lu %12lu %08X %d %d", RXCount++, micros(), rxmsg.id, rxmsg.ext, rxmsg.len);
    Serial.print(CANdataDisplay);
    for (uint8_t i = 0; i < rxmsg.len; i++) {
      char CANBytes[4];
      sprintf(CANBytes, " %02X", rxmsg.buf[i]);
      Serial.print(CANBytes);
    }
    Serial.println();
  }
}
