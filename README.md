# LearningCANandJ1939withTeensy
Enumerated demos and lessons progressing through different features of CAN and J1939 using the FlexCAN library for the Teensy 3.

## Software Versions
### Arduino
This repository was written and tested with Arduino Version 1.8.1 downloaded on 17 March 2017
from https://www.arduino.cc/en/main/software 

### Teensyduino 
Teensyduino version 1.35 downloaded on 17 March 2017
from https://www.pjrc.com/teensy/td_download.html

These two packages were installed on Windows 7 x64 using the defaults for the examples in this repository.

### FlexCAN Library
The FlexCAN Library used in these examples is from a fork of https://github.com/collin80/FlexCAN_Library
This library was installed on March 18, 2017. It will conflict with the Library for FlexCAN that was shipped with Teensyduino.
You may get a message (if you have the verbosity high) that says: 
   ```
   Multiple libraries were found for "FlexCAN.h"
   Used: C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\FlexCAN
   Not used: C:\Users\jeremy-daily\Documents\Arduino\libraries\FlexCAN_Library
   Using library FlexCAN in folder: C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\FlexCAN (legacy)
   ```

It is recommended to remove the folder with the original FlexCAN library so you know which library the software is using to compile.

## Lesson Overview
1. Receive CAN by Polling
   * Imports the FlexCAN library
   * Declares CAN_message_t varialbes
   * Program that checks to see if a new message is available and prints the raw hex data. 
   * A CAN message filter is applied to allow the reception of extended (29-bit) IDs.
   * Single CAN Channel
2. Receive messages using interrupts. 
   * Uses interrupts to process the CAN message
   * Sets up an Object Oriented Approach to handling recieved CAN frames.
   * Keeps track of message count
   * Prints recieved commands to the Serial console in the following format:
  
     ```RXCount micros() mailbox rxmsg.id std/ext rxmsg.len rxmsg.buffer```
   * Uses a general message handler
   * Can hang the Teensy with high busloads
3. Recieve messages with interrupts on both CAN channels from the Teensy 3.6
   * The system can hang with high busload
4. Add a second CAN channel when polling of the Teensy 3.6. 
   * Adapts the first lesson to a Teensy 3.6 with two CAN channels. 
   * Stable and doesn't hang the system.
   * Extracts the timestamp field from the FlexCAN system
5. Send simple messages in a schedule related to count and time.
   * Starts with the first example
   * Sets up an elapsedMillis timer to schedule transmission of messages.
   * Converts 32-bit numbers into CAN bytes with masking and bit shifting
6. Interpret some signals from J1939
   * Parses the extended CAN ID into parameter group number (PGN), source address (SA), destination address (DA), and priorty
   * Displays J1939 data
   * Interprets data based on PGNs for Cruise-Control Wheel Speed, Vehicle Distance, Engine Electronic Controller and High Resolution Vehicle Distance. 
   * Displays some signals defined in the standard.
   * Receive messages are gathered based on Polling.

### Future Lessons

7. Perform an test to determine baudrate before entering the logging loop. Sometimes this is refered to as autobaud.
8. Send simple messages on both channels for the Teensy 3.6
9. Transmit a quadrature encoder input counter on CAN.
10. Interpret J1939 Wheel Based Vehicle Speed and Engine Speed based on interrupts and masks
11. Respond to J1939 requests for Component ID
12. Request J1939 Component ID
13. Send arbitrary messages over J1939 Transport Layer
14. Receive arbitrary messages over the Transport Protocol in J1939 
15. Claim a J1939 Address
16. Interface with CAN using HID drivers.
17. Request ISO Messages
