/******************************************************
* usb HID
*******************************************************
* Implementation of the basic USB HID methods.  Main
* update loop, send, receive, etc.
*******************************************************/

#include "project.h"
#include "usbHIDComm_Impl.h"
#include "usbDefines.h"

static volatile bool connected = false;
static bool volatile suspended = false;
static volatile uint16_t usbEvents = 0;
static volatile uint16_t usbSOFs = 0;

// non-class methods.  Why?  they are also used by the USB ISR
bool initDevice();

bool configureEndpoint(endpoint_t ep, endpoint_type_t type, endpoint_dir_t dir, 
    endpoint_size_t size, endpoint_bank_t bank, endpoint_nyet_t nyet);

usbHID::usbHID()
{
    connected = false;
    suspended = false;
    blRequested = false;
    bEnabled = false;
    usbEvents = 0;
    reportRate = 0;
    deviceStatus = USB_DEVICE_STATUS_BUS_POWERED;
    configNumber = 0;

    // turn on voltage to USB and reset the controller
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        enableRegulator();
        disable();
        enable();
        enableVPads();
    }

    memset((void*)endpointStatus, 0x00, sizeof(uint8_t) * MAX_EP_NB);
}

usbHID::~usbHID()
{
    startPLL();
}

/**************************************
* update()
***************************************
* Call this periodically to catch
* USB events
**************************************/
void usbHID::update()
{
    dummyByte = 0;

    // new connection?
    if (isBusHigh() && !connected)
    {
        connected = true;
        SETBIT(usbEvents, EVT_USB_POWERED);
        enable();
        startDevice();
        attach();
    }

    // buh-bye
    if (!isBusHigh() && connected)
    {
        connected = false;
        configNumber = 0;
        SETBIT(usbEvents, EVT_USB_UNPOWERED);
        detach();
        freezeClock();
    }

    // host reset request
    if (GETBIT(usbEvents, EVT_USB_RESET))
    {
        CLRBIT(usbEvents, EVT_USB_RESET);
        resetEndpoint(EP_CONTROL);
        configNumber = 0;
    }

    // catch SETUP packets from host to control endpoint
    selectEndpoint(EP_CONTROL);
    if (isRxSetup())
    {
        pulseLEDRX();
        processRequest();
    }
}

/********************************************
* read()
*********************************************
* look for new bytes from the host.  Yes,
* the correct endpoint is the "OUT" endpoint,
* everything is referenced from the standpoint
* of the host.
********************************************/
bool usbHID::read(char* buff, size_t len)
{
    // limit write ot the RAM endpoint buffer
    // size
    if (len > (EP_OUT_LENGTH - 1))
    {
        len = EP_OUT_LENGTH - 1;
    }

    // select the "out" endpoint and check to
    // see if there are bytes out there
    selectEndpoint(EP_HID_OUT);
    if (isRXOut())
    {
        size_t bytesPulled = 0;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            // pull 'em
            while (isReadEnb() && bytesPulled < len)
            {
                *buff = (char)UEDATX;
                buff++;
                bytesPulled++;
                pulseLEDRX();
            }

            // let the USB controller know we're done
            ackRxOut();
        }
        return (true);
    }

    return (false);
}

/******************************************
* write()
*******************************************
* push those bytes out
******************************************/
bool usbHID::write(char* buff, size_t len)
{
    if (len > (EP_IN_LENGTH - 1))
    {
        len = EP_IN_LENGTH - 1;
    }
    
    // "IN" endpoint address, because bytes
    // are going in to host from device
    selectEndpoint(EP_HID_IN);
    size_t bytesSent = 0;

    //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        while (bytesSent < len)
        {
            while (!isWriteEnb());

            UEDATX = (char)*buff;
            buff++;
            bytesSent++;
            pulseLEDTX();
        }

        // let the USB controller know we're done
        ackInReady();
    }

    if (bytesSent == len)
    {
        return (true);
    }

    return (false);
}

