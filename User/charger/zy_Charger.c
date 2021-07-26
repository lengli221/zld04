#include "includes.h"

ModbusProFrame chargerProFrame = {0};
ZY_Cmd zyCmd = {0};
SubChkCharger subChkCharegr = {0};

/*
** get Charger Pro Frame
**	@return:
**		ModbusProFrame:bms ֡���Э��
*/
ModbusProFrame* get_ChargerProFrame(void){
	return &chargerProFrame;
}

/*
** chk Is Exit ZY Cmd
**	@return:
**		bool:true-->����ҵ�����̿���ָ�� False-->������ҵ�����̿���ָ��
*/
bool chk_IsExitZYCmd(void){
	return zyCmd.depth != 0?TRUE:FALSE;
}

/*
** get ZY Cmd
**	@return:
**		ZY_ChargerCmd:ZY ָ��
*/
ZY_ChargerCmd get_ZY_Cmd(void){
	uint8 i = 0;
	ZY_ChargerCmd cmd = {0};
	
	/*get cmd*/
	cmd = zyCmd.cmd[0];
	/*�ƶ���������*/
	for(i=0;i<zyCmd.depth-1;i++){
		zyCmd.cmd[i] = zyCmd.cmd[i+1];
	}
	/*����Լ�*/
	zyCmd.depth--;
	
	return cmd;
}

/*
** set ZY Cmd
**	@param:
**		uint8:ָ����
**		uint8:����
**		uint8:�豸��ַ
*/
void set_ZY_Cmd(uint8* cmd){
	uint8 depth = zyCmd.depth;
	
	/*���̿��������������,�Ƴ�����ָ��*/
	if(zyCmd.depth == ZY_Charger_Cmd_MaxDepth){
		get_ZY_Cmd();
	}
	
	/*�����������*/
	zyCmd.cmd[depth].flag = cmd[0];
	zyCmd.cmd[depth].cmd = cmd[1];
	zyCmd.cmd[depth].dev = cmd[2];
	
	/*����Լ�*/
	zyCmd.depth++;
}

/*
** get Charger Is Online
**	@return:
**		bool:true-->��������� false-->�������
*/
bool get_ChargerIsOnline(void){
	return subChkCharegr.bits.chargerOnline != 0?TRUE:FALSE;
}

/*
** proc Logic Ctr Charger
**	@param:
**		bool:�����Ƿ���
**		ModBusParse_Handle:�ص�����
**	@return:
**		int8:���̿���
**			-1:���̿�����
**			0:������������
**			1:���̳�ʱδ������Ӧ
*/
int8 proc_LogicCtr_Charger(bool* proFlag,uint16 rxlen,ModBusParse_Handle handle){
	int8 ret = -1;
	static uint8 step = 0;
	uint8 tx[256] = {0};
	uint16 txlen = 0;
	static uint32 itick = 0;
	static uint8 chargerOnlineCnt = 0;

	if(*proFlag == TRUE){
		switch(step){
			case 0:
				modbus_ProPackage(get_ChargerProFrame(),(uint8*)&tx[0],(uint16*)&txlen);
				set_SendData(2,(uint8*)&tx[0],txlen,rxlen);	
				/*����ز���*/
				Tickout((uint32*)&itick,0);
				/*������Ƿ����߼���*/
				chargerOnlineCnt++;
				step++;
				break;
			case 1:
				if(Tickout((uint32*)&itick,150) == FALSE){
					if(get_RecvIsFinshFlag(2) == TRUE){/*DMA������ɱ�־*/
					/*��������*/
						if(modbus_ProParse(get_ChargerProFrame(),(uint8*)getRecvDataPtr(2),rxlen,handle) == TRUE){
							chargerOnlineCnt = 0;
							/*���������*/
							subChkCharegr.bits.chargerOnline = 0x01;
							/*��rx DMA��������*/
							clear_RecData(2);
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
	
	if(chargerOnlineCnt >= 4){
		chargerOnlineCnt = 4;
		subChkCharegr.bits.chargerOnline = 0x00;
	}
	
	return ret;
}

/*
** zy Charger Fun
*/
void zy_ChargerFun(void){
	static bool procFlag = FALSE;
	static uint32 itick = 0;
	static ModBusParse_Handle handle = set_ChargerItem;
	static uint16 rxlen = 5;
	ZY_ChargerCmd cmd = {0};

	if(procFlag == FALSE){
		if(chk_IsExitZYCmd() != 0){/*�������̿�������*/
			cmd = get_ZY_Cmd();
			switch(cmd.cmd){
				case 0x01:/*���ÿ��ػ�*/
					procFlag = set_ChargerOnOff();
					handle = charger_FrameFun;
					rxlen = 8;/*��ע:8=hander+FunId+RegAdd+RegNum+CRC*/
					break;
				case 0x02:/*���ø��µ�ѹ����*/
					procFlag = set_ChargerVolCur();
					handle = charger_FrameFun;
					rxlen = 8;/*��ע:8=hander+FunId+RegAdd+RegNum+CRC*/
					break;
			}
		}else{/*���в�ѯ״̬����*/
			if(Tickout((uint32*)&itick,1000) == TRUE){
				Tickout((uint32*)&itick,0);
				procFlag = get_ChargerItem();
				handle = set_ChargerItem;
				rxlen = 0x1C + 5;/*��ע:5=hander+FunId+DataLen+CRC*/
			}
		}
	}
	
	/*�����̽����߼�*/
	proc_LogicCtr_Charger(&procFlag,rxlen,handle);
}
