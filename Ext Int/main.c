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

void main(void)
{
	Base_Config();
	Int_Config();
	GPIO_Config();

    while(1){
    }
}

void Base_Config(void){
	WDTCTL = WDTPW | WDTHOLD;   // Stop Watchdog
	BCSCTL1 = CALBC1_1MHZ;      // 2 thanh ghi cai dat tan so hoat dong
	DCOCTL = CALDCO_1MHZ;       // 1MHz
}

void Int_Config(void){
	Set(P1IE,BIT3); 			// Cho phep ngat chan P1.3
	Set(P1IES,BIT3); 			// Ngat theo suong xuong
	Clr(P1IFG,BIT3); 			// Xoa co ngat chan P1.3
	_BIS_SR(GIE); 				// Cho phep ngat toan cuc
}
void GPIO_Config(void){
    Set(P1DIR,BIT0);

    Clr(P1DIR,BIT3); 			// Set In
    Set(P1REN,BIT3); 			// Keo tro len nguon
}

void Delay(volatile unsigned int n){
	volatile unsigned int m;
	while(n--){
		m = 100;
		while (m--);
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void Port(void){
	Tog(P1OUT,BIT0);
	Clr(P1IFG,BIT3); 			// Xoa co ngat
}
