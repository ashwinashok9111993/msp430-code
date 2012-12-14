/*------------------------------------------------------------------*/
// MSP430G2231 simple ping pong
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
// most of the code is self explanatory
/*------------------------------------------------------------------*/

#include <msp430g2231.h>

volatile unsigned int i,j;

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
	
	P1DIR = 0b00001111;
	while(1)
	{
		for(j=0;j<8;j++)
		{
			P1OUT = 0b00000000;
		 if(j<4)
         	{
				P1OUT |= ( 0b000000001 << j); 
			   
		   }
                 else
	           {
				P1OUT |= ( 0b000001000 >> (j-4));

		   }
            
           			   __delay_cycles(1000000); 		   
		  }
		
		
	    }
	}