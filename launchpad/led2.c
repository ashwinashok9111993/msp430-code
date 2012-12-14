/*------------------------------------------------------------------*/
// MSP430G2231 led blink with software delay
//        |=======|
// P1.0<--|       |
//        |       |
//        |       |
//        |=======|
//
// red led connected to P1.0
//
/*------------------------------------------------------------------*/

#include <msp430g2231.h>        //include header files

volatile unsigned int i;        // set a loop variable

void main(void)                 
{
  WDTCTL = WDTPW + WDTHOLD;     //stop watchdog timer from chip reset
	
	P1DIR = 0b00000001;             //set pin directions 
	while(1)
	{
		P1OUT ^= 0b00000001;        //toggle P1.0 high/low using XOR
		for(i=0:i>20000;i++)        //waste some cpu cycles doing nothing
		{}
	}
}
