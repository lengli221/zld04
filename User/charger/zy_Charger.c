#include "includes.h"

ModbusProFrame chargerProFrame = {0};
ZY_Cmd zyCmd = {0};
SubChkCharger subChkCharegr = {0};

/*
** get Charger Pro Frame
**	@return:
**		ModbusProFrame:bms 帧框架协议
*/
ModbusProFrame* get_ChargerProFrame(void){
	return &chargerProFrame;
}

/*
** chk Is Exit ZY Cmd
**	@return:
**		bool:true-->存在业务流程控制指令 False-->不存在业务流程控制指令
*/
bool chk_IsExitZYCmd(void){
	return zyCmd.depth != 0?TRUE:FALSE;
}

/*
** get ZY Cmd
**	@return:
**		ZY_ChargerCmd:ZY 指令
*/
ZY_ChargerCmd get_ZY_Cmd(void){
	uint8 i = 0;
	ZY_ChargerCmd cmd = {0};
	
	/*get cmd*/
	cmd = zyCmd.cmd[0];
	/*移动队列命令*/
	for(i=0;i<zyCmd.depth-1;i++){
		zyCmd.cmd[i] = zyCmd.cmd[i+1];
	}
	/*深度自减*/
	zyCmd.depth--;
	
	return cmd;
}

/*
** set ZY Cmd
**	@param:
**		uint8:指令标记
**		uint8:命令
**		uint8:设备地址
*/
void set_ZY_Cmd(uint8* cmd){
	uint8 depth = zyCmd.depth;
	
	/*流程控制命令达最大深度,移除最早指令*/
	if(zyCmd.depth == ZY_Charger_Cmd_MaxDepth){
		get_ZY_Cmd();
	}
	
	/*设置命令参数*/
	zyCmd.cmd[depth].flag = cmd[0];
	zyCmd.cmd[depth].cmd = cmd[1];
	zyCmd.cmd[depth].dev = cmd[2];
	
	/*深度自加*/
	zyCmd.depth++;
}

/*
** get Charger Is Online
**	@return:
**		bool:true-->充电器在线 false-->电池在线
*/
bool get_ChargerIsOnline(void){
	return subChkCharegr.bits.chargerOnline != 0?TRUE:FALSE;
}

/*
** proc Logic Ctr Charger
**	@param:
**		bool:进程是否开启
**		ModBusParse_Handle:回调函数
**	@return:
**		int8:进程控制
**			-1:进程控制中
**			0:进程正常结束
**			1:进程超时未正常响应
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
				/*清相关参数*/
				Tickout((uint32*)&itick,0);
				/*充电器是否在线计数*/
				chargerOnlineCnt++;
				step++;
				break;
			case 1:
				if(Tickout((uint32*)&itick,150) == FALSE){
					if(get_RecvIsFinshFlag(2) == TRUE){/*DMA接收完成标志*/
					/*解析数据*/
						if(modbus_ProParse(get_ChargerProFrame(),(uint8*)getRecvDataPtr(2),rxlen,handle) == TRUE){
							chargerOnlineCnt = 0;
							/*充电器在线*/
							subChkCharegr.bits.chargerOnline = 0x01;
							/*清rx DMA接收数据*/
							clear_RecData(2);
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
		if(chk_IsExitZYCmd() != 0){/*存在流程控制命令*/
			cmd = get_ZY_Cmd();
			switch(cmd.cmd){
				case 0x01:/*设置开关机*/
					procFlag = set_ChargerOnOff();
					handle = charger_FrameFun;
					rxlen = 8;/*备注:8=hander+FunId+RegAdd+RegNum+CRC*/
					break;
				case 0x02:/*设置更新电压电流*/
					procFlag = set_ChargerVolCur();
					handle = charger_FrameFun;
					rxlen = 8;/*备注:8=hander+FunId+RegAdd+RegNum+CRC*/
					break;
			}
		}else{/*空闲查询状态命令*/
			if(Tickout((uint32*)&itick,1000) == TRUE){
				Tickout((uint32*)&itick,0);
				procFlag = get_ChargerItem();
				handle = set_ChargerItem;
				rxlen = 0x1C + 5;/*备注:5=hander+FunId+DataLen+CRC*/
			}
		}
	}
	
	/*检测进程交互逻辑*/
	proc_LogicCtr_Charger(&procFlag,rxlen,handle);
}
