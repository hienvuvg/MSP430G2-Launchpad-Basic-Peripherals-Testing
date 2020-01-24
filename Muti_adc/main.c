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
void ADC_Config(void);
void Get_ADC_Value(void);

__IO uint a,n;
unsigned int ADC[2];  // Array to hold ADC values

void main(void)
{
	Base_Config();
	PWM_Config();
	GPIO_Config();
	ADC_Config();

    while(1){
    	Get_ADC_Value();
    	if(Rea(P1IN,BIT3)){
    		while ( Rea(P1IN,BIT3));
    		if( a == 0 ) a= 1;
    		else a = 0;
    	}
    	if ( a == 0 ) n = ADC[0];
    	else n = ADC[1];

    	TA0CCR1 = n;
    }
}

void ADC_Config(void){
	  ADC10CTL1 = INCH_2 + CONSEQ_1;		// A2/A1/A0 once multi channel. Hightest of channels.
	  ADC10CTL0 = SREF_0 + ADC10SHT_0 + MSC + ADC10ON + ADC10IE;
	  ADC10AE0 = BIT1+BIT2;                 // P1.1, P1.2 ADC option select
	  ADC10DTC1 = 2;                        // 2 conversions
}

void Get_ADC_Value(void){
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
    ADC10SA = (int)ADC;                     /******* Data buffer start*******/
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion ready
    __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	//code from reading from res and sending it to AP
	__bic_SR_register_on_exit(CPUOFF);      // Clear CPUOFF bit from 0(SR)
}

void Base_Config(void){
	WDTCTL = WDTPW | WDTHOLD;    			//Stop Watchdog
	BCSCTL1 = CALBC1_1MHZ;       			//2 thanh ghi cai dat tan so hoat dong
	DCOCTL = CALDCO_1MHZ;        			//1MHz
}

void PWM_Config(void){
    TA0CCTL1 = OUTMOD_7;
    TA0CCR0 = 1000;
    TA0CCR1 = 0;
    TA0CTL = TASSEL_2 + MC_1 ;
}

void GPIO_Config(void){
    Set(P1DIR,BIT0);

    Set(P1DIR,BIT6);
    Set(P1SEL,BIT6);

    Set(P1REN,BIT3);
}

void Delay(volatile unsigned int n){
	volatile unsigned int m;
	while(n--){
		m = 100;
		while (m--);
	}
}
