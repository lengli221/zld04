#include "includes.h"

extern StateInfo stateInfo;

bool openDoorProcFlag = FALSE;

/*
** parse Open Door
**	@param:
**		uint8:�������
**		uint8*:������
*/
void parse_OpenDoor(uint8 len,uint8* item){
	uint8 tx[8] = {0};
	uint8 index = 0;
	
	/*֡���*/
	item[index] = 1;
	index += sizeof(uint8);
	/*���ݳ���*/
	item[index] = 1;
	index += sizeof(uint8);
	/*������--0x01-->׼������ 0x02-->���ֽ�����*/
	item[index] = openDoorProcFlag == TRUE?0x02:0x01;
	
	/*�����־λ*/
	if(openDoorProcFlag == FALSE){
		openDoorProcFlag = TRUE;
	}
	
	can_TransitData(1,index,(uint8*)&tx[0],0x03);
}

/*
** open Door Proc
*/
void openDoor_Proc(void){
	static uint8 step = 0;
	static uint32 itick = 0;
	static uint32 itick1 = 0; 
	static uint8 repCnt = 0;
	static int8 doorStateCnt = 0;
	static uint32 itick2 = 0;
	
	/*���ֽ��̴���*/
	if(openDoorProcFlag == TRUE){
		switch(step){
			case 0:
				hd_CtrDoor(TRUE);
				Tickout((uint32*)&itick,0);
				Tickout((uint32*)&itick1,0);
				step++;
				break;
			case 1:
				if(Tickout((uint32*)&itick,200) == TRUE){
					hd_CtrDoor(FALSE);
					if(xy_CtrDoor() == TRUE){
						stateInfo.sub.doorState = 0x01;/*���ſ�*/
						/*���־*/
						openDoorProcFlag = FALSE;
						repCnt = 0;
						step = 0;
					}else{
						if(Tickout((uint32*)&itick1,400) == TRUE){
							if(++repCnt>= 3){
								repCnt = 0;
								stateInfo.sub.doorState = 0x02;/*���Ź���*/
								/*���־*/
								openDoorProcFlag = FALSE;
							}
							step = 0;
						}
					}
				}
				break;
		}
	}
	
	/*����״̬����*/
	if(Tickout((uint32*)&itick2,20) == TRUE){
		Tickout((uint32*)&itick2,0);
		if(stateInfo.sub.doorState == 0x02){/*���Ź��ϲ��ٸ���״̬��Ϣ*/
			if(xy_CtrDoor() == TRUE){/*���ſ�����*/
				if(++doorStateCnt >= 5){
					doorStateCnt = 5;
					stateInfo.sub.doorState = 0x01;/*���ſ�*/
				}
			}else{/*���Źؼ���*/
				if(--doorStateCnt <= -5){
					doorStateCnt = -5;
					stateInfo.sub.doorState = 0x00;/*���Ź�*/
				}
			}
		}
	}
}
