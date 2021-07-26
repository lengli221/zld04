#ifndef __MODBUS_PRO_H_
#define __MODBUS_PRO_H_

typedef void(*ModBusParse_Handle)(uint8*,uint16 dataLen);

/*
** Bms Pro Frame
*/
typedef struct{
	uint8 addr;/*�豸��ַ*/
	uint8 funId;/*������*/
	uint16 regAddr;/*�Ĵ�����ַ*/
	uint16 regNum;/*�Ĵ�������*/
	uint16 dataLen;/*�������*/
	uint8 data[256];
}ModbusProFrame;

/*
** modbus Pro Package
**	@param:
**		void*:����֡���
**		void*:������
**		uint16*:�������
*/
void modbus_ProPackage(void* pPara,void* pBuf,uint16* len);

/*
** modbus Pro Package
**	@param:
**		void*:����֡���
**		void*:������
**		uint16:�������
**		handle:�ص�����
*/
bool modbus_ProParse(void* pPara,void* pBuf,uint16 len,ModBusParse_Handle handle);

#endif
