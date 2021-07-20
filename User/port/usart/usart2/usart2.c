#include "includes.h"

/*
** init Usart2 Gpio Cfg
*/
void init_Usart2GpioCfg(void){
	/* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOB);	
	
	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART2);

	/* connect port to USARTx_Tx */
	gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);	
	
	/* connect port to USARTx_Rx */
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);		
}


/*
** init Usart2 Cfg
*/
void init_Usart2Cfg(void){
	/*
	** init Usart1 Gpio Cfg
	*/
	init_Usart2GpioCfg();	
}

