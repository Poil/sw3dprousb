/******************************************************************************
 * File Name	: 3DPro.c
 * Project	: 3DP-Vert, Microsoft Sidewinder 3D Pro/PP/FFP to USB converter
 * Date		: 2005/05/31, 2006/12/14, 2008/02/12, 2009/06/26
 * Version	: 4.0
 * Target MCU	: AT90USB162/82, AT90USB646/1286, ATMEGA16U4/32U4
 * Tool Chain	: Atmel AVR Studio 4.18 716 / WinAVR 20100110
 * Author	: Detlef "Grendel" Mueller
 *		  detlef@gmail.com
 * References	: Sidewinder.c by Vojtech Pavlik
 *		  Patent #5628686, Apperatus and Method for Bidirectional
 *			Data Communication in a Gameport, by Microsoft
 * Description	: 3D Pro driver and hardware interface
 * Device	: Microsoft SideWinder 3D Pro, Precision Pro, Force Feedback Pro
 *
 * Release Notes:
 *
 *	Pinout AT90USBX2
 *	================
 *
 *	PB0	--	PC0	(XTAL2)	 PD0	Button1
 *	PB1	(SCLK)	PC1	(/RESET) PD1	Button2
 *	PB2	(MOSI)	PC2	--	 PD2	Button3
 *	PB3	(MISO)			 PD3	Button4
 *	PB4	X1 axis	PC4	--	 PD4	--
 *	PB5	Y2 axis	PC5	--	 PD5	--
 *	PB6	--	PC6	--	 PD6	LED
 *	PB7	--	PC7	--	 PD7	(HWB)
 *
 *	Pinout ATMEGAXU4
 *	================
 *
 *	PB0	--			PD0	Button1			PF0	--
 *	PB1	--			PD1	Button2			PF1	--
 *	PB2	--			PD2	Button3	PE2	--
 *	PB3	--			PD3	Button4
 *	PB4	X1 axis			PD4	--			PF4	POV X
 *	PB5	Y2 axis			PD5	--			PF5	POV Y
 *	PB6	--	PC6	--	PD6	LED	PE6	--	PF6	--
 *	PB7	--	PC7	--	PD7	--			PF7	--
 *
 *	Pinout AT90USBX6
 *	================
 *
 *	PA0	--	PB0	--	PC0	--	PD0	Button1	PE0	--	PF0	--
 *	PA1	--	PB1	--	PC1	--	PD1	Button2	PE1	--	PF1	--
 *	PA2	--	PB2	--	PC2	--	PD2	Button3	PE2	--	PF2	--
 *	PA3	--	PB3	--	PC3	--	PD3	Button4	PE3	--	PF3	--
 *	PA4	--	PB4	X1 axis	PC4	--	PD4	--	PE4	--	PF4	--
 *	PA5	--	PB5	Y2 axis	PC5	--	PD5	--	PE5	--	PF5	--
 *	PA6	--	PB6	--	PC6	--	PD6	LED	PE6	--	PF6	--
 *	PA7	--	PB7	--	PC7	--	PD7	--	PE7	--	PF7	--
 *
 * $Id: 3DPro.c 1.6 2010/04/23 05:27:52 Detlef Exp Detlef $
 ******************************************************************************/

#include "3DPro.h"

//------------------------------------------------------------------------------
//******************************************************************************
//------------------------------------------------------------------------------

#define	DDO( b )	_B1( b )	/* data direction: out */
#define	DDI( b )	_B0( b )	/* data direction: in  */
#define	PU1( b )	_B1( b )	/* pull up on (if DDI) */
#define	PU0( b )	_B0( b )	/* pull up off */

// Port B

#if defined(FFP_HW)
#define PBPU		(PU1( PB7) | PU1( PB6) | PU0( PB5) | PU0( PB4) | \
			 PU1( PB3) | PU1( PB2) | PU1( PB1) | PU0( PB0))
#else
#define PBPU		(PU1( PB7) | PU1( PB6) | PU0( PB5) | PU0( PB4) | \
			 PU1( PB3) | PU1( PB2) | PU1( PB1) | PU1( PB0))
