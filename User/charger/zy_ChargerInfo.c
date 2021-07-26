#include "includes.h"

ChargerInfo chargerInfo = {0};

/*
** set Charger Vol Cur Limit
**	@param:
**		uint16:电压--倍率--0.1
**		uint16:电流--倍率--0.01
*/
void set_ChargerVolCurLimit(uint16 vol,int16 cur){
	chargerInfo.set.vol = vol;
	chargerInfo.set.cur = cur;
}

/*
** set Charger Vol Cur
** 	@return:
**		bool:true-->用于标记打包数据
*/
bool set_ChargerVolCur(void){
	uint8 i = 0;
	uint16 temp = 0;
	ModbusProFrame* chargerFrame = get_ChargerProFrame();
	
	/*寄存器地址*/
	chargerFrame->addr = Charger_Device_Addr;
	/*功能吗*/
	chargerFrame->funId = 0x10;
	/*寄存器地址*/
	chargerFrame->regAddr = 200;
	/*寄存器数量*/
	chargerFrame->regNum = 4;
	/*数据项长度*/
	chargerFrame->dataLen = 8;
	/*copy data*/
	for(i=0;i<chargerFrame->regNum;i++){
		temp = Common_Bytes_BigLittleChange((uint8*)(((uint16*)&chargerInfo.set.vol)+i));
		memcpy((uint8*)&chargerFrame->data[2*i],(uint8*)&temp,sizeof(uint16));
	}
	
	return TRUE;
}

/*
** set Charger On Off Ctr
**	@param:
**		bool:true-->开机 false-->关机
*/
void set_ChargerOnOff_Ctr(bool flag){
	chargerInfo.set.onoff = flag == TRUE?0x0001:0x0000;
}

/*
** set Charger On Off
** 	@return:
**		bool:true-->用于标记打包数据
*/
bool set_ChargerOnOff(void){
	ModbusProFrame* chargerFrame = get_ChargerProFrame();
	uint16 temp = 0;
	
	/*寄存器地址*/
	chargerFrame->addr = Charger_Device_Addr;
	/*功能吗*/
	chargerFrame->funId = 0x10;
	/*寄存器地址*/
	chargerFrame->regAddr = 204;
	/*寄存器数量*/
	chargerFrame->regNum = 1;
	/*数据项长度*/
	chargerFrame->dataLen = 2;
	/*copy data*/
	temp = Common_Bytes_BigLittleChange((uint8*)&chargerInfo.set.onoff);
	memcpy((uint8*)&chargerFrame->data[0],(uint8*)&temp,sizeof(uint16));
	
	return TRUE;
}

/*
** charger Frame Fun
**	@param:
**		uint8*:数据项
**		uint16:数据长度
**	note:
**		用于防止框架协议指针为空
*/
void charger_FrameFun(uint8* data,uint16 len){
	
}

/*
** get Charger Item
** 	@return:
**		bool:true-->用于标记打包数据
*/
bool get_ChargerItem(void){
	ModbusProFrame* chargerFrame = get_ChargerProFrame();
	
	/*寄存器地址*/
	chargerFrame->addr = Charger_Device_Addr;
	/*功能吗*/
	chargerFrame->funId = 0x04;
	/*寄存器地址*/
	chargerFrame->regAddr = 400;
	/*寄存器数量*/
	chargerFrame->regNum = 10;
	
	return TRUE;
}

/*
** set Charger Item
**	@param:
**		uint8*:数据项
**		uint16:数据长度
*/
void set_ChargerItem(uint8* data,uint16 len){
	uint8 i = 0;
	uint16* dataTemp = (uint16*)&chargerInfo.get.chgCur;
	
	for(i=0;i<sizeof(ChargerGet)/sizeof(uint16);i++){
		*dataTemp = Common_Bytes_BigLittleChange((uint8*)(data+2*i));
	}
}

