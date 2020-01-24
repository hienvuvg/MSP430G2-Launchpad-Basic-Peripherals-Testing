/******************************************************************************
 *                          MSP430G2553 ACLK Test
 *
 * Description: This code can be used to test if you have installed the
 *              32.768kHz crystal on your LaunchPad correctly. Using this code
 *              LED1 (on P1.0) will turn on for 1 second, and off for one
 *              second. You can verify this with either an oscilloscope, logic
 *              analyzer, or by inspection. Simple as that!
 *
 * 				This code was originally created for "NJC's MSP430
 * 				LaunchPad Blog" and fixed by Hien Vu
 * Note       : Tránh gây nhiễu cho hai chân thạch anh Xin và Xout.
 * 				Hệ thống sẽ treo nếu có nhiễu, do đó cần thêm Watchdog timer.
 * Author:	Hien Vu
 * Web:	    msp430launchpad.com
 * Date:	31-01-2016
 ******************************************************************************/

#include  <msp430g2553.h>

#define Clr(A,B) A&=~B;
#define Set(A,B) A|=B;
#define Tog(A,B) A^=B; // Toggle State // Thay doi
#define Rea(A,B) (A&B) == B ? 0:1
#define __IO volatile
#define uint unsigned int

void Delay(volatile unsigned int n);
void Base_Config(void);
void Tim_Config(void);
void GPIO_Config(void);
void Base1_Config(void);

__IO uint currentMinutes, currentSeconds;

void main(void)
{
	Base_Config();
	Tim_Config();
	GPIO_Config();

	Set(P1OUT,BIT0);
	Delay(10);
	Clr(P1OUT,BIT0);
	Delay(10);

	while(1);
}

// Clock system is 32768 / 8
void Base1_Config(void){
	WDTCTL   = WDTPW + WDTHOLD;					// Stop WDT
	BCSCTL1 |= DIVA_3;							// ACLK/8
	BCSCTL3 |= XCAP_3;							//12.5pF cap- setting for 32768Hz crystal
}

// Clock system different ACLK
void Base_Config(void){
	WDTCTL  = WDTPW | WDTHOLD;   				//Stop Watchdog
	BCSCTL1 = CALBC1_1MHZ;      				//2 thanh ghi cai dat tan so hoat dong
	DCOCTL  = CALDCO_1MHZ;       				//16MHz
}

void Tim_Config(void){
	TA0CCR0 = 4095;								// Base1 : 512  -> 1 sec ( ACLK = Clock system )
												// Base  : 4096 -> 1 sec ( ACLK = Crystal)
	TA0CTL |= TASSEL_1 + ID_3 + MC_1 + TAIE;	// ACLK, /8, upmode
	_BIS_SR(GIE); 								// Cho phep ngat toan cuc
}

void GPIO_Config(void){
	Set(P1DIR,BIT0);
	Clr(P1OUT,BIT0);
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A (void)
{
	if(TA0IV == 10 ) Tog(P1OUT,BIT0);			// Toggle LED
}

void Delay(volatile unsigned int n){
	__IO uint m;
	while(n--){
		m = 100;
		while (m--);
	}
}
