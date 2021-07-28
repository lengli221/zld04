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
	uint8 bmsTotalTen = 101;
	uint8 i = 0;
	uint8 frameNum = 0;
	uint8 frameDataItemLen = 0;
	
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
			frameNum = (uint8)(sizeof(BmsInfo)/6);
			frameNum += sizeof(BmsInfo)%6 != 0?1:0;
			for(i=0;i<frameNum;i++){
				frameDataItemLen = i != (frameNum -1)?6:sizeof(BmsInfo)%6;
				packect_BmsInfo(i+2,frameDataItemLen,(uint8*)((uint8*)&bmsInfo.id.idLen + 6*1));
			}
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
