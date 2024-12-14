firmware to go with the xc68hc58 program
It is meant to be used on an Arduino ProMicro (Atmega32U4). The native USB has a more reliable serial for the Datalogging software that this project was meant for.
If using a UNO or other ATmega328 you will need to update  registers being set on spicycle function:  PORTB |= (1 << 2); and PORTB &= ~(1 << 2);    
