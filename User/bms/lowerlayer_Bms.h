#ifndef __LOWERLAYER_H_
#define __LOWERLAYER_H_

#include "type.h"
#include "modbus_Pro.h"

/*
** Sub Chk Bat 
*/
typedef union{
	uint16 flag;
	struct{
		uint16 batOnline:1;/*��1�������,��0��ز�����*/
		uint16 batErr:1;/*��1��ع���,��0�������*/
		uint16 res:1;/*����*/
	}bits;
}SubChkBat;

/*
** get Bms Pro Frame
**	@return:
**		ModbusProFrame:bms ֡���Э��
*/
ModbusProFrame* get_BmsProFrame(void);

/*
** lowerlayer Bms Fun
*/
void lowerlayer_BmsFun(void);

#endif