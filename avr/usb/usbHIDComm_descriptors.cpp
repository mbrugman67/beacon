/******************************************************
* usb HID
*******************************************************
* Implementation of methods called when responding to
* the host's request for USB descriptors
*******************************************************/

#include "project.h"
#include "usbHIDComm_Impl.h"
#include "usbDefines.h"

// store in program space (flash)
static const usbDeviceDescriptor USB_DEV_DESC =
{
    sizeof(usbDeviceDescriptor),
    DESCRIPTOR_DEVICE,  // descriptor type device
    0x200,              // USB spec
    0,                  // device class
    0,                  // device subclass
    0,                  // device protocol
    EP_CONTROL_LENGTH,  // length of control endpoint
    BTG_MFG_ID,         // MFG ID
    DEVICE_ID,          // device ID
    0x100,              // release
    1,                  // manufacturer index
    2,                  // product index
    0,                  // serial number index
    1                   // num configs
};

static const usbUserDeviceDescriptor USB_USER_DEV_DESC = 
{
    // struct S_usb_configuration_descriptor
    {
        sizeof(S_usb_configuration_descriptor),     // length
        DESCRIPTOR_CONFIGURATION,                   // descriptor type       
        sizeof(S_usb_configuration_descriptor)  +   // total size of usbUserDeviceDescriptor struct
        sizeof(S_usb_interface_descriptor)      +   // total size of usbUserDeviceDescriptor struct
        sizeof(S_usb_hid_descriptor)            +   // total size of usbUserDeviceDescriptor struct
        sizeof(S_usb_endpoint_descriptor)       +   // total size of usbUserDeviceDescriptor struct
        sizeof(S_usb_endpoint_descriptor),          // total size of usbUserDeviceDescriptor struct
        0x01,                                       // number of interfaces
        NUMBER_OF_CONFIGURATIONS,                   // number of configurations
        0x00,                                       // which configuration to use
        USB_CONFIG_BUSPOWERED,                      // attributes
        50                                          // max power (mA?)
    },
    // struct S_usb_interface_descriptor
    {
        sizeof(S_usb_interface_descriptor),         // length of this struct
        DESCRIPTOR_INTERFACE,                       // descriptor type
        0x00,                                       // interface number
        0x00,                                       // alternate setting
        0x02,                                       // number of endpoints
        0x03,                                       // interface class = HID
        0x00,                                       // no subclass (BIOS boot stuffs)
        0x00,                                       // no interface protocol
        0x00                                        // interface index
    },
    // struct S_usb_hid_descriptor
    {
        sizeof(S_usb_hid_descriptor),               // length of this struct
        DESCRIPTOR_HID,                             // descriptor type
        0x111,                                      // HID class specification version
        0x00,                                       // no country code
        0x01,                                       // number of class descriptors
        0x22,                                       // descriptor report
        sizeof(S_usb_hid_report_descriptor)         // size of the report descriptor struct
    },
    // control endpoint descriptor
    {
        sizeof(S_usb_endpoint_descriptor),          // length of this struct
        DESCRIPTOR_ENDPOINT,                        // descriptor type
        ENDPOINT_NB_1,                              // endoint number
        EP_ATTRIBUTES_1,                            // attributes
        EP_IN_LENGTH,                               // endpoint size
        EP_INTERVAL_1                               // interval time
    },
    // endpoint descriptor
    {
        sizeof(S_usb_endpoint_descriptor),          // length of this struct
        DESCRIPTOR_ENDPOINT,                        // descriptor type
        ENDPOINT_NB_2,                              // endoint number
        EP_ATTRIBUTES_2,                            // attributes
        EP_OUT_LENGTH,                              // endpoint size
        EP_INTERVAL_2
    }
};

static const S_usb_language_id usbLanguageID =
{
    sizeof(S_usb_language_id),
    DESCRIPTOR_STRING,                              // typeof descriptor (string)
    0x0409                                          // ??
};

