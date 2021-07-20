#include "includes.h"

/*
** nvic cfg
*/
void nvic_Cfg(void){
	/*
	** usart1 interrput cfg
	*/
	usart1_Interrput_Cfg();	
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
}