#endif
#define DDB		(DDI(DDB7) | DDI(DDB6) | DDI(DDB5) | DDI(DDB4) | \
			 DDI(DDB3) | DDI(DDB2) | DDI(DDB1) | DDI(DDB0))
// Port D

#if defined(__AVR_AT90USBX2__)

#define PDPU		(PU0( PD7) | _B0( PD6) | PU1( PD5) | PU1( PD4) | \
			 PU1( PD3) | PU1( PD2) | PU1( PD1) | PU1( PD0))

#elif defined(__AVR_AT90USB646__)

#define PDPU		(PU1( PD7) | _B0( PD6) | PU1( PD5) | PU1( PD4) | \
			 PU1( PD3) | PU1( PD2) | PU1( PD1) | PU1( PD0))

#elif defined(__AVR_ATmegaXU4__) || defined(__AVR_AT90USB1286__)

#define PDPU		(PU1( PD7) | _B1( PD6) | PU1( PD5) | PU1( PD4) | \
			 PU1( PD3) | PU1( PD2) | PU1( PD1) | PU1( PD0))
#endif

#define DDD		(DDI(DDD7) | DDO(DDD6) | DDI(DDD5) | DDI(DDD4) | \
			 DDI(DDD3) | DDI(DDD2) | DDI(DDD1) | DDI(DDD0))
// Ports A,C,E,F

#if defined(__AVR_AT90USBX2__)

#define PCPU		(PU1( PC7) | PU1( PC6) | PU1( PC5) | PU1( PC4) | \
			             PU1( PC2) | PU1( PC1) | PU1( PC0))
#define DDC		(DDI(DDC7) | DDI(DDC6) | DDI(DDC5) | DDI(DDC4) | \
			             DDI(DDC2) | DDI(DDC1) | DDI(DDC0))

#elif defined(__AVR_ATmegaXU4__)

#define PCPU		(PU1( PC7) | PU1( PC6))
#define DDC		(DDI(DDC7) | DDI(DDC6))

#define PEPU		(PU1( PE6) | PU0( PE2))
#define DDE		(DDI(DDE6) | DDI(DDE2))

#define PFPU		(PU1( PF7) | PU1( PF6) | PU1( PF5) | PU1( PF4) | \
						 PU1( PF1) | PU1( PF0))
#define DDF		(DDI(DDF7) | DDI(DDF6) | DDI(DDF5) | DDI(DDF4) | \
						 DDI(DDF1) | DDI(DDF0))

#elif defined(__AVR_AT90USBX6__)

#define PAPU		(PU1( PA7) | PU1( PA6) | PU1( PA5) | PU1( PA4) | \
			 PU1( PA3) | PU1( PA2) | PU1( PA1) | PU1( PA0))
#define DDA		(DDI(DDA7) | DDI(DDA6) | DDI(DDA5) | DDI(DDA4) | \
			 DDI(DDA3) | DDI(DDA2) | DDI(DDA1) | DDI(DDA0))
#define PCPU		(PU1( PC7) | PU1( PC6) | PU1( PC5) | PU1( PC4) | \
			 PU1( PC3) | PU1( PC2) | PU1( PC1) | PU1( PC0))
#define DDC		(DDI(DDC7) | DDI(DDC6) | DDI(DDC5) | DDI(DDC4) | \
			 DDI(DDC3) | DDI(DDC2) | DDI(DDC1) | DDI(DDC0))
#define PEPU		(PU1( PE7) | PU1( PE6) | PU1( PE5) | PU1( PE4) | \
			 PU1( PE3) | PU1( PE2) | PU1( PE1) | PU1( PE0))
#define DDE		(DDI(DDE7) | DDI(DDE6) | DDI(DDE5) | DDI(DDE4) | \
			 DDI(DDE3) | DDI(DDE2) | DDI(DDE1) | DDI(DDE0))