static const S_usb_manufacturer_string_descriptor usbManufacturerString
{
    sizeof(S_usb_product_string_descriptor),
    DESCRIPTOR_STRING,
    {
        USB_unicode('B'),       //  0
        USB_unicode('A'),       //  1
        USB_unicode('Y'),       //  2
        USB_unicode('T'),       //  3
        USB_unicode('E'),       //  4
        USB_unicode('K'),       //  5
        USB_unicode(' '),       //  6
        USB_unicode('G'),       //  7
        USB_unicode('A'),       //  8
        USB_unicode('M'),       //  9
        USB_unicode('E'),       // 10
        USB_unicode('S')        // 11
    }
};
/*
static const S_usb_hid_report_descriptor usbHIDReport =
{
    {
        0x06, 0xFF, 0xFF,         // 04|2   , Usage Page (vendordefined?)
        0x09, 0x01,               // 08|1   , Usage      (vendordefined
        0xA1, 0x01,               // A0|1   , Collection (Application)
        // IN report
        0x09, 0x02,               // 08|1   , Usage      (vendordefined)
        0x09, 0x03,               // 08|1   , Usage      (vendordefined)
        0x15, 0x00,               // 14|1   , Logical Minimum(0 for signed byte?)
        0x26 ,0xFF,0x00,           // 24|1   , Logical Maximum(255 for signed byte?)
        0x75, 0x08,               // 74|1   , Report Size(8) = field size in bits = 1 byte
        0x95, LENGTH_OF_REPORT_IN,   // 94|1:ReportCount(size) = repeat count of previous item
        0x81, 0x02,               // 80|1: IN report (Data,Variable, Absolute)
        // OUT report
        0x09, 0x04,               // 08|1   , Usage      (vendordefined)
        0x09, 0x05,               // 08|1   , Usage      (vendordefined)
        0x15, 0x00,               // 14|1   , Logical Minimum(0 for signed byte?)
        0x26, 0xFF,0x00,           // 24|1   , Logical Maximum(255 for signed byte?)
        0x75, 0x08,               // 74|1   , Report Size(8) = field size in bits = 1 byte
        0x95, LENGTH_OF_REPORT_OUT,   // 94|1:ReportCount(size) = repeat count of previous item
        0x91, 0x02,               // 90|1: OUT report (Data,Variable, Absolute)
        // Feature report
        0x09, 0x06,               // 08|1   , Usage      (vendordefined)
        0x09, 0x07,               // 08|1   , Usage      (vendordefined)
        0x15, 0x00,               // 14|1   , LogicalMinimum(0 for signed byte)
        0x26, 0xFF,0x00,          // 24|1   , Logical Maximum(255 for signed byte)
        0x75, 0x08,               // 74|1    , Report Size(8) =field size in bits = 1 byte
        0x95, 0x04,               // 94|1:ReportCount
        0xB1, 0x02,               // B0|1:   Feature report
        0xC0                      // C0|0    , End Collection
    }
};
*/
static const S_usb_hid_report_descriptor usbHIDReport =
{
    {
        0x06, 0xFF, 0xFF,         // 04|2   , Usage Page (vendordefined?)
        0x09, 0x01,               // 08|1   , Usage      (vendordefined
        0xA1, 0x01,               // A0|1   , Collection (Application)
        // IN report
        0x09, 0x02,               // 08|1   , Usage      (vendordefined)
        0x09, 0x03,               // 08|1   , Usage      (vendordefined)
        0x15, 0x00,               // 14|1   , Logical Minimum(0 for signed byte?)
        0x26 ,0xFF,0x00,           // 24|1   , Logical Maximum(255 for signed byte?)
        0x75, 0x08,               // 74|1   , Report Size(8) = field size in bits = 1 byte
        0x95, LENGTH_OF_REPORT_IN,   // 94|1:ReportCount(size) = repeat count of previous item
        0x81, 0x02,               // 80|1: IN report (Data,Variable, Absolute)
        // OUT report
        0x09, 0x04,               // 08|1   , Usage      (vendordefined)
        0x09, 0x05,               // 08|1   , Usage      (vendordefined)
        0x15, 0x00,               // 14|1   , Logical Minimum(0 for signed byte?)
        0x26, 0xFF,0x00,           // 24|1   , Logical Maximum(255 for signed byte?)
        0x75, 0x08,               // 74|1   , Report Size(8) = field size in bits = 1 byte
        0x95, LENGTH_OF_REPORT_OUT,   // 94|1:ReportCount(size) = repeat count of previous item
        0x91, 0x02,               // 90|1: OUT report (Data,Variable, Absolute)
        // Feature report
        0x09, 0x06,               // 08|1   , Usage      (vendordefined)
        0x09, 0x07,               // 08|1   , Usage      (vendordefined)
        0x15, 0x00,               // 14|1   , LogicalMinimum(0 for signed byte)
        0x26, 0xFF,0x00,          // 24|1   , Logical Maximum(255 for signed byte)
        0x75, 0x08,               // 74|1    , Report Size(8) =field size in bits = 1 byte
        0x95, 0x04,               // 94|1:ReportCount
        0xB1, 0x02,               // B0|1:   Feature report
        0xC0                      // C0|0    , End Collection
    }
};

