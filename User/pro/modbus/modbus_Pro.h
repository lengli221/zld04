#ifndef __MODBUS_PRO_H_
#define __MODBUS_PRO_H_

typedef void(*ModBusParse_Handle)(uint8*,uint16 dataLen);

/*
** Bms Pro Frame
*/
typedef struct{
	uint8 addr;/*设备地址*/
	uint8 funId;/*功能码*/
	uint16 regAddr;/*寄存器地址*/
	uint16 regNum;/*寄存器数量*/
	uint16 dataLen;/*数据项长度*/
	uint8 data[256];
}ModbusProFrame;

/*
** modbus Pro Package
**	@param:
**		void*:数据帧框架
**		void*:数据项
**		uint16*:数据项长度
*/
void modbus_ProPackage(void* pPara,void* pBuf,uint16* len);

/*
** modbus Pro Package
**	@param:
**		void*:数据帧框架
**		void*:数据项
**		uint16:数据项长度
**		handle:回调函数
*/
bool modbus_ProParse(void* pPara,void* pBuf,uint16 len,ModBusParse_Handle handle);

#endif
