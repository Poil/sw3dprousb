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

const prog_uint8_t
    usbHidReportDesc3DP[] =
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
	0x75,0x04,		//     REPORT_SIZE (4)
	0x15,0x00,		//     LOGICAL_MINIMUM (0)
	0x25,0x07,		//     LOGICAL_MAXIMUM (7)
#if HID_PHYS
	0x46,0x3B,0x01,		//     PHYSICAL_MAXIMUM (315)				-3 ! SIX
#endif
	0x09,0x39,		//     USAGE (Hat switch)
	0x81,0x42,		//     INPUT (Data,Var,Abs,Null)         4b Hat		-13 bytes
	0x95,0x04,		//     REPORT_COUNT (4)
	0x75,0x01,		//     REPORT_SIZE (1)
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
	0x75,0x05,		//   REPORT_SIZE (5)			 5b Fill
	0x81,0x01,		//   INPUT (Cnst,Ary,Abs)
	0xC0			// END_COLLECTION                       56b -> 7bytes
    } ;

//------------------------------------------------------------------------------

const prog_uint8_t
    usbHidReportDescFFP[] =
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

const usb_string_descriptor_t
    usbDescStringLan =
	{ USB_STR_LAN_SZ,     USBDESCR_STRING, { 0x0409 }          },
    usbDescStringSer =
	{ USB_STR_SER_SZ,     USBDESCR_STRING, { CFG_STR_SER }     },
    usbDescStringMan =
	{ USB_STR_MAN_SZ,     USBDESCR_STRING, { CFG_STR_MAN }     },
    usbDescStringDevice3DP =
	{ USB_STR_DEV_3DP_SZ, USBDESCR_STRING, { CFG_STR_DEV_3DP } },
    usbDescStringDevicePP =
	{ USB_STR_DEV_PP_SZ,  USBDESCR_STRING, { CFG_STR_DEV_PP }  },
    usbDescStringDeviceFFP =
	{ USB_STR_DEV_FFP_SZ, USBDESCR_STRING, { CFG_STR_DEV_FFP } } ;

//------------------------------------------------------------------------------

const prog_uint8_t
    usbDescDevice3DP[] =
    {				// USB device descriptor
	18,			// length of descriptor in bytes
	USBDESCR_DEVICE,	// descriptor type
	LVAL( 0x0200 ),		// USB version supported, 2.00
	USB_CFG_DEVICE_CLASS,
	USB_CFG_DEVICE_SUBCLASS,
	0,			// protocol
	ENDPOINT0_SIZE,		// EP0 size
	USB_CFG_MAN_ID,		// manufacturer id
	CFG_DEVICE_ID_3DP,	// device id
	USB_CFG_DEVICE_VERSION,	// device version
	USB_STR_IDX_MAN,	// manufacturer string index
	USB_STR_IDX_3DP,	// product string index
	USB_STR_IDX_SER,	// serial number string index
	1			// number of configurations
    } ;

//------------------------------------------------------------------------------

const prog_uint8_t
    usbDescDevicePP[] =
    {				// USB device descriptor
	18,			// length of descriptor in bytes
	USBDESCR_DEVICE,	// descriptor type
	LVAL( 0x0200 ),		// USB version supported, 2.00
	USB_CFG_DEVICE_CLASS,
	USB_CFG_DEVICE_SUBCLASS,
	0,			// protocol
	ENDPOINT0_SIZE,		// EP0 size
	USB_CFG_MAN_ID,		// manufacturer id
	CFG_DEVICE_ID_PP,	// device id
	USB_CFG_DEVICE_VERSION,	// device version
	USB_STR_IDX_MAN,	// manufacturer string index
	USB_STR_IDX_PP,		// product string index
	USB_STR_IDX_SER,	// serial number string index
	1			// number of configurations
    } ;

//------------------------------------------------------------------------------

const prog_uint8_t
    usbDescDeviceFFP[] =
    {				// USB device descriptor
	18,			// length of descriptor in bytes
	USBDESCR_DEVICE,	// descriptor type
	LVAL( 0x0200 ),		// USB version supported
	USB_CFG_DEVICE_CLASS,
	USB_CFG_DEVICE_SUBCLASS,
	0,			// protocol
	ENDPOINT0_SIZE,		// EP0 size
	USB_CFG_MAN_ID,		// manufacturer id
	CFG_DEVICE_ID_FFP,	// device id
	USB_CFG_DEVICE_VERSION,	// device version
	USB_STR_IDX_MAN,	// manufacturer string index
	USB_STR_IDX_FFP,	// product string index
	USB_STR_IDX_SER,	// serial number string index
	1			// number of configurations
    } ;

