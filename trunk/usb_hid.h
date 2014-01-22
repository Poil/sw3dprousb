/*******************************************************************************
 * File Name	: usb_hid.h
 * Project	: 3DP-Vert, Microsoft Sidewinder 3D Pro/PP/FFP to USB converter
 * Date		: 2009/06/27
 * Version      : 1.0
 * Target MCU	: AT90USB162/82, AT90USB646/1286, ATMEGA16U4/32U4
 * Tool Chain	: Atmel AVR Studio 4.18 716 / WinAVR 20100110
 * Author       : Detlef "Grendel" Mueller
 *                detlef@gmail.com
 * Release Notes:
 *
 * $Id: usb_hid.h 1.5 2009/10/26 07:34:30 Detlef Exp Detlef $
 ******************************************************************************/

#ifndef __usb_hid_h__
#define __usb_hid_h__

//------------------------------------------------------------------------------

extern void
    usb_init( void ) ;			// Initialize USB module

extern uint8_t
    usb_configured( void ) ;		// Check if USB is active

extern uint8_t
    usb_IN_busy( void ) ;		// Check if IN pipe is busy

extern uint8_t				// Send data via IN pipe
    usb_send_IN( uint8_t *data, uint8_t len ) ;

//------------------------------------------------------------------------------

extern volatile uint8_t
    usb_configuration,			// Selected USB configuration, 0 if none
    usb_suspend ;			// Suspended if TRUE

//------------------------------------------------------------------------------

#define ENDPOINT0_SIZE		64

#define EP_HID			4
#define EP_HID_SZ		8	/* next up would be 16 */

//------------------------------------------------------------------------------

#ifdef __usb_hid__

//------------------------------------------------------------------------------
// usb_hid.c local defines
//------------------------------------------------------------------------------

#define	USBLV( h, l )		(((uint16_t)(h) << 8) + (l))

//------------------------------------------------------------------------------

#if defined(__AVR_AT90USBX2__)

 #define HW_CONFIG()
 #define PLL_CONFIG()	(PLLCSR = _B1(PLLE)   | _B0(PLOCK)   | \
				  _B0(PLLP2)  | _B0(PLLP1)   | _B1(PLLP0))
 #define USB_CONFIG()	(USBCON = _B1(USBE)   | _B0(FRZLK))

#elif defined(__AVR_AT90USBX6__)

 #define HW_CONFIG()	(UHWCON = _B1(UVREGE) | \
				  _B1(UIMOD)  | _B0(UIDE)    | _B0(UVCONE))
 #define USB_CONFIG()	(USBCON = _B1(USBE)   | _B0(FRZCLK)  | \
 				  _B0(HOST)   | _B1(OTGPADE) | _B0(IDTE) | _B0(VBUSTE))

 #if defined(__AVR_AT90USB646__)
  #define PLL_CONFIG()	(PLLCSR = _B1(PLLE)   | _B0(PLOCK)   | \
				  _B1(PLLP2)  | _B1(PLLP1)   | _B0(PLLP0))
 #elif defined(__AVR_AT90USB1286__)
  #define PLL_CONFIG()	(PLLCSR = _B1(PLLE)   | _B0(PLOCK)   | \
				  _B1(PLLP2)  | _B0(PLLP1)   | _B1(PLLP0))
 #endif

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

#define EP_SIZE( s )	((s) <=   8 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) <=  16 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) <=  32 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B0(EPSIZE0)) : \
			/* 64 */      (_B0(EPSIZE2) | _B1(EPSIZE1) | _B1(EPSIZE0)))))

#elif defined(__AVR_AT90USBX6__) && ! defined(__AVR_ATmegaXU4__)

// 256 & 128 for EP1 only

#define EP_SIZE( s )	((s) <=   8 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) <=  16 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) <=  32 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) <=  64 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) <= 128 ? (_B1(EPSIZE2) | _B0(EPSIZE1) | _B0(EPSIZE0)) : \
			/* 256 */     (_B1(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)))))))

#elif defined(__AVR_ATmegaXU4__)

#define EP_SIZE( s )	((s) <=   8 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) <=  16 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) <=  32 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) <=  64 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) <= 128 ? (_B1(EPSIZE2) | _B0(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) <= 256 ? (_B1(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)) : \
			/* 512 */     (_B1(EPSIZE2) | _B1(EPSIZE1) | _B0(EPSIZE0))))))))
#endif

//------------------------------------------------------------------------------

#if defined(__AVR_AT90USBX2__)

 #define MAX_ENDPOINT		4

#elif defined(__AVR_ATmegaXU4__) || defined(__AVR_AT90USBX6__)

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

#if defined(__AVR_AT90USBX2__)
 #define EP_HID_BUFFER		((EP_HID  <= 2) ? EP_SINGLE_BUFFER : EP_DOUBLE_BUFFER)
#else
 #define EP_HID_BUFFER		/* EP_SINGLE_BUFFER */ EP_DOUBLE_BUFFER
#endif

//------------------------------------------------------------------------------

#endif // __usb_hid__

//------------------------------------------------------------------------------

#endif // __usb_hid_h__

//------------------------------------------------------------------------------
