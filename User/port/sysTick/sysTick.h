#ifndef __SYSTICK_H_
#define __SYSTICK_H_

#include "type.h"
#include "gd32f10x.h"
/*
** sysTick
*/
typedef struct{
	uint32 msCnt;
}Sys_Tick;

/*
** init sysTick Cfg
*/
void init_SysTick_Cfg(void);

/*
** Tickout
**	@param:
**		uint32*:计时
**		uint32:时间基准
**	@return:
**		bool:true-->超时 false-->未超时
*/
bool Tickout(uint32* tick,uint32 cnt);

#endif
