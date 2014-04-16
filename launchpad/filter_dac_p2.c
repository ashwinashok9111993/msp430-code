/* filter test */
//////////////////////////////////////////////////////////////////////////////////////////////
//
//
// use port 2 as 8 bit r -2r ladder dac
// p1.5 as analog input
// sample rate 40 khz
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////

#include<msp430g2452.h>
#include<legacymsp430.h>

#define NCoef 4
#define DCgain 1

char iir(char NewSample) {
    char ACoef[NCoef+1] = {
           12,
          -48,
           72,
          -48,
           12
    };

    char BCoef[NCoef+1] = {
          128,
            0,
           62,
            0,
            2
    };

    static int y[NCoef+1]; //output samples
    //Warning!!!!!! This variable should be signed (input sample width + Coefs width + 4 )-bit width to avoid saturation.

    static char x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
        y[0] += ACoef[n] * x[n] - BCoef[n] * y[n];

    y[0] /= BCoef[0];
    
    return y[0] / 16;
}
void clock_init()
{
	BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
}

void watchdog_init()
{
	WDTCTL = WDTPW + WDTHOLD;
}

void port_init()
{
  P2SEL &= ~(BIT6 + BIT7);	 // Configure XIN & XOUT to GPIO
  P2OUT = 0x00;				 // Drive all Port 2 pins low
  P2DIR = 0xFF;	;
}

void timer_init()
{
	TACTL = TASSEL_2 + MC_1;
	TACCR0=400-1;
	TACCTL0 = CCIE;                             // CCR0 interrupt enabled
}

void adc_init()
{
	ADC10CTL0 = ADC10ON;	// 16 clock ticks, ADC On, enable ADC interrupt
	ADC10CTL1 = ADC10SSEL_3 + INCH_5;		//internal temp sensor
}

int main(void)
{

watchdog_init();
clock_init();
port_init();
timer_init();
adc_init();	
__bis_SR_register(LPM0_bits + GIE);

return 0;
}

interrupt(TIMER0_A0_VECTOR) timer_a(void)
{   
	//TACCTL0 = 0;
	ADC10CTL0 |= ENC + ADC10SC;
	while (ADC10CTL1 & ADC10BUSY);             	// Enable and start conversion
	P2OUT = iir(ADC10MEM>>2);
	//P2OUT = (ADC10MEM>>2);
    ADC10CTL0 &= ~ENC;
    //TACCTL0 = CCIE;
    	
}


