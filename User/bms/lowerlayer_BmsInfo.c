#include "includes.h"

BmsInfo bmsInfo = {0};

/*
** get SOC
**	@return:
**		uint16:SCO
*/
uint16 get_SOC(void){
	return bmsInfo.item.part1.soc;
}

/*
** get Bms Id
** 	@return:
**		bool:true-->用于标记打包数据
*/
bool get_BmsId(void){
	ModbusProFrame* bmsFrame = get_BmsProFrame();
	
	/*寄存器地址*/
	bmsFrame->addr = Bms_DeviceAddr;
	/*功能吗*/
	bmsFrame->funId = 3;
	/*寄存器地址*/
	bmsFrame->regAddr = 1000;
	/*寄存器数量*/
	bmsFrame->regNum = 14;
	
	return TRUE;
}

/*
** set Bms ID
**	@param:
**		uint8*:数据项
**		uint16:数据长度
*/
void set_BmsId(uint8* data,uint16 len){
	uint32 res = 0;
	uint32 endId = 0xEEEE;
	
	/*检测电池ID的兼容性*/
	memcpy((uint8*)&endId,(uint8*)&data[12*sizeof(uint16)],sizeof(uint32));
	/*处理BMS ID长度*/
	bmsInfo.id.idLen = endId == res?12:14;
	/*copy Data*/
	memcpy((uint8*)&bmsInfo.id.id[0],(uint8*)&data[0],bmsInfo.id.idLen*sizeof(uint16));
}

/*
** get Bms Item
** 	@return:
**		bool:true-->用于标记打包数据
*/
bool get_BmsItem(void){
	ModbusProFrame* bmsFrame = get_BmsProFrame();
	
	/*寄存器地址*/
	bmsFrame->addr = Bms_DeviceAddr;	
	/*功能吗*/
	bmsFrame->funId = 3;
	/*寄存器地址*/
	bmsFrame->regAddr = 0;
	/*寄存器数量*/
	bmsFrame->regNum = 52;
	
	return TRUE;
}

/*
** set Bms Item
**	@param:
**		uint8*:数据项
**		uint16:数据长度
*/
void set_BmsItem(uint8* data,uint16 len){
	uint8 i = 0;
	uint16* dataTemp =  (uint16*)&bmsInfo.item.part1.batVol;
	
	for(i=0;i<sizeof(BmsItemPart1)/sizeof(uint16);i++){
		*dataTemp = Common_Bytes_BigLittleChange((uint8*)(data+2*i));
	}
	
	/*增加预留项偏置(40):电芯21电压~电芯电压40*/
	dataTemp = (uint16*)&bmsInfo.item.part2.reqChgCur;
	for(i=0;i<sizeof(BmsItemPart2)/sizeof(uint16);i++){
		*dataTemp = Common_Bytes_BigLittleChange((uint8*)\
					(data+sizeof(BmsItemPart1)+40+2*i));
	}
}

/*
** get Bms Err
** 	@return:
**		bool:true-->用于标记打包数据
*/
bool get_BmsErr(void){
	ModbusProFrame* bmsFrame = get_BmsProFrame();

	/*寄存器地址*/
	bmsFrame->addr = Bms_DeviceAddr;	
	/*功能吗*/
	bmsFrame->funId = 1;
	/*寄存器地址*/
	bmsFrame->regAddr = 0;
	/*寄存器数量*/
	bmsFrame->regNum = 52;	
	
	return TRUE;
}

/*
** set Bms Err
**	@param:
**		uint8*:数据项
**		uint16:数据长度
*/
void set_BmsErr(uint8* data,uint16 len){
	memcpy((uint8*)&bmsInfo.err.errDetail.flag,(uint8*)&data[0],sizeof(BmsErr));
}
