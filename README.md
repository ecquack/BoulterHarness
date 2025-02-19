# BoulterHarness

This is the egg packer wiring harness tester controller software for Boulter Machine Works. 
It is of no use to you unless you are Colby, but there's nothing particularly proprietary in here
so feel free to have a look.

It runs on an Adafruit Huzzah32 EPS32 Feather, your basic original ESP32 board with a USB to serial converter on the board. 4MB flash, no PSRAM, dual core CPU. Massive overkill for this job, but we wanted the WiFi interface for the web server.

It uses the SPIFFS filing system. It is important to load the file system image in addition to the firmware.
If you switch to an ESP32-S2 board you'll have to use FATFS. 

This project is built using PlatformIO run from VS Code. It uses the Arduino framework and some Arduino libraries.

It uses the I2C bus to attach to a string of four PCF8575 GPIO extenders. This provides 64 pins which are used to energize and sense the connections on the wiring harness, as each IO extender has 16 GPIO pins in two banks of 8. I2C addresses can be from 0x20 to 0x27. We're using 0x20-0x23. If we had a more complex wiring harness we could add another 64 wire endpoints. 

The web based control interface appears on "http://htester.local" via mDNS on the local internal network. It uses port 80.

Note:

The datasheet numbers the pins 0-7 and 10-17, but the PCF library uses 0-15 contiguous.

The pins are not truly bidirectional. They will sink current when you turn them ON (LED cathode to GPIO pin, LED anode to +3.3V) and are thus
inverters. 

When reading the state, they are normally pulled high. Shorting the pin to ground will give a reading of 1, thus reads are also inverted.

HTTP endpoints:

/               serves file index.html from SPIFFS partition
/index.html     serves file index.html from SPIFFS partition
/setpin?pin= < pin > & val = < val >

    Example:

    htester.local/setpin?pin=20&val=1

    will set pin number 4 on GPIO chip number two.

/getpin?pin= < pin >

    Example:

    htester.local/getpin?pin=2

/scanmap       returns a JSON file with a list of scan errors to report (or zero if it passes)

To adapt this to your own wiring harness, hook up the harness and call the function PairScan(). Place the results into the KnownGood[] array. Add descriptions of the pins and connectors to the PinDescription[] array. Note that we assume that any one pin will not be connected to more than 3 other pins. If your harness is more complex (lots of shared grounds, etc.) then you'll need to widen the KnownGood array. 


/getmap

This returns a JSON file with the default KnownGood map of wires and their descriptions. 

/setmap

This accepts a JSON file which overrides the built in wiring map and pin/connector descriptions

/scanmap

This runs a scan and returns a JSON file of success or any errors found