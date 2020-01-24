#include <msp430g2553.h>

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

__IO uint n;

void main(void)
{
	Base_Config();
	GPIO_Config();
	Set(P1OUT,BIT0);
	Delay(10);
	Clr(P1OUT,BIT0);
	Delay(10);

    while(1){
    	while(Rea(P1IN,BIT3));
    	WDTCTL = WDT_MRST_32;
    }
}

void Base_Config(void){
	WDTCTL = WDT_MRST_32;
	BCSCTL1 = CALBC1_1MHZ;       	//2 thanh ghi cai dat tan so hoat dong
	DCOCTL = CALDCO_1MHZ;        	//1MHz
}


void GPIO_Config(void){
    Set(P1DIR,BIT0); 				// Out put pin P1.0
    Clr(P1OUT,BIT0);

    Clr(P1DIR,BIT3);
    Set(P1REN,BIT3);
}

void Delay(volatile unsigned int n){
	__IO uint m;
	while(n--){
		m = 100;
		while (m--);
	}
}
