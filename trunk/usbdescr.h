/*******************************************************************************
 * File Name	: usbdescr.h
 * Project	: Microsoft Sidewinder 3D Pro to USB converter
 * Date		: 2005/05/31, 2006/12/14
 * Version	: 2.0
 * Target MCU	: AT90USB162/AT90USB82, ATMEGA16U4/ATMEGA32U4
 * Tool Chain	: Atmel AVR Studio 4.17 666 / WinAVR 20090313
 * Author	: Detlef "Grendel" Mueller
 *		  detlef@gmail.com
 * Release Notes:
 *
 * $Id: usbdescr.h 1.7 2010/04/23 05:34:15 Detlef Exp Detlef $
 ******************************************************************************/

#ifndef	_USBDESCR_H_
#define	_USBDESCR_H_

//------------------------------------------------------------------------------

#define LSB( n )			( (uint16_t)(n)       & 0xFF)
#define MSB( n )			(((uint16_t)(n) >> 8) & 0xFF)

#define	LVAL( n )			LSB( n ), MSB( n )

#define	wUSB( n )			((uint16_t)(n))

//------------------------------------------------------------------------------

#define USB_MANUFACTURER_ID		wUSB( 0x045E )	/* Microsoft */

#define	USB_PRODUCT_ID_3DP		wUSB( 0x0002 )
#define	USB_PRODUCT_ID_PP		wUSB( 0x0004 )
#define	USB_PRODUCT_ID_FFP		wUSB( 0x0005 )
#define	USB_PRODUCT_ID_6DP		wUSB( 0xBEEF )

#if defined(FFP_HW)
 #define USB_DEVICE_VERSION		wUSB( 0x0401 )	/* 4.01 */
#else
 #define USB_DEVICE_VERSION		wUSB( 0x0400 )	/* 4.00 */
#endif

#define USB_STRING_MAN			'D','e','t','l','e','f',' ', \
					'<','G','r','e','n','d','e','l','>', ' ', \
					'M','u','e','l','l','e','r'
#define USB_STRING_MAN_SZ		24

#define USB_STRING_PRO_3DP		'M','i','c','r','o','s','o','f','t',' ', \
					'S','i','d','e','W','i','n','d','e','r',' ', \
					'3' + SIXAXIS * 3,'D',' ','P','r','o',' ', \
					'(','U','S','B',')'
#define USB_STRING_PRO_3DP_SZ		33

#define USB_STRING_PRO_PP		'M','i','c','r','o','s','o','f','t',' ', \
					'S','i','d','e','W','i','n','d','e','r',' ', \
					'P','r','e','c','i','s','i','o','n',' ',\
					'P','r','o',' ','(','U','S','B',')'
#define USB_STRING_PRO_PP_SZ		40

#define USB_STRING_PRO_FFP		'M','i','c','r','o','s','o','f','t',' ', \
					'S','i','d','e','W','i','n','d','e','r',' ', \
					'F','o','r','c','e',' ', \
					'F','e','e','d','b','a','c','k',' ', \
					'P','r','o',' ','(','U','S','B',')'
#define USB_STRING_PRO_FFP_SZ		45

#define USB_STRING_SER			'0', '3', '3', '3'
#define USB_STRING_SER_SZ		4

#define	USB_MAXPOWER			100		/* 100mA */

#define	USB_EP_POLL_INTERVAL		2		/*  2ms polling */

//-------------------------------------------------------------------------------
// Report descriptor sizes

#define	USB_HIDREP_DESC_3DP_SZ		(84 + HID_PHYS * 16 + SIXAXIS * 4)
#define	USB_HIDREP_DESC_FFP_SZ		(92 + HID_PHYS * 18)	/* PP, FFP */

//------------------------------------------------------------------------------

// USB Descriptor definitions

//------------------------------------------------------------------------------

typedef struct
    usb_string_desc_struct
    {
	uint8_t  bLength ;		// Size of this descriptor in bytes
	uint8_t  bDescriptorType ;	// String (0x03)
	uint16_t wString[] ;		// Unicode encoded string
    }
    usb_string_desc_t ;

//------------------------------------------------------------------------------

#define	USB_STRING_DESC_TYPE		0x03
#define USB_STRING_DESC_SZ( sz )	(sizeof( uint8_t ) * 2 + sizeof( int16_t ) * (sz))

#define	USB_STRING_LAN			wUSB( 0x0409 )
#define	USB_STRING_LAN_SZ		1

