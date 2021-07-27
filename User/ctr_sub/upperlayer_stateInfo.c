#include "includes.h"

/*
** packect State Info
*/
void packect_StateInfo(void);

StateInfo stateInfo = {0};

/*
** get Ctr Protocol
**	@return:
**		uint8:Э���
*/
uint8 get_CtrProtocol(void){
	return stateInfo.ctr.protocol;
}

/*
** parse State Info
**	@param:
**		uint8:�������
**		uint8*:������
*/
void parse_StateInfo(uint8 len,uint8* item){
	uint8 index = 0;
	
	/*Э���*/
	if(item[index] >= 102){
		stateInfo.ctr.protocol = item[index];
		index += sizeof(uint8);
		/*����汾��*/
		stateInfo.ctr.softVer = item[index];
		index += sizeof(uint8);
		/*����״̬*/
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
	
	/*��������:�ֶ�λ,����״̬*/
	if(get_SOC() >= get_SOCLimit()){
		stateInfo.sub.doorLoc = 0x02;/*����*/
	}else{
		if(get_BatIsOnline() == TRUE){/*�������,Ĭ��--�������*/
			stateInfo.sub.doorLoc = 0x01;/*�����*/
			stateInfo.sub.err.bits.batIsOnline = 0x01;/*��λ�������*/
		}else{
			stateInfo.sub.doorLoc = 0x00;/*����*/
			stateInfo.sub.err.bits.batIsOnline = 0x00;/*����������*/
		}
	}
	
	/*copy data*/
	memcpy((uint8*)&tx[0],(uint8*)&stateInfo.sub.protocl,sizeof(SubState));
	
	can_TransitData(1,sizeof(SubState),(uint8*)&tx[0],0x01);
}
