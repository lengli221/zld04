#ifndef __UPPERLAYER_DOORRESULT_H_
#define __UPPERLAYER_DOORRESULT_H_

#include "type.h"
#include "gd32f10x.h"

/*
** Open Door Result
*/
typedef struct{
	bool flag;/*true-->上报完成结果,FALSE-->等待*/
	uint8 result;/*结果:0->开仓成功 2-->仓门故障*/
}OpenDoorResult;

/*
** set OpenDoorResult 
**	@param:
**		uint8:设定开仓结果
*/
void set_OpenDoorResult(uint8 result);

/*
** parse Open Door Result
**	@param:
**		uint8:数据项长度
**		uint8*:数据项
*/
void parse_OpenDoorResult(uint8 len,uint8* item);

/*
** openDoorResult Proc
*/
void openDoorResult_Proc(void);

#endif

