# AVR-Control

## Scope:
- Create an interface between a Raspberry Pi and a Pioneer A-20 AVR receiver.
- This is done by sending IR-codes with an ATtiny85 to the IR-in jack based on GPIO states of the Raspberry Pi.

## Hardware:
The following components are required:
- an ATtiny85
- a NPN transistor such as the BC548
- a 10k pull-up resistor for the output signal (R1)
- a 2k2 resistor for limiting the base curreont of the transistor (R2)
- three jumper wires for connecting the Raspberry Pi to the ATtiny board
- some pin headers
- a 3.5mm audio jack (mono)
