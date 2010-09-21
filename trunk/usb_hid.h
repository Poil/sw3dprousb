/*******************************************************************************
 * File Name	: usb_hid.h
 * Project	: 3DP-Vert, Microsoft Sidewinder 3D Pro/PP/FFP to USB converter
 * Date		: 2009/06/27
 * Version      : 1.0
 * Target MCU	: AT90USB162/AT90USB82, AT90USB646, ATMEGA16U4/ATMEGA32U4
 * Tool Chain	: Atmel AVR Studio 4.17 666 / WinAVR 20090313
 * Author       : Detlef "Grendel" Mueller
 *                detlef@gmail.com
 * Release Notes:
 *
 * $Id: usb_hid.h 1.5 2009/10/26 07:34:30 Detlef Exp Detlef $
 *
 * $Log: usb_hid.h $
 * Revision 1.5  2009/10/26 07:34:30  Detlef
 * Using prog_* or PROGMEM instead of VA_PROGMEM().
 *
 * Revision 1.4  2009/10/26 06:39:57  Detlef
 * Removed HID_IDLE switch, we always want it
 *
 * Revision 1.3  2009/10/26 06:32:46  Detlef
 * Added Mega32U4 support (Teensy 2.0)
 *
 * Revision 1.2  2009/09/29 03:39:49  Detlef
 * Some cleanup.
 *
 * Revision 1.1  2009/07/10 07:04:38  Detlef
 * Initial revision
 *
 ******************************************************************************/

#ifndef __usb_hid_h__
#define __usb_hid_h__

//------------------------------------------------------------------------------

extern void
    usb_init( void ) ;			// Initialize USB module

extern uint8_t
    usb_IN_busy( void ) ;		// Check if IN pipe is busy

extern uint8_t				// Send data via IN pipe
    usb_send_IN( uint8_t *data, uint8_t len ) ;

extern volatile uint8_t
    usb_configuration ;			// Selected USB configuration,
					// 1 if enumerated, else 0

//------------------------------------------------------------------------------
// USB descriptor constants

#define USBDESCR_DEVICE		1
#define USBDESCR_CONFIG		2
#define USBDESCR_STRING		3
#define USBDESCR_INTERFACE	4
#define USBDESCR_ENDPOINT	5
#define USBDESCR_HID		0x21
#define USBDESCR_HID_REPORT	0x22
#define USBDESCR_HID_PHYS	0x23

//------------------------------------------------------------------------------

#define USB_CFG_INTR_POLL_INTERVAL 5

//------------------------------------------------------------------------------

#define ENDPOINT0_SIZE		64

#define HID_ENDPOINT		1
#define HIDEP_SIZE		8	/* next up would be 16 */

//------------------------------------------------------------------------------

#ifdef __usb_hid__		// usb_hid.c local defines

//------------------------------------------------------------------------------

#define	USBLV( h, l )		(((uint16_t)(h) << 8) + (l))

//------------------------------------------------------------------------------

#if defined(__AVR_AT90USBX2__)

 #define HW_CONFIG()
 #define PLL_CONFIG()	(PLLCSR = _B1(PLLE)   | _B0(PLOCK)   | \
				  _B0(PLLP2)  | _B0(PLLP1)   | _B1(PLLP0))
 #define USB_CONFIG()	(USBCON = _B1(USBE)   | _B0(FRZLK))

#elif defined(__AVR_AT90USB646__)

 #define HW_CONFIG()	(UHWCON = _B1(UVREGE) | \
				  _B1(UIMOD)  | _B0(UIDE)    | _B0(UVCONE))
 #define PLL_CONFIG()	(PLLCSR = _B1(PLLE)   | _B0(PLOCK)   | \
				  _B1(PLLP2)  | _B1(PLLP1)   | _B0(PLLP0))
 #define USB_CONFIG()	(USBCON = _B1(USBE)   | _B0(FRZCLK)  | \
 				  _B0(HOST)   | _B1(OTGPADE) | _B0(IDTE) | _B0(VBUSTE))

