/*******************************************************************************
 * File Name	: includes.h
 * Project	: Generic AVR based
 * Date		: 2013/11/12
 * Version      : 1.1
 * Target MCU   : AT90USB8/162, ATMEGA16/32U4, AT90USB64/1286
 * Tool Chain   : Atmel AVR Studio 4.19 730 / WinAVR 20100110
 * Author       : Detlef Mueller
 *                detlef@gmail.com
 * Release Notes:
 *
 * $Id$
 *
 * $Log$
 ******************************************************************************/

#ifndef	__includes_h__
#define	__includes_h__

//------------------------------------------------------------------------------

#if defined(__AVR_AT90USB162__) || defined(__AVR_AT90USB82__)
 #define __AVR_AT90USBX2__
#elif defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
 #define __AVR_ATmegaXU4__
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
 #define __AVR_AT90USBX6__
#else
 #error "Unsupported device"
#endif

//------------------------------------------------------------------------------

#ifdef	__ASSEMBLER__

//------------------------------------------------------------------------------

#define	__SFR_OFFSET	0
//#define	_VECTOR(N)	__vector_ ## N	/* io.h does not define this for asm */

#include <avr/io.h>

//------------------------------------------------------------------------------

#else  // ! __ASSEMBLER__

//------------------------------------------------------------------------------

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/power.h>

//-------------------------------------------------------------------------------
// Things not defined in iom32u4.h

#if defined(__AVR_ATmegaXU4__)

#define	PRTIM4		4	/* Power reduction register bit 4 for Timer 4 */

#define	PB7		PORTB7
#define	PB6		PORTB6
#define	PB5		PORTB5
#define	PB4		PORTB4
#define	PB3		PORTB3
#define	PB2		PORTB2
#define	PB1		PORTB1
#define	PB0		PORTB0

#define	PC7		PORTC7
#define	PC6		PORTC6

#define	PD7		PORTD7
#define	PD6		PORTD6
#define	PD5		PORTD5
#define	PD4		PORTD4
#define	PD3		PORTD3
#define	PD2		PORTD2
#define	PD1		PORTD1
#define	PD0		PORTD0

#define	PE6		PORTE6
#define	PE2		PORTE2

#define	PF7		PORTF7
#define	PF6		PORTF6
#define	PF5		PORTF5
#define	PF4		PORTF4
#define	PF1		PORTF1
#define	PF0		PORTF0

#endif

//------------------------------------------------------------------------------

#define set_bit( sfr, bit )	(_SFR_BYTE(sfr) |=  _BV(bit))
#define clr_bit( sfr, bit )	(_SFR_BYTE(sfr) &= ~_BV(bit))
#define tog_bit( sfr, bit )	(_SFR_BYTE(sfr) ^=  _BV(bit))

#define set_bits( sfr, msk )	(_SFR_BYTE(sfr) |=  (msk))
#define clr_bits( sfr, msk )	(_SFR_BYTE(sfr) &= ~(msk))
#define tog_bits( sfr, msk )	(_SFR_BYTE(sfr) ^=  (msk))

#define	bits_are_set( n, msk )	 (((n) & (msk)) == (msk))
#define	bits_are_clear( n, msk ) (!((n) & (msk)))

//------------------------------------------------------------------------------

#define	__FA__( _func, ... )	__attribute__((__VA_ARGS__)) _func

#define	FA_NAKED( _f )		__FA__( _f, __naked__ )
#define	FA_NORETURN( _f )	__FA__( _f, __noreturn__ )
#define	FA_NOINLINE( _f )	__FA__( _f, __noinline__ )
#define	FA_NOINRET( _f )	__FA__( _f, __noinline__,__noreturn__ )
#define	FA_INIT3( _f )		__FA__( _f, __used__,__naked__,__section__(".init3") )

#define	VA_PROGMEM( _v )	_v PROGMEM
#define	VA_NOINIT( _v )		_v __attribute__((__section__(".noinit")))

#define	TA_PROGMEM( _t )	PROGMEM _t

