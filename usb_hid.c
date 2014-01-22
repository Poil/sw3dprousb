/*******************************************************************************
 * File Name	: usb_hid.c
 * Project	: 3DP-Vert, Microsoft Sidewinder 3D Pro/PP/FFP to USB converter
 * Date		: 2009/06/27
 * Version      : 1.0
 * Target MCU	: AT90USB162/82, AT90USB646/1286, ATMEGA16U4/32U4
 * Tool Chain	: Atmel AVR Studio 4.18 716 / WinAVR 20100110
 * Author       : Detlef "Grendel" Mueller
 *                detlef@gmail.com
 * References	: usb_mouse.c by PRJC
 * Release Notes: Extremly simplyfied chapter 9 implementation.
 *
 * $Id: usb_hid.c 1.7 2010/04/23 05:35:08 Detlef Exp Detlef $
 ******************************************************************************/

#include "3DPro.h"

#define __usb_hid__

#include "usb_hid.h"
#include "usbdescr.h"

#include <avr/eeprom.h>

//******************************************************************************
//  Endpoint Buffer Configuration
//******************************************************************************

static const uint8_t
    endpoint_config_table[] PROGMEM =
    {
	FALSE,
	FALSE,
	FALSE,
	TRUE, EP_TYPE_INTERRUPT_IN, EP_SIZE( EP_HID_SZ ) | EP_HID_BUFFER
      #if defined(__AVR_ATmegaXU4__) || defined(__AVR_AT90USBX6__)
	,FALSE
	,FALSE
      #endif
    } ;

#define	SUPPORT_ENDPOINT_HALT	1

//******************************************************************************
//  Descriptor Data
//******************************************************************************

// These tables map a host descriptor request to the actual data.

#define	IN_RAM		0
#define	IN_ROM		1
#define	IN_EEP		2

// If any descriptor id > 255 bytes in size (ie. HID Report
// Descriptors), define LARGE_DESC (probably could be automated)

#if defined(LARGE_DESC)
 typedef uint16_t desc_sz_t ;
#else
 typedef uint8_t  desc_sz_t ;
#endif

// With a single Interface, wIndex can be ignored for descriptor look-ups
// Note: multi language would have to enable this as well.

#if USB_CONFIG_NUM_INTERF > 1
 #define M_IF( _c )		_c
 #define M_IF_C( _c )		_c,
#else
 #define M_IF( _c )
 #define M_IF_C( _c)
#endif

typedef struct
    {
	uint16_t	wValue ;
  M_IF( uint16_t	wIndex ; )
	const uint8_t	*addr ;
	uint8_t		where ;
	desc_sz_t	length ;
    }
    descriptor_list_t ;

