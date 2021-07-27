#include "includes.h"

extern BmsInfo bmsInfo;

/*
** parse Bms Info
**	@param:
**		uint8:数据项长度
**		uint8*:数据项
*/
void parse_BmsInfo(uint8 len,uint8* item){
	volatile uint8 label = 0;
	uint8 datalen = 0;
	uint8 index = 0;
	uint8 bmsTotalTen = 94;
	uint8 i = 2;
	
	/*帧标号*/
	label = item[index];
	index += sizeof(uint8);
	/*数据长度*/
	datalen += item[index];
	index += sizeof(uint8);
	
	/*data item*/
	switch(item[index]){
		case 1:
			packect_BmsInfo(1,1,(const uint8*)&bmsTotalTen);
			break;
		case 2:
			/*电池ID长度起始*/
			for(i=2;i<10;i++){
				packect_BmsInfo(i,6,(uint8*)&bmsInfo.id.idLen);
			}
			/*请求电流起始*/
			packect_BmsInfo(10,6,(uint8*)&bmsInfo.item.reqChgCur);
			/*电芯电压起始*/
			for(i=11;i<17;i++){
				packect_BmsInfo(i,6,(uint8*)&bmsInfo.item.batCoreVol[0]);
			}
			packect_BmsInfo(17,4,(uint8*)&bmsInfo.item.batCoreVol[18]);
			/*故障信息*/
			packect_BmsInfo(18,2,(uint8*)&bmsInfo.err.errDetail.flag);
			packect_BmsInfo(19,5,(uint8*)&bmsInfo.err.err[0]);
			break;
		case 3:
			
			break;
	}
}

/*
** packect Bms Info
**	@param:
**		uint8:帧标号
**		uint8:帧数据长度
**		uint8*:数据项指针
*/
void packect_BmsInfo(uint8 label,uint8 datalen,const uint8* data){
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
	
	can_TransitData(1,index,(uint8*)&tx[0],0x80);
}
