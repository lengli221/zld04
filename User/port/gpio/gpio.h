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
**				TRUE-->开继电器
**				FALSE-->关继电器
*/
void hd_CtrDoor(bool flag);

/*
** xy Ctr Door
** @return:
**	bool:
**		TRUE-->开
**		FALSE-->关
*/
bool xy_CtrDoor(void);

#endif