static const descriptor_list_t
    str_descriptors[] PROGMEM =
    {
	{ USBLV( USB_STRING_DESC_TYPE, USB_STRING_IDX_LAN ),     M_IF_C(0x0000)
	  VP( &usbStringDescLan  ),    IN_ROM, USB_STRING_DESC_LAN_SZ },
	{ USBLV( USB_STRING_DESC_TYPE, USB_STRING_IDX_MAN ),     M_IF_C(0x0409)
	  VP( &usbStringDescMan ),     IN_ROM, USB_STRING_DESC_MAN_SZ },
	{ USBLV( USB_STRING_DESC_TYPE, USB_STRING_IDX_SER ),     M_IF_C(0x0409)
	  NULL,                        IN_EEP, USB_STRING_DESC_SER_SZ },
	{ USBLV( USB_STRING_DESC_TYPE, USB_STRING_IDX_PRO_3DP ), M_IF_C(0x0409)
	  VP( &usbStringDescPro3DP ),  IN_ROM, USB_STRING_DESC_PRO_3DP_SZ },
	{ USBLV( USB_STRING_DESC_TYPE, USB_STRING_IDX_PRO_PP  ), M_IF_C(0x0409)
	  VP( &usbStringDescProPP ),   IN_ROM, USB_STRING_DESC_PRO_PP_SZ  },
	{ USBLV( USB_STRING_DESC_TYPE, USB_STRING_IDX_PRO_FFP ), M_IF_C(0x0409)
	  VP( &usbStringDescProFFP ),  IN_ROM, USB_STRING_DESC_PRO_FFP_SZ }
    },
    cfg_descriptors_3DP[] PROGMEM =
    {
	{  USBLV( USB_DEVICE_DESC_TYPE, 0 ),	M_IF_C(0)
	   VP( &usbDeviceDesc3DP ),		IN_ROM, sizeof( usb_device_desc_t )
	},
	{  USBLV( USB_CONFIG_DESC_TYPE, 0 ),	M_IF_C(0)
	   VP( &usbConfigurationDesc3DP ),	IN_ROM, sizeof( usb_configuration_desc_t )
	},
	{  USBLV( USB_HID_DESC_TYPE, 0 ),	M_IF_C(USB_INTERF_NUM)
	  VP( &usbConfigurationDesc3DP.hid ),	IN_ROM, sizeof( usb_hid_desc_t )
	},
	{ USBLV( USB_HIDREP_DESC_TYPE, 0 ),	M_IF_C(USB_INTERF_NUM)
	  VP( &usbHidRepDesc3DP ),		IN_ROM, sizeof( usbHidRepDesc3DP )
	}
    },
    cfg_descriptors_PP[] PROGMEM =
    {
	{  USBLV( USB_DEVICE_DESC_TYPE, 0 ),	M_IF_C(0)
	   VP( &usbDeviceDescPP ),		IN_ROM, sizeof( usb_device_desc_t )
	},
	{  USBLV( USB_CONFIG_DESC_TYPE, 0 ),	M_IF_C(0)
	   VP( &usbConfigurationDescFFP ),	IN_ROM, sizeof( usb_configuration_desc_t )
	},
	{  USBLV( USB_HID_DESC_TYPE, 0 ),	M_IF_C(USB_INTERF_NUM)
	  VP( &usbConfigurationDescFFP.hid ),	IN_ROM, sizeof( usb_hid_desc_t )
	},
	{ USBLV( USB_HIDREP_DESC_TYPE, 0 ),	M_IF_C(USB_INTERF_NUM)
	  VP( &usbHidRepDescFFP ),		IN_ROM, sizeof( usbHidRepDescFFP )
	}
    },
    cfg_descriptors_FFP[] PROGMEM =
    {
	{  USBLV( USB_DEVICE_DESC_TYPE, 0 ),	M_IF_C(0)
	   VP( &usbDeviceDescFFP ),		IN_ROM, sizeof( usb_device_desc_t )
	},
	{  USBLV( USB_CONFIG_DESC_TYPE, 0 ),	M_IF_C(0)
	   VP( &usbConfigurationDescFFP ),	IN_ROM, sizeof( usb_configuration_desc_t )
	},
	{  USBLV( USB_HID_DESC_TYPE, 0 ),	M_IF_C(USB_INTERF_NUM)
	  VP( &usbConfigurationDescFFP.hid ),	IN_ROM, sizeof( usb_hid_desc_t )
	},
	{ USBLV( USB_HIDREP_DESC_TYPE, 0 ),	M_IF_C(USB_INTERF_NUM)
	  VP( &usbHidRepDescFFP ),		IN_ROM, sizeof( usbHidRepDescFFP )
	}
    } ;

//******************************************************************************
//  Variables
//******************************************************************************

volatile uint8_t
    usb_configuration,			// zero when not configured
    usb_suspend ;			// Suspended

//******************************************************************************

static void chk_serial ( void )
{
    uint8_t
	i, *s, *d ;

    d = NULL ;
    i = USB_STRING_DESC_SER_SZ ;

    if ( eeprom_read_byte( d ) == i )
	return ;

    for ( s = VP( &usbStringDescSer ) ; i-- ; ++s, ++d )
	eeprom_write_byte( d, pgm_read_byte( s ) ) ;
}

//******************************************************************************
//  Public Functions
//******************************************************************************

// Initialize USB system

void usb_init ( void )
{
    chk_serial() ;

    HW_CONFIG() ;

    // fire up USB

    USBCON = _B1(USBE) | _B1(FRZCLK) ;	// enable USB module w/ frozen clock

    // config PLL, wait for PLL lock

    for ( PLL_CONFIG() ; bit_is_clear( PLLCSR, PLOCK ) ; )
	;

    USB_CONFIG() ;			// start USB clock

    UDCON = _B0(DETACH) ;		// enable attach resistor

    UDIEN = _B1(EORSTE) |		// setup RX
//	    _B1(SOFE) |			// SOF
//	    _B1(WAKEUPE) |		// wakeup
	    _B1(SUSPE)  ;		// suspend
}

//------------------------------------------------------------------------------

// Check if the USB is configured

uint8_t usb_configured ( void )
{
    uint8_t
	f ;

    CRITICAL_VAR() ;

    ENTER_CRITICAL() ;

    f = (usb_configuration && ! usb_suspend) ;

    EXIT_CRITICAL_RET( f ) ;
}

//------------------------------------------------------------------------------

// Check if the IN pipe is busy

