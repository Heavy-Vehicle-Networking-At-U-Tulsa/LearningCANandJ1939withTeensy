#include <FlexCAN.h>

//Create a counter to keep track of message traffic
uint32_t RXCount = 0;

//Keep track of how long its been since a CAN message was received
elapsedMillis CANRXTimer; 

//Keep track of the LED state so we can toggle it.
boolean LEDstate = 0;

//Declare the CAN message variable that includes
//ID, Data Length Code, and Data Buffer
static CAN_message_t rxmsg;

void setup() {
  // put your setup code here, to run once:
  
  //enable the LED Pin
  pinMode(LED_BUILTIN, OUTPUT);
  
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
  if (Can0.available()) {
    Can0.read(rxmsg);
    RXCount++;
    CANRXTimer = 0;
    LEDstate = !LEDstate; // Toggle values
    digitalWrite(LED_BUILTIN,LEDstate);
    
    //Extract J1939 data fields from the extended CAN ID
    uint8_t SA = (rxmsg.id & 0xFF); //mask the last two hex digits (8 bits) //Source Address
    uint32_t PGN = (rxmsg.id & 0x03FFFF00) >> 8; //Parameter Group Number
    uint8_t DA;
    if (PGN >= 0xF000) DA = 0xFF; //Broadcast message to a global address
    else {
      DA = (rxmsg.id & 0x0000FF00) >> 8; //Destination specific address
      PGN = (PGN & 0x03FF00); //set the PGN value to have zeros on the second byte.
    }
    uint8_t priority = (rxmsg.id & 0x1C000000) >> 26;

    //Format the output string
    char timeCountIDandDLCdigits[100]; 
    sprintf(timeCountIDandDLCdigits,"%10ul\t%10ul\t%08X\t%3i\t%8i\t%06X\t%6i\t%3i\t%3i",
      RXCount,micros(),rxmsg.id,rxmsg.len,priority,PGN,PGN,DA,SA);
    Serial.print(timeCountIDandDLCdigits);
    //Display the bytes  
    for (uint8_t i = 0; i<rxmsg.len;i++){ 
      char byteDigits[4]; 
      sprintf(byteDigits,"\t%02X",rxmsg.buf[i]);
      Serial.print(byteDigits); 
    }
    //interpret some messages on the fly
    if (PGN == 65265) // Cruise Control Vehicle Speed
    {
      Serial.print(F("\tCCVS"));
      //
      Serial.print(F("\tSpeed (mph):\t"));
      float SPN84 = word(rxmsg.buf[2],rxmsg.buf[1])/256.0; //Speed in km/h
      float speedMPH = SPN84 * 0.621271;
      if (SPN84 <  251)
        Serial.print(speedMPH);
      else if (SPN84 >= 255) Serial.print(F("Not Available"));
      else Serial.print(F("Out of Range"));
      
      Serial.print(F("\tBrake Switch:\t"));
      uint8_t SPN597 = (rxmsg.buf[3] & 0b00001100) >> 2;
      Serial.print(SPN597,BIN);
      if      (SPN597 == 0) Serial.print(F("\tBrake pedal released"));
      else if (SPN597 == 1) Serial.print(F("\tBrake pedal depressed"));
      else if (SPN597 == 2) Serial.print(F("\tError"));
      else                  Serial.print(F("\tNot Available"));
      
    }
    else if (PGN == 65248) // Vehicle Distance
    {
      Serial.print(F("\tVD"));

      Serial.print(F("\tTotal Vehicle Distance (miles):\t"));
      //Distance in km, least significant bit is 1/8 of a km.
      float SPN244 = (rxmsg.buf[7] << 24 | rxmsg.buf[6] << 16 | rxmsg.buf[5] << 8 | rxmsg.buf[4])*0.125; 
      float SPN244miles = SPN244 * 0.621271;
      Serial.print(SPN244miles);
    } 
    else if (PGN == 61444) // Engine Speed
    {
      Serial.print(F("\tEEC1"));
      //Electronic Engine Controller 1
      Serial.print(F("\tEngine Speed (RPM):\t"));
      //Actual engine speed which is calculated over a minimum crankshaft angle of 720 degrees divided by the number of cylinders.
      float SPN190 = (rxmsg.buf[4] << 8 | rxmsg.buf[3])*0.125; 
      Serial.print(SPN190);
    } 
    Serial.println(); //Go to the next line
  }
  
  if (CANRXTimer > 200){
     //Turn oon the LED solid if no CAN traffic is present.
     LEDstate = HIGH;
     digitalWrite(LED_BUILTIN,LEDstate);
  }
     
}

