# LearningCANandJ1939withTeensy
Enumerated demos and lessons progressing through different features of CAN and J1939 using the FlexCAN library for the Teensy 3

## Versions
This repository was written and tested with Arduino Version 1.8.1 downloaded on 17 March 2017
from https://www.arduino.cc/en/main/software 
and 
Teensyduino version 1.35 downloaded on 17 March 2017
from https://www.pjrc.com/teensy/td_download.html

These two packages were installed on Windows 7 x64 using the defaults for the examples in this repository.

## Lesson Overview
1. Receive CAN by Polling. The goal of this sketch is to compile a program that checks to see if a new message is available and prints the raw hex data of ID and Data. These receive requests are limited to the default configurations of for the mailboxes.
2. Receive messages using interrupts. There are no configurations for mailboxes
3. Set up mailboxes to receive extended IDs. 
4. Add a second CAN channel for the Teensy 3.6. 
5. Log all RX data to an SD Card
6. Perform an test to determine baudrate before entering the logging loop. Sometimes this is refered to as autobaud.
7. Send simple messages related to count and time.
8. Send simple messages on both channels for the Teensy 3.6
9. Transmit a quadrature encoder input counter on CAN.
10. Interpret J1939 Wheel Based Vehicle Speed and Engine Speed
11. Respond to J1939 requests for Component ID
12. Request J1939 Component ID
13. Send arbitrary messages over J1939 Transport Layer
14. Receive arbitrary messages over the Transport Protocol in J1939 
15. Claim a J1939 Address
16. Interface with CAN using HID drivers.
17. Request ISO Messages

## Lesson Details
 1. Print Recieved messages to the Serial console using polling. 
 a. Import the FlexCAN Library  ```#include <FlexCAN.h>```
 b. Add the extended ID filter to get messages with 29-bit IDs. 
    
