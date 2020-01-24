#include <msp430g2553.h>

#define Clr(A,B) A&=~B;
#define Set(A,B) A|=B;
#define Tog(A,B) A^=B; // Toggle State // Thay doi
#define __IO volatile
#define uint unsigned int

void Delay(volatile unsigned int n);
void Base_Config(void);
void Tim_Config(void);
void GPIO_Config(void);

__IO uint n;

void main(void)
{
	Base_Config();
	Tim_Config();
	GPIO_Config();

	Set(P1OUT,BIT0);
	Set(P1OUT,BIT6);
	Delay(10);
	Clr(P1OUT,BIT0);
	Delay(10);

    while(1){
    }
}

void Base_Config(void){
	WDTCTL = WDTPW | WDTHOLD;   //Stop Watchdog
	BCSCTL1 = CALBC1_16MHZ;     //2 thanh ghi cai dat tan so hoat dong
	DCOCTL = CALDCO_16MHZ;      //16MHz
}

void Tim_Config(void){
	TA1CCR0 = 16000;
	TA1CTL |= TASSEL_2 + MC_1 + TAIE;
	_BIS_SR(GIE); 				// Cho phep ngat toan cuc
	//_EINT(); 					// enable interrupt
}
void GPIO_Config(void){
    Set(P1DIR,BIT0); 			// Out put pin P1.0
    Set(P1DIR,BIT6); 			// Out put pin P1.6
}

void Delay(volatile unsigned int n){
	__IO uint m;
	while(n--){
		m = 100;
		while (m--);
	}
}

// Ngắt tùm lum nhiều trường hợp cùng lúc
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer(void){
	 switch( TA1IV )
	 {
	   case  2: break;           // CCR1 not used
	   case  4: break;           // CCR2 not used
	   case 10: n++;break;       // overflow
	 }
	if( n > 500 ){
		n = 0;
		Tog(P1OUT,BIT0);
		Tog(P1OUT,BIT6);
	}
}
