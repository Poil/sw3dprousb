/*******************************************************************************
 * File Name	: main.c
 * Project	: 3DP-Vert, Microsoft Sidewinder 3D Pro/PP/FFP to USB converter
 * Date		: 2006/12/15, 2008/02/12, 2009/06/26
 * Version	: 4.0
 * Target MCU	: AT90USB162/AT90USB82, ATMEGA16U4/ATMEGA32U4
 * Tool Chain	: Atmel AVR Studio 4.17 666 / WinAVR 20090313
 * Author	: Detlef "Grendel" Mueller
 *		  detlef@gmail.com
 * References	:
 * Description	: USB to 3D Pro converter main program
 * Device	: Microsoft SideWinder 3D Pro, Precision Pro, Force Feedback Pro
 *
 * Release Notes:
 *
 * $Id: main.c 1.7 2010/04/23 05:32:26 Detlef Exp Detlef $
 ******************************************************************************/

#include "3DPro.h"
#include "usb_hid.h"

//------------------------------------------------------------------------------
// Delay until reading stick after last report was sent

 #define READ_DEL	MS2TM( 3, 1024 )
 #define IDLE_DEL	MS2TM( 4  , 64 )

//------------------------------------------------------------------------------

uint8_t
    sw_reportsz ;				// Size of SW report in bytes

uint8_t
    idle_rate,					// idle rate in 4ms clicks, 0 for indefinite
    idle_cnt,					// idle timer (count down)
    sw_repsav[SW_REPSZ_3DP] ;			// USB report data saved

//------------------------------------------------------------------------------
// Watchdog is not turned off by a reset, see avr-libc's wdt.h documentation

// uint8_t VA_NOINIT( mcusr_cpy ) ;

void FA_INIT3( Init3 ) ( void )
{
//  mcusr_cpy = MCUSR ;				// If we need to examine reset reason
    MCUSR = 0 ;
    wdt_disable() ;
}

//------------------------------------------------------------------------------
// Check if current report differs from the last saved one and save it.

static uint8_t sw_repchg ( void )
{
    uint8_t
	r = FALSE,
	i = sw_reportsz ;

    for ( ; i-- ; )
	if ( sw_repsav[i] != sw_report[i] )
	{
	    sw_repsav[i] = sw_report[i] ;
	    r = TRUE ;
	}

    return ( r ) ;
}

//------------------------------------------------------------------------------

int FA_NORETURN( main ) ( void )
{
    uint16_t
	timer ;
    uint8_t
	susp,
	sw_sendrep ;

    // Initialize..

    init_hw() ;					// hardware.

    sw_reportsz =				// report size
	(sw_id == SW_ID_3DP ? SW_REPSZ_3DP : SW_REPSZ_FFP) ;

    usb_init() ;				// USB stack

    sei() ;					// Enable interrupts

    SetTMPS( 1, 64 ) ;				// Set T1 prescaler to /64
    SetTMPS( 0, 1024 ) ;			// Set T0 prescaler to / 1024 for delay

    susp = TRUE ;
    sw_sendrep = sw_repchg() ;			// Init send report flag, saved report
    timer = 0 ;

    for ( ;; )					// Forever..
    {
	wdt_reset() ;				// Calm watchdog

	if ( usb_configured() )
	{
	    if ( susp )				// Coming out of suspend
	    {
		susp = FALSE ;

		// Turn things back on

		// Set clock divider to 1, full speed
		clock_prescale_set( clock_div_1 ) ;

		clr_bits( PRR0, _BV(PRTIM0) | _BV(PRTIM1) ) ;

		idle_cnt = idle_rate ;		// reset idle counter

		ResetTM( 0, READ_DEL ) ;
		ResetTM( 1, IDLE_DEL ) ;

		wdt_enable( WDTO_500MS ) ;	// Unleash watchdog
	    }

	    if ( TMexp( 0 ) )			// Time to read the stick
	    {
		SetTMPS( 0, 64 ) ;		// Set T0 prescaler to / 64 for query
		getdata() ;			// read 3DP data

		SetTMPS( 0, 1024 ) ;		// Set T0 prescaler to / 1024 for delay
		ResetTM( 0, READ_DEL ) ;

		if ( sw_repchg() )		// Report changed,
		    sw_sendrep = TRUE ;		// need to send it
	    }

	    if ( TMexp( 1 ) )			// Idle timed out
	    {
		ResetTM( 1, IDLE_DEL ) ;

		if ( ! --idle_cnt && idle_rate )// Idle counter expired, rate not indef.
		    sw_sendrep = TRUE ;		// need to send current report
	    }

	    if ( sw_sendrep && ! usb_IN_busy() )
	    {
		LED_on() ;
						// Send report
		usb_send_IN( sw_report, sw_reportsz ) ;

		sw_sendrep = FALSE ;		// reset send report flag
		idle_cnt   = idle_rate ;	// reset idle counter
		ResetTM( 1, IDLE_DEL ) ;	// reset idle timer

		LED_off() ;
	    }
	}
	else
	if ( usb_suspend )
	{
	    if ( ! susp )			// Going into suspend
	    {
		susp = TRUE ;

		// Shut down things we don't need

		wdt_disable() ;

		set_bits( PRR0, _BV(PRTIM0) | _BV(PRTIM1) ) ;

		// Set clock divider to 2, half speed

		clock_prescale_set( clock_div_2 ) ;

		timer    = 0xFFFF ;
		idle_cnt = 10 ;
	    }

	    if ( ! --timer )
	    {
		timer = 0xFFFF ;

		if ( ! --idle_cnt )
		{
		    if ( LED_sts() )
		    {
			timer    = 0xFFFF ;
			idle_cnt = 10 ;
			LED_off() ;
		    }
		    else
		    {
			timer    = 30 ;
			idle_cnt = 1 ;
			LED_on() ;
		    }
		}
	    }
	}
	else
	{
	    ResetTM( 1, IDLE_DEL ) ;		// reset idle timer
	    ResetTM( 0, READ_DEL ) ;		// reset SW timer
	}
    }
}

//------------------------------------------------------------------------------