/********************************************
* processRequest()
*********************************************
* Entry point for any SETUP packet received
* on the control endpoint
********************************************/
void usbHID::processRequest()
{
    // yes, yes; we saw the packet
    ackControlOut();

    uint8_t requestType;
    uint8_t request;

    // first word of any SETUP packet is the 
    // request and type of request
    requestType = UEDATX;
    request = UEDATX;

    DBG_COUNT_ON_0(request);
    DBG_COUNT_ON_1(requestType);

    switch (request)
    {
        // 0x06 - 6
        case SETUP_GET_DESCRIPTOR:
        {
            // 0x80 - 128
            if (requestType == USB_SETUP_GET_STAND_DEVICE)
            {
                // generic process to get the requested
                // USB descriptor.  We may or may not 
                // actually handle the request
                if (getDescriptor())
                {
                    return;
                }
            }
        }  break;

        // 0x08 - 8
        case SETUP_GET_CONFIGURATION:
        {
            // 0x80 - 128
            if (requestType == USB_SETUP_GET_STAND_DEVICE)
            {
                getConfiguration();
                return;
            }
        }  break;

        // 0x0a - 10
        case SETUP_GET_INTERFACE:
        {
            // 0x81 - 129
            if (requestType == USB_SETUP_GET_STAND_INTERFACE)
            {
                if (getInterface())
                return;
            }
        }  break;

        // 0x05 - 5
        case SETUP_SET_ADDRESS:
        {
            // 0x00 - 0
            if (requestType == USB_SETUP_SET_STAND_DEVICE)
            {
                setAddress();
                return;
            }
        }  break;

        // 0x09 - 9
        case SETUP_SET_CONFIGURATION:
        {
            // 0x00 - 0
            if (requestType == USB_SETUP_SET_STAND_DEVICE)
            {
                if (setConfiguration())
                {
                    return;
                }
            }
        }  break;
        
        // 0x0b - 11
        case SETUP_SET_INTERFACE:
        {
            // 0x00 - 0   
            if (requestType == USB_SETUP_SET_STAND_INTERFACE)
            {
                if (setInterface())
                return;
            }
        }  break;

        // 0x00 - 0
        case SETUP_GET_STATUS:
        {
            if (getStatus(requestType))
            {
                return;
            }
        }  break;

        // 0x01 - 1
        case SETUP_CLEAR_FEATURE:
        {
            if (clearFeature(requestType))
            {
                return;
            }
        }  break;

        // 0x03 - 3
        case SETUP_SET_FEATURE:
        {
            if (setFeature(requestType))
            {
                return;
            }
        }  break;
    }

    // other SETUP requests here.
    if (!userReadRequest(requestType, request))
    {
        // if we get all the way here, we didn't 
        // send a response to the host.  Send the 
        // "Stall" to the host so it knows we're 
        // not going to do anything about their request.
        enableStallHandshake();
        ackRxSetup();
        endpointStatus[(EP_CONTROL & MSK_EP_DIR)] = 0x01;
    }

}

/*****************************************
* startDevice()
******************************************
* called when connection is detected.
* start the PLL controller and the clocks,
* and get ready for data
*****************************************/
bool usbHID::startDevice()
{
    // general stuffs to get the clocks going and 
    // attach the device
    freezeClock();
    startPLL();

    while (!isPLLReady());

    cli();
    thawClock();
    attach();
    resetMacroOnly();

    // start with the control endpoint;
    initDevice();

    enableSuspendInt();
    enableResetInt();
    sei();
    return (true);
}

/********************************************
* sendPGMSpaceDataControlEP()
*********************************************
* Generic method to send data out the 
* control endpoint (endpoint zero).  The 
* 'PGMSpace' part of the function name 
* refers to the fact that we're going to be
* getting data from Flash (const attribute)
* instead of RAM.
*
* PREREQUISITES:
*   Before calling this function you must:
*   + set 'pgmSpaceBuffer' to point to the 
*     data you want to send.  Again, make
*     sure that it is data located in Flash,
*     not RAM.
*   + set 'dataToTransfer' to the number
*     of bytes to send.
*
* The 'wLength' parameter should contain the
* maximum number of bytes that the host
* is willing to take.  You should have 
* learned that from the request before
* calling this.
*
* The function will set the pointer to null
* and the number of bytes to write to zero
* when it is done
*********************************************/
void usbHID::sendPGMSpaceDataControlEP(uint16_t wLength)
{
    // catch the case where this was called without the buffers being setup properly
    if (!pgmSpaceBuffer || !dataToTransfer)
    {
        return;
    }

    bool zlp = false;
    uint8_t byteCount;
    
    // ack the setup command was received
    ackRxSetup();

    // decide how much to transfer. 
    if (wLength > dataToTransfer)
    {
        // if the number of bytes to transfer is equal to the endpoint's length,
        // we'll need to send a zero length packet after so the host knows it 
        // got the whole thing and we're done
        if (!(dataToTransfer % EP_CONTROL_LENGTH))  
        {
            zlp = true;
        }
    }
    else
    {
        // if we have more bytes to send than the host can handle, just send
        // what can be taken
        dataToTransfer = (uint8_t)wLength;
    }

    // ack this now to detect if we get a NAK later
    ackNAKOut();

    // make sure the host is ready for our data
    // and that there's actually data to send
    while (dataToTransfer && !isNAKOutSent())
    {
        // wait until host is ready for data
        while (!isReadControlEnb())
        {
            // if the host sends a NAK, get outta here and quit
            if (isNAKOutSent())
            {
                break;
            }
        }

        // zero byte count for this pass of the outer loop.  Making sure
        // we don't overrun RAM for the endpoint
        byteCount = 0;

        // start transferring out
        while (dataToTransfer)
        {
            // never send more bytes than the actual size of the control endpoint.  
            // Just break out of this inner loop and we'll get more on the next
            // pass of the outer loop
            if (byteCount++ == EP_CONTROL_LENGTH)
            {
                break;
            }

            // next byte into the USB buffer
            UEDATX = *pgmSpaceBuffer;
            ++pgmSpaceBuffer;
            --dataToTransfer;

            // winky light
            pulseLEDTX();

            DBG_COUNT_ON_4(1);
        }

        // host send a NAK while we were writing?
        // If so, quit now
        if (isNAKOutSent())
        {
            break;
        }
        else
        {
            // if not, let the host know we're done with
            // this packet
            sendControlIn();
        }
    }

    // do we have to send a zero-byte packet to let
    // the host know we're really done?
    if (zlp && !isNAKOutSent())
    {
        while (!isReadControlEnb())
        {
            sendControlIn();
        }
    }

    // now wait for the NAK to let us know the host has
    // finished receiving/processing the packet(s)
    while (!isNAKOutSent());

    // let host know we saw it and we're done
    ackNAKOut();
    ackControlOut();   
    
    // clear these things just in case this method gets called
    // again without having data set up
    pgmSpaceBuffer = NULL;
    dataToTransfer = 0;
}

