#include "includes.h"

extern ChargerInfo chargerInfo;

/*
** packect Charger Info
**	@param:
**		uint8:֡���
**		uint8:֡���ݳ���
**		uint8*:������ָ��
*/
void packect_ChargerInfo(uint8 label,uint8 datalen,const uint8* data);

/*
** parse Charger Info
**	@param:
**		uint8:�������
**		uint8*:������
*/
void parse_ChargerInfo(uint8 len,uint8* item){
	uint8 data[6] = {0};
	uint8 index = 0;
	
	/*���õ�ѹ,����,���ػ���Ϣ*/
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.set.vol,sizeof(uint16));
	index += sizeof(uint16);
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.set.cur,sizeof(uint16));
	index += sizeof(uint16);	
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.set.onoff,sizeof(uint16));
	index += sizeof(uint16);		
	packect_ChargerInfo(1,6,(uint8*)&data[0]);
	index = 0;
	memset((uint8*)&data[0],0,6);
	
	/*������,��ص�ѹ,�����ѹ*/
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.get.chgCur,sizeof(uint16));
	index += sizeof(uint16);	
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.get.batVol,sizeof(uint16));
	index += sizeof(uint16);	
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.get.outputVol,sizeof(uint16));
	index += sizeof(uint16);	
	packect_ChargerInfo(2,6,(uint8*)&data[0]);
	index = 0;
	memset((uint8*)&data[0],0,6);

	/*������Ϣ*/
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.get.err.flag,sizeof(uint16));
	index += sizeof(uint16);	
	memcpy((uint8*)&data[index],(uint8*)&chargerInfo.get.state,sizeof(uint16));
	index += sizeof(uint16);
	packect_ChargerInfo(3,4,(uint8*)&data[0]);
}

/*
** packect Charger Info
**	@param:
**		uint8:֡���
**		uint8:֡���ݳ���
**		uint8*:������ָ��
*/
void packect_ChargerInfo(uint8 label,uint8 datalen,const uint8* data){
	uint8 tx[8] = {0};
	uint8 index = 0;
	
	/*֡���*/
	tx[index] = label;
	index += sizeof(uint8);
	/*���ݳ���*/
	tx[index] = datalen;
	index += sizeof(uint8);
	
	/*copy data*/
	memcpy((uint8*)&tx[index],(uint8*)&data[0],datalen);
	index += datalen;
	
	can_TransitData(1,index,(uint8*)&tx[0],0x81);	
}
