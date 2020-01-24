//******************************************************************************
// MSP430G2xx Demo - WDT, Low Power Mode, Interval Overflow ISR, DCO SMCLK
//
// Description: Go to LowPowerMode using software timed by the WDT ISR. The LED
// will turn off when sleeping, Port 1 interrupt will awake the mcu and turn on
// LED. The mcu will sleep at approximately 250 * 32ms based on default
// DCO/SMCLK clock source used in this example for the WDT.
// ACLK = n/a, MCLK = SMCLK = default
//
// MSP430G2xxx
// -----------------
// /|\| XIN|-
// | | |
// --|RST XOUT|-
// | |
// | P1.0|-->LED
//
// Aldo Briano
// Texas Instruments Inc.
// July 2010
//******************************************************************************
#include <msp430g2553.h>

#define Clr(A,B) A&=~B;
#define Set(A,B) A|=B;
#define Tog(A,B) A^=B; // Toggle State // Thay doi
#define __IO volatile
#define uint unsigned int

void Delay(volatile unsigned int n);
void Base_Config(void);
void Int_Config(void);
void GPIO_Config(void);

unsigned int wdtCounter = 0;

void main(void)
{
	Base_Config();
	Int_Config();
	GPIO_Config();

	while(1);
}

// Watchdog Timer interrupt service routine
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
	if(wdtCounter == 100){
		Clr(P1OUT,BIT0); 			// Turn off LED at 1.0
		wdtCounter = 0;
		_BIS_SR(LPM3_bits + GIE); 	// Enter Low Power Mode 3 w/interrupt
	}
	else
		wdtCounter++;				// WDT like counter
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	_BIC_SR(LPM3_EXIT); 			// wake up from low power mode
	wdtCounter = 0; 				// reset watchdog timer counter
	Set(P1OUT,BIT0); 				// Turn on LED at 1.0
	Clr(P1IFG,BIT3); 				// Xoa co ngat chan P1.3
}

void Base_Config(void){
	WDTCTL = WDT_MDLY_32; 			// Set Watchdog Timer interval to ~32ms
	BCSCTL1 = CALBC1_1MHZ;      	// 2 thanh ghi cai dat tan so hoat dong
	DCOCTL = CALDCO_1MHZ;       	// 1MHz
}
void Int_Config(void){
	IE1 |= WDTIE; 					// Enable WDT interrupt
	Set(P1IE,BIT3); 				// Cho phep ngat chan P1.3
	Set(P1IES,BIT3); 				// Ngat theo suong xuong
	Clr(P1IFG,BIT3); 				// Xoa co ngat chan P1.3
	_BIS_SR(GIE); 					// Cho phep ngat toan cuc
	__enable_interrupt();
}
void GPIO_Config(void){
	Set(P1DIR,BIT0); 				// Set P1.0 to output direction
	Set(P1OUT,BIT0); 				// Turn on LED at 1.0
	Clr(P1DIR,BIT3); 				// Set button In
	Set(P1REN,BIT3); 				// Keo tro len nguon
}

void Delay(volatile unsigned int n){
	volatile unsigned int m;
	while(n--){
		m = 100;
		while (m--);
	}
}