#define	USB_STRING_DESC_LAN_SZ		USB_STRING_DESC_SZ( USB_STRING_LAN_SZ )
#define	USB_STRING_DESC_MAN_SZ		USB_STRING_DESC_SZ( USB_STRING_MAN_SZ )
#define	USB_STRING_DESC_PRO_3DP_SZ	USB_STRING_DESC_SZ( USB_STRING_PRO_3DP_SZ )
#define	USB_STRING_DESC_PRO_PP_SZ	USB_STRING_DESC_SZ( USB_STRING_PRO_PP_SZ )
#define	USB_STRING_DESC_PRO_FFP_SZ	USB_STRING_DESC_SZ( USB_STRING_PRO_FFP_SZ )
#define	USB_STRING_DESC_SER_SZ		USB_STRING_DESC_SZ( USB_STRING_SER_SZ )

#define	USB_STRING_IDX_NUL		0
#define	USB_STRING_IDX_LAN		0
#define	USB_STRING_IDX_MAN		1
#define	USB_STRING_IDX_SER		2
#define	USB_STRING_IDX_PRO_3DP		3
#define	USB_STRING_IDX_PRO_PP		4
#define	USB_STRING_IDX_PRO_FFP		5

//------------------------------------------------------------------------------

typedef struct
    usb_device_desc_struct
    {
	uint8_t  bLength ;		// Size of this descriptor in bytes (18)
	uint8_t  bDescriptorType ;	// Device (0x01)
	uint16_t bcdUSB ;		// USB Specification which device complies too
	uint8_t  bDeviceClass ;		// Class code. 0 if interf. defines class
	uint8_t  bDeviceSubClass ;	// Subclass code
	uint8_t  bDeviceProtocol ;	// Protocol code
	uint8_t  bMaxPacketSize ;	// Max. packet size for EP0. 8, 16,32, or 64
	uint16_t idVendor ;		// Vendor ID
	uint16_t idProduct ;		// Product ID
	uint16_t bcdDevice ;		// Device release number
	uint8_t  iManufacturer ;	// Index of manufacturer string descr.
	uint8_t  iProduct ;		// Index of product string descr.
	uint8_t  iSerialNumber ;	// Index of serial number string descr.
	uint8_t  bNumConfigurations ;	// Number of possible configurations (1)
    }
    usb_device_desc_t ;

//------------------------------------------------------------------------------

#define	USB_DEVICE_DESC_SZ		18
#define	USB_DEVICE_DESC_TYPE		0x01

#define	USB_DEVICE_USBVER		wUSB( 0x0200 )
#define	USB_DEVICE_CLASS		0
#define	USB_DEVICE_SUBCLASS		0
#define	USB_DEVICE_PROTOCOL		0
#define	USB_DEVICE_MAXPKTSZ		ENDPOINT0_SIZE

#define USB_DEVICE_MAN_ID		USB_MANUFACTURER_ID
#define	USB_DEVICE_PRO_ID_3DP		USB_PRODUCT_ID_3DP
#define	USB_DEVICE_PRO_ID_PP		USB_PRODUCT_ID_PP
#define	USB_DEVICE_PRO_ID_FFP		USB_PRODUCT_ID_FFP
#define USB_DEVICE_DEVICE_VERSION	USB_DEVICE_VERSION

#define	USB_DEVICE_IDX_MAN		USB_STRING_IDX_MAN
#define	USB_DEVICE_IDX_PRO_3DP		USB_STRING_IDX_PRO_3DP
#define	USB_DEVICE_IDX_PRO_PP		USB_STRING_IDX_PRO_PP
#define	USB_DEVICE_IDX_PRO_FFP		USB_STRING_IDX_PRO_FFP
#define	USB_DEVICE_IDX_SER		USB_STRING_IDX_SER

#define	USB_DEVICE_NUM_CONFIGS		1

//------------------------------------------------------------------------------

typedef struct
    usb_config_desc_struct
    {
	uint8_t  bLength ;		// Size of this descriptor in bytes (9)
	uint8_t  bDescriptorType ;	// Config (0x02)
	uint16_t wTotalLength ;		// Total size of this configuration
	uint8_t  bNumInterfaces ;	// Number of interfaces in this configuration
	uint8_t  bConfigurationValue ;	// Value to use to select this configuration
	uint8_t  iConfiguration ;	// Index of string descr. this configuration
	uint8_t  bmAttributes ;		// Configuration characteristics
	uint8_t  bMaxPower ;		// Max. power consumption in 2mA units
    }
    usb_config_desc_t ;

//------------------------------------------------------------------------------

#define	USB_CONFIG_DESC_SZ		9
#define	USB_CONFIG_DESC_TYPE		0x02

#define	USB_CONFIG_NUM_INTERF		1

#define	USB_CONFIG_VAL			1
#define	USB_CONFIG_IDX			USB_STRING_IDX_NUL

#define USB_CONFIG_ATTR_BUSPOWER	0x80	/* Mandatory attr., see USB 2.0 9.6.3 */
#define USB_CONFIG_ATTR_SELFPOWER	0x40
#define USB_CONFIG_ATTR_REMOTEWAKE	0x20

