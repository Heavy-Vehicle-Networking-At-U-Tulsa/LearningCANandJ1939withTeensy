#include <FlexCAN.h>

//Set up timing variables
#define TXPeriod100 100
elapsedMillis TXTimer100;

//Create a counter to keep track of message traffic
uint32_t RXCount = 0;
uint32_t TXCount = 0;

//Declare the CAN message variable that includes
//ID, Data Length Code, and Data Buffer
static CAN_message_t txmsg,rxmsg;

//Keep track of the LED state so we can toggle it.
boolean LEDstate = 0;

void setup() {
  // put your setup code here, to run once:
  
  //enable the LED Pin
  pinMode(LED_BUILTIN, OUTPUT);
  
  //Start the CAN message on the first CAN controller (works on both the Teensy 3.2 and Teensy 3.6)
  Can0.begin(250000);
  Can1.begin(500000);

  //The default filters exclude the extended IDs, so we have to set up CAN filters to allow those to pass.
  CAN_filter_t allPassFilter;
  allPassFilter.ext=1;
  for (uint8_t filterNum = 8; filterNum < 16;filterNum++){ //only use half the available filters for the extended IDs
    Can0.setFilter(allPassFilter,filterNum); 
  }

  //define the non changing parts of the CAN message to be transmitted.
  txmsg.ext=1;
  txmsg.id=0x101;
  txmsg.len=8;
}

void loop() {
  // put your main code here, to run repeatedly:

  //Poll for new messages
  if (Can0.available()) {
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

  if (TXTimer100 >= TXPeriod100){
    TXTimer100 = 0; //Reset the timer

    //Convert the 32-bit timestamp into 4 bytes with the most significant byte (MSB) first (Big endian).
    uint32_t sysMicros = micros();
    txmsg.buf[0] = (sysMicros & 0xFF000000) >> 24;
    txmsg.buf[1] = (sysMicros & 0x00FF0000) >> 16;
    txmsg.buf[2] = (sysMicros & 0x0000FF00) >>  8;
    txmsg.buf[3] = (sysMicros & 0x000000FF);

    //Convert the 32-bit transmit counter into 4 bytes with the most significant byte (MSB) first (Big endian). 
    TXCount++;
    txmsg.buf[4] = (TXCount & 0xFF000000) >> 24;
    txmsg.buf[5] = (TXCount & 0x00FF0000) >> 16;
    txmsg.buf[6] = (TXCount & 0x0000FF00) >>  8;
    txmsg.buf[7] = (TXCount & 0x000000FF);
    Can0.write(txmsg);
    Can1.write(txmsg);
    
    LEDstate = !LEDstate;
    digitalWrite(LED_BUILTIN, LEDstate);
  }

  /*
   * CHALLENGE Problems:
   * 
   * 1. Request component information from an ECU.  You may need to look for a response.
   * 2. Develop a bus flooding device
   * 3. 
   */


  
}
