#ifndef __ZY_CHARGER_H_
#define __ZY_CHARGER_H_

#include "type.h"
#include "modbus_Pro.h"

/*
** ZY Charger Cmd Max Depth
*/
#define ZY_Charger_Cmd_MaxDepth											(uint8)20

/*
** ZY Charger Cmd
*/
typedef struct{
	uint8 flag;/*指令标志*/
	uint8 cmd;/*命令*/
	uint8 dev;/*预留设备地址*/
}ZY_ChargerCmd;

/*
** ZY Cmd
**	note:
**		用于处理业务流程发起的模块控制指令
*/
typedef struct{
	ZY_ChargerCmd cmd[ZY_Charger_Cmd_MaxDepth];/*命令*/
	uint8 depth;/*命令深度*/
}ZY_Cmd;

/*
** Sub Chk Charger
*/
typedef union{
	uint16 flag;
	struct{
		uint16 chargerOnline:1;/*置1充电器在线,清0充电器不在线*/
		uint16 res:15;/*保留*/
	}bits;
}SubChkCharger;

/*
** get Charger Pro Frame
**	@return:
**		ModbusProFrame:bms 帧框架协议
*/
ModbusProFrame* get_ChargerProFrame(void);

/*
** zy Charger Fun
*/
void zy_ChargerFun(void);

#endif
