#include <msp430g2553.h>

#define Clr(A,B) A&=~B;
#define Set(A,B) A|=B;
#define Tog(A,B) A^=B; // Toggle State // Thay doi
#define Rea(A,B) (A&B) == B ? 0:1
#define __IO volatile
#define uint unsigned int

void Delay(volatile unsigned int n);
void Base_Config(void);
void PWM_Config(void);
void GPIO_Config(void);

void main(void)
{
	Base_Config();
	PWM_Config();
	GPIO_Config();

    while(1){
    	if(TA0CCR1 < 999 ) TA0CCR1++;
    	else TA0CCR1 = 1;
    	Delay(1);
    }
}

void Base_Config(void){
	WDTCTL = WDTPW | WDTHOLD;    //Stop Watchdog
	BCSCTL1 = CALBC1_1MHZ;       //2 thanh ghi cai dat tan so hoat dong
	DCOCTL = CALDCO_1MHZ;        //1MHz
}

void PWM_Config(void){
    TA0CCTL1 = OUTMOD_7;		 // PWM output mode: 7 - PWM reset/set
    TA0CCR0 = 1000;
    TA0CCR1 = 0;
    TA0CTL = TASSEL_2 + MC_1 ;
}
void GPIO_Config(void){
    Set(P1DIR,BIT6);
    Set(P1SEL,BIT6);
}

void Delay(volatile unsigned int n){
	volatile unsigned int m;
	while(n--){
		m = 100;
		while (m--);
	}
}
