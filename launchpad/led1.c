/*------------------------------------------------------------------*/
// MSP430G2231 helloworld program
//        |=======|
// P1.0<--|       |
//        |       |
//        |       |
//        |=======|
//
// red led connected to P1.0
// written by ASHOK ASHWIN
/*------------------------------------------------------------------*/

#include <msp430g2231.h> //include file

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD; // stop the watchdog timer from chip reset
	
	P1DIR = 0b00000001;       // set direction of P1.0 to output
	while(1)                  // endless loop
	{
		P1OUT = 0b00000001;     //set P1.0 to high
	}
}