#define PFPU		(PU1( PF7) | PU1( PF6) | PU1( PF5) | PU1( PF4) | \
			 PU1( PF3) | PU1( PF2) | PU1( PF1) | PU1( PF0))
#define DDF		(DDI(DDF7) | DDI(DDF6) | DDI(DDF5) | DDI(DDF4) | \
			 DDI(DDF3) | DDI(DDF2) | DDI(DDF1) | DDI(DDF0))
#endif

#define	dis3DP_INT()	clr_bit( EIMSK, INT0 )

#define TRG_pull()	__WRAP__( {				\
				clr_bit( TRGDDR, TRGX1BIT ) ;	\
				clr_bit( TRGDDR, TRGY2BIT ) ;	\
			} )
#define TRG_rel()	__WRAP__( {				\
				set_bit( TRGDDR, TRGX1BIT ) ;	\
				set_bit( TRGDDR, TRGY2BIT ) ;	\
			} )

//------------------------------------------------------------------------------
//******************************************************************************
//------------------------------------------------------------------------------

uint8_t
    sw_id,					// ID of detected stick
    sw_report[SW_REPSZ_3DP] ;			// USB report data

static uint8_t
    sw_buttons,					// button buffer
    sw_problem ;				// problem counter

#if SIXAXIS

static uint16_t
    AcntrX,					// Analog center X
    AcntrY ;					// Analog center Y

#endif

//------------------------------------------------------------------------------
//******************************************************************************
//------------------------------------------------------------------------------

// Delay using T1 as timing reference, assuming prescaler /1024

static void FA_NOINLINE( Delay_1024 ) ( uint16_t time )
{
    set_bit( GTCCR, PSRSYNC ) ;			// reset prescaler

    ResetTM( 1, time ) ;			// set timer

    for ( ; ! TMexp( 1 ) ; )			// wait until timer expired
	;
}

//------------------------------------------------------------------------------

// Delay using T0 as timing reference, assuming prescaler /64

static void FA_NOINLINE( Delay_64 ) ( uint8_t time )
{
    set_bit( GTCCR, PSRSYNC ) ;			// reset prescaler

    ResetTM( 0, time ) ;			// set timer

    for ( ; ! TMexp( 0 ) ; )			// wait until timer expired
	;
}

//------------------------------------------------------------------------------

// Trigger the stick

static void FA_NOINLINE( Trigger ) ( void )
{
    TRG_pull() ;
    Delay_64( T6DEL48US ) ;			// _delay_us( 47.8125 ) is max @ 16MHz
    TRG_rel() ;
}

//------------------------------------------------------------------------------

// Check if the FFP/PP data packet at *pkt is valid by xoring all bits together.
// Result must be 1 for the packet to have correct parity.

static uint8_t FA_NOINLINE( CheckFFPPkt ) ( uint8_t *pkt )
{
    uint8_t
	x ;

    x  = pkt[0] ^ pkt[1] ^ pkt[2] ^ pkt[3] ^ pkt[4] ^ pkt[5] ;

    x ^= x >> 4 ;
    x ^= x >> 2 ;
    x ^= x >> 1 ;

    return ( x & 1 ) ;
}

//------------------------------------------------------------------------------

// Check if the 3DPro data packet at *pkt is valid by adding all nibbles
// (result must be 0) and checking the N bits of each byte.

static uint8_t FA_NOINLINE( Check3DPPkt ) ( uint8_t *pkt )
{
    uint8_t
	b,
	i   = 8,				// 8 bytes
	sum = 0 ;				// clear CHKSUM

    do						// for every byte
    {
	b = *pkt++ ;

//	To speed things up let's drop the N flag test. CHKSUM should be enough.
//
//	if (   (b & 0x80)			// N flag set ?
//	     && i != 1 )			// Not last byte ?
//	    return ( 0 ) ;			// bad packet

	sum += b ;				// add LSN

	__asm__ __volatile__
	(
	    "swap %0\n\t" : "+r" (b) : "0" (b)
	) ;

	sum += b ;				// add MSN
    }
    while ( --i ) ;

    return ( ! (sum & 0x0F) ) ;			// good if sum of all nibbs == 0
}

