#include "includes.h"

OpenDoorResult openDoorResult = {FALSE,0};

/*
** set OpenDoorResult 
**	@param:
**		uint8:设定开仓结果
*/
void set_OpenDoorResult(uint8 result){
	openDoorResult.flag = TRUE;
	openDoorResult.result = result;
}

/*
** parse Open Door Result
**	@param:
**		uint8:数据项长度
**		uint8*:数据项
*/
void parse_OpenDoorResult(uint8 len,uint8* item){
	openDoorResult.flag = FALSE;
}

/*
** openDoorResult Proc
*/
void openDoorResult_Proc(void){
	uint8 tx[8] = {0};
	uint8 index = 0;
	static uint8 step = 0;
	static uint32 itick = 0;
	static uint8 repCnt = 0;
	
	if(openDoorResult.flag == TRUE){
		switch(step){
			case 0:
				Tickout((uint32*)&itick,0);
				step++;
				break;
			case 1:/*定时目的:开仓之后,延时上报结果定时器,等待更新状态帧数据*/
				if(Tickout((uint32*)&itick,200) == TRUE){
					/*data item*/
					tx[index] = openDoorResult.result;
					index += sizeof(uint8);
					can_TransitData(1,index,(uint8*)&tx[0],0x04);	
					if(++repCnt >= 5){/*判定回复超时,不再上报*/
						repCnt = 0;
						openDoorResult.flag = FALSE;
					}
					step = 0;
				}					
				break;
		}
	}else{
		step = 0;
		repCnt = 0;
	}
}
