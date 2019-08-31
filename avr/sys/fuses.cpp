/*************************************************
* Fuses
*   Low Fuse Byte 0xff:
*       Clock divide by 8   : 1 - full speed ahead
*       Clock out           : 1 - don't output clock onto PB0
*       Start up time 1     :
*       Start up time 2     :
*       Clock sel 3:0       : external crystal resonator
*
*   High Fuse Byte 0xda:
*       External reset disable              : 1 - external reset enabled
*       Debug wire enable                   : 1 - debug wire disabled
*       Serial programming enable           : 0 - serial (SPI) programming enabled
*       Watchdog timer on                   : 1 - wdt always on
*       EESave through programming cycle    : 0 - 1024 words of bootloader (16 pages)
*       Bootsize 2:1                        : 1 - application 0x0000 - 0x3bff, bootloader 0x3c00 - -x3bff
*       Boot reset vector                   : 0 - vectors in application area
*
*   Extended Fuse Byte 0xfd:
*       Brownout level detect 2:0           : 0b101 - 2.5 Volts
***************************************************/

#include <avr/io.h>


FUSES = {
    .low      = 0xff,
    .high     = 0xd8,
    .extended = 0xcb
};