//------------------------------------------------------------------------------

// Hang out 12ms w/ the LED on for the initial 4ms

static void FA_NOINLINE( Flash_LED_12MS ) ( void )
{
    LED_on() ;
    Delay_1024( T0DEL4MS ) ;
    LED_off() ;
    Delay_1024( T0DEL8MS ) ;
}

//------------------------------------------------------------------------------

// Initialize FFP/PP by reading the ID. This has to be done, else the PP will
// only use B0/1 for data transfer. Actually only the 3D Pro Plus does that.
// Do it anyways for type determination.
// QueryFFP() needs to be called before this function for a valid sw_clkcnt.

static uint8_t InitFFPro ( void )
{
    uint8_t
        butt,
        idpktsz ;

    butt = BUTPIN ;				// Save current button data

    idpktsz = sw_clkcnt ;			// remember packetsize

    QueryFFP( -(sw_clkcnt - 6), 126 ) ;		// Read ID, signal kick at -6 clk of data packet

    Delay_1024( T0DEL8MS ) ;			// Give the stick some time..

    idpktsz = sw_clkcnt - idpktsz ;		// Sizeof ID packet

    if ( ! QueryFFP( 0, DATSZFFP ) )		// Signal for a regular packet, 16 triplets
	return ( FALSE ) ;			// Timed out..

    if ( CheckFFPPkt( ffp_packet ) )		// Packet parity checks out
    {
	if ( idpktsz == IDSZPP )
	    sw_id = SW_ID_PP ;			// Found PP
	else
	if ( idpktsz == IDSZFFP )
	    sw_id = SW_ID_FFP ;			// Found FFP
	else
	if ( idpktsz == 0 )			// 10nF timer, FFP or PP in 3bit mode
	{
	    if ( (butt & BUTMSK) == BUTMSK )	// PP
		sw_id = SW_ID_PP ;
	    else				// FFP
		sw_id = SW_ID_FFP ;
	}
	else
	    return ( FALSE ) ;			// Unknown stick..

	CopyFFPData( ffp_packet ) ;		// Copy data to report buffer

	return ( TRUE ) ;			// Got it
    }

    return ( FALSE ) ;				// Stick not found
}

//------------------------------------------------------------------------------

// Initialize 3D Pro by setting it into digital mode followed by reading the ID.
// This has to be done, else the 3DPro will only use B0/1 for data transfer.
// The ID packet contains: "(ppPNP0F1F\01.00.08)" w/ pp being the encoded PNP
// version no. (1.00)
// QueryFFP() needs to be called before this function for a valid sw_clkcnt.

static uint8_t Init3DPro ( void )
{
    if ( ! sw_clkcnt )				// Previous trigger came up empty
    {
	// Set to digital mode sequence

	dis3DP_INT() ;				// Don't want INT to run..

	Trigger() ;

	Delay_64( T6DEL140US ) ;		// Wait 140us

	Trigger() ;

	Delay_64( (uint8_t)T6DEL865US ) ;	// Wait 140+725us

	Trigger() ;

	Delay_64( T6DEL440US ) ;		// Wait 140+300us

	Trigger() ;

	Delay_1024( T0DEL4MS ) ;		// Hang out 4ms to let everything calm down

						// Try to read a data packet,
	Query3DP( 0, 126 ) ;			// don't know how long (64 or 66) - let it time out

	if ( ! sw_clkcnt || ! ~sw_clkcnt )	// nothing..
	    return ( FALSE ) ;

	Delay_1024( T0DEL4MS ) ;		// Another break for the stick
    }

    if ( sw_clkcnt == 64 )			// 3DP in 1-bit mode
    {						// Read ID, signal kick at -8 clk of data packet
						// ID packet is 160 long + 64 or 66
	Query3DP( -(sw_clkcnt - 8), sw_clkcnt + IDSZ3DP ) ;

	Delay_1024( T0DEL8MS ) ;		// Give the 3DP some rest..
    }
    else
    if ( sw_clkcnt != 66 )			// Not 3DP in 3-bit mode
	return ( FALSE ) ;

    if ( ! Query3DP( 0, DATSZ3DP ) )		// Signal for a regular packet, 22 triplets
	return ( FALSE ) ;			// Timed out..

    if ( Check3DPPkt( sw_packet1 ) )		// Packet 1 CRC checks out
    {
	Copy3DPData( sw_packet1 ) ;		// Copy data to report buffer

	sw_id = SW_ID_3DP ;			// Found 3DP

	return ( TRUE ) ;
    }

// It should work the 1st time, let's save some space.
//
//    Delay_1024( T0DEL500US ) ;		// Wait a bit to have 2nd packet ready
//
//    if ( Check3DPPkt( sw_packet2 ) )		// Packet 2 CRC checks out
//    {
//	Copy3DPData( sw_packet2 ) ;		// Copy data to report buffer
//
//	sw_id = SW_ID_3DP ;			// Found 3DP
//
//	return ( TRUE ) ;
//    }

    return ( FALSE ) ;				// Stick not found
}

