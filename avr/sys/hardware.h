#ifndef _HARDWARE_C_
#define _HARDWARE_C_

//#define USB_DEBUGGING
#define USE_DBG_PINS

/**************************************
* OUTPUT LED PINS
***************************************
* PRT PIN WHAT
*  C   6  Top red (AL D5)
*  D   7  Top green (AL D6)
*  E   6  Top blue (AL D7)
*  B   4  Bottom red (AL D8)
*  B   5  Bottom green (AL D9)
*  B   6  Bottom blue (AL D10)
*  D   1  Button red (AL D2)
*  D   0  Button green (AL D3)
*  D   4  Button blue (AL D4)
*
*  B   0  RX LED (AL RXLED)
*  D   5  TX LED (AL TXLED)
*
*  F   7  AUX FET 1 (AL A0)
*  F   6  AUX FET 2 (AL A1)
*
*  B   7  Debug pin 0 (AL D11)
*  D   6  Debug pin 1 (AL D12)
****************************************/

#define LED_RX_OFF()            (SETBIT(PORTB, PORTB0))     // Yah, these look opposite of what
#define LED_TX_OFF()            (SETBIT(PORTD, PORTD5))     // you would expect...
#define LED_RX_ON()             (CLRBIT(PORTB, PORTB0))     // the LEDs have are pulled up to
#define LED_TX_ON()             (CLRBIT(PORTD, PORTD5))     // Vcc, so set output off (to sink) to turn LED on
#define LED_RX(x)               {if (x) {CLRBIT(PORTB, PORTB0);} else {SETBIT(PORTB, PORTB0);}}
#define LED_TX(x)               {if (x) {CLRBIT(PORTD, PORTD5);} else {SETBIT(PORTD, PORTD5);}}
#define TGL_LED_RX()            (SETBIT(PINB, PORTB0))
#define TGL_LED_TX()            (SETBIT(PIND, PORTD5))
#define IS_LED_RX_ON            (GETBIT(PORTB, PORTB0))
#define IS_LED_TX_ON            (GETBIT(PORTD, PORTD5))

#ifndef USB_DEBUGGING
    #define TOP_RED_LED_ON()        (SETBIT(PORTC, PORTC6))
    #define TOP_GRN_LED_ON()        (SETBIT(PORTD, PORTD7))
    #define TOP_BLU_LED_ON()        (SETBIT(PORTE, PORTE6))
    #define TOP_RED_LED_OFF()       (CLRBIT(PORTC, PORTC6))
    #define TOP_GRN_LED_OFF()       (CLRBIT(PORTD, PORTD7))
    #define TOP_BLU_LED_OFF()       (CLRBIT(PORTE, PORTE6))
    #define TGL_TOP_RED_LED()       (SETBIT(PINC, PINC6))
    #define TGL_TOP_GRN_LED()       (SETBIT(PIND, PIND7))
    #define TGL_TOP_BLU_LED()       (SETBIT(PINE, PINE6))
    #define IS_TOP_RED_LED_ON       (GETBIT(PORTC, PORTC6))
    #define IS_TOP_GRN_LED_ON       (GETBIT(PORTD, PORTD7))
    #define IS_TOP_BLU_LED_ON       (GETBIT(PORTE, PORTE6))

    #define BOT_RED_LED_ON()        (SETBIT(PORTB, PORTB4))
    #define BOT_GRN_LED_ON()        (SETBIT(PORTB, PORTB5))
    #define BOT_BLU_LED_ON()        (SETBIT(PORTB, PORTB6))
    #define BOT_RED_LED_OFF()       (CLRBIT(PORTB, PORTB4))
    #define BOT_GRN_LED_OFF()       (CLRBIT(PORTB, PORTB5))
    #define BOT_BLU_LED_OFF()       (CLRBIT(PORTB, PORTB6))
    #define TGL_BOT_RED_LED()       (SETBIT(PINB, PINB4))
    #define TGL_BOT_GRN_LED()       (SETBIT(PINB, PINB5))
    #define TGL_BOT_BLU_LED()       (SETBIT(PINB, PINB6))
    #define IS_BOT_RED_LED_ON       (GETBIT(PORTB, PORTB4))
    #define IS_BOT_GRN_LED_ON       (GETBIT(PORTB, PORTB5))
    #define IS_BOT_BLU_LED_ON       (GETBIT(PORTB, PORTB6))

    #define BTN_RED_LED_ON()        (SETBIT(PORTD, PORTD1))
    #define BTN_GRN_LED_ON()        (SETBIT(PORTD, PORTD0))
    #define BTN_BLU_LED_ON()        (SETBIT(PORTD, PORTD4))
    #define BTN_RED_LED_OFF()       (CLRBIT(PORTD, PORTD1))
    #define BTN_GRN_LED_OFF()       (CLRBIT(PORTD, PORTD0))
    #define BTN_BLU_LED_OFF()       (CLRBIT(PORTD, PORTD4))
    #define TGL_BTN_RED_LED()       (SETBIT(PIND, PIND1))
    #define TGL_BTN_GRN_LED()       (SETBIT(PIND, PIND0))
    #define TGL_BTN_BLU_LED()       (SETBIT(PIND, PIND4))
    #define IS_BTN_RED_LED_ON       (GETBIT(PORTD, PORTD1))
    #define IS_BTN_GRN_LED_ON       (GETBIT(PORTD, PORTD0))
    #define IS_BTN_BLU_LED_ON       (GETBIT(PORTD PORTD4))

    #define AUX_FET_0_ON()          (SETBIT(PORTF, PORTF7))
    #define AUX_FET_0_OFF()         (CLRBIT(PORTF, PORTF7))
    #define TGL_AUX_FET_0()         (SETBIT(PINF, PINF7))
    #define IS_AUX_FET_0_ON         (GETBIT(PORTF, PORTF7))
    #define AUX_FET_1_ON()          (SETBIT(PORTF, PORTF6))
    #define AUX_FET_1_OFF()         (CLRBIT(PORTF, PORTF6))
    #define TGL_AUX_FET_1()         (SETBIT(PINF, PINF6))
    #define IS_AUX_FET_1_ON         (GETBIT(PORTF, PORTF6))


    #ifdef USE_DBG_PINS
        #define DEBUG_PIN0(x)           {if (x) {SETBIT(PORTB, PORTB7);} else {CLRBIT(PORTB, PORTB7);}}
        #define DEBUG_PIN1(x)           {if (x) {SETBIT(PORTD, PORTD6);} else {CLRBIT(PORTD, PORTD6);}}
        #define TGL_DEBUG_PIN0()        (SETBIT(PINB, PINB7))
        #define TGL_DEBUG_PIN1()        (SETBIT(PIND, PORTD6)) 
        #define IS_DEBUG_PIN0           (GETBIT(PORTB, PORTB7))
        #define IS_DEBUG_PIN1           (GETBIT(PORTD, PORTD6))
    #endif

    #define DBG_COUNT_ON_0(x)
    #define DBG_COUNT_ON_1(x)
    #define DBG_COUNT_ON_2(x)
    #define DBG_COUNT_ON_3(x)
    #define DBG_COUNT_ON_4(x)
    #define DBG_COUNT_ON_5(x)

