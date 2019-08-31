/***************************************
* Mostly from ATMEL app notes, with 
* some additions by BTG
***************************************/

#ifndef USB_DEFINES_H_
#define USB_DEFINES_H_

#include <avr/pgmspace.h>

/********************************************
* This is the idVendor that is used by
* BayTek Games.  If you change this, you'll
* have to change a bunch of other stuff on 
* the host side; the application, udev 
* rules, ...  Just don't do it
*
* b337 makes me think of "beer".  
* Beer is good.
********************************************/
#define BTG_MFG_ID          0xb337


#define USB_unicode(a)      ((uint16_t)(a))

// 18 bytes
struct usbDeviceDescriptor
{
    uint8_t     bLength;
    uint8_t 	bDescriptorType;
    uint16_t 	bscUSB;
    uint8_t 	bDeviceClass;
    uint8_t 	bDeviceSubClass;
    uint8_t 	bDeviceProtocol; 
    uint8_t 	bMaxPacketSize0;
    uint16_t 	idVendor;
    uint16_t 	idProduct; 
    uint16_t 	bcdDevice; 
    uint8_t 	iManufacturer;
    uint8_t 	iProduct;
    uint8_t 	iSerialNumber;
    uint8_t 	bNumConfigurations;
};

// 9 bytes
struct S_usb_configuration_descriptor
{
    uint8_t      bLength;              
    uint8_t      bDescriptorType;      
    uint16_t     wTotalLength;         
    uint8_t      bNumInterfaces;       
    uint8_t      bConfigurationValue;  
    uint8_t      iConfiguration;       
    uint8_t      bmAttibutes;          
    uint8_t      MaxPower;             
};

// 9 bytes
struct S_usb_interface_descriptor
{
    uint8_t      bLength;               
    uint8_t      bDescriptorType;       
    uint8_t      bInterfaceNumber;      
    uint8_t      bAlternateSetting;     
    uint8_t      bNumEndpoints;         
    uint8_t      bInterfaceClass;       
    uint8_t      bInterfaceSubClass;    
    uint8_t      bInterfaceProtocol;    
    uint8_t      iInterface;            
};

// 9 bytes
struct S_usb_hid_descriptor
{
    uint8_t     bLength;               // Size of this descriptor in bytes
    uint8_t     bDescriptorType;       // HID descriptor type
    uint16_t    bscHID;                // Binary Coded Decimal Spec. release
    uint8_t     bCountryCode;          // Hardware target country
    uint8_t     bNumDescriptors;       // Number of HID class descriptors to follow
    uint8_t     bRDescriptorType;      // Report descriptor type
    uint16_t    wDescriptorLength;     // Total length of Report descriptor
};

#define SIZE_OF_REPORT          0x35

// 35 bytes
struct S_usb_hid_report_descriptor
{
    uint8_t      report[SIZE_OF_REPORT];
};

// 7 bytes
struct S_usb_endpoint_descriptor
{
    uint8_t      bLength;               
    uint8_t      bDescriptorType;       
    uint8_t      bEndpointAddress;      
    uint8_t      bmAttributes;          
    uint16_t     wMaxPacketSize;        
    uint8_t      bInterval;             
};

// 41 bytes
struct usbUserDeviceDescriptor
{
   S_usb_configuration_descriptor cfg;      //  9 bytes
   S_usb_interface_descriptor     ifc;      //  9 bytes
   S_usb_hid_descriptor           hid;      //  9 bytes
   S_usb_endpoint_descriptor      ep1;      //  7 bytes
   S_usb_endpoint_descriptor      ep2;      //  7 bytes
};

struct S_usb_language_id
{
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    uint16_t    wlangid;
};

struct S_usb_manufacturer_string_descriptor
{
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    uint16_t    wstring[16];
};

struct S_usb_product_string_descriptor
{
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    uint16_t    wstring[32];
};

#define NUMBER_OF_CONFIGURATIONS      1         // number of configurations available

#define EVT_USB_POWERED               1         // USB plugged
#define EVT_USB_UNPOWERED             2         // USB un-plugged
#define EVT_USB_DEVICE_FUNCTION       3         // USB in device
#define EVT_USB_HOST_FUNCTION         4         // USB in host
#define EVT_USB_SUSPEND               5         // USB suspend
#define EVT_USB_WAKE_UP               6         // USB wake up
#define EVT_USB_RESUME                7         // USB resume
#define EVT_USB_RESET                 8         // USB reset

