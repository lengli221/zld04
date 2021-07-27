#include "includes.h"

extern BmsInfo bmsInfo;

/*
** parse Bms Info
**	@param:
**		uint8:�������
**		uint8*:������
*/
void parse_BmsInfo(uint8 len,uint8* item){
	volatile uint8 label = 0;
	uint8 datalen = 0;
	uint8 index = 0;
	uint8 bmsTotalTen = 94;
	uint8 i = 2;
	
	/*֡���*/
	label = item[index];
	index += sizeof(uint8);
	/*���ݳ���*/
	datalen += item[index];
	index += sizeof(uint8);
	
	/*data item*/
	switch(item[index]){
		case 1:
			packect_BmsInfo(1,1,(const uint8*)&bmsTotalTen);
			break;
		case 2:
			/*���ID������ʼ*/
			for(i=2;i<10;i++){
				packect_BmsInfo(i,6,(uint8*)&bmsInfo.id.idLen);
			}
			/*���������ʼ*/
			packect_BmsInfo(10,6,(uint8*)&bmsInfo.item.reqChgCur);
			/*��о��ѹ��ʼ*/
			for(i=11;i<17;i++){
				packect_BmsInfo(i,6,(uint8*)&bmsInfo.item.batCoreVol[0]);
			}
			packect_BmsInfo(17,4,(uint8*)&bmsInfo.item.batCoreVol[18]);
			/*������Ϣ*/
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
**		uint8:֡���
**		uint8:֡���ݳ���
**		uint8*:������ָ��
*/
void packect_BmsInfo(uint8 label,uint8 datalen,const uint8* data){
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
	
	can_TransitData(1,index,(uint8*)&tx[0],0x80);
}
