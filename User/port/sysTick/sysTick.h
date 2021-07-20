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
**		uint32*:��ʱ
**		uint32:ʱ���׼
**	@return:
**		bool:true-->��ʱ false-->δ��ʱ
*/
bool Tickout(uint32* tick,uint32 cnt);

#endif
