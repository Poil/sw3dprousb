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
 *
 * $Log: main.c $
 * Revision 1.7  2010/04/23 05:32:26  Detlef
 * Some cleanup.
 *
 * Revision 1.6  2009/10/26 07:34:30  Detlef
 * Using prog_* or PROGMEM instead of VA_PROGMEM().
 *
 * Revision 1.5  2009/10/26 06:39:57  Detlef
 * Removed HID_IDLE switch, we always want it
 *
 * Revision 1.4  2009/10/26 06:32:46  Detlef
 * Added Mega32U4 support (Teensy 2.0)
 *
 * Revision 1.3  2009/09/29 03:38:10  Detlef
 * Some cleanup.
 *
 * Revision 1.2  2009/07/23 10:31:15  Detlef
 * Added use of SetTMPS macro.
 *
 * Revision 1.1  2009/07/10 07:04:38  Detlef
 * Initial revision
 *
 ******************************************************************************/

#include "3DPro.h"
#include "usb_hid.h"

//------------------------------------------------------------------------------
// Delay until reading stick after last report was sent

//#define READ_DEL	MS2TM( USB_CFG_INTR_POLL_INTERVAL - .9, 64 ) ;

 #define READ_DEL	MS2TM( 4.5, 1024 )
 #define IDLE_DEL	MS2TM( 4  , 64 )

//------------------------------------------------------------------------------

uint8_t
    sw_reportsz ;				// Size of SW report in bytes

uint8_t
    idle_rate,					// idle rate in 4ms clicks, 0 for indefinite
    idle_cnt,					// idle timer (count down)
    sw_repsav[7+COMPDEV+SIXAXIS] ;		// USB report data saved

#if COMPDEV

static uint8_t
    kb_report[4],				// Keyboard report
    kb_repsav[4] ;				// Last KB report sent

#endif // COMPDEV

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

#if COMPDEV

//------------------------------------------------------------------------------

#define MOD_CONTROL_LEFT	_B1(0)
#define MOD_SHIFT_LEFT		_B1(1)
#define MOD_ALT_LEFT		_B1(2)
#define MOD_GUI_LEFT		_B1(3)
#define MOD_CONTROL_RIGHT	_B1(4)
#define MOD_SHIFT_RIGHT		_B1(5)
#define MOD_ALT_RIGHT		_B1(6)
#define MOD_GUI_RIGHT		_B1(7)

#define	KEY_NONE		 0

#define	KEY_D			 7
#define	KEY_L			15
#define	KEY_R			21
#define	KEY_U			24

#define	KEY_END			77
#define	KEY_PGDN		78

#define	KEY_PADm		86
#define	KEY_PADp		87
#define	KEY_PAD1		89
#define	KEY_PAD3		91

#define	_KEY_UP			KEY_PADm
#define	_KEY_DWN		KEY_PADp
#define	_KEY_RGT		KEY_PAD3
#define	_KEY_LFT		KEY_PAD1

static prog_uint8_t
    keys[][2] =
    {
	{ _KEY_UP , KEY_NONE },		// N
	{ _KEY_UP , _KEY_RGT },		// NE
	{ _KEY_RGT, KEY_NONE },		// E
	{ _KEY_RGT, _KEY_DWN },		// SE
	{ _KEY_DWN, KEY_NONE },		// S
	{ _KEY_DWN, _KEY_LFT },		// SW
	{ _KEY_LFT, KEY_NONE }, 	// W
	{ _KEY_LFT, _KEY_UP  },		// NW
	{ KEY_NONE, KEY_NONE }		// idle
    } ;

static uint8_t kb_repchg ( void )
{
    uint8_t
	pov ;

    // Extract POV info from sw_report

    if ( sw_id == SW_ID_3DP )
    {
	// 3DP report structure
	// -------0 -------1 -------2 -------3 -------4 -------5 -------6 +1
	// XXXXXXXX YYYYYYXX RRRRYYYY HHHRRRRR BBBBBBBH TTTTTTTB 00000TTT
	// 76543210 54321098 32109876 21087654 76543213 65432108      987

	pov = ((sw_report[4] >> 5) | (sw_report[5] << 3)) & 0x0F ;
    }
    else
    {
	// FFP report structure
	// -------0 -------1 -------2 -------3 -------4 -------5 +1
	// XXXXXXXX YYYYYYXX HHHHYYYY BBRRRRRR TBBBBBBB 00TTTTTT
	// 76543210 54321098 32109876 21543210 09876543   654321

	pov = sw_report[3] >> 4 ;
    }

    // Fill in report

    if ( pov > 8 )				// Limit pov value to 8
	pov = 8 ;

    *(uint16_t *)(kb_report + 2) = pgm_read_word( keys[pov] ) ;

    if ( *(uint16_t *)(kb_report + 2) != *(uint16_t *)(kb_repsav + 2) )
    {
	*(uint16_t *)(kb_repsav + 2) = *(uint16_t *)(kb_report + 2) ;

	return ( TRUE ) ;
    }

    return ( FALSE ) ;
}