//------------------------------------------------------------------------------

#define	__WRAP__( _c )		do _c while (0)

//------------------------------------------------------------------------------

#define	CRITICAL_VAR()		uint8_t __sSREG
#define	ENTER_CRITICAL()	__WRAP__( { __sSREG = SREG ; cli() ; } )
#define	EXIT_CRITICAL()		(SREG = __sSREG)
#define	EXIT_CRITICAL_RET( n )	__WRAP__( { SREG = __sSREG ; return ( n ) ; } )

//------------------------------------------------------------------------------

#define	RET()			__asm__ __volatile__ ( "ret\n\t" :: )
#define	SLEEP()			__asm__ __volatile__ ( "sleep\n\t" :: )
#define RESET()			__asm__ __volatile__ ( "jmp 0\n\t" :: )
#define	NOP()			__asm__ __volatile__ ( "nop\n\t" :: )

//------------------------------------------------------------------------------

#define	VP( p )			(void *)(p)
#define ARRSZ( a )		(sizeof(a) / sizeof(*(a)))

#ifndef	FALSE
 #define FALSE			0
#endif

#ifndef	TRUE
 #define TRUE			(! FALSE)
#endif

#ifndef NULL
 #define NULL			0
#endif

//------------------------------------------------------------------------------

#if defined(__AVR_AT90USB162__)                // Teensy 1.0
#define	jmp_bootloader()		__asm__ __volatile__("jmp 0x3E00")
#elif defined(__AVR_ATmega32U4__)              // Teensy 2.0
#define	jmp_bootloader()		__asm__ __volatile__("jmp 0x7E00")
#elif defined(__AVR_AT90USB646__)              // Teensy++ 1.0
#define	jmp_bootloader()		__asm__ __volatile__("jmp 0xFC00")
#elif defined(__AVR_AT90USB1286__)             // Teensy++ 2.0
#define	jmp_bootloader()		__asm__ __volatile__("jmp 0x1FC00")
#endif 

#if FOR_REFERENCE

#if defined(__AVR_AT90USB162__)                // Teensy 1.0
#define	bootloader()		__WRAP__( {					\
	EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0;			\
	TIMSK0 = 0; TIMSK1 = 0; UCSR1B = 0;					\
	DDRB = 0; DDRC = 0; DDRD = 0;						\
	PORTB = 0; PORTC = 0; PORTD = 0;					\
	jmp_bootloader(); }
#elif defined(__AVR_ATmega32U4__)              // Teensy 2.0
#define	bootloader()		__WRAP__( {					\
	EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;		\
	TIMSK0 = 0; TIMSK1 = 0; TIMSK3 = 0; TIMSK4 = 0; UCSR1B = 0; TWCR = 0;	\
	DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0; TWCR = 0;		\
	PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;			\
	jmp_bootloader(); }
#elif defined(__AVR_AT90USB646__)              // Teensy++ 1.0
#define	bootloader()		__WRAP__( {					\
	EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;		\
	TIMSK0 = 0; TIMSK1 = 0; TIMSK2 = 0; TIMSK3 = 0; UCSR1B = 0; TWCR = 0;	\
	DDRA = 0; DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0;		\
	PORTA = 0; PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;	\
	jmp_bootloader(); }
#elif defined(__AVR_AT90USB1286__)             // Teensy++ 2.0
#define	bootloader()		__WRAP__( {					\
	EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;		\
	TIMSK0 = 0; TIMSK1 = 0; TIMSK2 = 0; TIMSK3 = 0; UCSR1B = 0; TWCR = 0;	\
	DDRA = 0; DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0;		\
	PORTA = 0; PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;	\
	jmp_bootloader(); }
#endif 

#endif

//------------------------------------------------------------------------------

#endif // __ASSEMBLER__

//------------------------------------------------------------------------------

#define	_B1( b )		_BV(b)
#define	_B0( b )		0

//------------------------------------------------------------------------------

#endif

//------------------------------------------------------------------------------