#define	USB_CONFIG_ATTR			(USB_CONFIG_ATTR_BUSPOWER)

#define	USB_CONFIG_MAXPOWER		(USB_MAXPOWER / 2)

//------------------------------------------------------------------------------

typedef struct
    usb_interf_desc_struct
    {
	uint8_t  bLength ;		// Size of this descriptor in bytes (9)
	uint8_t  bDescriptorType ;	// Interface (0x04)
	uint8_t  bInterfaceNumber ;	// Number of this interface (0 based)
	uint8_t  bAlternativeSetting ;	// Value to select alternative setting
	uint8_t  bNumEndpoints ;	// Number of EPs for this interface
	uint8_t  bInterfaceClass ;	// Class code
	uint8_t  bInterfaceSubClass ;	// Subclass code
	uint8_t  bInterfaceProtocol ;	// Protocol code
	uint8_t  iInterface ;		// Index of string descr. this interface
    }
    usb_interf_desc_t ;

//------------------------------------------------------------------------------

#define	USB_INTERF_DESC_SZ		9
#define	USB_INTERF_DESC_TYPE		0x04

#define	USB_INTERF_NUM			0

#define	USB_INTERF_ALTSET_NONE		0

#define	USB_INTERF_NUMEP		1

#define	USB_INTERF_CLASS_HID		3
#define	USB_INTERF_SUBCLASS_NONE	0
#define	USB_INTERF_SUBCLASS_BOOT	1
#define	USB_INTERF_PROTOCOL_NONE	0
#define	USB_INTERF_PROTOCOL_KBD		1

#define	USB_INTERF_IDX			USB_STRING_IDX_NUL

//------------------------------------------------------------------------------

typedef struct
    usb_hid_desc_struct
    {
	uint8_t  bLength ;		// Size of this descriptor in bytes (9)
	uint8_t  bDescriptorType ;	// HID (0x21)
	uint16_t bcdHID ;		// HID class version suported (1.11)
	uint8_t  bCountryCode ;		// Country code
	uint8_t  bNumDescriptors ;	// Number of class descriptors (1, report)
	uint8_t  bDescrType ;		// Class descriptor type (0x22, HID report)
	uint16_t wDescrLength ;		// Class descriptor length
    }
    usb_hid_desc_t ;

//------------------------------------------------------------------------------

#define	USB_HID_DESC_SZ			9
#define	USB_HID_DESC_TYPE		0x21

#define	USB_HID_VERSION			wUSB( 0x0111 )
#define	USB_HID_COUNTRY_ANY		0
#define	USB_HID_NUM_DESC_1		1

#define	USB_HIDREP_DESC_TYPE		0x22

//------------------------------------------------------------------------------

typedef struct
    usb_ep_desc_struct
    {
	uint8_t  bLength ;		// Size of this descriptor in bytes (7)
	uint8_t  bDescriptorType ;	// Endpoint (0x05)
	uint8_t  bEndpointAddress ;	// Endpoint address
	uint8_t  bmAttributes ;		// Endpoint attributes
	uint16_t wMaxPacketSize ;	// Max. packet size this EP can transfer
	uint8_t  bInterval ;		// Polling interval for this EP in ms
    }
    usb_ep_desc_t ;

//------------------------------------------------------------------------------

#define	USB_EP_DESC_SZ			7
#define	USB_EP_DESC_TYPE		0x05

#define	USB_EP_IN			0x80
#define	USB_EP_INTR			0x03

//------------------------------------------------------------------------------

typedef struct
    usb_configuration_desc_struct
    {
	usb_config_desc_t cfg ;

	usb_interf_desc_t itrf ;
	usb_hid_desc_t    hid ;
	usb_ep_desc_t     ep ;
    }
    usb_configuration_desc_t ;

//------------------------------------------------------------------------------

// Descriptor declarations

extern const uint8_t
    usbHidRepDesc3DP[USB_HIDREP_DESC_3DP_SZ] PROGMEM,
    usbHidRepDescFFP[USB_HIDREP_DESC_FFP_SZ] PROGMEM ;

extern const usb_string_desc_t
    usbStringDescLan PROGMEM,
    usbStringDescMan PROGMEM,
    usbStringDescPro3DP PROGMEM,
    usbStringDescProPP PROGMEM,
    usbStringDescProFFP PROGMEM,
    usbStringDescSer PROGMEM ;

extern const usb_device_desc_t
    usbDeviceDesc3DP PROGMEM,
    usbDeviceDescPP PROGMEM,
    usbDeviceDescFFP PROGMEM ;

extern const usb_configuration_desc_t
    usbConfigurationDesc3DP PROGMEM,
    usbConfigurationDescFFP PROGMEM ;

//------------------------------------------------------------------------------

#endif	// _USBDESCR_H_

//------------------------------------------------------------------------------
