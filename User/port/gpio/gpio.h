#ifndef __GPIO_H_
#define __GPIO_H_

#include "type.h"
#include "gd32f10x.h"

#define Led_Ctr_Off		(GPIO_BOP(GPIOC) = GPIO_PIN_13)
#define Led_Ctr_On		(GPIO_BC(GPIOC) = GPIO_PIN_13) 

/*
** Init GPIO CFG 
*/
void init_Gpio_Cfg(void);

/*
** hardware Ctr Door
**	@param:
**		bool:
**				TRUE-->���̵���
**				FALSE-->�ؼ̵���
*/
void hd_CtrDoor(bool flag);

/*
** xy Ctr Door
** @return:
**	bool:
**		TRUE-->��
**		FALSE-->��
*/
bool xy_CtrDoor(void);

#endif

