# J1850VPW Module
This module is meant to be able to transmit and receive data on the GM class 2 serial network. This module makes use of the older Motorola XC68HC58 datalink IC. I would recommend you review the datasheet for this IC before you get started.
![PCB](Modules.jpg)

# Gerber Files
To make this module yourself download the .zip file. It contains all the gerber files needed. you can then upload this with your PCB fab house.

# Building The Module
To build this module, you will need to be able to solder with hot air or hotplate (due to the xtal). Everything else can actually be soldered with just a soldering iron. Use the .html file for your older BOM. You can ignore the mounting holes in the BOM; it's just how Altuim output it, and I didn't edit it. That BOM (the .HTML) is for REV A. Rev B and C have significantly different parts on them vs Rev A. Rev B and C were done in KiCAD.

# Sketch
Use the write example .ino to get started in Arduino with this module. This makes use of Arduino built-in SPI drivers, but no special plugins are needed. This First sketch only transmits. Receive Example is working and tested. It is the second sketch.

# Rev B
See the Rev B folder for the Schematic. Gerber Files won't be available for Rev B due to a Silkscreen Error. Once complete, Rev C will have files.
![PCB](RevB.jpg)

# Rev C
Rev C is about the same as Rev B but with silkscreen corrections. The boards I am selling on eBay right now are Rev C.

# Support me!
If you want to buy one from me you should be able to find them on my eBay account. https://www.ebay.com/usr/mattus.i8wvojtcv
You can also support me by contributing code/project. A good way to reach me is matt@untilbroken.com 
