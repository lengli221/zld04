#ifndef __GPIO_H_
#define __GPIO_H_

#define Led_Ctr_Off		(GPIO_BOP(GPIOC) = GPIO_PIN_13)
#define Led_Ctr_On		(GPIO_BC(GPIOC) = GPIO_PIN_13) 

/*
** Init GPIO CFG 
*/
void init_Gpio_Cfg(void);

#endif

