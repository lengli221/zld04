#include "includes.h"

extern ChargerInfo chargerInfo;

/*
** packect Charger Info
**	@param:
**		uint8:帧标号
**		uint8:帧数据长度
**		uint8*:数据项指针
*/
void packect_ChargerInfo(uint8 label,uint8 datalen,const uint8* data);

/*
** parse Charger Info
**	@param:
**		uint8:数据项长度
**		uint8*:数据项
*/
void parse_ChargerInfo(uint8 len,uint8* item){
	uint8 data[6] = {0};
	uint8 index = 0;
	
	/*设置电压,电流,开关机信息*/
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.set.vol,sizeof(uint16));
	index += sizeof(uint16);
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.set.cur,sizeof(uint16));
	index += sizeof(uint16);	
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.set.onoff,sizeof(uint16));
	index += sizeof(uint16);		
	packect_ChargerInfo(1,6,(uint8*)&data[0]);
	index = 0;
	memset((uint8*)&data[0],0,6);
	
	/*充电电流,电池电压,输出电压*/
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.get.chgCur,sizeof(uint16));
	index += sizeof(uint16);	
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.get.batVol,sizeof(uint16));
	index += sizeof(uint16);	
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.get.outputVol,sizeof(uint16));
	index += sizeof(uint16);	
	packect_ChargerInfo(2,6,(uint8*)&data[0]);
	index = 0;
	memset((uint8*)&data[0],0,6);

	/*故障信息*/
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.get.err.flag,sizeof(uint16));
	index += sizeof(uint16);	
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.get.state,sizeof(uint16));
	index += sizeof(uint16);
	packect_ChargerInfo(3,4,(uint8*)&data[0]);
}

/*
** packect Charger Info
**	@param:
**		uint8:帧标号
**		uint8:帧数据长度
**		uint8*:数据项指针
*/
void packect_ChargerInfo(uint8 label,uint8 datalen,const uint8* data){
	uint8 tx[8] = {0};
	uint8 index = 0;
	
	/*帧标号*/
	tx[index] = label;
	index += sizeof(uint8);
	/*数据长度*/
	tx[index] = datalen;
	index += sizeof(uint8);
	
	/*copy data*/
	memcpy((uint8*)&tx[index],(uint8*)&data[0],datalen);
	index += datalen;
	
	can_TransitData(1,index,(uint8*)&tx[0],0x81);	
}
