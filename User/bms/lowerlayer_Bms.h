#ifndef __LOWERLAYER_H_
#define __LOWERLAYER_H_

#include "type.h"
#include "modbus_Pro.h"
#include "gd32f10x.h"

/*
** Sub Chk Bat 
*/
typedef union{
	uint16 flag;
	struct{
		uint16 batOnline:1;/*置1电池在线,清0电池不在线*/
		uint16 batErr:1;/*置1电池故障,清0电池正常*/
		uint16 res:1;/*保留*/
	}bits;
}SubChkBat;

/*
** get Bms Pro Frame
**	@return:
**		ModbusProFrame:bms 帧框架协议
*/
ModbusProFrame* get_BmsProFrame(void);

/*
** get Bat Is Online
**	@return:
**		bool:true-->电池在线 false-->电池不在线
*/
bool get_BatIsOnline(void);

/*
** lowerlayer Bms Fun
*/
void lowerlayer_BmsFun(void);

#endif