uint8_t usb_IN_busy ( void )
{
    uint8_t
	f ;

    CRITICAL_VAR() ;

    ENTER_CRITICAL() ;

    if ( ! usb_configuration || usb_suspend )
	EXIT_CRITICAL_RET( FALSE ) ;	// Pipe closed

    UENUM = EP_HID ;			// select EP

    f = bit_is_clear( UEINTX, RWAL ) ;

    EXIT_CRITICAL_RET( f ) ;
}

//------------------------------------------------------------------------------

// Send data via IN pipe

uint8_t usb_send_IN ( uint8_t *data, uint8_t len )
{
    CRITICAL_VAR() ;

    ENTER_CRITICAL() ;

    if ( ! usb_configuration || usb_suspend )
	EXIT_CRITICAL_RET( FALSE ) ;	// Pipe closed

    UENUM = EP_HID ;

    // We really don't need the timeout, since main() only
    // calls if ! usb_IN_busy().
#if DEAD_CODE

    uint8_t
	to ;

    for ( to = UDFNUML + 10 ; bit_is_clear( UEINTX, RWAL ) ; )
    {
	EXIT_CRITICAL() ;

	// Timeout ?

	if ( UDFNUML == to )
	    return ( FALSE ) ;

	ENTER_CRITICAL() ;

	// has the USB gone offline ?

	if ( ! usb_configuration )
	    EXIT_CRITICAL_RET( FALSE ) ;

	UENUM = EP_HID ;
    }
#endif

    for ( ; len-- ; )			// Write data to FIFO
	UEDATX = *data++ ;

    UEINTX =
	  _B0(FIFOCON)			// Send FIFO data & switch bank
	| _B0(NAKINI)			// Clear previous NAK
	| _B0(RWAL)			// Don't mess w/ RWAL
	| _B1(NAKOUTI)			// no effect on the bit
	| _B1(RXSTPI)			// no effect on the bit
	| _B0(RXOUTI)			// no effect on the bit
	| _B1(STALLEDI)			// no effect on the bit
	| _B0(TXINI) ;			// clear TX ready int flag

    EXIT_CRITICAL_RET( TRUE ) ;
}

//******************************************************************************
//  Private Functions
//******************************************************************************

// USB Device Interrupt

ISR( USB_GEN_vect )
{
    uint8_t
	intbits ;

    intbits = UDINT ;
    UDINT   = 0 ;

    if ( intbits & _BV( EORSTI ) )	// USB reset
    {
	UENUM   = 0 ;
	UECONX  = 1 ;
	UECFG0X = EP_TYPE_CONTROL ;
	UECFG1X = EP_SIZE( ENDPOINT0_SIZE ) | EP_SINGLE_BUFFER ;
	UEIENX  = _BV( RXSTPE ) ;

	usb_configuration = 0 ;
	usb_suspend = FALSE ;

	return ;
    }

    if ( usb_configuration )
    {
	if ( (intbits & _BV(SUSPI)) && bit_is_set( UDIEN, SUSPE ) )
	{
	    // Enter suspend

	    clr_bit( UDIEN, SUSPE ) ;		// Disable suspend IRQ
	    set_bit( UDIEN, WAKEUPE ) ;		// Enable wakeup IRQ

	    set_bit( USBCON, FRZCLK ) ;		// Stop USB clcok
	    clr_bit( PLLCSR, PLLE ) ;		// Stop PLL

	    usb_suspend = TRUE ;
	}

	if ( (intbits & _BV(WAKEUPI)) && bit_is_set( UDIEN, WAKEUPE ) )
	{
	    // Exit suspend

	    for ( set_bit( PLLCSR, PLLE ) ; bit_is_clear( PLLCSR, PLOCK ) ; )
		;				// Start PLL

	    clr_bit( USBCON, FRZCLK ) ;		// Enable USB clock

	    set_bit( UDIEN, SUSPE ) ;		// Enable suspend IRQ
	    clr_bit( UDIEN, WAKEUPE ) ;		// Disable wakeup IRQ

	    usb_suspend = FALSE ;
	}

// SOF disabled, don't need it
//
//	if ( intbits & _BV( SOFI ) )
//	{
//
//	}
    }
}

//------------------------------------------------------------------------------

// Misc functions to wait for ready and send/receive packets

// Wait until packet TX

static inline void usb_wait_in_ready ( void )
{
    for ( ; bit_is_clear( UEINTX, TXINI ) ; )
	;
}

// Trigger TX

static inline void usb_send_in ( void )	
{
    UEINTX = ~_BV( TXINI ) ;
}

// Wait until packet RX

static inline void usb_wait_receive_out ( void )
{
    for ( ; bit_is_clear( UEINTX, RXOUTI ) ; )
	;
}

