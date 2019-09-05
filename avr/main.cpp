/**************************************************************
* Beacon RGB (addressable) lighting
***************************************************************/
#include "project.h"

static volatile uint16_t isrMilliseconds;
uint16_t volatile milliseconds = 0;
static uint16_t volatile lastMS = 0;

void setBootloaderAndGo();
uint16_t bootLoaderCrazyValue __attribute__ ((section (".noinit")));
void (*jumpToBL) (void) = (void (*)(void))0x3800;

// global
beacon flow;

int main(void)
{
    // if we were previously running and some host sent us the 
    // feature set SETUP command that sez "Go to bootloader",
    // it would have set this crazy non-init'd value and forced
    // a device reset.  So go to bootloader.
    if (bootLoaderCrazyValue == BOOTLOADER_CODE)
    {
        // don't keep doing it!
        bootLoaderCrazyValue = 0;
        jumpToBL();
    }

    setupPins();        // GPIO pins
    setupTimer0();      // 1ms system tick

    sei();

    // comms to the game (host)
    HOST_COMMS hostComms;

    // loop every millisecond
    while (true)
    {
        // handle tx/rx LED
        handleTXRX();
        
        flow.update();

        // request for bootloader?  Set the non-clearable-on-reset value and force a restart 
        if (hostComms.bootLoaderRequested())
        {
            setBootloaderAndGo();
        }
        
        // Loop here during "dead time" of one ms tick
        do
        {
            // handle USB tasks
            hostComms.update();
            
            // prevent shearing
            ATOMIC_BLOCK(ATOMIC_FORCEON)
            {
                milliseconds = isrMilliseconds;
            } 

        } while (milliseconds == lastMS);

        lastMS = milliseconds;
    }
 
    return 0;
}

void setBootloaderAndGo()
{
    // this variable has been placed in an area of RAM that is not touched
    // by the startup library.  Set it to some crazy unlikely-to-be-randomly
    // set on actual powerup.  
    bootLoaderCrazyValue = BOOTLOADER_CODE;
    
    // detach USB
    CLRBIT(USBCON, USBE);
    
    // short delay to allow the detach to finish
    for (uint16_t detachDelay = 0; detachDelay < 50000; ++detachDelay);

    // enable the watchdog timer, set a short timeout
    wdt_enable(WDTO_500MS);

    // had loop until the watchdog timer resets us
    while(true);
}

/*****************************************************
* System tick ISR.  Once each millisecond
*****************************************************/
ISR(TIMER0_COMPA_vect, ISR_BLOCK)
{
    ++isrMilliseconds;
}
