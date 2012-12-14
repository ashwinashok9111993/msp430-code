/*------------------------------------------------------------------*/
// MSP430G2231 double led blink with software delay
//        |=======|
// P1.0<--|       |
//        |       |
//        |       |-->P1.6
//        |=======|
//
// red led connected to P1.0
// green led connected to P1.0
/*------------------------------------------------------------------*/

#include <msp430g2231.h>

int main(void) {
    /* Hold the watchdog timer so it doesn't reset our chip */
    WDTCTL = WDTPW + WDTHOLD;

    /* Configure pins 0,6 on port 1 as output pins */
    P1DIR =  0b00000001|0b01000000 ;

    P1OUT = 0b00000001;
    /* some initial setup -try 0b00000000 and see the difference*/

    /* infinite loop */
    for( ; ; ) {
        /* The following two lines implement a very crude delay loop.
         */
        volatile int i;
        for( i = 0; i < 20000; i++ );

        /* Toggle the state of pins 0,6 on port 1 by exclusive or'ing with
           the mask that represents those pins. */
        P1OUT = P1OUT ^ (0b00000001 | 0b01000000);
    }
}
