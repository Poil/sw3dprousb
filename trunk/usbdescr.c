/*******************************************************************************
 * File Name	: usbdescr.c
 * Project	: Microsoft Sidewinder 3D Pro to USB converter
 * Date		: 2007/06/11, 2009/06/26
 * Version	: 3.0
 * Target MCU	: AT90USB162/AT90USB82, ATMEGA16U4/ATMEGA32U4
 * Tool Chain	: Atmel AVR Studio 4.17 666 / WinAVR 20090313
 * Author	: Detlef "Grendel" Mueller
 *		  detlef@gmail.com
 * References	:
 * Description	: USB to 3D Pro converter USB descriptors
 * Device	: Microsoft Sidewinder 3D Pro/Precision Pro/Force Feedback Pro
 *
 * Release Notes:
 *
 * $Id: usbdescr.c 1.7 2010/04/23 05:33:41 Detlef Exp Detlef $
 ******************************************************************************/

#include "3DPro.h"
#include "usb_hid.h"
#include "usbdescr.h"

//------------------------------------------------------------------------------

const uint8_t
    usbHidRepDesc3DP[] PROGMEM =
    {
	0x05,0x01,		// USAGE_PAGE (Generic Desktop)
	0x09,0x04,		// USAGE (Joystick)
	0xA1,0x01,		// COLLECTION (Application)
	0x09,0x01,		//   USAGE (Pointer)
	0xA1,0x00,		//   COLLECTION (Physical)
	0x95,0x02,		//     REPORT_COUNT (2)
	0x75,0x0A,		//     REPORT_SIZE (10)
	0x16,0x00,0xFE,		//     LOGICAL_MINIMUM (-512)
	0x26,0xFF,0x01,		//     LOGICAL_MAXIMUM (511)
#if HID_PHYS
	0x35,0x00,		//     PHYSICAL_MINIMUM (0)				-2
	0x46,0xFF,0x03,		//     PHYSICAL_MAXIMUM (1023)				-3
#endif
	0xA4,			//     PUSH
	0x09,0x30,		//     USAGE (X)
	0x09,0x31,		//     USAGE (Y)
	0x81,0x02,		//     INPUT (Data,Var,Abs)             20b X/Y
#if SIXAXIS
	0x75,0x08,		//     REPORT_SIZE (8)
	0x15,0x80,		//     LOGICAL_MINIMUM (-128)
	0x25,0x7F,		//     LOGICAL_MAXIMUM (127)
#if HID_PHYS
	0x46,0xFF,0x00,		//     PHYSICAL_MAXIMUM (255)				-3 SIX
#endif
	0x09,0x33,		//     USAGE (Rx)
	0x09,0x34,		//     USAGE (Ry)
	0x81,0x02,		//     INPUT (Data,Var,Abs)             16b Rx/Ry	+15 bytes
#endif
	0x95,0x01,		//     REPORT_COUNT (1)
	0x75,0x09,		//     REPORT_SIZE (9)
	0x16,0x00,0xFF,		//     LOGICAL_MINIMUM (-256)
	0x26,0xFF,0x00,		//     LOGICAL_MAXIMUM (255)
#if HID_PHYS
	0x45,0x2E,		//     PHYSICAL_MAXIMUM (46)				-2
	0x65,0x14,		//     UNIT (Eng Rot:Angular Pos)			-2
#endif
	0x09,0x35,		//     USAGE (Rz)
	0x81,0x02,		//     INPUT (Data,Var,Abs)              9b Rz
#if ! SIXAXIS
	0x75,0x04,		//     REPORT_SIZE (4)
	0x15,0x00,		//     LOGICAL_MINIMUM (0)
	0x25,0x07,		//     LOGICAL_MAXIMUM (7)
#if HID_PHYS
	0x46,0x3B,0x01,		//     PHYSICAL_MAXIMUM (315)				-3 ! SIX
#endif
	0x09,0x39,		//     USAGE (Hat switch)
	0x81,0x42,		//     INPUT (Data,Var,Abs,Null)         4b Hat		-13 bytes
#endif
	0x95,0x04,		//     REPORT_COUNT (4)
	0x75,0x01,		//     REPORT_SIZE (1)
#if SIXAXIS
	0x15,0x00,		//     LOGICAL_MINIMUM (0)				 +2 bytes
#endif
	0x25,0x01,		//     LOGICAL_MAXIMUM (1)
#if HID_PHYS
	0x45,0x01,		//     PHYSICAL_MAXIMUM (1)				-2
	0x65,0x00,		//     UNIT (None)					-2
#endif
	0x05,0x09,		//     USAGE_PAGE (Button)
	0x19,0x01,		//     USAGE_MINIMUM (Button 1)
	0x29,0x04,		//     USAGE_MAXIMUM (Button 4)
	0x81,0x02,		//     INPUT (Data,Var,Abs)              4b B1-4
	0xC0,			//   END_COLLECTION
	0x19,0x05,		//   USAGE_MINIMUM (Button 5)
	0x29,0x08,		//   USAGE_MAXIMUM (Button 8)
	0x81,0x02,		//   INPUT (Data,Var,Abs)                4b B5-8
	0xB4,			//   POP
	0x95,0x01,		//   REPORT_COUNT (1)
	0x09,0x36,		//   USAGE (Slider)
	0x81,0x02,		//   INPUT (Data,Var,Abs)               10b Throttle
#if SIXAXIS
	0x75,0x01,		//   REPORT_SIZE (1)			 1b Fill
#else
	0x75,0x05,		//   REPORT_SIZE (5)			 5b Fill
#endif
	0x81,0x01,		//   INPUT (Cnst,Ary,Abs)
	0xC0			// END_COLLECTION                       56b -> 7bytes, 8 bytes for SIXAXIS
    } ;

