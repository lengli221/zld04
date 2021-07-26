#include "includes.h"

ModbusProFrame bmsProFrame = {0};
SubChkBat subChkBat = {0};


/*
** get Bat Is Online
**	@return:
**		bool:true-->电池在线 false-->电池不在线
*/
bool get_BatIsOnline(void){
	return subChkBat.bits.batOnline != 0?TRUE:FALSE;
}

/*
** get Bms Pro Frame
**	@return:
**		ModbusProFrame:bms 帧框架协议
*/
ModbusProFrame* get_BmsProFrame(void){
	return &bmsProFrame;
}

/*
** proc Logic Ctr
**	@param:
**		bool:进程是否开启
**		ModBusParse_Handle:回调函数
**	@return:
**		int8:进程控制
**			-1:进程控制中
**			0:进程正常结束
**			1:进程超时未正常响应
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
				/*清相关参数*/
				Tickout((uint32*)&itick,0);
				/*电池是否在线计数*/
				batOnlineCnt++;
				step++;
				break;
			case 1:
				if(Tickout((uint32*)&itick,550) == FALSE){
					if(get_RecvIsFinshFlag(1) == TRUE){/*DMA接收完成标志*/
						/*解析数据*/
						if(modbus_ProParse(get_BmsProFrame(),(uint8*)getRecvDataPtr(1),rxlen,handle) == TRUE){
							batOnlineCnt = 0;
							/*电池在线*/
							subChkBat.bits.batOnline = 0x01;
							/*清rx DMA接收数据*/
							clear_RecData(1);
							ret = 0;
						}else{
							ret = 1;/*解析异常*/
						}
					}
				}else{
					Tickout((uint32*)&itick,0);
					ret = 1;/*解析超时*/
				}
				break;
		}
		
	}
	/*检测状态信息变更*/
	if(ret != -1){
		*proFlag = FALSE;
		step = 0;
	}
	
	/*电池不在线*/
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
	
	/*检测电池不在线,更新step*/
	if(subChkBat.bits.batOnline == 0){
		if(step != 0 || step != 1){
			step = 0;
		}
	}
	
	/*电池检测流程控制*/
	switch(step){
		case 0:
			procFlag = get_BmsId();
			handle = set_BmsId;
			rxlen = 0x1C + 5;/*备注:5=hander+FunId+DataLen+CRC*/
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
	
	/*检测是否启用交互进程逻辑*/
	procResult = proc_LogicCtr(&procFlag,rxlen,handle);
}
