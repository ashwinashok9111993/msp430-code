/*------------------------------------------------------------------*/
// MSP430G2231 simple 4-bit clock with __delay_cycles()
//        |=======|
// P1.0<--|       |
// P1.1<--|       |
// P1.2<--|       |
// P1.3<--|       |
//        |=======|
//
// external led connected to P1.0--please remove jumper at J5
// external led connected to P1.1
// external led connected to P1.2
// external led connected to P1.3
/*------------------------------------------------------------------*/

#include <msp430g2231.h>

volatile unsigned int i,j;

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
	
	P1DIR = 0b00001111;
	while(1)
	{
		for(j=0;j<16;j++)
		{
			P1OUT = 0b00000000;         // initialization
			P1OUT |= j ;                //assign pins with binary values
		    __delay_cycles(1000000);
		   
		  }
		
		
	    }
	}