//------------------------------------------------------------------------------

const prog_uint8_t
    usbDescConfiguration3DP[] =
    {				// USB configuration descriptor
	9,			// length of descriptor in bytes
	USBDESCR_CONFIG,	// descriptor type
	LVAL( 9 + 9 + 9 + 7 ),	// total length of data returned (including inlined descriptors)
	1,			// number of interfaces in this configuration
	1,			// index of this configuration
	0,			// configuration name string index, none
	USBATTR_BUSPOWER,	// attributes
	USB_CFG_MAX_BUS_POWER/2,// max USB current in 2mA units
				// Interface descriptor
	9,			// length of descriptor in bytes
	USBDESCR_INTERFACE,	// descriptor type
	0,			// index of this interface
	0,			// alternate setting for this interface, none
	1,			// endpoints excl 0: number of endpoint descriptors to follow
	USB_CFG_INTERFACE_CLASS,
	USB_CFG_INTERFACE_SUBCLASS,
	USB_CFG_INTERFACE_PROTOCOL,
	0,			// string index for interface, none
				// HID descriptor
	9,			// length of descriptor in bytes
	USBDESCR_HID,		// descriptor type: HID
	LVAL( 0x0111 ),		// BCD representation of HID version, 1.11
	0,			// target country code
	1,			// number of HID report (or other HID class) descriptor infos to follow
	USBDESCR_HID_REPORT,	// descriptor type: report
	LVAL( sizeof( usbHidReportDesc3DP ) ), // total length of report descriptor
				// Endpoint descriptor for endpoint 1
	7,			// length of descriptor in bytes
	USBDESCR_ENDPOINT,	// descriptor type = endpoint
	0x80 | HID_ENDPOINT,	// IN endpoint number
	0x03,			// attrib: interrupt endpoint
	LVAL( HIDEP_SIZE ),	// packet size
	USB_CFG_INTR_POLL_INTERVAL // poll interval in ms
    } ;

//------------------------------------------------------------------------------

const prog_uint8_t
    usbDescConfigurationFFP[] =
    {				// USB configuration descriptor
	9,			// length of descriptor in bytes
	USBDESCR_CONFIG,	// descriptor type
	LVAL( 9 + 9 + 9 + 7 ),	// total length of data returned (including inlined descriptors)
	1,			// number of interfaces in this configuration
	1,			// index of this configuration
	0,			// configuration name string index, none
	USBATTR_BUSPOWER,	// attributes
	USB_CFG_MAX_BUS_POWER/2,// max USB current in 2mA units
				// Interface descriptor
	9,			// length of descriptor in bytes
	USBDESCR_INTERFACE,	// descriptor type
	0,			// index of this interface
	0,			// alternate setting for this interface, none
	1,			// endpoints excl 0: number of endpoint descriptors to follow
	USB_CFG_INTERFACE_CLASS,
	USB_CFG_INTERFACE_SUBCLASS,
	USB_CFG_INTERFACE_PROTOCOL,
	0,			// string index for interface, none
				// HID descriptor
	9,			// length of descriptor in bytes
	USBDESCR_HID,		// descriptor type: HID
	LVAL( 0x0111 ),		// BCD representation of HID version, 1.11
	0,			// target country code
	1,			// number of HID report (or other HID class) descriptor infos to follow
	USBDESCR_HID_REPORT,	// descriptor type: report
	LVAL( sizeof( usbHidReportDescFFP ) ), // total length of report descriptor
				// Endpoint descriptor for endpoint 1
	7,			// length of descriptor in bytes
	USBDESCR_ENDPOINT,	// descriptor type = endpoint
	0x80 | HID_ENDPOINT,	// IN endpoint number
	0x03,			// attrib: interrupt endpoint
	LVAL( HIDEP_SIZE ),	// packet size
	USB_CFG_INTR_POLL_INTERVAL // poll interval in ms
    } ;

//------------------------------------------------------------------------------
