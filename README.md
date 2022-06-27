# J1850VPW Module
This module is meant for being able to transmit and recieve data on the GM class 2 serial network. This module makes use of the older motorola XC68HC58 datalink IC. I recommend reviewing the datasheet for this IC before getting started.
![PCB](Modules.jpg)

# Gerber Files
To make this module yourself download the .zip file. It contains all the gerber files needed. you can then upload this with your PCB fab house. I recommend JLC PCB.
PCBs from $0 https://jlcpcb.com/IYB (Free Setup, Free Stencil and Coupons!) 

# Building The Module
To build this module you will need to be able to solder with hotair or hotplate (due to the xtal) everthing else can actually be solder with just a soldering iron. Use the .html file for your oldering BOM. You can ignore the mounting holes in the BOM its just how Altuim output it and I didn't edit it.
Check out the video for more details. https://youtu.be/fsicVHIBsBg

# Sketch
Use the write exmaple .ino to get started in arduino with this module. This makes use of Arduinos built in SPI drivers but no special plugins needed.
