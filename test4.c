/*--------------------------------------------------*/
/* TEST4 - A test program for using xitoa module    */
/*--------------------------------------------------*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "xitoa.h"

#define	SYSCLK		7372800
#define	BAUD		9600


/*------------------------------------------------*/
/* Device output function to be used via xitoa    */

void send (char c)
{
	loop_until_bit_is_set(USR, UDRE);
	UDR = c;
}


/* An example to control an LCD
void lcd_put (char c)
{
	switch (c) {
		case 12 :
			// clear all and return to home
			break;
		case '\r' :
			// return cursor to home
			break;
		default :
			// put a character with screen managment
	}
}
*/

/* An example to control 7-segment LEDs

volatile uint8_t DispBuff[LEDS];		// Display buffer being scanned by refresh process

void led_put (char c)
{
	static const prog_uint8_t segpat[] = { // pgfedcba
		0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,		// "01234"
		0b01101101, 0b01111101, 0b00100111, 0b01111111, 0b01101111,		// "56789"
		0b00000000, 0b01000000, 0b01111001, 0b01010000, 0b01110011 };	// " -ErP"
	static uint8_t pos;	// position


	if((c >= '0') && (c <= '9')) {
		c -= '0';
	}
	else {
		switch (c) {
			case ' ' :
				c = 10; break;
			case '-' :
				c = 11; break;
			case 'E' :
				c = 12; break;
			case 'r' :
				c = 13; break;
			case 'P' :
				c = 14; break;
			case '\r' :
				pos = 0;
			default :
				return;
		}
	}

	if(pos < LEDS)
		DispBuff[pos++] = pgm_read_byte(&segpat[(uint16_t)c]);
}
*/


/*------------------------------------------------*/
/* Main Process                                   */


int main (void)
{
	static const prog_char str_P[] = "ROM string";
	static const char str[] = "RAM string";


	/* Initialize UART */
	UBRR = SYSCLK / BAUD / 16 - 1;
	UCR = _BV(TXEN);

	/* Join my output function and xitoa module */
	xdev_out(send);


	xputs(PSTR("\nTest for xitoa()\n"));
	xputs(PSTR("\nxitoa(1234, 10, 0); ==>"));
	xitoa(1234, 10, 0);
	xputs(PSTR("\nxitoa(1234, 10, 6); ==>"));
	xitoa(1234, 10, 6);
	xputs(PSTR("\nxitoa(1234, 10, -6); ==>"));
	xitoa(1234, 10, -6);
	xputs(PSTR("\nxitoa(-100, 10, 0); ==>"));
	xitoa(-100, 10, 0);
	xputs(PSTR("\nxitoa(-100, -10, 0); ==>"));
	xitoa(-100, -10, 0);
	xputs(PSTR("\nxitoa(0x400, 16, 4); ==>"));
	xitoa(0x400, 16, 4);
	xputs(PSTR("\nxitoa(0x400, 16, -4); ==>"));
	xitoa(0x400, 16, -4);
	xputs(PSTR("\nxitoa(0x55, 2, -8); ==>"));
	xitoa(0x55, 2, -8);

	xputs(PSTR("\n\nTest for xprintf()\n"));
	xprintf(PSTR("%%c, 0x41 -->%c\n"), 0x41);
	xprintf(PSTR("%%u, 0xFFFF -->%u\n%%d, 0xFFFF -->%d\n%%u, 0x7FFF -->%u\n%%d, 0x7FFF -->%d\n"), 0xFFFF, 0xFFFF, 0x7FFF, 0x7FFF);
	xprintf(PSTR("%%8d, 100 -->%8d\n"), 100);
	xprintf(PSTR("%%08b, 0x55 -->%08b\n%%04X, 1024 -->%04X\n"), 0x55, 1024);
	xprintf(PSTR("%%s -->%s\n"), str);
	xprintf(PSTR("%%S -->%S\n"), str_P);
	xprintf(PSTR("%%ld, 0xFFFFFFFF -->%ld\n%%lu, 0xFFFFFFFF -->%lu\n"),
					0xFFFFFFFFUL, 0xFFFFFFFFUL);
	xprintf(PSTR("\n%%08lX, 0x55ABCD -->%08lX\n%%10ld, -1000L -->%10ld\n%%10lu, 1000UL -->%10lu\n"),
					0x55ABCDUL, -1000L, 1000UL);

	xputs(PSTR("\ncompleted.\n"));
	for(;;);
}

