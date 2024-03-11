# J1850VPW Module
This module is meant to be able to transmit and receive data on the GM class 2 serial network. This module makes use of the older Motorola XC68HC58 datalink IC. I would recommend you review the datasheet for this IC before you get started.
![PCB](Modules.jpg)

# Gerber Files
To make this module yourself download the .zip file. It contains all the gerber files needed. you can then upload this with your PCB fab house.

# Building The Module
To build this module, you will need to be able to solder with hot air or hotplate (due to the xtal). Everything else can actually be soldered with just a soldering iron. Use the .html file for your older BOM. You can ignore the mounting holes in the BOM; it's just how Altuim output it, and I didn't edit it.
Check out the video for more details. https://youtu.be/fsicVHIBsBg

# Sketch
Use the write example .ino to get started in Arduino with this module. This makes use of Arduino built-in SPI drivers, but no special plugins are needed. This First sketch only transmits. Receive Example is working and tested. It is the second sketch.

# Rev B
See the Rev B folder for the Schematic. Gerber Files won't be available for Rev B due to a Silkscreen Error. Once complete, Rev C will have files.
