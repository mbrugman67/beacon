/******************************************************
* usb HID
*******************************************************
* Implementation of inline functions and some 
* generic static/global functions that are shared
* with the ISR
*******************************************************/

#ifndef USB_SERIAL_IMPL
#define USB_SERIAL_IMPL

inline void usbHID::enable()         {SETBIT(USBCON, USBE);}
inline void usbHID::disable()        {CLRBIT(USBCON, USBE);}
inline void usbHID::resetAll()       {SETBIT(UDCON, RSTCPU);}
inline void usbHID::resetMacroOnly() {CLRBIT(UDCON, RSTCPU);}

inline bool usbHID::isBusHigh()              {return ((bool)(GETBIT(USBSTA, VBUS)));}

inline void usbHID::enableRegulator()        {SETBIT(UHWCON, UVREGE);}
inline void usbHID::disableRegulator()       {CLRBIT(UHWCON, UVREGE);}
inline void usbHID::enableVPads()            {SETBIT(USBCON, OTGPADE);}
inline void usbHID::disableVPads()           {CLRBIT(USBCON, OTGPADE);}
inline void usbHID::attach()                 {CLRBIT(UDCON, DETACH);}
inline void usbHID::detach()                 {SETBIT(UDCON, DETACH);}

inline bool usbHID::isRxSetup()              {return ((bool)(GETBIT(UEINTX, RXSTPI)));}
inline void usbHID::ackRxSetup()             {CLRBIT(UEINTX, RXSTPI);}
inline bool usbHID::isNAKOutSent()           {return ((bool)(GETBIT(UEINTX, NAKOUTI)));}
inline void usbHID::ackNAKOut()              {CLRBIT(UEINTX, NAKOUTI);}
inline bool usbHID::isReadControlEnb()       {return ((bool)(GETBIT(UEINTX, TXINI)));}
inline void usbHID::sendControlIn()          {CLRBIT(UEINTX, TXINI);}
inline void usbHID::ackInReady()             {CLRBIT(UEINTX, TXINI); CLRBIT(UEINTX, FIFOCON);}
inline void usbHID::resetDataToggle()        {SETBIT(UECONX, RSTDT);}

inline void usbHID::ackControlOut()          {CLRBIT(UEINTX, RXOUTI);}
inline void usbHID::ackRxOut()               {CLRBIT(UEINTX, RXOUTI); CLRBIT(UEINTX, FIFOCON);}
inline bool usbHID::isRXOut()                {return ((bool)(GETBIT(UEINTX, RXOUTI)));}

inline bool usbHID::isReadEnb()              {return ((bool)(GETBIT(UEINTX, RWAL)));}
inline bool usbHID::isWriteEnb()             {return ((bool)(GETBIT(UEINTX, RWAL)));}

inline void usbHID::disableStallHandshake()  {SETBIT(UECONX, STALLRQC);}
inline void usbHID::enableStallHandshake()   {SETBIT(UECONX, STALLRQ);}

inline void usbHID::enableAddress()          {SETBIT(UDADDR, ADDEN);}           
inline void usbHID::disableAddress()         {CLRBIT(UDADDR, ADDEN);}          
inline void usbHID::configAddress(uint8_t x) {UDADDR = (SETBIT(UDADDR, ADDEN)) | (x & MSK_UADD);}

inline uint16_t usbHID::getFrameNumber()     {return ((uint16_t)((((uint16_t)UDFNUMH) << 8) | ((uint16_t)UDFNUML)));}

inline void freezeClock()            {SETBIT(USBCON, FRZCLK);}
inline void thawClock()              {CLRBIT(USBCON, FRZCLK);}
inline bool isClockFrozen()          { return ((bool)GETBIT(USBCON, FRZCLK));}

inline void startPLL()
{
    // 96 MHz pll; datasheet sez that's ideal for a 5V system
    // with a 16MHz crystal/resonator.  Then divide by 1.5 to get
    // a 65MHz high speed clock and divide by 2 to get the standard
    // USB 48MHz clock
    PLLFRQ &= ~(BIT(PDIV3) | BIT(PDIV2) | BIT(PDIV1) | BIT(PDIV0));
    PLLFRQ = BIT(PLLUSB) | BIT(PDIV3) | BIT(PDIV1);

    // Enable using the 16MHz crystal onboard
    PLLCSR = BIT(PINDIV) | BIT(PLLE);
}