#define MAX_EP_NB             7

#define PIPE_CONTROL          0
#define PIPE_0                0
#define PIPE_1                1
#define PIPE_2                2
#define PIPE_3                3
#define PIPE_4                4
#define PIPE_5                5
#define PIPE_6                6
#define PIPE_7                7

// USB EndPoint
#define MSK_EP_DIR            0x7F
#define MSK_UADD              0x7F
#define MSK_EPTYPE            0xC0
#define MSK_EPSIZE            0x70
#define MSK_EPBK              0x0C
#define MSK_DTSEQ             0x0C
#define MSK_NBUSYBK           0x03
#define MSK_CURRBK            0x03
#define MSK_DAT               0xFF  // UEDATX
#define MSK_BYCTH             0x07  // UEBCHX
#define MSK_BYCTL             0xFF  // UEBCLX
#define MSK_EPINT             0x7F  // UEINT
#define MSK_HADDR             0xFF  // UHADDR

// USB Pipe
#define MSK_PNUM              0x07  // UPNUM
#define MSK_PRST              0x7F  // UPRST
#define MSK_PTYPE             0xC0  // UPCFG0X
#define MSK_PTOKEN            0x30
#define MSK_PEPNUM            0x0F
#define MSK_PSIZE             0x70  // UPCFG1X
#define MSK_PBK               0x0C

#define MSK_NBUSYBK           0x03

#define MSK_ERROR             0x1F

#define MSK_PTYPE             0xC0  // UPCFG0X
#define MSK_PTOKEN            0x30
#define MSK_TOKEN_SETUP       0x30
#define MSK_TOKEN_IN          0x10
#define MSK_TOKEN_OUT         0x20
#define MSK_PEPNUM            0x0F

#define MSK_PSIZE             0x70  // UPCFG1X
#define MSK_PBK               0x0C

#define LANG_ID               0x00
#define MAN_INDEX             0x01
#define PROD_INDEX            0x02  

#define  USB_DEVICE_STATUS_BUS_POWERED       0x00
#define  USB_DEVICE_STATUS_SELF_POWERED      0x01
#define  USB_DEVICE_STATUS_REMOTEWAKEUP      0x02
#define  USB_DEVICE_STATUS_BATTERYPOWERED    0x04

#define USB_CONFIG_ATTRIBUTES_RESERVED      0x80
#define USB_CONFIG_ATTRIBUTES_REMOTEWAKEUP  0x20
#define USB_CONFIG_ATTRIBUTES_SELFPOWERED   0x40
#define USB_CONFIG_BUSPOWERED               (USB_CONFIG_ATTRIBUTES_RESERVED)
#define USB_CONFIG_REMOTEWAKEUP             (USB_CONFIG_ATTRIBUTES_RESERVED | USB_CONFIG_ATTRIBUTES_REMOTEWAKEUP)
#define USB_CONFIG_SELFPOWERED              (USB_CONFIG_ATTRIBUTES_RESERVED | USB_CONFIG_ATTRIBUTES_SELFPOWERED)

// bit 7 is direction: 0 = host to device; 1 = device to host
#define  USB_SETUP_DIR_HOST_TO_DEVICE        (0<<7)
#define  USB_SETUP_DIR_DEVICE_TO_HOST        (1<<7)

// bits 6 & 5 are type: 0 = standard; 1 = class; 2 = vendor; 3 = reserved
#define  USB_SETUP_TYPE_STANDARD             (0<<5)
#define  USB_SETUP_TYPE_CLASS                (1<<5)
#define  USB_SETUP_TYPE_VENDOR               (2<<5)

// lower nibble is recipient:  0 = device; 1 = interface; 2 = endpoint; 3 = other; 4 - 31 reserved
#define  USB_SETUP_RECIPIENT_DEVICE          (0)
#define  USB_SETUP_RECIPIENT_INTERFACE       (1)
#define  USB_SETUP_RECIPIENT_ENDPOINT        (2)
#define  USB_SETUP_RECIPIENT_OTHER           (3)

