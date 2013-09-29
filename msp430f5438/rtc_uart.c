#include <msp430.h>
#include <legacymsp430.h>
#include <stdio.h>

void uart_init()
{
	P3SEL = 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 9;                              // 1MHz 115200 (see User's Guide)
  UCA0BR1 = 0;                              // 1MHz 115200
  UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

void RTC_init(int year,int month,int day ,int hour ,int min ,int sec)
{

  RTCCTL1 = 0b00100000;
  RTCYEAR = year;                         
  RTCMON = month;
  RTCDAY = day;
  
  RTCHOUR = hour;
  RTCMIN = min;
  RTCSEC = sec;
  

}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  uart_init();
  
  
 
 


  RTC_init(2013,10,10,10,10,10);
  

  __bis_SR_register(GIE);       // Enter LPM0, interrupts enabled
  __no_operation();                         // For debugger

}


int putchar(int a)
{
	while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    UCA0TXBUF = a;
    return 0;
}


interrupt(USCI_A0_VECTOR)USCI_A0_ISR(void)
{
  if (UCA0RXBUF == 'c')
    {
	RTC_init(2013,10,10,10,10,10);
    RTCCTL1 |= RTCRDY;
	printf(" %d %d %d %d %d %d \n",RTCYEAR,RTCMON,RTCDAY,RTCHOUR,RTCMIN,RTCSEC);
	RTCCTL1 &= ~RTCRDY;
	}
    if (UCA0RXBUF == 't')
    {
	RTC_init(0,0,0,0,0,0);
    RTCCTL1 |= RTCRDY;
	printf(" %d %d %d %d %d %d \n",RTCYEAR,RTCMON,RTCDAY,RTCHOUR,RTCMIN,RTCSEC);
	RTCCTL1 &= ~RTCRDY;
	}
	if (UCA0RXBUF == 's')
    {
	
    RTCCTL1 |= RTCRDY;
	printf(" %d %d %d %d %d %d \n",RTCYEAR,RTCMON,RTCDAY,RTCHOUR,RTCMIN,RTCSEC);
	RTCCTL1 &= ~RTCRDY;
	}
}