//------------------------------------------------------------------------------

// Reboot converter. Kill USB and wait for the watchdog to catch us.
// Note: the watchdog is unleashed in main() after init_hw() returns.

static void FA_NORETURN( reboot ) ( void )
{
    cli() ;					// Disable interrupts

    UDCON  = _B1(DETACH) ;
    USBCON = _B0(USBE) | _B1(FRZCLK) ;		// Kill USB

    for ( ;; )					// Wait for watchdog to bite (.5s)
	;
}

//------------------------------------------------------------------------------

#if SIXAXIS

// Need the ADC ISR handler for wakeup from sleep.

EMPTY_INTERRUPT( ADC_vect ) 

//------------------------------------------------------------------------------

#define	ADC_RX		0b0100
#define	ADC_RY		0b0101

#define	ADC_CRX		0b1100
#define	ADC_CRY		0b1101

static void ReadADC ( uint8_t chan )
{
    uint8_t
	adc8 ;
    uint16_t
	adc16 ;

    if ( chan & 0x01 )
	set_bit( ADMUX, MUX0 ) ;		// select channel 5, Ry
    else
	clr_bit( ADMUX, MUX0 ) ;		// select channel 4, Rx

    sleep_enable() ;

    do
	sleep_cpu() ;				// ADCNR/IDLE sleep, starts conv.
    while ( bit_is_set( ADCSRA, ADSC ) ) ;

    sleep_disable() ;

    adc16 = ADC ;				// Read A value, left adj. (!)

    switch ( chan )
    {
	case ADC_RX :
	    if ( adc16 >= AcntrX )
		adc8 = -((adc16 - AcntrX) >> 8) ;
	    else
	    {
		adc8 = (AcntrX - adc16) >> 8 ;

		if ( adc8 > 0x7F )
		    adc8 = 0x7F ;
	    }

	    sw_report[2] = (sw_report[2] & 0x0F) | (adc8 << 4) ;
	    sw_report[3] = (sw_report[3] & 0xF0) | (adc8 >> 4) ;

	    break ;

	case ADC_RY :
	    if ( adc16 >= AcntrY )
		adc8 = -((adc16 - AcntrY) >> 8) ;
	    else
	    {
		adc8 = (AcntrY - adc16) >> 8 ;

		if ( adc8 > 0x7F )
		    adc8 = 0x7F ;
	    }

	    sw_report[3] = (sw_report[3] & 0x0F) | (adc8 << 4) ;
	    sw_report[4] = (sw_report[4] & 0xF0) | (adc8 >> 4) ;

	    break ;

	case ADC_CRX :			// center Rx
	    AcntrX = adc16 ;
	    break ;

	case ADC_CRY :			// center Ry
	    AcntrY = adc16 ;
	    break ;
    }
}

#endif

//------------------------------------------------------------------------------

// Initialize the hardware