/************************************************
* getDescriptor()
*************************************************
* entry point to a SETUP command from host to
* get one of the many USB descriptors
************************************************/
bool usbHID::getDescriptor()
{
    uint16_t wLength;

    uint8_t stringType;
    uint8_t descriptorType;

    // if the request is for a string, get the string ID
    stringType = UEDATX;

    // get the descriptor type.  See usbDefines.h 
    descriptorType = UEDATX;

    DBG_COUNT_ON_2(descriptorType);
    DBG_COUNT_ON_3(stringType);

    switch (descriptorType)
    {
        // about this device
        case DESCRIPTOR_DEVICE:
        {
            dataToTransfer = sizeof(usbDeviceDescriptor);
            pgmSpaceBuffer = &USB_DEV_DESC.bLength;
        }  break;

        // about the one and only configuration for
        // our device
        case DESCRIPTOR_CONFIGURATION:
        {
            dataToTransfer = sizeof(usbUserDeviceDescriptor);
            pgmSpaceBuffer = &USB_USER_DEV_DESC.cfg.bLength;
        }  break;

        // host wants one of the strings
        case DESCRIPTOR_STRING:
        {
            switch (stringType)
            {
                case LANG_ID:
                {
                    dataToTransfer = sizeof(S_usb_language_id),
                    pgmSpaceBuffer = &(usbLanguageID.bLength);
                }  break;

                case MAN_INDEX:
                {
                    dataToTransfer = sizeof(S_usb_manufacturer_string_descriptor),
                    pgmSpaceBuffer = &(usbManufacturerString.bLength);
                }  break;

                case PROD_INDEX:
                {
                    dataToTransfer = sizeof(S_usb_product_string_descriptor),
                    pgmSpaceBuffer = &(usbProductString.bLength);
                }  break;

                default:
                {
                    return (false);
                }
            }
        }  break;

        default:
        {
            return (false);
        }
    } // which descriptor does the host want?

    // read past bytes
    dummyByte = UEDATX;
    dummyByte = UEDATX;

    // get the max length that the host can accept
    LSB(wLength) = UEDATX;
    MSB(wLength) = UEDATX;

    // send it out
    sendPGMSpaceDataControlEP(wLength);

    return (true);
}

/**********************************************
* getHIDDescriptor()
***********************************************
* Get the descriptors for the specified 
* interface.  But there's really only one
* interface.
**********************************************/
void usbHID::getHIDDescriptor()
{
    uint16_t length;
    uint16_t interface;

    LSB(interface) = UEDATX;
    MSB(interface) = UEDATX;
    LSB(length) = UEDATX;
    MSB(length) = UEDATX;

    dataToTransfer = sizeof(USB_USER_DEV_DESC.hid);
    pgmSpaceBuffer = &(USB_USER_DEV_DESC.hid.bLength);
    
    sendPGMSpaceDataControlEP(length);
}

