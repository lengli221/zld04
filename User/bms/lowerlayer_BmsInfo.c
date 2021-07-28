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
**		bool:true-->���ڱ�Ǵ������
*/
bool get_BmsId(void){
	ModbusProFrame* bmsFrame = get_BmsProFrame();
	
	/*�Ĵ�����ַ*/
	bmsFrame->addr = Bms_DeviceAddr;
	/*������*/
	bmsFrame->funId = 3;
	/*�Ĵ�����ַ*/
	bmsFrame->regAddr = 1000;
	/*�Ĵ�������*/
	bmsFrame->regNum = 14;
	
	return TRUE;
}

/*
** set Bms ID
**	@param:
**		uint8*:������
**		uint16:���ݳ���
*/
void set_BmsId(uint8* data,uint16 len){
	uint32 res = 0;
	uint32 endId = 0xEEEE;
	
	/*�����ID�ļ�����*/
	memcpy((uint8*)&endId,(uint8*)&data[12*sizeof(uint16)],sizeof(uint32));
	/*����BMS ID����*/
	bmsInfo.id.idLen = endId == res?12:14;
	/*copy Data*/
	memcpy((uint8*)&bmsInfo.id.id[0],(uint8*)&data[0],bmsInfo.id.idLen*sizeof(uint16));
}

/*
** get Bms Item
** 	@return:
**		bool:true-->���ڱ�Ǵ������
*/
bool get_BmsItem(void){
	ModbusProFrame* bmsFrame = get_BmsProFrame();
	
	/*�Ĵ�����ַ*/
	bmsFrame->addr = Bms_DeviceAddr;	
	/*������*/
	bmsFrame->funId = 3;
	/*�Ĵ�����ַ*/
	bmsFrame->regAddr = 0;
	/*�Ĵ�������*/
	bmsFrame->regNum = 52;
	
	return TRUE;
}

/*
** set Bms Item
**	@param:
**		uint8*:������
**		uint16:���ݳ���
*/
void set_BmsItem(uint8* data,uint16 len){
	uint8 i = 0;
	uint16* dataTemp =  (uint16*)&bmsInfo.item.part1.batVol;
	
	for(i=0;i<sizeof(BmsItemPart1)/sizeof(uint16);i++){
		*dataTemp = Common_Bytes_BigLittleChange((uint8*)(data+2*i));
	}
	
	/*����Ԥ����ƫ��(40):��о21��ѹ~��о��ѹ40*/
	dataTemp = (uint16*)&bmsInfo.item.part2.reqChgCur;
	for(i=0;i<sizeof(BmsItemPart2)/sizeof(uint16);i++){
		*dataTemp = Common_Bytes_BigLittleChange((uint8*)\
					(data+sizeof(BmsItemPart1)+40+2*i));
	}
}

/*
** get Bms Err
** 	@return:
**		bool:true-->���ڱ�Ǵ������
*/
bool get_BmsErr(void){
	ModbusProFrame* bmsFrame = get_BmsProFrame();

	/*�Ĵ�����ַ*/
	bmsFrame->addr = Bms_DeviceAddr;	
	/*������*/
	bmsFrame->funId = 1;
	/*�Ĵ�����ַ*/
	bmsFrame->regAddr = 0;
	/*�Ĵ�������*/
	bmsFrame->regNum = 52;	
	
	return TRUE;
}

/*
** set Bms Err
**	@param:
**		uint8*:������
**		uint16:���ݳ���
*/
void set_BmsErr(uint8* data,uint16 len){
	memcpy((uint8*)&bmsInfo.err.errDetail.flag,(uint8*)&data[0],sizeof(BmsErr));
}