// or them all together to get setup packet request
#define  USB_SETUP_SET_STAND_DEVICE          (USB_SETUP_DIR_HOST_TO_DEVICE |USB_SETUP_TYPE_STANDARD |USB_SETUP_RECIPIENT_DEVICE)    // 0x00
#define  USB_SETUP_GET_STAND_DEVICE          (USB_SETUP_DIR_DEVICE_TO_HOST |USB_SETUP_TYPE_STANDARD |USB_SETUP_RECIPIENT_DEVICE)    // 0x80
#define  USB_SETUP_SET_STAND_INTERFACE       (USB_SETUP_DIR_HOST_TO_DEVICE |USB_SETUP_TYPE_STANDARD |USB_SETUP_RECIPIENT_INTERFACE) // 0x01
#define  USB_SETUP_GET_STAND_INTERFACE       (USB_SETUP_DIR_DEVICE_TO_HOST |USB_SETUP_TYPE_STANDARD |USB_SETUP_RECIPIENT_INTERFACE) // 0x81
#define  USB_SETUP_SET_STAND_ENDPOINT        (USB_SETUP_DIR_HOST_TO_DEVICE |USB_SETUP_TYPE_STANDARD |USB_SETUP_RECIPIENT_ENDPOINT)  // 0x02
#define  USB_SETUP_GET_STAND_ENDPOINT        (USB_SETUP_DIR_DEVICE_TO_HOST |USB_SETUP_TYPE_STANDARD |USB_SETUP_RECIPIENT_ENDPOINT)  // 0x82

#define  USB_SETUP_SET_CLASS_DEVICE          (USB_SETUP_DIR_HOST_TO_DEVICE |USB_SETUP_TYPE_CLASS |USB_SETUP_RECIPIENT_DEVICE)       // 0x20
#define  USB_SETUP_GET_CLASS_DEVICE          (USB_SETUP_DIR_DEVICE_TO_HOST |USB_SETUP_TYPE_CLASS |USB_SETUP_RECIPIENT_DEVICE)       // 0xA0
#define  USB_SETUP_SET_CLASS_INTER           (USB_SETUP_DIR_HOST_TO_DEVICE |USB_SETUP_TYPE_CLASS |USB_SETUP_RECIPIENT_INTERFACE)    // 0x21
#define  USB_SETUP_GET_CLASS_INTER           (USB_SETUP_DIR_DEVICE_TO_HOST |USB_SETUP_TYPE_CLASS |USB_SETUP_RECIPIENT_INTERFACE)    // 0xA1
#define  USB_SETUP_SET_CLASS_ENDPOINT        (USB_SETUP_DIR_HOST_TO_DEVICE |USB_SETUP_TYPE_CLASS |USB_SETUP_RECIPIENT_ENDPOINT)     // 0x22
#define  USB_SETUP_GET_CLASS_ENDPOINT        (USB_SETUP_DIR_DEVICE_TO_HOST |USB_SETUP_TYPE_CLASS |USB_SETUP_RECIPIENT_ENDPOINT)     // 0xA2
#define  USB_SETUP_SET_CLASS_OTHER           (USB_SETUP_DIR_HOST_TO_DEVICE |USB_SETUP_TYPE_CLASS |USB_SETUP_RECIPIENT_OTHER)        // 0x23
#define  USB_SETUP_GET_CLASS_OTHER           (USB_SETUP_DIR_DEVICE_TO_HOST |USB_SETUP_TYPE_CLASS |USB_SETUP_RECIPIENT_OTHER)        // 0xA3
#define  USB_SETUP_SET_VENDOR_DEVICE         (USB_SETUP_DIR_HOST_TO_DEVICE |USB_SETUP_TYPE_VENDOR |USB_SETUP_RECIPIENT_DEVICE)      // 0x40
#define  USB_SETUP_GET_VENDOR_DEVICE         (USB_SETUP_DIR_DEVICE_TO_HOST |USB_SETUP_TYPE_VENDOR |USB_SETUP_RECIPIENT_DEVICE)      // 0xC0

#define  SETUP_GET_STATUS                    0x00
#define  SETUP_GET_DEVICE                    0x01
#define  SETUP_CLEAR_FEATURE                 0x01
#define  SETUP_GET_STRING                    0x03
#define  SETUP_SET_FEATURE                   0x03
#define  SETUP_SET_ADDRESS                   0x05
#define  SETUP_GET_DESCRIPTOR                0x06
#define  SETUP_SET_DESCRIPTOR                0x07
#define  SETUP_GET_CONFIGURATION             0x08
#define  SETUP_SET_CONFIGURATION             0x09
#define  SETUP_GET_INTERFACE                 0x0A
#define  SETUP_SET_INTERFACE                 0x0B
#define  SETUP_SYNCH_FRAME                   0x0C

