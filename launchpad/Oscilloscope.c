#include <msp430g2553.h>
#include <legacymsp430.h>

char buffer[4];
int count=0;

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
	UCA0BR0=0x22; //0x8A for 115200
	UCA0BR1=0;
	UCA0MCTL = UCBRS0;    
	P1SEL = BIT1 + BIT2;                // P1.1 = RXD, P1.2=TXD
    P1SEL2= BIT1 + BIT2;                     // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;
    IE2 |= UCA0RXIE;  
     __bis_SR_register(LPM0_bits + GIE);     
                         // **Initialize USCI state machine**
}

void port_init()
{
	P1DIR = BIT6 + BIT0;
	P1OUT = BIT6 + BIT0;
}


int main(void)
{

watchdog_init();
			
	ADC10CTL0 = ADC10ON;	// 16 clock ticks, ADC On, enable ADC interrupt
	ADC10CTL1 = ADC10SSEL_3 + INCH_5;		//internal temp sensor
	ADC10CTL0 |=  ENC + ADC10SC;  
clock_init();
port_init();
uart_init();



return 0;
}

void Single_Measure(void)
{
	
	ADC10CTL0 |= ENC + ADC10SC;            	// Enable and start conversion
	putchar((ADC10MEM)>>2);
	
	count += 1;
	if(count == 128)
	{
		P1OUT ^= BIT0;
		count = 0;
	}ADC10CTL0 &= ~ENC;				// Disable ADC
	
}

int putchar(int b)
{
	while (!(IFG2 & UCA0TXIFG));              
	UCA0TXBUF = b;
	return 0;
}


interrupt(USCIAB0RX_VECTOR) USCI0RX_ISR(void)
{
  
  if (UCA0RXBUF == 'y')
   Single_Measure();                   // TX -> RXed character
}