#elif defined(__AVR_ATmegaXU4__)

 #define HW_CONFIG()	(UHWCON = _B1(UVREGE))
 #define PLL_CONFIG()	(PLLCSR = _B1(PLLE)   | _B0(PLOCK)   | _B1(PINDIV))
 #define USB_CONFIG()	(USBCON = _B1(USBE)   | _B0(FRZCLK)  | \
				  _B1(OTGPADE)| _B0(VBUSTE))
#else
 #error "Current device not supported"
#endif

//------------------------------------------------------------------------------

#define EP_TYPE_CONTROL		(_B0(EPTYPE1) | _B0(EPTYPE0) | _B0(EPDIR))
#define EP_TYPE_BULK_IN		(_B1(EPTYPE1) | _B0(EPTYPE0) | _B1(EPDIR))
#define EP_TYPE_BULK_OUT	(_B1(EPTYPE1) | _B0(EPTYPE0) | _B0(EPDIR))
#define EP_TYPE_INTERRUPT_IN	(_B1(EPTYPE1) | _B1(EPTYPE0) | _B1(EPDIR))
#define EP_TYPE_INTERRUPT_OUT	(_B1(EPTYPE1) | _B1(EPTYPE0) | _B0(EPDIR))
#define EP_TYPE_ISOCHRONOUS_IN	(_B0(EPTYPE1) | _B1(EPTYPE0) | _B1(EPDIR))
#define EP_TYPE_ISOCHRONOUS_OUT	(_B0(EPTYPE1) | _B1(EPTYPE0) | _B0(EPDIR))

#define EP_SINGLE_BUFFER	(_B0(EPBK1) | _B0(EPBK0) | _B1(ALLOC))
#define EP_DOUBLE_BUFFER	(_B0(EPBK1) | _B1(EPBK0) | _B1(ALLOC))

//------------------------------------------------------------------------------

#if defined(__AVR_AT90USBX2__)

#define EP_SIZE( s )	((s) == 64 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) == 32 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) == 16 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)) : \
			0x00)))

#elif defined(__AVR_AT90USB646__) || defined(__AVR_ATmegaXU4__)

// 256 & 128 for EP1 only

#define EP_SIZE( s )	((s) == 256 ? (_B1(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) == 128 ? (_B1(EPSIZE2) | _B0(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) ==  64 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) ==  32 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) ==  16 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)) : \
			0x00)))))

#elif defined(__AVR_ATmegaXU4__)

#define EP_SIZE( s )	((s) == 512 ? (_B1(EPSIZE2) | _B1(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) == 256 ? (_B1(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) == 128 ? (_B1(EPSIZE2) | _B0(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) ==  64 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) ==  32 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) ==  16 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)) : \
			0x00))))))
#endif

//------------------------------------------------------------------------------

#if defined(__AVR_AT90USBX2__)

 #define MAX_ENDPOINT		4

#elif defined(__AVR_ATmegaXU4__) || defined(__AVR_AT90USB646__)

 #define MAX_ENDPOINT		6

#endif

//------------------------------------------------------------------------------
// Standard control endpoint request types

#define GET_STATUS		0
#define CLEAR_FEATURE		1
#define SET_FEATURE		3
#define SET_ADDRESS		5
#define GET_DESCRIPTOR		6
#define GET_CONFIGURATION	8
#define SET_CONFIGURATION	9
#define GET_INTERFACE		10
#define SET_INTERFACE		11

//------------------------------------------------------------------------------
// HID (human interface device)

#define HID_GET_REPORT		1
#define HID_GET_IDLE		2
#define HID_GET_PROTOCOL	3
#define HID_SET_REPORT		9
#define HID_SET_IDLE		10
#define HID_SET_PROTOCOL	11

#define HIDEP_BUFFER		EP_SINGLE_BUFFER /* EP_DOUBLE_BUFFER */

//------------------------------------------------------------------------------

#endif // __usb_hid__

//------------------------------------------------------------------------------

#endif // __usb_hid_h__

//------------------------------------------------------------------------------