#define  SETUP_HID_GET_REPORT                0x01
#define  SETUP_HID_GET_IDLE                  0x02
#define  SETUP_HID_GET_PROTOCOL              0x03
#define  SETUP_HID_SET_REPORT                0x09
#define  SETUP_HID_SET_IDLE                  0x0A
#define  SETUP_HID_SET_PROTOCOL              0x0B

#define  REPORT_TYPE_INPUT                   0x01
#define  REPORT_TYPE_OUTPUT                  0x02
#define  REPORT_TYPE_FEATURE                 0x03

#define  FEATURE_DEVICE_REMOTE_WAKEUP        0x01
#define  FEATURE_DEVICE_TEST                 0x02
#define  FEATURE_DEVICE_OTG_B_HNP_ENABLE     0x03
#define  FEATURE_DEVICE_OTG_A_HNP_SUPPORT    0x04
#define  FEATURE_DEVICE_OTG_A_ALT_HNP_SUPPORT 0x05
#define  FEATURE_ENDPOINT_HALT               0x00

#define  DESCRIPTOR_DEVICE                   0x01
#define  DESCRIPTOR_CONFIGURATION            0x02
#define  DESCRIPTOR_STRING                   0x03
#define  DESCRIPTOR_INTERFACE                0x04
#define  DESCRIPTOR_ENDPOINT                 0x05
#define  DESCRIPTOR_DEVICE_QUALIFIER         0x06
#define  DESCRIPTOR_CONF_OTHER_SPEED         0x07
#define  DESCRIPTOR_OTG                      0x09
#define  DESCRIPTOR_IAD                      0x0B

#define  DESCRIPTOR_HID                       0x21
#define  DESCRIPTOR_REPORT                    0x22
#define  DESCRIPTOR_PHYSICAL                  0x23

#define EP_CONTROL_LENGTH     32

#define  USB_ENDPOINT_BULK                   0x02
#define  USB_ENDPOINT_INTERRUPT              0x03
#define  USB_ENDPOINT_OUT                    0x00
#define  USB_ENDPOINT_IN                     0x80
#define  USB_ENDPOINT_DIR_MASK               0x80
#define  USB_ENDPOINT_NUM_MASK               (~USB_ENDPOINT_DIR_MASK)

// USB Endpoint 1 descriptor FS
#define ENDPOINT_NB_1       (EP_HID_IN | USB_ENDPOINT_IN)
#define EP_ATTRIBUTES_1     0x03          // BULK = 0x02, INTERUPT = 0x03
#define EP_IN_LENGTH        64
#define EP_SIZE_1           EP_IN_LENGTH
#define EP_INTERVAL_1       20 //interrupt pooling from host

// USB Endpoint 1 descriptor FS
#define ENDPOINT_NB_2       (EP_HID_OUT)
#define EP_ATTRIBUTES_2     0x03          // BULK = 0x02, INTERUPT = 0x03
#define EP_OUT_LENGTH       64
#define EP_SIZE_2           EP_OUT_LENGTH
#define EP_INTERVAL_2       20 //interrupt pooling from host


typedef enum _ep 
{
    EP_CONTROL = 0,
    EP_HID_IN,
    EP_HID_OUT,
    EP_3,
    EP_4,
    EP_5,
    EP_6,
    EP_7
} endpoint_t;

typedef enum _endpoint_parameter
{
    ep_num = 0,
    ep_type, 
    ep_direction, 
    ep_size, 
    ep_bank, 
    nyet_status
} endpoint_parameter_t;

typedef enum _ep_type 
{
    TYPE_CONTROL = 0,
    TYPE_BULK, 
    TYPE_ISOCHRONOUS, 
    TYPE_INTERRUPT
} endpoint_type_t;

typedef enum ep_dir 
{
    DIRECTION_OUT = 0,
    DIRECTION_IN
} endpoint_dir_t;

typedef enum ep_size 
{
    SIZE_8 = 0,  
    SIZE_16,  
    SIZE_32,  
    SIZE_64,
    SIZE_128, 
    SIZE_256, 
    SIZE_512, 
    SIZE_1024
} endpoint_size_t;

typedef enum ep_bank 
{
    ONE_BANK = 0, 
    TWO_BANKS
} endpoint_bank_t;

typedef enum ep_nyet 
{
    NYET_ENABLED = 0,
    NYET_DISABLED 
} endpoint_nyet_t;

#endif // USB_DEFINES_H_
