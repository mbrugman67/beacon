/******************************************************
* usb HID
*******************************************************
* Implementation of methods call in response to the
* host's request to get/set configuration
*******************************************************/

#include "project.h"
#include "usbHIDComm_Impl.h"
#include "usbDefines.h"

void usbHID::getConfiguration()
{
    ackRxSetup();
    UEDATX = configNumber;
    
    ackInReady();
    while (!isRXOut());

    ackRxOut();
}

void usbHID::setAddress()
{
    uint8_t address = UEDATX;

    UDADDR = address & MSK_UADD;

    ackRxSetup();
    sendControlIn();

    while (!isReadControlEnb());

    enableAddress();

    DBG_COUNT_ON_4(address);
}

bool usbHID::clearFeature(uint8_t requestType)
{
    uint8_t value;
    endpoint_t index;

    if (requestType == USB_SETUP_SET_STAND_ENDPOINT)
    {
        value = UEDATX;
        dummyByte = UEDATX;

        if (value == FEATURE_ENDPOINT_HALT)
        {
            index = (endpoint_t)(UEDATX & MSK_EP_DIR);
            selectEndpoint(index);
            if (!isEndpointEnabled())
            {
                selectEndpoint(EP_CONTROL);
                return (false);
            }   

            if (index != EP_CONTROL)
            {
                disableStallHandshake();
                resetEndpoint(index);
                resetDataToggle();
            }

            selectEndpoint(EP_CONTROL);
            endpointStatus[index] = 0;
            ackRxSetup();
            sendControlIn();
            
            return (true);
        }
    }  

    return (false);
}

bool usbHID::setFeature(uint8_t requestType)
{
    uint8_t value;
    endpoint_t index;

    if (requestType == USB_SETUP_SET_STAND_ENDPOINT)
    {
        value = UEDATX;
        dummyByte = UEDATX;

        if (value == FEATURE_ENDPOINT_HALT)
        {
            index = (endpoint_t)(UEDATX & MSK_EP_DIR);

            if (index == EP_CONTROL)
            {
                enableStallHandshake();
                ackRxSetup();
            }

            selectEndpoint(index);
            if (!isEndpointEnabled())
            {
                selectEndpoint(EP_CONTROL);
            }
            else
            {
                enableStallHandshake();
                selectEndpoint(EP_CONTROL);
                endpointStatus[index] = 0x01;
                ackRxSetup();
                sendControlIn();

                return (true);
            }
        }
    }

    return (false);
}

bool usbHID::getStatus(uint8_t requestType)
{
    endpoint_t index;

    dummyByte = UEDATX;
    dummyByte = UEDATX;
    index = (endpoint_t)(UEDATX & MSK_EP_DIR);

    switch (requestType)
    {
        case USB_SETUP_GET_STAND_DEVICE:
        {
            ackRxSetup();
            UEDATX = deviceStatus;
        }  break;

        case USB_SETUP_GET_STAND_INTERFACE:
        {
            ackRxSetup();
            UEDATX = 0;     // only one interface on this beast
        }  break;

        case USB_SETUP_GET_STAND_ENDPOINT:
        {
            ackRxSetup();
            UEDATX = endpointStatus[index];
        }  break;

        default:
        {
            return (false);
        }
    }

    UEDATX = 0;
    sendControlIn();

    while (!isRXOut());
    ackRxOut();

    return (true);
}

bool usbHID::getInterface()
{
    uint16_t interface;
    uint8_t valueLo;
    uint8_t valueHi;

    valueLo = UEDATX;
    valueHi = UEDATX;

    LSB(interface) = UEDATX;
    MSB(interface) = UEDATX;

    if (valueLo || valueHi)
    {
        return (false);
    }

    ackRxSetup();
    UEDATX = 0; // only one interface, no alternates
    sendControlIn();

    while (!isRXOut());
    ackRxOut();

    return (true);
}

bool usbHID::setInterface()
{
    uint16_t interface;
    uint8_t valueHi;

    dummyByte = UEDATX;
    valueHi = UEDATX;

    LSB(interface) = UEDATX;
    MSB(interface) = UEDATX;

    if (valueHi)
    {
        return (false);
    }

    ackRxSetup();

    // only one interface
    if (interface == 0)
    {
        selectEndpoint(EP_HID_IN);
        disableStallHandshake();
        resetEndpoint(EP_HID_IN);
        resetDataToggle();

        selectEndpoint(EP_HID_OUT);
        disableStallHandshake();
        resetEndpoint(EP_HID_OUT);
        resetDataToggle();
    }

    selectEndpoint(EP_CONTROL);

    sendControlIn();
    while (!isReadControlEnb());

    return (true);
}
