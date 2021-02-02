# AVR-Control

## Scope:
- Create an interface between a Raspberry Pi and a Pioneer A-20 AVR receiver.
- This is done by sending IR-codes with an ATtiny85 to the IR-in jack based on GPIO states of the Raspberry Pi.

## Hardware:
The following components are required:
- an ATtiny85
- a NPN transistor such as the BC547
- a pull-up resistor for the output signal
- a resistor for limiting the base curreont of the transistor
- three jumper wires for connecting the Raspberry Pi to the ATtiny board