//------------------------------------------------------------------------------

const uint8_t
    usbHidRepDescFFP[] PROGMEM =
    {
	0x05, 0x01,		// USAGE_PAGE (Generic Desktop)
	0x09, 0x04,		// USAGE (Joystick)
	0xA1, 0x01,		// COLLECTION (Application)
	0x09, 0x01,		//  USAGE (Pointer)
	0xA1, 0x00,		//   COLLECTION (Physical)
	0x95, 0x02,		//     REPORT_COUNT (2)
	0x75, 0x0A,		//     REPORT_SIZE (10)
	0x16, 0x00, 0xFE,	//     LOGICAL_MINIMUM (-512)
	0x26, 0xFF, 0x01,	//     LOGICAL_MAXIMUM (511)
#if HID_PHYS
	0x35, 0x00,		//     PHYSICAL_MINIMUM (0)				-2
	0x46, 0xFF, 0x03,	//     PHYSICAL_MAXIMUM (1023)				-3
#endif
	0x09, 0x30,		//     USAGE (X)
	0x09, 0x31,		//     USAGE (Y)
	0x81, 0x02,		//     INPUT (Data,Var,Abs)		20b X/Y
	0x95, 0x01,		//     REPORT_COUNT (1)
	0x75, 0x04,		//     REPORT_SIZE (4)
	0x15, 0x00,		//     LOGICAL_MINIMUM (0)
	0x25, 0x07,		//     LOGICAL_MAXIMUM (7)
#if HID_PHYS
	0x46, 0x3B, 0x01,	//     PHYSICAL_MAXIMUM (315)				-3
	0x65, 0x14,		//     UNIT (Eng Rot:Angular Pos)			-2
#endif
	0x09, 0x39,		//     USAGE (Hat switch)
	0x81, 0x42,		//     INPUT (Data,Var,Abs,Null)	 4b Hat
	0x75, 0x06,		//     REPORT_SIZE (6)
	0x15, 0xE0,		//     LOGICAL_MINIMUM (-32)
	0x25, 0x1F,		//     LOGICAL_MAXIMUM (31)
#if HID_PHYS
	0x45, 0x2E,		//     PHYSICAL_MAXIMUM (46)				-2
#endif
	0x09, 0x35,		//     USAGE (Rz)
	0x81, 0x02,		//     INPUT (Data,Var,Abs)		 6b Rz
	0x95, 0x04,		//     REPORT_COUNT (4)
	0x75, 0x01,		//     REPORT_SIZE (1)
	0x15, 0x00,		//     LOGICAL_MINIMUM (0)
	0x25, 0x01,		//     LOGICAL_MAXIMUM (1)
#if HID_PHYS
	0x45, 0x01,		//     PHYSICAL_MAXIMUM (1)				-2
	0x65, 0x00,		//     UNIT (None)					-2
#endif
	0xA4,			//     PUSH
	0x05, 0x09,		//     USAGE_PAGE (Button)
	0x19, 0x01,		//     USAGE_MINIMUM (Button 1)
	0x29, 0x04,		//     USAGE_MAXIMUM (Button 4)
	0x81, 0x02,		//     INPUT (Data,Var,Abs)		 4b B1-4
	0xC0,			//   END_COLLECTION
	0x95, 0x05,		//   REPORT_COUNT (5)
	0x19, 0x05,		//   USAGE_MINIMUM (Button 5)
	0x29, 0x09,		//   USAGE_MAXIMUM (Button 9)
	0x81, 0x02,		//   INPUT (Data,Var,Abs)		 5b B5-9
	0xB4,			//   POP
	0x95, 0x01,		//   REPORT_COUNT (1)
	0x75, 0x07,		//   REPORT_SIZE (7)
	0x15, 0xC0,		//   LOGICAL_MINIMUM (-64)
	0x25, 0x3F,		//   LOGICAL_MAXIMUM (63)
#if HID_PHYS
	0x45, 0x7F,		//   PHYSICAL_MAXIMUM (127)				-2
#endif
	0x09, 0x36,		//   USAGE (Slider)
	0x81, 0x02,		//   INPUT (Data,Var,Abs)		 7b Throttle
	0x75, 0x02,		//   REPORT_SIZE (2)
	0x81, 0x01,		//   INPUT (Cnst,Ary,Abs)		 2b Fill
	0xC0			// END_COLLECTION			48b -> 6bytes
    } ;

