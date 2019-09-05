#ifndef _PROJECT_H_INCLUDED
#define _PROJECT_H_INCLUDED

#define VERSION_MAJOR       1
#define VERSION_MINOR       0

#define true    1
#define false   0

#ifndef BIT
#define BIT(x) 1 << x
#endif

#ifndef SETBIT
#define	SETBIT(a, b)	(a |= BIT(b))
#endif

#ifndef GETBIT
#define	GETBIT(a, b)	(a & BIT(b))
#endif

#ifndef CLRBIT
#define CLRBIT(a, b)	(a &= ~(BIT(b)))
#endif

/*******************************************
* Project globals
*******************************************/
#include <stdint.h>
extern volatile uint16_t milliseconds;

#define BOOTLOADER_CODE     0xdead

// Configuration structure.  This will be slammed
// down via USB from the host
struct host_command_t
{
    uint16_t    ptrn;
    uint16_t    posn;
    uint16_t    red;
    uint16_t    grn;
    uint16_t    blu;
    uint16_t    len;
};

struct config_t
{
    uint16_t        signature;
    uint16_t        bootCount;
    host_command_t  cmd;

};

struct response_to_host_t
{
    uint16_t sequence;      //  0 /  2
    uint16_t bootCount;     //  2 /  4
    uint16_t versionMajor;  //  4 /  6
    uint16_t versionMinor;  //  6 /  8
};

/*********************************************
* System and std lib includes and definitions
*********************************************/
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/*********************************************
* Project includes and definitions
**********************************************/
#include "sys/hardware.h"
#include "usb/usbHIDComm.h"
#include "ws2812/WS2812.h"
#include "beacon.h"

/*********************************************
* config
*********************************************/
#include "config.h"

/**********************************************
* LED winky light classes
**********************************************/
extern beacon flow;

/**********************************************
* comms to the host (game)
**********************************************/
#include "hostComms.h"

/**********************************************
* USB Device descriptors
***********************************************
* Define product string - up to 31 characters
***********************************************/
static const S_usb_product_string_descriptor usbProductString
{
    sizeof(S_usb_product_string_descriptor),
    DESCRIPTOR_STRING,
    {
        USB_unicode('B'),       //  0
        USB_unicode('e'),       //  1
        USB_unicode('a'),       //  2
        USB_unicode('c'),       //  3
        USB_unicode('o'),       //  4
        USB_unicode('n'),       //  5
        USB_unicode(' '),       //  6
        USB_unicode('\0')       //  7
    }
};

/**********************************************
* USB Device descriptors
***********************************************
* Define device ID, 2 byte value
***********************************************/
#define DEVICE_ID   0x01

/**********************************************
* Report Sizes
***********************************************
* The data packet that goes from host to this
* device is the "OUT Report"; data back to
* host is the "IN Report".  We need to let the
* host know the size of these reports
**********************************************/
#define LENGTH_OF_REPORT_IN     sizeof(response_to_host_t)
#define LENGTH_OF_REPORT_OUT    sizeof(host_command_t)


// uint16_t/uint32_t endian handlers
#ifdef LITTLE_ENDIAN     // => 16bit: (LSB,MSB), 32bit: (LSW,MSW) or (LSB0,LSB1,LSB2,LSB3) or (MSB3,MSB2,MSB1,MSB0)
#  define MSB(uint16_t)        (((uint8_t* )&uint16_t)[1])
#  define LSB(uint16_t)        (((uint8_t* )&uint16_t)[0])
#  define MSW(uint32_t)        (((uint16_t*)&uint32_t)[1])
#  define LSW(uint32_t)        (((uint16_t*)&uint32_t)[0])
#  define MSB0(uint32_t)       (((uint8_t* )&uint32_t)[3])
#  define MSB1(uint32_t)       (((uint8_t* )&uint32_t)[2])
#  define MSB2(uint32_t)       (((uint8_t* )&uint32_t)[1])
#  define MSB3(uint32_t)       (((uint8_t* )&uint32_t)[0])
#  define LSB0(uint32_t)       MSB3(uint32_t)
#  define LSB1(uint32_t)       MSB2(uint32_t)
#  define LSB2(uint32_t)       MSB1(uint32_t)
#  define LSB3(uint32_t)       MSB0(uint32_t)
#else // BIG_ENDIAN         => 16bit: (MSB,LSB), 32bit: (MSW,LSW) or (LSB3,LSB2,LSB1,LSB0) or (MSB0,MSB1,MSB2,MSB3)
#  define MSB(uint16_t)        (((uint8_t* )&uint16_t)[0])
#  define LSB(uint16_t)        (((uint8_t* )&uint16_t)[1])
#  define MSW(uint32_t)        (((uint16_t*)&uint32_t)[0])
#  define LSW(uint32_t)        (((uint16_t*)&uint32_t)[1])
#  define MSB0(uint32_t)       (((uint8_t* )&uint32_t)[0])
#  define MSB1(uint32_t)       (((uint8_t* )&uint32_t)[1])
#  define MSB2(uint32_t)       (((uint8_t* )&uint32_t)[2])
#  define MSB3(uint32_t)       (((uint8_t* )&uint32_t)[3])
#  define LSB0(uint32_t)       MSB3(uint32_t)
#  define LSB1(uint32_t)       MSB2(uint32_t)
#  define LSB2(uint32_t)       MSB1(uint32_t)
#  define LSB3(uint32_t)       MSB0(uint32_t)
#endif

#endif // _PROJECT_H_INCLUDED
