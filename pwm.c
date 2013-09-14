#include <msp430.h>
 
void main(void)
{
 WDTCTL = WDTPW + WDTHOLD; // Stop WDT
 P1DIR |= 0x0C; // P1.2 and P1.3 output
 P1SEL |= 0x0C; // P1.2 and P1.3 TA1/2 options
 CCR0 = 1024; // PWM Period
 CCTL1 = OUTMOD_7; // CCR1 reset/set
 CCR1 = 20; // CCR1 PWM duty cycle
 TACTL = TASSEL_2 + MC_1; // ACLK, up mode
 
 ADC10CTL0 = ADC10SHT_2 + ADC10ON ; // ADC10ON, interrupt enabled
 ADC10CTL1 = INCH_5; // input A1
 
 
for (;;)
 {
 ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start
 while (ADC10CTL1 & ADC10BUSY); // ADC10BUSY?
 CCR1 = ADC10MEM;
 }
 }