/*************************************************
* getReportDescriptor()
**************************************************
* The USB specification dictates that there the 
* data passed back and forth between a host and
* HID device are called "reports".  The spec
* further says that there is a descriptor to 
* define the report size, format, etc.
**************************************************/
void usbHID::getReportDescriptor()
{
    uint16_t length;
    uint16_t interface;

    LSB(interface) = UEDATX;
    MSB(interface) = UEDATX;
    LSB(length) = UEDATX;
    MSB(length) = UEDATX;

    dataToTransfer = sizeof(S_usb_hid_report_descriptor);
    pgmSpaceBuffer = &(usbHIDReport.report[0]);

    sendPGMSpaceDataControlEP(length);
}

/*****************************************************
* userReadRequest()
******************************************************
* SETUP commands to get/set interface stuff
*****************************************************/
bool usbHID::userReadRequest(uint8_t requestType, uint8_t request)
{
    dummyByte = UEDATX;
    uint8_t valueHi = UEDATX;

    DBG_COUNT_ON_2(valueHi);

    //                        0x81 - 129                               0x06 - 6
    if (requestType == USB_SETUP_GET_STAND_INTERFACE && request == SETUP_GET_DESCRIPTOR)
    {
        switch (valueHi)
        {
            // 0x21 - 33
            case DESCRIPTOR_HID:
            {
                getHIDDescriptor();
                return (true);
            }  break;

            // 0x22 - 34
            case DESCRIPTOR_REPORT:
            {
                getReportDescriptor();
                return (true);
            }  break;
        }
    }

    if (requestType == USB_SETUP_SET_CLASS_INTER)
    {
        switch (request)
        {
            case SETUP_HID_SET_REPORT:
            {
                switch (valueHi)
                {
                    case REPORT_TYPE_INPUT:
                    {
                        ;
                    }  break;

                    case REPORT_TYPE_OUTPUT:
                    {
                        ackRxSetup();
                        sendControlIn();
                        while (!isRXOut());

                        ackRxOut();
                        sendControlIn();

                        return (true);
                    }  break;

                    // jump to bootloader request handled here
                    case REPORT_TYPE_FEATURE:
                    {
                        ackRxSetup();
                        sendControlIn();
                        while (!isRXOut());

                        if (UEDATX == 0x55)
                        {
                            if (UEDATX == 0xaa)
                            {
                                if (UEDATX == 0x55)
                                {
                                    if (UEDATX == 0xaa)
                                    {
                                        blRequested = true;
                                    }
                                }
                            }
                        }

                        ackRxOut();
                        sendControlIn();

                        while (!isReadControlEnb());

                        return (true);

                    }  break;
                }
            }  break; // SETUP_HID_SET_REPORT

            case SETUP_HID_SET_IDLE:
            {
                setIdle(valueHi);
                return (true);
            }  break;

            case SETUP_HID_SET_PROTOCOL:
            {
                ;
            }  break;
        }
    }

    if (requestType == USB_SETUP_GET_CLASS_INTER)
    {
        switch (request)
        {
            case SETUP_HID_GET_REPORT:
            {
                ;
            }  break;

            case SETUP_HID_GET_IDLE:
            {
                getIdle();
            }  break;

            case SETUP_HID_GET_PROTOCOL:
            {
                ;
            }  break;
        }
    }

    return (false);
}

// These would be more exciting if more than one interface was implemented
void usbHID::setIdle(uint8_t duration)
{
    dummyByte = UEDATX;
    dummyByte = UEDATX;

    reportRate = duration;

    ackRxSetup();
    sendControlIn();

    while (!isReadControlEnb());
}

void usbHID::getIdle()
{
    uint16_t length;

    dummyByte = UEDATX;
    dummyByte = UEDATX;
    LSB(length) = UEDATX;
    MSB(length) = UEDATX;

    if (length)
    {
        UEDATX = reportRate;
        sendControlIn();
    }

    while (!isRXOut());
    ackRxOut();
}

