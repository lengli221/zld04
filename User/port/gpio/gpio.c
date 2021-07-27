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
**				TRUE-->���̵���
**				FALSE-->�ؼ̵���
*/
void hd_CtrDoor(bool flag){
	if(flag == TRUE){/*���̵���*/
	
	}else{/*�ؼ̵���*/
	
	}
}

/*
** xy Ctr Door
** @return:
**	bool:
**		TRUE-->��
**		FALSE-->��
*/
bool xy_CtrDoor(void){
	bool ret = FALSE;
	
	return ret;
}
