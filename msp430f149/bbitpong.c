/*------------------------------------------------------------------*/
// MSP430G2231 simple 8 bit pong
//             |=======|
// vcc-->P2.0--|       |
// vcc-->P2.1--|       |
// vcc-->P2.2--|       |
// vcc-->P2.3--|       |
// vcc-->P2.4--|       |
// vcc-->P2.5--|       |
// vcc-->P2.6--|       |
// vcc-->P2.7--|       |
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
		for(j=0;j<16;j++)
		{
			P2OUT |= 0xff;
		 if(j<8)
         	{
				P2OUT &=~ ( 1 << j); 
			   
		   }
                 else
	           {
				P2OUT &=~ ( 256 >> (j-8));

		   }
            
           			   __delay_cycles(60000); 		   
		  }
		
		
	    }
	}
