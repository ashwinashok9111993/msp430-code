/*------------------------------------------------------------------*/
// MSP430F149 led blink program
//            |=======|
// vcc--->P2.0|       |
//            |       |
//            |       |
//            |=======|
//
// red led connected to P2.0
// written by ASHOK ASHWIN
/*------------------------------------------------------------------*/


#include <msp430f149.h>

int main( void )
{

// Stop watchdog timer to prevent time out reset
WDTCTL = WDTPW + WDTHOLD;

P2DIR |= (BIT0) ;
while(1)
{

P2OUT^=(BIT0); // P2.0 Low
__delay_cycles(80000);
}
return 0;
}
