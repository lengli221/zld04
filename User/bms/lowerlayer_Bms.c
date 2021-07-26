#include "includes.h"

ModbusProFrame bmsProFrame = {0};
SubChkBat subChkBat = {0};


/*
** get Bat Is Online
**	@return:
**		bool:true-->������� false-->��ز�����
*/
bool get_BatIsOnline(void){
	return subChkBat.bits.batOnline != 0?TRUE:FALSE;
}

/*
** get Bms Pro Frame
**	@return:
**		ModbusProFrame:bms ֡���Э��
*/
ModbusProFrame* get_BmsProFrame(void){
	return &bmsProFrame;
}

/*
** proc Logic Ctr
**	@param:
**		bool:�����Ƿ���
**		ModBusParse_Handle:�ص�����
**	@return:
**		int8:���̿���
**			-1:���̿�����
**			0:������������
**			1:���̳�ʱδ������Ӧ
*/
int8 proc_LogicCtr(bool* proFlag,uint16 rxlen,ModBusParse_Handle handle){
	int8 ret = -1;
	static uint8 step = 0;
	uint8 tx[256] = {0};
	uint16 txlen = 0;
	static uint32 itick = 0;
	static uint8 batOnlineCnt = 0;
	
	if(*proFlag == TRUE){
		switch(step){
			case 0:
				modbus_ProPackage(get_BmsProFrame(),(uint8*)&tx[0],(uint16*)&txlen);
				set_SendData(1,(uint8*)&tx[0],txlen,rxlen);
				/*����ز���*/
				Tickout((uint32*)&itick,0);
				/*����Ƿ����߼���*/
				batOnlineCnt++;
				step++;
				break;
			case 1:
				if(Tickout((uint32*)&itick,550) == FALSE){
					if(get_RecvIsFinshFlag(1) == TRUE){/*DMA������ɱ�־*/
						/*��������*/
						if(modbus_ProParse(get_BmsProFrame(),(uint8*)getRecvDataPtr(1),rxlen,handle) == TRUE){
							batOnlineCnt = 0;
							/*�������*/
							subChkBat.bits.batOnline = 0x01;
							/*��rx DMA��������*/
							clear_RecData(1);
							ret = 0;
						}else{
							ret = 1;/*�����쳣*/
						}
					}
				}else{
					Tickout((uint32*)&itick,0);
					ret = 1;/*������ʱ*/
				}
				break;
		}
		
	}
	/*���״̬��Ϣ���*/
	if(ret != -1){
		*proFlag = FALSE;
		step = 0;
	}
	
	/*��ز�����*/
	if(batOnlineCnt >= 4){
		batOnlineCnt = 4;
		subChkBat.bits.batOnline = 0x00;
	}
	
	return ret;
}

/*
** lowerlayer Bms Fun
*/
void lowerlayer_BmsFun(void){
	static uint8 step = 0;
	static bool procFlag = FALSE;
	static int8 procResult = -1;
	static ModBusParse_Handle handle = set_BmsId;
	static uint16 rxlen = 5;
	
	/*����ز�����,����step*/
	if(subChkBat.bits.batOnline == 0){
		if(step != 0 || step != 1){
			step = 0;
		}
	}
	
	/*��ؼ�����̿���*/
	switch(step){
		case 0:
			procFlag = get_BmsId();
			handle = set_BmsId;
			rxlen = 0x1C + 5;/*��ע:5=hander+FunId+DataLen+CRC*/
			step++;
			break;
		case 1:
			if(procResult != -1){
				step = procResult == 0?2:0;
			}
			break;
		case 2:
			procFlag = get_BmsItem();
			handle = set_BmsItem;
			rxlen = 0x68 + 5;
			step++;			
			break;
		case 3:
			if(procResult != -1){
				step++;
			}
			break;
		case 4:
			procFlag = get_BmsErr();
			handle = set_BmsErr;
			rxlen = 0x07 + 5;
			step++;				
			break;
		case 5:
			if(procResult != -1){
				step = 2;
			}
			break;
		default:
			
			break;
	}
	
	/*����Ƿ����ý��������߼�*/
	procResult = proc_LogicCtr(&procFlag,rxlen,handle);
}
