#include "includes.h"

/*
** packect State Info
*/
void packect_StateInfo(void);

StateInfo stateInfo = {0};

/*
** get Ctr Protocol
**	@return:
**		uint8:协议号
*/
uint8 get_CtrProtocol(void){
	return stateInfo.ctr.protocol;
}

/*
** parse State Info
**	@param:
**		uint8:数据项长度
**		uint8*:数据项
*/
void parse_StateInfo(uint8 len,uint8* item){
	uint8 index = 0;
	
	/*协议号*/
	if(item[index] >= 102){
		stateInfo.ctr.protocol = item[index];
		index += sizeof(uint8);
		/*软件版本号*/
		stateInfo.ctr.softVer = item[index];
		index += sizeof(uint8);
		/*运行状态*/
		stateInfo.ctr.err.flag = item[index];
		index += sizeof(uint8);		
		
		/*
		** packect State Info
		*/
		packect_StateInfo();			
	}
}	

/*
** packect State Info
*/
void packect_StateInfo(void){
	uint8 tx[8] = {0};
	
	/*copy data*/
	memcpy((uint8*)&tx[0],(uint8*)&stateInfo.sub.protocl,sizeof(SubState));
	
	can_TransitData(1,sizeof(SubState),(uint8*)&tx[0],0x01);
}
