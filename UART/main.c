#include "msp430g2553.h"
#include "Library/UART.h"

#define Clr(A,B) A&=~(1>>B);
#define Set(A,B) A|=(1>>B);

int abc=134;
float float1=14454.3486;

char temp;

void Delay(volatile unsigned int n);
void Base_Config(void);
void GPIO_Config(void);

void main(void)
{
    Base_Config();
    GPIO_Config();

    Set(P1OUT,0);
    Delay(10);
    UART_Init();				// Sua baudrate trong file.h
    UART_Write_Char(10);  		//Ky tu xuong dong
    UART_Write_Char('A'); 		//A
    UART_Write_Char(10);
    UART_Write_String("Chuong trinh test UART");
    UART_Write_Char(10);
    Clr(P1OUT,0);
    UART_Write_Reg("P1IN",P1IN); //0b00000010
    UART_Write_Char(10);
    UART_Write_Int(abc);
    UART_Write_Char(10);
    UART_Write_Float(float1,5);
    UART_Write_Char(10);
    abc=UART_Read_Char();
    UART_Write_Char(abc);
    __bis_SR_register(LPM0);
}

void GPIO_Config(void){
	P1DIR |= 0x01;				// Set P1.0 to output direction
}

void Base_Config(void){
	WDTCTL = WDTPW | WDTHOLD;    //Stop Watchdog
	BCSCTL1 = CALBC1_1MHZ;       //2 thanh ghi cai dat tan so hoat dong
	DCOCTL = CALDCO_1MHZ;        //1MHz
}

void Delay(volatile unsigned int n){
	volatile unsigned int m;
	while(n--){
		m = 1000;
		while (m--);
	}
}