#else
    #define DEBUG_PIN0(x)           {if (x) {SETBIT(PORTB, PORTB7);} else {CLRBIT(PORTB, PORTB7);}}
    #define DEBUG_PIN1(x)           {if (x) {SETBIT(PORTD, PORTD6);} else {CLRBIT(PORTD, PORTD6);}}
    #define DEBUG_PIN2(x)           {if (x) {SETBIT(PORTD, PORTD1);} else {CLRBIT(PORTD, PORTD1);}}
    #define DEBUG_PIN3(x)           {if (x) {SETBIT(PORTD, PORTD0);} else {CLRBIT(PORTD, PORTD0);}}
    #define DEBUG_PIN4(x)           {if (x) {SETBIT(PORTD, PORTD4);} else {CLRBIT(PORTD, PORTD4);}}
    #define DEBUG_PIN5(x)           {if (x) {SETBIT(PORTC, PORTC6);} else {CLRBIT(PORTC, PORTC6);}}

    #define TGL_DEBUG_PIN0()        (SETBIT(PINB, PORTB7))
    #define TGL_DEBUG_PIN1()        (SETBIT(PIND, PORTD6)) 
    #define TGL_DEBUG_PIN2()        (SETBIT(PIND, PORTD1)) 
    #define TGL_DEBUG_PIN3()        (SETBIT(PIND, PORTD0)) 
    #define TGL_DEBUG_PIN4()        (SETBIT(PIND, PORTD4)) 
    #define TGL_DEBUG_PIN5()        (SETBIT(PINC, PORTC6)) 

    inline void DBG_COUNT_ON_0(uint8_t x)
    {
        DEBUG_PIN0(false);
        for (uint8_t ii = 0; ii < x; ii++)
        {
            DEBUG_PIN0(true);
            for (size_t dlay = 0; dlay < 500; dlay++);
            DEBUG_PIN0(false);
        }
    }
    inline void DBG_COUNT_ON_1(uint8_t x)
    {
        DEBUG_PIN1(false);
        for (uint8_t ii = 0; ii < x; ii++)
        {
            DEBUG_PIN1(true);
            for (size_t dlay = 0; dlay < 500; dlay++);
            DEBUG_PIN1(false);
        }
    }
    inline void DBG_COUNT_ON_2(uint8_t x)
    {
        DEBUG_PIN2(false);
        for (uint8_t ii = 0; ii < x; ii++)
        {
            DEBUG_PIN2(true);
            for (size_t dlay = 0; dlay < 500; dlay++);
            DEBUG_PIN2(false);
        }
    }
    inline void DBG_COUNT_ON_3(uint8_t x)
    {
        DEBUG_PIN3(false);
        for (uint8_t ii = 0; ii < x; ii++)
        {
            DEBUG_PIN3(true);
            for (size_t dlay = 0; dlay < 500; dlay++);
            DEBUG_PIN3(false);
        }
    }
    inline void DBG_COUNT_ON_4(uint8_t x)
    {
        DEBUG_PIN4(false);
        for (uint8_t ii = 0; ii < x; ii++)
        {
            DEBUG_PIN4(true);
            for (size_t dlay = 0; dlay < 500; dlay++);
            DEBUG_PIN4(false);
        }
    }
    inline void DBG_COUNT_ON_5(uint8_t x)
    {
        DEBUG_PIN5(false);
        for (uint8_t ii = 0; ii < x; ii++)
        {
            DEBUG_PIN5(true);
            for (size_t dlay = 0; dlay < 500; dlay++);
            DEBUG_PIN5(false);
        }
    }
#endif

extern uint16_t ledTXPulseTime;
extern uint16_t ledRXPulseTime;

void setupPins(void);
void setupTimer0(void);

void pulseLEDTX();
void pulseLEDRX();
void handleTXRX();

#endif // define _HARDWARE_C_