// Rearm RX

static inline void usb_ack_out ( void )
{
    UEINTX = ~_BV( RXOUTI ) ;
}

//------------------------------------------------------------------------------

// Send data block via EP0

static void FA_NOINLINE( usb_send_EP0 ) ( const uint8_t *p, desc_sz_t len, uint8_t where )
{
    uint8_t
	i, n ;

    do
    {
	// wait until IN packet sent

	do
	{
	    i = UEINTX ;
	}
	while ( bits_are_clear( i, _BV( TXINI ) | _BV( RXOUTI ) ) ) ;

	if ( i & _BV( RXOUTI ) )
	    return ;				// Got OUT packet, abort

	// Set up IN packet

	n = len < ENDPOINT0_SIZE ? len : ENDPOINT0_SIZE ;

	for ( i = n ; i-- ; ++p )
	    if ( where == IN_RAM )
		UEDATX = *p ;
	    else
	    if ( where == IN_ROM )
		UEDATX = pgm_read_byte( p ) ;
	    else
	    if ( where == IN_EEP )
		UEDATX = eeprom_read_byte( p ) ;

	usb_send_in() ;

	len -= n ;
    }
    while ( len || n == ENDPOINT0_SIZE ) ;	// Send ZLP if ! (len % EP0sz)
}

//------------------------------------------------------------------------------

// Look up a descriptor

static uint8_t * FA_NOINLINE( lookup )
    ( uint16_t wValue, M_IF_C( uint16_t wIndex ) const descriptor_list_t *desc, uint8_t len )
{
    for ( ; len-- ; ++desc )
	if (          pgm_read_word( &desc->wValue ) == wValue
	     M_IF( && pgm_read_word( &desc->wIndex ) == wIndex )
	   )
	    return ( VP( desc ) ) ;

    return ( NULL ) ;
}

//------------------------------------------------------------------------------

// USB Endpoint Interrupt - endpoint 0 is handled here.

