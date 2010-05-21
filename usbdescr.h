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
 *
 * $Log: usbdescr.h $
 * Revision 1.7  2010/04/23 05:34:15  Detlef
 * Added USB_STR_SZ(), some cleanup.
 *
 * Revision 1.6  2009/10/27 09:20:38  Detlef
 * Added const qualifiers
 *
 * Revision 1.5  2009/10/26 07:34:30  Detlef
 * Using prog_* or PROGMEM instead of VA_PROGMEM().
 *
 * Revision 1.4  2009/10/26 06:32:46  Detlef
 * Added Mega32U4 support (Teensy 2.0)
 *
 * Revision 1.3  2009/09/29 03:38:58  Detlef
 * Some cleanup.
 *
 * Revision 1.2  2009/07/23 10:32:10  Detlef
 * Added HID_PHYS macro - if defined > 0, include physical attributes
 * in the HID report descriptor, else omit them.
 *
 * Revision 1.1  2009/07/10 07:04:38  Detlef
 * Initial revision
 *
 ******************************************************************************/

#ifndef	_USBDESCR_H_
#define	_USBDESCR_H_

//------------------------------------------------------------------------------
// Descriptor defines

#define	LVAL( n )		LSB( n ), MSB( n )

#define USB_CFG_MAN_ID		LVAL( 0x045E )		/* Microsoft */

#define CFG_DEVICE_ID_3DP	LVAL( 0x0002 )
#define CFG_DEVICE_ID_PP	LVAL( 0x0004 )
#define CFG_DEVICE_ID_FFP	LVAL( 0x0005 )
#define CFG_DEVICE_ID_6DP	LVAL( 0xBEEF )

#define USB_CFG_DEVICE_VERSION	LVAL( 0x0400 )		/* 4.00 */

#define USB_CFG_MAX_BUS_POWER	100			/* in mA */

#define CFG_STR_SER		'0', '3', '0', '3'	/* 1st ATUSB proto */
#define CFG_STR_SER_SZ		4

#define CFG_STR_MAN		'D','e','t','l','e','f',' ', \
				'<','G','r','e','n','d','e','l','>', ' ', \
				'M','u','e','l','l','e','r'
#define CFG_STR_MAN_SZ		24

#define CFG_STR_DEV_3DP		'M','i','c','r','o','s','o','f','t',' ', \
				'S','i','d','e','W','i','n','d','e','r',' ', \
				'3' + SIXAXIS * 3,'D',' ','P','r','o',' ', \
				'(','U','S','B',')'
#define CFG_STR_DEV_3DP_SZ	33

#define CFG_STR_DEV_PP		'M','i','c','r','o','s','o','f','t',' ', \
				'S','i','d','e','W','i','n','d','e','r',' ', \
				'P','r','e','c','i','s','i','o','n',' ',\
				'P','r','o',' ','(','U','S','B',')'
#define CFG_STR_DEV_PP_SZ	40

#define CFG_STR_DEV_FFP		'M','i','c','r','o','s','o','f','t',' ', \
				'S','i','d','e','W','i','n','d','e','r',' ', \
				'F','o','r','c','e',' ', \
				'F','e','e','d','b','a','c','k',' ', \
				'P','r','o',' ','(','U','S','B',')'
#define CFG_STR_DEV_FFP_SZ	45

#define CFG_HID_REPORT_DESC_SZ_3DP (84 + HID_PHYS * 16 + COMPDEV * 39 + SIXAXIS * 4)
#define CFG_HID_REPORT_DESC_SZ_FFP (92 + HID_PHYS * 18 + COMPDEV * 39)	/* PP, FFP */

#define USB_STR_SZ( sz )	(sizeof( uint8_t ) * 2 + sizeof( int16_t ) * (sz))

#define	USB_STR_LAN_SZ		USB_STR_SZ( 1 )
#define	USB_STR_MAN_SZ		USB_STR_SZ( CFG_STR_MAN_SZ )
#define	USB_STR_SER_SZ		USB_STR_SZ( CFG_STR_SER_SZ )
#define	USB_STR_DEV_3DP_SZ	USB_STR_SZ( CFG_STR_DEV_3DP_SZ )
#define	USB_STR_DEV_PP_SZ	USB_STR_SZ( CFG_STR_DEV_PP_SZ )
#define	USB_STR_DEV_FFP_SZ	USB_STR_SZ( CFG_STR_DEV_FFP_SZ )

#define	USB_STR_IDX_LAN		0
#define	USB_STR_IDX_MAN		1
#define	USB_STR_IDX_SER		2
#define	USB_STR_IDX_3DP		3
#define	USB_STR_IDX_PP		4
#define	USB_STR_IDX_FFP		5

#define	USB_DEVICE_DESC_SZ      18
#define	USB_CONFIG_DESC_SZ      (9 + 9 + 9 + 7)

//------------------------------------------------------------------------------
// Descriptor declarations

extern const prog_uint8_t
    usbHidReportDesc3DP[CFG_HID_REPORT_DESC_SZ_3DP],
    usbHidReportDescFFP[CFG_HID_REPORT_DESC_SZ_FFP] ;

typedef struct
    usb_string_descriptor_struct
    {
	uint8_t bLength ;
	uint8_t bDescriptorType ;
	int16_t wString[] ;
    }
    TA_PROGMEM( usb_string_descriptor_t ) ;

extern const usb_string_descriptor_t
    usbDescStringLan,
    usbDescStringMan,
    usbDescStringSer,
    usbDescStringDevice3DP,
    usbDescStringDevicePP,
    usbDescStringDeviceFFP ;

extern const prog_uint8_t
    usbDescDevice3DP[USB_DEVICE_DESC_SZ],
    usbDescDevicePP [USB_DEVICE_DESC_SZ],
    usbDescDeviceFFP[USB_DEVICE_DESC_SZ] ;

extern const prog_uint8_t
    usbDescConfiguration3DP[USB_CONFIG_DESC_SZ],
    usbDescConfigurationFFP[USB_CONFIG_DESC_SZ] ;

//------------------------------------------------------------------------------

#endif	// _USBDESCR_H_

//------------------------------------------------------------------------------
