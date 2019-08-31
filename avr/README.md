# Squiggle-lighting-board

This is the ATMega32u4 code to control the lighting board on Squiggle.

The board has 3 sets of RGB PWM'd outputs and two separate FETs. The RBG output are for "BUTTON" - this is the colors around the drop button; "CONSOLE" - this is the flat oval-ish panel on to which the drop button is mounted; and "PLAYFIELD" which is the playfield area including the pachincos and squiggle chutes for the red ball.

The FET outputs are also PWM'd to control the brightness of the bottom section of the cabinet where the TV and clear ball tube reside.

It is meant to be controlled via USB.  On power-up it will start doing some basic patterns that would be sufficient for game play.  The USB interface allows for more choreography with gameplay.

The code is written to be compiled with avr-gcc.  There is a Makefile which will build and optionally load the code to the device.  Loading the code uses avr-dude and a custom OSX command-line application that will throw the device into bootloader mode (assuming it has been loaded at least once).  See the Makefile for details.