ISR( USB_COM_vect )
{
    uint16_t
	wValue, wIndex, wLength ;
    uint8_t
	bmRequestType, bRequest,
	i, n,
	*p ;

    UENUM = 0 ;				// Select EP0

    i = UEINTX ;

    if ( i & _BV( RXSTPI ) )
    {
	bmRequestType = UEDATX ;
	bRequest      = UEDATX ;

	wValue   =  UEDATX ;
	wValue  |= (UEDATX << 8) ;

	wIndex   =  UEDATX ;
	wIndex  |= (UEDATX << 8) ;

	wLength  =  UEDATX ;
	wLength |= (UEDATX << 8) ;

	UEINTX = ~(_BV( RXSTPI ) | _BV( RXOUTI ) | _BV( TXINI )) ;

	if ( bRequest == GET_DESCRIPTOR )
	{
	    if ( ! (p = lookup( wValue, M_IF_C(wIndex) str_descriptors, ARRSZ( str_descriptors ) )) )
	    {
		if ( sw_id == SW_ID_3DP )
		    p = lookup( wValue, M_IF_C(wIndex) cfg_descriptors_3DP, ARRSZ( cfg_descriptors_3DP ) ) ;
		else
		if ( sw_id == SW_ID_PP )
		    p = lookup( wValue, M_IF_C(wIndex) cfg_descriptors_PP,  ARRSZ( cfg_descriptors_PP ) ) ;
		else
		if ( sw_id == SW_ID_FFP )
		    p = lookup( wValue, M_IF_C(wIndex) cfg_descriptors_FFP, ARRSZ( cfg_descriptors_FFP ) ) ;

		if ( ! p )
		{
		    UECONX = _BV( STALLRQ ) | _BV( EPEN ) ; // stall
		    return ;
		}
	    }

	  #if defined(LARGE_DESC)
	    // Get length and start address

	    wIndex = pgm_read_word( &((descriptor_list_t *)p)->length ) ;

	    if ( wLength > wIndex )
		wLength = wIndex ;

	    n = pgm_read_byte( &((descriptor_list_t *)p)->where ) ;

	    p = VP( pgm_read_word( &((descriptor_list_t *)p)->addr ) ) ;

	    usb_send_EP0( p, wLength, n ) ;
	  #else
	    // Get length and start address

	    n = pgm_read_byte( &((descriptor_list_t *)p)->length ) ;

	    // Note: code limits max. descriptor size to 255

	    if ( wLength & 0xFF00 )
		i = 255 ;
	    else
		i = wLength ;

	    if ( i > n )
		i = n ;

	    n = pgm_read_byte( &((descriptor_list_t *)p)->where ) ;

	    p = VP( pgm_read_word( &((descriptor_list_t *)p)->addr ) ) ;

	    usb_send_EP0( p, i, n ) ;
	  #endif

	    return ;
	}

	if ( bRequest == SET_ADDRESS )
	{
	    usb_send_in() ;			// Ack via ZLP
	    usb_wait_in_ready() ;		// Wait until ZLP sent

	    UDADDR = wValue | _BV( ADDEN ) ;	// Set & enable USB address

	    return ;
	}

	if ( bRequest == SET_CONFIGURATION && bmRequestType == 0 )
	{
	    usb_send_in() ;			// Ack via ZLP

	    usb_configuration = wValue ;	// Remember configuration

	    p = VP( endpoint_config_table ) ;

	    for ( i = 1 ; i <= MAX_ENDPOINT ; ++i )
	    {
		UENUM = i ;			// Select EP

		n = pgm_read_byte( p++ ) ;

		UECONX = n ;

		if ( n )			// If enabled,
		{				// configure EP
		    UECFG0X = pgm_read_byte( p++ ) ;
		    UECFG1X = pgm_read_byte( p++ ) ;
		}
	    }

	    // Reset EP FIFOs

	    UERST =
		#if defined(__AVR_ATmegaXU4__) || defined(__AVR_AT90USBX6__)
		    _B1(EPRST6) | _B1(EPRST5) |
		#endif
		    _B1(EPRST4) | _B1(EPRST3) | _B1(EPRST2) | _B1(EPRST1) ;

	    UERST = 0 ;

	    return ;
	}

	if ( bRequest == GET_CONFIGURATION && bmRequestType == 0x80 )
	{
	    UEDATX = usb_configuration ;

	    usb_send_in() ;

	    return ;
	}

	if ( bRequest == GET_STATUS )
	{
	  #if SUPPORT_ENDPOINT_HALT
	    i = 0 ;

	    if ( bmRequestType == 0x82 )
	    {
		UENUM = wIndex ;

		if ( bit_is_set( UECONX, STALLRQ ) )
		    i = 1 ;

		UENUM = 0;
	    }

	    UEDATX = i ;
	  #else
	    UEDATX = 0 ;
	  #endif

	    UEDATX = 0 ;

	    usb_send_in() ;

	    return ;
	}

      #if SUPPORT_ENDPOINT_HALT

	if (   (bRequest == CLEAR_FEATURE || bRequest == SET_FEATURE)
	     && bmRequestType == 0x02
	     && wValue == 0 )
	{
	    n = wIndex & 0x7F ;

	    if ( n >= 1 && n <= MAX_ENDPOINT )
	    {
		usb_send_in() ;			// Ack via ZLP

		UENUM = n ;

		if ( bRequest == SET_FEATURE )
		    UECONX = _B1(STALLRQ) | _B1(EPEN) ;
		else
		{
		    UECONX = _B1(STALLRQC) | _B1(RSTDT) | _B1(EPEN) ;
		    UERST  = _B1(n) ;
		    UERST  = 0 ;
		}

		return ;
	    }
	}
      #endif

	if ( bRequest == HID_GET_REPORT && bmRequestType == 0xA1 )
	{
	    // wValue contains report type (h) and id (l), see HID 1,11, 7.2.1
	    // type shoud be "input" (3), id should be 0 or 1/2
	    // wLength should match sizeof( hid_report )

	    usb_send_EP0( VP( sw_report ), sw_reportsz, IN_RAM ) ;

	    return ;
	}

	if ( bRequest == HID_GET_IDLE && bmRequestType == 0xA1 )
	{
	    // LSB(wValue): report ID
	    // wIndex: interface

	    usb_send_EP0( VP( &idle_rate ), sizeof( idle_rate ), IN_RAM ) ;

	    return ;
	}

	if ( bRequest == HID_SET_IDLE && bmRequestType == 0x21 )
	{
	    usb_send_in() ;			// Ack via ZLP

	    // MSB(wValue): duration in 4ms clicks, 0 for indefinite
	    // LSB(wValue): report ID
	    // wIndex: interface

	    i = MSB( wValue ) ;

	    if ( ! i )				// Indefinite
		idle_cnt = 0 ;
	    else
	    {
		if ( ! idle_rate || (idle_rate - idle_cnt) >= i )
		    idle_cnt = 1 ;		// Was indefinite or new idle expired
		else
		    idle_cnt = i - idle_cnt ;	// Correct counter to new idle time
	    }

	    idle_rate = i ;

	    return ;
	}
    }

    UECONX = _BV( STALLRQ ) | _BV( EPEN ) ;	// stall
}

//------------------------------------------------------------------------------
