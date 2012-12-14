

#include <msp430f149.h>

#define SWITCH               BIT0
#define SWITCH_PRESSED       0x00
#define LED1                 BIT0
#define LED2                 BIT1

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  
  P3DIR &=~ SWITCH;
                                   
    P3OUT |= SWITCH;
    
    P2DIR = LED1+LED2;
    P2OUT &=~ LED1;
    P2OUT |= LED2;
    
       while (1)
    {
        if ((P3IN & SWITCH) == SWITCH_PRESSED)
        {
            /* Hang until the switch is released. */
            while ((P3IN & SWITCH) == SWITCH_PRESSED); 

            P2OUT ^= LED1 | LED2;                 
        }
    }
}