void init_hw ( void )
{
    // Power down..

    ACSR  = _BV( ACD ) ;			// analog comp

  #if defined(__AVR_AT90USBX2__)

    PRR0  = _BV( PRSPI ) ;			// SPI
    PRR1  = _BV( PRUSART1 ) ;			// USART1

  #elif defined(__AVR_AT90USBX6__)

    PRR0 =					// TWI, Tim2, SPI, ADC
	_BV( PRTWI ) | _BV( PRTIM2 ) | _BV( PRSPI ) | _BV( ADC ) ;
    PRR1 = _BV( PRTIM3 ) | _BV( PRUSART1 ) ;	// Tim3, USART1

  #elif defined(__AVR_ATmegaXU4__)

    PRR0  =					// TWI, SPI, ADC
	_BV( PRTWI ) | _BV( PRSPI ) | _BV( ADC ) ;
    PRR1  =					// Tim4, Tim3, USART1
	_BV( PRTIM4 ) | _BV( PRTIM3 ) | _BV( PRUSART1 ) ;
    DIDR1 = _BV( AIN0D ) ;			// Disable digital input buffer
  #endif

    // Initialize ports

  #if defined(__AVR_AT90USBX6__)

    DDRA  = DDA ;
    PORTA = PAPU ;
  #endif

    DDRB  = DDB ;
    PORTB = PBPU ;
    DDRC  = DDC ;
    PORTC = PCPU ;
    DDRD  = DDD ;
    PORTD = PDPU ;

  #if defined(__AVR_ATmegaXU4__) || defined(__AVR_AT90USBX6__)

    DDRE  = DDE ;
    PORTE = PEPU ;
    DDRF  = DDF ;
    PORTF = PFPU ;
  #endif

    clock_prescale_set( clock_div_1 ) ;		// Set clock divider to 1, full speed

    // Initialize timers

    SetTMPS( 0, 64 ) ;				// Set T0 prescaler to / 64 for query & us delay
    SetTMPS( 1, 1024 ) ;			// Set T1 prescaler to / 1024 for ms delay

    EICRA  = _B1(ISC01) | _B1(ISC00) ;		// Need INT0 on rising edges

    Delay_1024( T0DEL200MS ) ;			// Allow the stick to boot

    LED_off() ;
						// Hold 3 & 4, then switch from * --> **
    if ( (BUTPIN & BUTMSK) == (_B1(BUT1) | _B1(BUT2) | _B0(BUT3) | _B0(BUT4)) )
	jmp_bootloader() ;

//    if ( ClrSerial() )			// Clear serial no. condition met ?
//	eeprom_write_byte( NULL, 0xFF ) ;	// Invalidate serial no.

    for ( ;; )					// Forever..
    {
	Flash_LED_12MS() ;			// Flash LED and wait
	Delay_1024( T0DEL200MS ) ;
						// Try to read a data packet,
	QueryFFP( 0, 126 ) ;			// don't know how long - let it time out

	Flash_LED_12MS() ;

	// Analyze clock count

	if ( ! ~sw_clkcnt )
	    sw_clkcnt = 0 ;

	if (    sw_clkcnt == (16 + 1)		// FFP/PP in 3-bit mode
	     || sw_clkcnt == (48 + 1) )		// 3DPP in 1-bit mode
	{
	    if ( InitFFPro() )			// found FFP/PP
		break ;				// break forever
	}
	else
	if ( Init3DPro() )			// Check for 3DP
	{					// found 3DP
	  #if SIXAXIS

	    clr_bit( PRR0, ADC ) ;		// power up ADC

	    set_sleep_mode( SLEEP_MODE_ADC ) ;	// Works w/ USB module ? Seems to.

	    Delay_1024( T0DEL4MS ) ;		// Wait for all activity to stop

	    clr_bits( PORTF, _B1(PF4) | _B1(PF5) ) ;// Turn off analog pin pull-up's

	    DIDR0 = _B1(ADC5D) | _B1(ADC4D) ;	// Disable digital input buffers

	    ADMUX  =
		_B0(REFS1) | _B1(REFS0) |       // AVcc w/ cap on AREF
		_B1(ADLAR) |                    // left adjusted result (!)
		4 ;                             // Preselect channel 4

	    ADCSRA =				// Initialize ADC for Rx/Ry
		_B1(ADEN) |			//   enable
		_B0(ADSC) |			// ! start conversion
		_B0(ADATE) |			// ! auto trigger enable
		_B1(ADIF) |			//   clear interrupt flag
		_B1(ADIE) |			//   enable interrupt
		_B0(ADPS2) | _B1(ADPS1) | _B1(ADPS0) ; // prescaler / 8 --> 125kHz

	    ReadADC( ADC_CRX ) ;		// Start up ADC, read AcntrX
	    ReadADC( ADC_CRY ) ;		// Read AcntrY

//	    if ( sw_packet1[3] & 0x20 )		// B9 in pos. II, could sig. calibrate
//	    {
//	    }
	  #endif

	    break ;				// break forever
	}

	dis3DP_INT() ;
    }

    dis3DP_INT() ;				// Disable INT

    cli() ;					// Disable interrupts

    sw_buttons = BUTMSK ;			// All buttons released
}

