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
	uint8 flag;/*ָ���־*/
	uint8 cmd;/*����*/
	uint8 dev;/*Ԥ���豸��ַ*/
}ZY_ChargerCmd;

/*
** ZY Cmd
**	note:
**		���ڴ���ҵ�����̷����ģ�����ָ��
*/
typedef struct{
	ZY_ChargerCmd cmd[ZY_Charger_Cmd_MaxDepth];/*����*/
	uint8 depth;/*�������*/
}ZY_Cmd;

/*
** Sub Chk Charger
*/
typedef union{
	uint16 flag;
	struct{
		uint16 chargerOnline:1;/*��1���������,��0�����������*/
		uint16 res:15;/*����*/
	}bits;
}SubChkCharger;

/*
** get Charger Pro Frame
**	@return:
**		ModbusProFrame:bms ֡���Э��
*/
ModbusProFrame* get_ChargerProFrame(void);

/*
** zy Charger Fun
*/
void zy_ChargerFun(void);

#endif