//------------------------------------------------------------------------------

const usb_string_desc_t
    usbStringDescLan PROGMEM = { USB_STRING_DESC_LAN_SZ, USB_STRING_DESC_TYPE, { USB_STRING_LAN } },
    usbStringDescMan PROGMEM = { USB_STRING_DESC_MAN_SZ, USB_STRING_DESC_TYPE, { USB_STRING_MAN } },
    usbStringDescSer PROGMEM = { USB_STRING_DESC_SER_SZ, USB_STRING_DESC_TYPE, { USB_STRING_SER } },
    usbStringDescPro3DP PROGMEM = { USB_STRING_DESC_PRO_3DP_SZ, USB_STRING_DESC_TYPE, { USB_STRING_PRO_3DP } },
    usbStringDescProPP  PROGMEM = { USB_STRING_DESC_PRO_PP_SZ,  USB_STRING_DESC_TYPE, { USB_STRING_PRO_PP  } },
    usbStringDescProFFP PROGMEM = { USB_STRING_DESC_PRO_FFP_SZ, USB_STRING_DESC_TYPE, { USB_STRING_PRO_FFP } } ;

//------------------------------------------------------------------------------

const usb_device_desc_t
    usbDeviceDesc3DP PROGMEM =
    {
	USB_DEVICE_DESC_SZ,		// Size of this descriptor in bytes (18)
	USB_DEVICE_DESC_TYPE,		// Device (0x01)
	USB_DEVICE_USBVER,		// USB Specification which device complies too
	USB_DEVICE_CLASS,		// Class code. 0 if interf. defines class
	USB_DEVICE_SUBCLASS,		// Subclass code
	USB_DEVICE_PROTOCOL,		// Protocol code
	USB_DEVICE_MAXPKTSZ,		// Max. packet size for EP0. 8, 16,32, or 64
	USB_DEVICE_MAN_ID,		// Vendor ID
	USB_DEVICE_PRO_ID_3DP,		// Product ID
	USB_DEVICE_DEVICE_VERSION,	// Device release number
	USB_DEVICE_IDX_MAN,		// Index of manufacturer string descr.
	USB_DEVICE_IDX_PRO_3DP,		// Index of product string descr.
	USB_DEVICE_IDX_SER,		// Index of serial number string descr.
	USB_DEVICE_NUM_CONFIGS		// Number of possible configurations (1)
    },
    usbDeviceDescPP PROGMEM =
    {
	USB_DEVICE_DESC_SZ,		// Size of this descriptor in bytes (18)
	USB_DEVICE_DESC_TYPE,		// Device (0x01)
	USB_DEVICE_USBVER,		// USB Specification which device complies too
	USB_DEVICE_CLASS,		// Class code. 0 if interf. defines class
	USB_DEVICE_SUBCLASS,		// Subclass code
	USB_DEVICE_PROTOCOL,		// Protocol code
	USB_DEVICE_MAXPKTSZ,		// Max. packet size for EP0. 8, 16,32, or 64
	USB_DEVICE_MAN_ID,		// Vendor ID
	USB_DEVICE_PRO_ID_PP,		// Product ID
	USB_DEVICE_DEVICE_VERSION,	// Device release number
	USB_DEVICE_IDX_MAN,		// Index of manufacturer string descr.
	USB_DEVICE_IDX_PRO_PP,		// Index of product string descr.
	USB_DEVICE_IDX_SER,		// Index of serial number string descr.
	USB_DEVICE_NUM_CONFIGS		// Number of possible configurations (1)
    },
    usbDeviceDescFFP PROGMEM =
    {
	USB_DEVICE_DESC_SZ,		// Size of this descriptor in bytes (18)
	USB_DEVICE_DESC_TYPE,		// Device (0x01)
	USB_DEVICE_USBVER,		// USB Specification which device complies too
	USB_DEVICE_CLASS,		// Class code. 0 if interf. defines class
	USB_DEVICE_SUBCLASS,		// Subclass code
	USB_DEVICE_PROTOCOL,		// Protocol code
	USB_DEVICE_MAXPKTSZ,		// Max. packet size for EP0. 8, 16,32, or 64
	USB_DEVICE_MAN_ID,		// Vendor ID
	USB_DEVICE_PRO_ID_FFP,		// Product ID
	USB_DEVICE_DEVICE_VERSION,	// Device release number
	USB_DEVICE_IDX_MAN,		// Index of manufacturer string descr.
	USB_DEVICE_IDX_PRO_FFP,		// Index of product string descr.
	USB_DEVICE_IDX_SER,		// Index of serial number string descr.
	USB_DEVICE_NUM_CONFIGS		// Number of possible configurations (1)
    } ;

