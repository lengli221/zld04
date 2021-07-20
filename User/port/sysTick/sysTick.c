#include "includes.h"

Sys_Tick sysTick = {0};

/*
** init sysTick Cfg
*/
void init_SysTick_Cfg(void){
	/* setup systick timer for 1000Hz interrupts */
	if (SysTick_Config(SystemCoreClock / 1000U)){
			/* capture error */
			while (1){
			}
	}

	/* configure the systick handler priority */
	NVIC_SetPriority(SysTick_IRQn, 0x00U);		
}

/*
** SysTick Handler
*/
void SysTick_Handler(void){
	sysTick.msCnt++;
	if(sysTick.msCnt == 0xFFFFFFFF){
		sysTick.msCnt = 0;
	}
}

/*
** Tickout
**	@param:
**		uint32*:计时
**		uint32:时间基准
**	@return:
**		bool:true-->超时 false-->未超时
*/
bool Tickout(uint32* tick,uint32 cnt){
	bool ret = FALSE;
	
	if(cnt == 0){
		*tick = sysTick.msCnt;
		ret = TRUE;
	}else{
		if((sysTick.msCnt - *tick) >= cnt){
			ret = TRUE;
		}else{
			ret = FALSE;
		}
	}
	
	return ret;
}

