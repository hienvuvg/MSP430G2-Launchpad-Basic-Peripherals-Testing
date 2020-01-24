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
unsigned int Get_ADC_Value(void);

__IO uint n;

void main(void)
{
	Base_Config();
	PWM_Config();
	GPIO_Config();
	ADC_Config();

    while(1){
    	n = Get_ADC_Value();
    	TA0CCR1 = n;

    	Set(P1OUT,BIT0);
    	Delay(10);
    	Clr(P1OUT,BIT0);
    	Delay(500);
    }
}

void ADC_Config(void){
	ADC10CTL1 = INCH_5 | ADC10SSEL_0;	// Chon kenh dau vao va nguon clock cho ADC
										// INCH_5 		: Kenh dau vao A5 - P1.5 ; Selects Channel 5
										// ADC10SSEL_0 	: Clock cua ADC = ADC10OSC ~ 5MHz

	ADC10AE0 = BIT5;					// Enable Analog Input correspond channel 5
										// Thanh ghi 8 bit tuong ung 8 kenh dau vao Analog

	ADC10CTL0 = SREF_0; 				// VR+ = AVCC and VR- = AVSS
										// Vref = VR+ - VR- = 3,3V
	
	ADC10CTL0 |= ADC10ON + ENC; 		// ADC10ON 	: ADC10 On/Enable
										// ENC 		: ADC10 Enable Conversion
}

unsigned int Get_ADC_Value(void){
	ADC10CTL0 |= ADC10SC;
	while( ADC10CTL1 & BUSY);
	return ADC10MEM;
}

void Base_Config(void){
	WDTCTL = WDTPW | WDTHOLD;    		//Stop Watchdog
	BCSCTL1 = CALBC1_1MHZ;       		//2 thanh ghi cai dat tan so hoat dong
	DCOCTL = CALDCO_1MHZ;        		//1MHz
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
}

void Delay(volatile unsigned int n){
	volatile unsigned int m;
	while(n--){
		m = 100;
		while (m--);
	}
}
