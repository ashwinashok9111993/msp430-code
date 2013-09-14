#include <msp430g2553.h>
#include <legacymsp430.h>



void clock_init()
{
	BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
}

void watchdog_init()
{
	WDTCTL = WDTPW + WDTHOLD;
}

void uart_init()
{
	UCA0CTL1 = UCSWRST;
	UCA0CTL1 = UCSSEL_2;
	UCA0BR0=0x8A; //0x8A for 115200
	UCA0BR1=0;
	UCA0MCTL = UCBRS0;    
	P1SEL = BIT1 + BIT2;                // P1.1 = RXD, P1.2=TXD
    P1SEL2= BIT1 + BIT2;                     // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;
    IE2 |= UCA0RXIE;  
       
                         // **Initialize USCI state machine**
}



void timer_init()
{
 P1DIR |= BIT6; // P1.6
 P1SEL |= BIT6; // P1.6
 TACCR0 = 0xffff; // PWM Period
 TACCTL1 = OUTMOD_7; // CCR1 reset/set
 TACCR1 = 0; // CCR1 PWM duty cycle
 TACTL = TASSEL_2 + MC_1 + ID_3 ;
 __bis_SR_register(LPM3_bits + GIE);
}

int main(void)
{
watchdog_init();
clock_init();
uart_init();
timer_init();
return 0;
}




interrupt(USCIAB0RX_VECTOR) USCI0RX_ISR(void)
{
  while (!(IFG2 & UCA0TXIFG));              
	UCA0TXBUF = TACCR1 >> 8;
  if(UCA0RXBUF == 'y')
  TACCR1+=1;
  if(TACCR1 > 0xffff)
  TACCR1 = 0  ;              // TX -> RXed character
}
