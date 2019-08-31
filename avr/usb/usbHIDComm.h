/******************************************************
* usb HID
*******************************************************
* USB communications between device and host
* This device has 3 endpoints - 
*   0 - control
*   1 - "in", direction in from device to host
*   2 - "out", direction out from host to device
*******************************************************/

#ifndef USB_SERIAL_COMM_H_
#define USB_SERIAL_COMM_H_

#include "usbDefines.h"
#include <avr/pgmspace.h>   // to manage "const" structs

class usbHID
{
public:
    usbHID();
    ~usbHID();
    
    // update() must be called periodically to watch for and 
    // handle events like hotplug, etc.
    void update();

    // The host can request device jump to bootloader by sending a control packet
    // that is a request to set a "feature" report with data 0x55aa55aa.  If so,
    // this method will return true.  It is up to the application program to 
    // decide if it really does jump to bootloader.  This class will not do
    // it on it's own - it only lets you know someone wants us to do it.
    bool isBootloaderRequested()        {return (blRequested);}

    // returns true if this device has been enumerated and configured by the host
    bool isEnumerated()                 {return ((bool)(configNumber != 0));}

    // read and write raw data
    bool read(char* buff, size_t len);
    bool write(char* buff, size_t len);

private:
    bool blRequested;
    bool bEnabled;

    uint8_t const* pgmSpaceBuffer;      // RAM pointer to Flash memory byte
    uint8_t dataToTransfer;
    uint8_t deviceStatus;
    uint8_t configNumber;
    uint8_t reportRate;
    uint8_t dummyByte;                  // sometimes we just read unused bytes - this prevents compiler warnings
    uint8_t endpointStatus[MAX_EP_NB];

    bool startDevice();
    void processRequest();
    bool getDescriptor();
    void getHIDDescriptor();
    void getReportDescriptor();
    void getConfiguration();
    bool setConfiguration();
    bool clearFeature(uint8_t requestType);
    bool setFeature(uint8_t requestType);
    bool getStatus(uint8_t requestType);
    bool getInterface();
    bool setInterface();
    void setAddress();
    bool userReadRequest(uint8_t requestType, uint8_t request);
    void sendPGMSpaceDataControlEP(uint16_t wLength);
    void setIdle(uint8_t duration);
    void getIdle();

    /****************************************************
    * All of these inline methods are generally USB
    * register manipulations.  They could have been
    * created with "#define" instead of inline functions,
    * but I wanted tighter control over argument types
    * and such
    *
    * They are all defined in the _Impl.h file
    *****************************************************/
    inline void enable();
    inline void disable();
    inline void resetAll();
    inline void resetMacroOnly();
    inline void enableRegulator();
    inline void disableRegulator();
    inline void enableVPads();
    inline void disableVPads();
    inline void attach();
    inline void detach();
    inline bool isBusHigh();
    inline void resetDataToggle();
    inline bool isRxSetup();    
    inline void ackRxSetup();
    inline bool isNAKOutSent();
    inline void ackNAKOut();
    inline bool isReadControlEnb();
    inline void sendControlIn();
    inline void ackInReady();
    inline void ackControlOut();
    inline void ackRxOut();
    inline bool isRXOut();
    inline bool isReadEnb();
    inline bool isWriteEnb();
    inline void disableStallHandshake();
    inline void enableStallHandshake();
    inline void enableAddress();
    inline void disableAddress();
    inline void configAddress(uint8_t x);

    inline uint16_t getFrameNumber();
};

#endif  // #define USB_SERIAL_COMM_H_