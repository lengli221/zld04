#include "includes.h"

/*
** nvic cfg
*/
void nvic_Cfg(void){
	/*
	** usart1 interrput cfg
	*/
	usart1_Interrput_Cfg();
	/*
	** can0 interrupt cfg
	*/
	can0_Interrupt_Cfg();	
}

/*
** bsp init
*/
void bsp_init(void){
	/*
	** Init GPIO CFG 
	*/
	init_Gpio_Cfg();
	/*
	** init sysTick Cfg
	*/
	init_SysTick_Cfg();	
	/*
	** nvic cfg
	*/
	nvic_Cfg();	
	/*
	** init usart Cfg
	*/
	init_Usart_Cfg();	
	/*
	** init can
	*/
	init_Can();	
}