//------------------------------------------------------------------------------

// Patch current 3DP buttons into report

static void patchbutt ( void )
{
    uint8_t
	b = sw_buttons ;

  #if SIXAXIS

    __asm__ __volatile__
    (
	"com	%0		\n\t"
	"lsl	%0		\n\t"
	"swap	%0		\n\t"
	"andi	%0,0b11100001	\n\t"
	: "+r" (b) : "0" (b) : "cc"
    ) ;

    // Patch buttons into report data

    sw_report[5] = (sw_report[5] & 0x1F) | (b & 0xE0) ;
    sw_report[6] = (sw_report[6] & 0xFE) | (b & 0x01) ;
  #else

    __asm__ __volatile__
    (
	"com	%0		\n\t"
	"lsl	%0		\n\t"
	"andi	%0,0b00011110	\n\t"
	: "+r" (b) : "0" (b) : "cc"
    ) ;

    // Patch buttons into report data

    sw_report[4] = (sw_report[4] & 0xE1) | b ;
  #endif
}

//------------------------------------------------------------------------------

// Read the stick and create a report

void getdata ( void )
{
    uint8_t
	i ;

    if ( sw_id == SW_ID_3DP )
    {
      #if SIXAXIS
	ReadADC( ADC_RX ) ;			// Read Rx
	ReadADC( ADC_RY ) ;			// Read Ry
      #endif

	sw_buttons = BUTPIN ;			// Save current button data

	i = Query3DP( 0, DATSZ3DP ) ;		// Query 3DP
    }
    else
	i = QueryFFP( 0, DATSZFFP ) ;		// Query FFP

    dis3DP_INT() ;

    if ( ! i )					// If query timed out,
    {
	if ( ++sw_problem > 10 )		// 11th problem in a row
	    reboot() ;				// We lost the stick, lets start over

	if ( sw_id == SW_ID_3DP )
	    patchbutt() ;			// patch button data into last good info (3DP only)
    }
    else
    {
	sw_problem = 0 ;			// Reset problem counter

	if ( sw_id == SW_ID_3DP )
	{
	    if ( Check3DPPkt( sw_packet1 ) )	// If 3DP packet ok
	    {
	 	// LED_on() ;			// Signal good packet read

	 	Copy3DPData( sw_packet1 ) ;	// Copy data into report
	    }
	    else
	    {
		// wait 300us, try packet 2
		// not: a FUed CRCpkt1 is most likely caused by missed INT,
		// let's just go on w/ the show.

		patchbutt() ;			// patch button data into last good info
	    }
	}
	else
	if ( CheckFFPPkt( ffp_packet ) )	// If PP/FFP packet ok
	{
	    // LED_on() ;			// Signal good packet read

	    CopyFFPData( ffp_packet ) ;		// Copy data into report
	}
    }
}

//------------------------------------------------------------------------------
