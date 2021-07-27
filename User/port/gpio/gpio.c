#include "includes.h"

/*
** Init GPIO CFG 
*/
void init_Gpio_Cfg(void){
	/* enable the led clock */
	rcu_periph_clock_enable(RCU_GPIOC);
	
	/* configure led GPIO port */
	gpio_init(GPIOC,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_13);
	
	/*operate*/
	Led_Ctr_Off;
}

/*
** hardware Ctr Door
**	@param:
**		bool:
**				TRUE-->开继电器
**				FALSE-->关继电器
*/
void hd_CtrDoor(bool flag){
	if(flag == TRUE){/*开继电器*/
	
	}else{/*关继电器*/
	
	}
}

/*
** xy Ctr Door
** @return:
**	bool:
**		TRUE-->开
**		FALSE-->关
*/
bool xy_CtrDoor(void){
	bool ret = FALSE;
	
	return ret;
}