/**************************************************
* setConfiguration()
***************************************************
* When getting enumerated and set up by the host,
* the host will send a request for us to set 
* configuration.  We only have one configuration;
* one endpoint out, one in.
***************************************************/
bool usbHID::setConfiguration()
{
    // get the requested configuration number.  
    // Some devices have more than one configuration,
    // but not this - we're just simple country 
    // folk here.
    uint8_t cfgNumber;
    cfgNumber = UEDATX;

    DBG_COUNT_ON_4(cfgNumber);

    // don't allow more than one configuration
    if (cfgNumber > NUMBER_OF_CONFIGURATIONS)
    {
        return (false);
    }

    // yes, host; we saw the request
    ackRxSetup();
    sendControlIn();

    configNumber = cfgNumber;

    // configure the endpoint to send packets in to host
    selectEndpoint(EP_HID_IN);
    configureEndpoint(EP_HID_IN,  TYPE_INTERRUPT, DIRECTION_IN, SIZE_64, ONE_BANK, NYET_ENABLED);
    
    // configure the endpoint to accept packets out from host
    selectEndpoint(EP_HID_OUT);
    configureEndpoint(EP_HID_OUT, TYPE_INTERRUPT, DIRECTION_OUT, SIZE_64, ONE_BANK, NYET_ENABLED);

    return (true);
}

/*************************************************
* initDevice()
**************************************************
* In our case, the only thing to initialize is 
* to be sure the control endpoint is set up and
* ready for packets
*************************************************/
bool initDevice()
{
    selectEndpoint(EP_CONTROL);
    if (!isEndpointEnabled())
    {
        return (configureEndpoint(EP_CONTROL, TYPE_CONTROL, DIRECTION_OUT, SIZE_32, ONE_BANK, NYET_DISABLED));
    }  
    
    return (false);
}

/********************************************************
* configureEndpoint()
*********************************************************
* endpoint configuration is across a couple of registers,
* and basically consists of setting direction and 
* allocating the RAM for it
********************************************************/
bool configureEndpoint(endpoint_t ep, endpoint_type_t type, endpoint_dir_t dir, 
    endpoint_size_t size, endpoint_bank_t bank, endpoint_nyet_t nyet)
{
    enableEndpoint();
    UECFG0X = ((uint8_t)type << 6) | ((uint8_t)nyet << 1) | ((uint8_t)dir);
    UECFG1X = (UECFG1X & BIT(ALLOC)) | ((uint8_t)size << 4) | ((uint8_t)bank << 2);
    allocateUSBRAM();

    return (isEndpointCfgOK());
}

/*********************************************************
* ISR
**********************************************************
* Interrupt service routine for anything USB related.
* Handles host requests to suspend, resume.  Lets us know
* when we get the start of the next frame.  Stuff
*********************************************************/
ISR(USB_GEN_vect, ISR_BLOCK)
{
    // start of frame
    if (isSOFIntEnb() && isSOFIntAct())
    {
        ++usbSOFs;
        ackSOFInt();
    }

    if (isSuspendIntEnb() && isSuspendIntAct())
    {
        suspended = true;
        ackWakeUpInt();
        SETBIT(usbEvents, EVT_USB_SUSPEND);
        ackSuspendInt();
        enableWakeUpInt();
        disableResumeInt();
        freezeClock();
        stopPLL();
    }

    if (isWakeUpIntEnb() && isWakeUpIntAct())
    {
        if (!isPLLReady())
        {
            startPLL();
            while(!isPLLReady());
        }

        if (isClockFrozen())
        {
            thawClock();
        }

        // clocks must be running before ack'ing this interrupt
        ackWakeUpInt();

        if (suspended)
        {
            enableResumeInt();
            enableResetInt();
            ackWakeUpInt();
            disableWakeUpInt();
            SETBIT(usbEvents, EVT_USB_WAKE_UP);
            enableSuspendInt();
            enableResumeInt();
            enableResetInt();
        }
    }

    if (isResumeIntEnb() && isResumeIntAct())
    {
        suspended = false;
        disableWakeUpInt();
        ackResumeInt();
        disableResumeInt();
        SETBIT(usbEvents, EVT_USB_RESUME);
    }

    if (isResetIntEnb() && isResetIntAct())
    {
        ackResetInt();
        initDevice();
        SETBIT(usbEvents, EVT_USB_RESET);
    }
}
