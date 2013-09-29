#include <msp430.h>
#include <legacymsp430.h>
#include <stdio.h>

/*
*MSP430  based 4bit lcd code..


-Connections:
RS -> P1.0
EN -> P1.1
D7 to D4  -> P1.7 to P1.4
R/W to GROUND
 
-Compiler used: msp430-gcc
-Command line: msp430-gcc -mmcu=msp430fXXXX rtc_uart_1602lcd.c 
>mspdebug rf2500
> prog a.out
> run
 
*/
 

 
#define RS(X)     P1OUT = ((P1OUT & ~(BIT0)) | (X))
#define EN(X)   P1OUT = ((P1OUT & ~(BIT1)) | (X<<1))
#define LCD_STROBE do{EN(1);EN(0);}while(0)
#define databits P1OUT  // P1.7 - D7, ....., P1.4 - D4
#define LINE1 cmd(0x80)
#define LINE2 cmd(0xc0)

char buffer[5];

 
void port_init()
{
    P1OUT = 0 ;
    P1DIR = 0xff;
}
 
void data(unsigned char c)
{
    RS(1);
    __delay_cycles(4000);  //40 us delay
    databits = (databits & 0x0f) | (c & 0xf0);
    LCD_STROBE;
    databits = (databits & 0x0f) | (c << 4) ;
    LCD_STROBE;
}
 
void cmd(unsigned char c)
{
    RS(0);
    __delay_cycles(4000); //40 us delay
    databits = (databits & 0x0f) | (c & 0xf0);
    LCD_STROBE;
    databits = (databits & 0x0f) | (c << 4) ;
    LCD_STROBE;
}
 
void pseudo_8bit_cmd(unsigned char c)
{
    RS(0);
    __delay_cycles(150000); 
    databits = (c & 0xf0);
    LCD_STROBE;
}
void clear(void)
{
    cmd(0x01);
    __delay_cycles(30000); 
}
 
void lcd_init()
{
    pseudo_8bit_cmd(0x30); //this command is like 8 bit mode command
    pseudo_8bit_cmd(0x30); //lcd expect 8bit mode commands at first
    pseudo_8bit_cmd(0x30); //for more details, check any 16x2 lcd spec
    pseudo_8bit_cmd(0x20);
    cmd(0x28);             //4 bit mode command started, set two line
    cmd(0x0c);             // Make cursorinvisible
    clear();               // Clear screen
    cmd(0x6);              // Set entry Mode(auto increment of cursor)
}
 
void string(char *p)
{
    while(*p) data(*p++);
}

void clock_init()
{
   
  
  P5SEL |= 0x0C;                            // Port select XT2

  UCSCTL6 &= ~XT2OFF;                       // Enable XT2
  UCSCTL3 |= SELREF_2;                      // FLLref = REFO
                                            // Since LFXT1 is not used,
                                            // sourcing FLL with LFXT1 can cause
                                            // XT1OFFG flag to set
  UCSCTL4 |= SELA_2;                        // ACLK=REFO,SMCLK=DCO,MCLK=DCO

  // Loop until XT1,XT2 & DCO stabilizes
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

  UCSCTL6 &= ~XT2DRIVE0;                    // Decrease XT2 Drive according to
                                            // expected frequency
  UCSCTL4 |= SELS_5 + SELM_5;               // SMCLK=MCLK=XT2
}



void uart_init()
{
  P3SEL = 0x30;                           // P3.4,5 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 0x8A;                              // 16MHz 115200 (see User's Guide)
  UCA0BR1 = 0;                              // 16MHz 115200
  UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

void RTC_init(int year,int month,int day ,int hour ,int min ,int sec)
{

  RTCCTL1 = 0b01100000;
  RTCYEAR = year;                         
  RTCMON = month;
  RTCDAY = day;
  
  RTCHOUR = hour;
  RTCMIN = min;
  RTCSEC = sec;
  RTCCTL1 = 0b00100000;

}
void timer_init()
{
	 TA1CTL = TASSEL_1 + MC_1 + TACLR + TAIE;
	 TA1CCR0 = 0xffff/2;
	 
}
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  clock_init();                 // Stop WDT
  uart_init();
 
  port_init();
  lcd_init();
  timer_init();


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
  if (UCA0RXBUF == 'c') //calibrate
    {
	RTC_init(2013,10,10,10,10,10);
    RTCCTL1 |= RTCRDY;
	printf(" %d %d %d %d %d %d \n",RTCYEAR,RTCMON,RTCDAY,RTCHOUR,RTCMIN,RTCSEC);
	
	RTCCTL1 &= ~RTCRDY;
	}
    if (UCA0RXBUF == 't') //reset
    {
	RTC_init(0,0,0,0,0,0);
    RTCCTL1 |= RTCRDY;
	printf(" %d %d %d %d %d %d \n",RTCYEAR,RTCMON,RTCDAY,RTCHOUR,RTCMIN,RTCSEC);
	RTCCTL1 &= ~RTCRDY;
	}
	
	if (UCA0RXBUF == 's') //show
    {
	
    RTCCTL1 |= RTCRDY;
	printf(" %d %d %d %d %d %d \n",RTCYEAR,RTCMON,RTCDAY,RTCHOUR,RTCMIN,RTCSEC);	
	RTCCTL1 &= ~RTCRDY;
	}
}

//timer used to update the contents of the lcd
interrupt(TIMER1_A1_VECTOR) TIMER1_A1_ISR(void)
{
  if(TA1IV == 14)
 
     
	
	{   RTCCTL1 |= RTCRDY;
		
		LINE1;
		clear();
		itoa(RTCHOUR,buffer,10);
		string(buffer);
		string(":");
		itoa(RTCMIN,buffer,10);
		string(buffer);
		string(":");
		itoa(RTCSEC,buffer,10);
		string(buffer);
	RTCCTL1 &= ~RTCRDY;
	}
		                // overflow
         
  
}


