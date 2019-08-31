#include "project.h"

uint16_t ledTXPulseTime = 0;
uint16_t ledRXPulseTime = 0;

/**************************************
* OUTPUT LED PINS
***************************************
* PRT PIN WHAT
*  B   0  RX LED (AL RXLED)
*  B   4  Bottom red (AL D8)
*  B   5  Bottom green (AL D9)
*  B   6  Bottom blue (AL D10)
*  B   7  Debug pin 0 (AL D11)
*
*  C   6  Top red (AL D5)
*
*  D   0  Button green (AL D3)
*  D   1  Button red (AL D2)
*  D   4  Button blue (AL D4)
*  D   5  TX LED (AL TXLED)
*  D   6  Debug pin 1 (AL D12)
*  D   7  Top green (AL D6)
*
*  E   6  Top blue (AL D7)
*
*  F   6  AUX FET 2 (AL A1)
*  F   7  AUX FET 1 (AL A0)
****************************************/


/***************************************
* setupDebugPins()
****************************************/
void setupPins(void) 
{
    TWCR = 0;

    // PORT B:  pins 0, 4, 5, 6, & 7 are outputs
    DDRB  = 0xF1;
    PORTB = 0x00;

    // PORT C:  pin 6 is an output
    DDRC  = 0x40;  
    PORTC = 0x00;

    // PORT D:  pins 0, 1, 4, 5, 6, & 7 are outputs
    DDRD  = 0xF3;    
    PORTD = 0x00;

    // PORT E:  pin 6 is an output
    DDRE  = 0x40;    
    PORTE = 0x00;

    // PORT F: pins 6 & 7 are outputs
    DDRF  = 0xC0;
    PORTF = 0x00;
}

/***************************************
* setupTimer0()
****************************************
* one milliesecond update tick timer
***************************************/
void setupTimer0(void)
{
    // no output hardware, just internal counts; CTC mode
    TCCR0A = BIT(WGM01);
    
    // prescaler = 64, count = 250
    OCR0A = 250;
    TCCR0B = BIT(CS00) | BIT(CS01);
    
    // enable interrupt on A
    TIMSK0 = BIT(OCIE0A); 
}

void pulseLEDTX()
{
    if (ledTXPulseTime < 50)
    {
        ledTXPulseTime = 50;
    }
}

void pulseLEDRX()
{
    if (ledRXPulseTime < 50)
    {
        ledRXPulseTime = 50;
    }
}

void handleTXRX()
{
    if (ledTXPulseTime)
    {
        if (--ledTXPulseTime)
        {
            LED_TX_ON();
        }
    }
    else 
    {
        LED_TX_OFF();
    }

    if (ledRXPulseTime)
    {
        if (--ledRXPulseTime)
        {
            LED_RX_ON();
        }
    }
    else 
    {
        LED_RX_OFF();
    }    
}