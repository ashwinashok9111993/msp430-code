/*------------------------------------------------------------------*/
// MSP430G2231 simple 4-bit clock with __delay_cycles()
//             |=======|
// vcc-->P2.0--|       |
// vcc-->P2.1--|       |
// vcc-->P2.2--|       |
// vcc-->P2.3--|       |
// vcc-->P2.4--|       |
// vcc-->P2.5--|       |
// vcc-->P2.6--|       |
// vcc-->P2.6--|       |
// external leds connected to P2
/*------------------------------------------------------------------*/

#include <msp430f149.h>

volatile unsigned int i,j;

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
	
	P2DIR = 0xff;
	while(1)
	{
		for(j=0;j<256;j++)
		{
			P2OUT=0xff;            // initialization
                        P2OUT &=~ j ;          //assign pins with binary values
		    __delay_cycles(100000);
		   
		  }
		
		
	    }
	}