//------------------------------------------------------------------------------

const usb_configuration_desc_t
    usbConfigurationDesc3DP PROGMEM =
    {
	{				// Configuration Descriptor
	    USB_CONFIG_DESC_SZ,		// Size of this descriptor in bytes (9)
	    USB_CONFIG_DESC_TYPE,	// Config (0x02)
	    wUSB( sizeof( usb_configuration_desc_t ) ),// Total size of this configuration
	    USB_CONFIG_NUM_INTERF,	// Number of interfaces in this configuration
	    USB_CONFIG_VAL,		// Value to use to select this configuration
	    USB_CONFIG_IDX,		// Index of string descr. this configuration
	    USB_CONFIG_ATTR,		// Configuration characteristics
	    USB_CONFIG_MAXPOWER		// Max. power consumption in 2mA units
	},
	{				// Interface Descriptor
	    USB_INTERF_DESC_SZ,		// Size of this descriptor in bytes (9)
	    USB_INTERF_DESC_TYPE,	// Interface (0x04)
	    USB_INTERF_NUM,		// Number of this interface (0 based)
	    USB_INTERF_ALTSET_NONE,	// Value to select alternative setting
	    USB_INTERF_NUMEP,		// Number of EPs for this interface
	    USB_INTERF_CLASS_HID,	// Class code
	    USB_INTERF_SUBCLASS_NONE,	// Subclass code
	    USB_INTERF_PROTOCOL_NONE,	// Protocol code
	    USB_INTERF_IDX		// Index of string descr. this interface
	},
	{				// HID Descriptor Keyboard
	    USB_HID_DESC_SZ,		// Size of this descriptor in bytes (9)
	    USB_HID_DESC_TYPE,		// HID (0x21)
	    USB_HID_VERSION,		// HID class version suported (1.11)
	    USB_HID_COUNTRY_ANY,	// Country code
	    USB_HID_NUM_DESC_1,		// Number of class descriptors (1, report)
	    USB_HIDREP_DESC_TYPE,	// Class descriptor type (0x22, HID report)
	    wUSB( sizeof( usbHidRepDesc3DP ) )	// Class descriptor length
	},
	{				// EP Descriptor Keyboard
	    USB_EP_DESC_SZ,		// Size of this descriptor in bytes (7)
	    USB_EP_DESC_TYPE,		// Endpoint (0x05)
	    (EP_HID | USB_EP_IN),	// Endpoint address
	    USB_EP_INTR,		// Endpoint attributes
	    wUSB( EP_HID_SZ ),		// Max. packet size this EP can transfer
	    USB_EP_POLL_INTERVAL	// Polling interval for this EP in ms
	}
    },
    usbConfigurationDescFFP PROGMEM =
    {
	{				// Configuration Descriptor
	    USB_CONFIG_DESC_SZ,		// Size of this descriptor in bytes (9)
	    USB_CONFIG_DESC_TYPE,	// Config (0x02)
	    wUSB( sizeof( usb_configuration_desc_t ) ),// Total size of this configuration
	    USB_CONFIG_NUM_INTERF,	// Number of interfaces in this configuration
	    USB_CONFIG_VAL,		// Value to use to select this configuration
	    USB_CONFIG_IDX,		// Index of string descr. this configuration
	    USB_CONFIG_ATTR,		// Configuration characteristics
	    USB_CONFIG_MAXPOWER		// Max. power consumption in 2mA units
	},
	{				// Interface Descriptor
	    USB_INTERF_DESC_SZ,		// Size of this descriptor in bytes (9)
	    USB_INTERF_DESC_TYPE,	// Interface (0x04)
	    USB_INTERF_NUM,		// Number of this interface (0 based)
	    USB_INTERF_ALTSET_NONE,	// Value to select alternative setting
	    USB_INTERF_NUMEP,		// Number of EPs for this interface
	    USB_INTERF_CLASS_HID,	// Class code
	    USB_INTERF_SUBCLASS_NONE,	// Subclass code
	    USB_INTERF_PROTOCOL_NONE,	// Protocol code
	    USB_INTERF_IDX		// Index of string descr. this interface
	},
	{				// HID Descriptor Keyboard
	    USB_HID_DESC_SZ,		// Size of this descriptor in bytes (9)
	    USB_HID_DESC_TYPE,		// HID (0x21)
	    USB_HID_VERSION,		// HID class version suported (1.11)
	    USB_HID_COUNTRY_ANY,	// Country code
	    USB_HID_NUM_DESC_1,		// Number of class descriptors (1, report)
	    USB_HIDREP_DESC_TYPE,	// Class descriptor type (0x22, HID report)
	    wUSB( sizeof( usbHidRepDescFFP ) )	// Class descriptor length
	},
	{				// EP Descriptor Keyboard
	    USB_EP_DESC_SZ,		// Size of this descriptor in bytes (7)
	    USB_EP_DESC_TYPE,		// Endpoint (0x05)
	    (EP_HID | USB_EP_IN),	// Endpoint address
	    USB_EP_INTR,		// Endpoint attributes
	    wUSB( EP_HID_SZ ),		// Max. packet size this EP can transfer
	    USB_EP_POLL_INTERVAL	// Polling interval for this EP in ms
	}
    } ;

//------------------------------------------------------------------------------
