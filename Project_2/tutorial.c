/*
Author: Nobody
*/

#include <MKL25Z4.H>
#include <stdio.h>
#include <math.h>
#include "utils.h"
#define red_LED_on()    {GPIOB_PDOR &= ~((uint32_t)(1 << 18));}
#define red_LED_turn_off()   {GPIOB_PDOR |=  ((uint32_t)(1 << 18));}
#define green_LED_on()  {GPIOB_PDOR &= ~((uint32_t)(1 << 19));}
#define green_LED_off() {GPIOB_PDOR |=  ((uint32_t)(1 << 19));}
#define red_LED_toggle() { GPIOB_PTOR |= ((uint32_t)(1 << 18));}
#define green_LED_toggle() { GPIOB_PTOR |= ((uint32_t)(1 << 19));} //Define led toggle,turn on and turn of commands

volatile int isr_in = 0; // indicates isr1 loop number
volatile int sw1 = 0; // indicates which switch is pressed in isr1
volatile int count = 0; // stores the current number of isr2
void Ext_Led_Cont(int bit1, int bit2, int bit3, int bit4);


void Port_Init (){
	//Port Clock Enable
	SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK;
	PORTA->PCR[1] = 0x103;      /* make PTA1 pin as GPIO and enable pullup, this port for count*/
	PTA->PDDR = 0x00;						/* make PTA7-0 as input pins */
	
	SIM->SCGC5 |=  SIM_SCGC5_PORTC_MASK;
	PORTC->PCR[5] = 0x100;      /* make PTC5 pin as GPIO*/
  PORTC->PCR[3] = 0x100;      /* make PTC3 pin as GPIO*/
	PORTC->PCR[4] = 0x100;  		/* make PTC4 pin as GPIO*/
	PORTC->PCR[7] = 0x100;  		/* make PTC7 pin as GPIO*/
	GPIOC_PDOR |= (uint32_t)(0 << 5); /* PTC5 is driven LOW */
	GPIOC_PDOR |= (uint32_t)(0 << 3); /* PTC3 is driven LOW*/
	GPIOC_PDOR |= (uint32_t)(0 << 4); /* PTC4 is driven LOW */
	GPIOC_PDOR |= (uint32_t)(0 << 7); /* PTC7 is driven LOW initally */
	PTC->PDDR = 0xB8;						/* make necessary PTC pins as output pins, these ports for light led */
	
	SIM->SCGC5 |=  SIM_SCGC5_PORTB_MASK;
	PORTB_PCR18 = (uint32_t)(0x100); /* PTB19 as GPIO */
	PORTB_PCR19 = (uint32_t)(0x100); /* PTB19 as GPIO these ports for kl25 led */
	GPIOB_PDDR |= (uint32_t)(1 << 18); /* PTB18 is Output */
	GPIOB_PDDR |= (uint32_t)(1 << 19); /* PTB19 is Output */
	green_LED_off()
	red_LED_turn_off()
	
	SIM->SCGC5 |=  SIM_SCGC5_PORTD_MASK;
	PORTD->PCR[2] = 0x103;      /* make PTD2 pin as GPIO */
  PORTD->PCR[3] = 0x103;      /* make PTD3 pin as GPIO */
	PTD->PDDR = 0x00;           /* make PTD7-0 as input pins, these ports isr1 */
}
void Interrupt_Set_and_Enable	(){
	PORTD->PCR[2] &= ~0xF0000;
	PORTD->PCR[3] &= ~0xF0000;
	PORTA->PCR[1] &= ~0xF0000;
	NVIC_SetPriority (PORTD_IRQn, 128);
	NVIC_SetPriority (PORTA_IRQn, 64);
	NVIC_SetPriority (SysTick_IRQn, 3);
	PORTA->PCR[1] |=  0x80000;
	SysTick->CTRL = 0;
	NVIC_EnableIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);
	PORTD->PCR[2] |=  0x80000;
	PORTD->PCR[3] |=  0x80000;
	SysTick ->VAL = 0;  // protect from inital value
	SysTick->LOAD = (1690000) - 1; // Set the reload value for 1.3s
	SysTick->CTRL  = SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;  

	
	
}

void SysTick_Handler(){
	
	if (isr_in > 0){
		red_LED_turn_off();
		isr_in = isr_in-1;
		if (sw1==1)
		{
			green_LED_toggle();
		}
		else if (isr_in == 0){
			sw1=0; // setup for exiting isr1 and going into main function
			SysTick->VAL=0;
			SysTick->LOAD = 1690000-1;
			green_LED_off();
		}
	}
	else{
		green_LED_off();
		red_LED_toggle();
	}
}
void PORTD_IRQHandler(){
	if(PORTD->ISFR & 0x00000008){
		sw1=1;
	isr_in = 10;
	SysTick->VAL=0;
	red_LED_turn_off();
	}
	else{
		sw1=0;
		red_LED_turn_off();
		isr_in=1;
		SysTick->LOAD = 910000-1;
		SysTick->VAL=0;
		
	}
	
	PORTD->ISFR |= 0x00000012; // clearing interrupt for protecting again going in
}
void PORTA_IRQHandler(){ // check counter value and update it 
	
	
	if (count <= 15){
		count = count +1;
	}
	else {
		count = 1; //1 olabilir bu
	}
	Ext_Led_Cont(count&1,count&2,count&4,count&8);
	Delay(300000);  // This delay put for avoid receiving incorrect data
	PORTA->ISFR |= 0x00000002;
}
void Ext_Led_Cont(int bit1, int bit2, int bit3, int bit4){
	if(bit1){
		GPIOC_PDOR |= (uint32_t)(1 << 5); /* PTC5 is turned on */}
	else {
		GPIOC_PDOR &= ~(uint32_t)(1 << 5); /* PTC5 is turned off*/}
	if(bit2){
		GPIOC_PDOR |= (uint32_t)(1 << 3); /* PTC3 is turned on */}
	else {
		GPIOC_PDOR &= ~(uint32_t)(1 << 3); /* PTC3 is turned off */}
	if(bit3)
		GPIOC_PDOR |= (uint32_t)(1 << 4); /* PTC4 is turned on */
	else 
		GPIOC_PDOR &= ~(uint32_t)(1 << 4); /* PTC4 is turned off */
	if(bit4)
		GPIOC_PDOR |= (uint32_t)(1 << 7); /* PTC7 is turned on */
	else 
		GPIOC_PDOR &= ~(uint32_t)(1 << 7); /* PTC7 is turned off */
}
int main() {
	__disable_irq();
  Port_Init();
	Interrupt_Set_and_Enable();
	__enable_irq();
	while (1){
		Ext_Led_Cont(count&1,count&2,count&4,count&8);
	}

}