//------------------------------------------------------------------------------

int FA_NORETURN( main ) ( void )
{
    uint8_t
	kb_sendrep ;				// Flag: need to send kb report
						// Initialize..
    init_hw() ;					// hardware. Note: defined as naked !
    usb_init() ;				// USB stack

    sw_reportsz = (sw_id == SW_ID_3DP ? SW_REPSZ_3DP : SW_REPSZ_FFP) ;

    sw_report[0] = 1 ;				// Preset report IDs
    kb_report[0] =
    kb_repsav[0] = 2 ;

    kb_sendrep   = FALSE ;

    idle_rate =
    idle_cnt  = 500 / 4 ;			// 500ms initial idle rate

    wdt_enable( WDTO_500MS ) ;			// Unleash watchdog

    sei() ;					// Enable interrupts

    TCCR0B = T0PS_1024 ;			// Set T0 prescaler to / 1024 for delay

    for ( ;; )					// Forever..
    {
	wdt_reset() ;				// Calm watchdog

	if ( usb_configuration )
	{
	    if ( usb_IN_busy() )		// Report not sent yet
	    {
//		TCNT0 = READ_DEL ;		// reset timer
//		TIFR0 = _B1( TOV0 ) ;		// clear overflow flag
	    }
	    else
	    if ( TMexp( 0 ) )			// Timed out
	    {
		TCCR0B = T0PS_64 ;		// Set T0 prescaler to / 64 for query
		getdata() ;			// Read 3DP data

		TCCR0B = T0PS_1024 ;		// reset T0 prescaler to / 1024 for delay

		usb_send_IN( sw_report, sw_reportsz ) ;

		if ( kb_repchg() )		// Create KB report from SW report
		    kb_sendrep = TRUE ;		// changed, need to send the report

		LED_off() ;			// may have been turned on in getdata()

		ResetTM( 0, READ_DEL ) ;	// reset timer
	    }
	    else
	    if ( kb_sendrep )			// Need to send kb report
	    {
		usb_send_IN( kb_report, sizeof( kb_report ) ) ;

		kb_sendrep = FALSE ;		// Clear send kb report flag
		idle_cnt = idle_rate ;		// reset idle counter
		ResetTM( 1, IDLE_DEL ) ;	// reset idle timer

		ResetTM( 0, READ_DEL ) ;	// reset timer
	    }

	    if ( TMexp( 1 ) )			// 4ms timed out
	    {
		ResetTM( 1, IDLE_DEL ) ;	// reset idle timer

		if ( ! --idle_cnt && idle_rate )// Idle counter expired, rate not indef.
		    kb_sendrep = TRUE ;		// need to send a report
	    }
	}
	else
	{
	    ResetTM( 1, IDLE_DEL ) ;		// reset idle timer
	    ResetTM( 0, READ_DEL ) ;		// reset SW timer
	}
    } // for
}

//------------------------------------------------------------------------------

#else // ! COMPDEV

//------------------------------------------------------------------------------

int FA_NORETURN( main ) ( void )
{
    uint8_t
	sw_sendrep ;

    // Initialize..

    init_hw() ;					// hardware. Note: defined as naked !

    sw_reportsz =				// report size
	(sw_id == SW_ID_3DP ? SW_REPSZ_3DP : SW_REPSZ_FFP) ;

    usb_init() ;				// USB stack

    sei() ;					// Enable interrupts

    wdt_enable( WDTO_500MS ) ;			// Unleash watchdog

    SetTMPS( 1, 64 ) ;				// Set T1 prescaler to /64
    SetTMPS( 0, 1024 ) ;			// Set T0 prescaler to / 1024 for delay

    sw_sendrep = sw_repchg() ;			// Init send report flag, saved report

    for ( ;; )					// Forever..
    {
	wdt_reset() ;				// Calm watchdog

	if ( usb_configuration )
	{
//	    SLEEP() ;				// Wait..

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
	{
	    ResetTM( 1, IDLE_DEL ) ;		// reset idle timer
	    ResetTM( 0, READ_DEL ) ;		// reset SW timer
	}
    }
}

//------------------------------------------------------------------------------

#endif // COMPDEV

//------------------------------------------------------------------------------