inline void stopPLL()
{
    CLRBIT(PLLCSR, PLLE);
    PLLCSR = 0;
}

inline bool isPLLReady()             {return ((bool)(GETBIT(PLLCSR, PLOCK)));}

inline void enableEndpoint()         {SETBIT(UECONX, EPEN);}
inline void disableEndpoint()        {CLRBIT(UECONX, EPEN);} 
inline bool isEndpointEnabled()      {return ((bool)(GETBIT(UECONX, EPEN)));}
inline bool isEndpointCfgOK()        {return ((bool)(GETBIT(UESTA0X, CFGOK)));}
inline void selectEndpoint(endpoint_t e)     {UENUM = (uint8_t)e;}
inline void resetEndpoint(endpoint_t e)      {UERST = BIT(e); UERST = 0;}
inline void allocateUSBRAM()         {SETBIT(UECFG1X, ALLOC);}
inline void freeUSBRAM()             {CLRBIT(UECFG1X, ALLOC);}

inline uint8_t getAllInterrupts()    {return (GETBIT(UDINT, UDIEN));}
inline void ackAllInterrupts()       {UDINT = ~(BIT(UDIEN));}

inline void enableRemoteWakeInt()    {SETBIT(UDIEN, RMWKUP);}
inline void disableRemoteWakeInt()   {CLRBIT(UDIEN, RMWKUP);}
inline bool isRemoteWakeIntEnb()     {return ((bool)(GETBIT(UDIEN, RMWKUP)));}
inline bool isRemoteWakeIntAct()     {return ((bool)(GETBIT(UDINT, UPRSMI)));}
inline void ackRemoteWakeInt()       {CLRBIT(UDINT, UPRSMI);}

inline void enableResumeInt()        {SETBIT(UDIEN, EORSME);}
inline void disableResumeInt()       {CLRBIT(UDIEN, EORSME);}
inline bool isResumeIntEnb()         {return ((bool)(GETBIT(UDIEN, EORSME)));}
inline bool isResumeIntAct()         {return ((bool)(GETBIT(UDINT, EORSMI)));}
inline void ackResumeInt()           {CLRBIT(UDINT, EORSMI);}

inline void enableWakeUpInt()        {SETBIT(UDIEN, WAKEUPE);}
inline void disableWakeUpInt()       {CLRBIT(UDIEN, WAKEUPE);}
inline bool isWakeUpIntEnb()         {return ((bool)(GETBIT(UDIEN, WAKEUPE)));}
inline bool isWakeUpIntAct()         {return ((bool)(GETBIT(UDINT, WAKEUPI)));}
inline void ackWakeUpInt()           {CLRBIT(UDINT, WAKEUPI);}

inline void enableResetInt()         {SETBIT(UDIEN, EORSTE);}
inline void disableResetInt()        {CLRBIT(UDIEN, EORSTE);}
inline bool isResetIntEnb()          {return ((bool)(GETBIT(UDIEN, EORSTE)));}
inline bool isResetIntAct()          {return ((bool)(GETBIT(UDINT, EORSTI)));}
inline void ackResetInt()            {CLRBIT(UDINT, EORSTI);}

inline void enableSOFInt()           {SETBIT(UDIEN, SOFE);}
inline void disableSOFInt()          {CLRBIT(UDIEN, SOFE);}
inline bool isSOFIntEnb()            {return ((bool)(GETBIT(UDIEN, SOFE)));}
inline bool isSOFIntAct()            {return ((bool)(GETBIT(UDINT, SOFI)));}
inline void ackSOFInt()              {CLRBIT(UDINT, SOFI);}    

inline void enableSuspendInt()       {SETBIT(UDIEN, SUSPE);}           
inline void disableSuspendInt()      {CLRBIT(UDIEN, SUSPE);}          
inline bool isSuspendIntEnb()        {return ((bool)(GETBIT(UDIEN, SUSPE)));}
inline bool isSuspendIntAct()        {return ((bool)(GETBIT(UDINT, SUSPI)));}
inline void ackSuspendInt()          {CLRBIT(UDINT, SUSPI);}      

#endif
