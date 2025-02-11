# BoulterHarness

This is the wiring harness tester controller software for Boulter Machine Works. 
It is of no use to you unless you are Colby, but there's nothing particularly proprietary in here
so feel free to have a look.

It runs on an Adafruit Huzzah32 EPS32 Feather, your basic ESP32 with a serial port interface.

It uses the SPIFFS filing system.


It uses the I2C bus to attach to a string of PCF8575 GPIO extenders. This provides 64 pins which are used to energize and sense the connections on the wiring harness. Each IO extender has 16 GPIO pins in two banks of 8. I2C addresses can be from 0x20 to 0x27. 

The web based control interface appears on "htester.local" via mDNS on the local internal network. It usess port 80.

The web server code is limited by available memory- it is not smart enough to serve files larger than will fit in the available RAM.

Note:

The datasheet numbers the pins 0-7 and 10-17, but the PCF library uses 0-15 contiguous.