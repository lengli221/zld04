#include "includes.h"

/*
** ModBus CRC16
**	@param:
**		const void*s:������
**		int:�������
**	@return:
**		uint16:CRC16��
*/
uint16 modBusCRC16(const void* s,int n){
	uint16 c = 0xFFFF, b = 0;
	int i = 0, k =0;
	
	for(k = 0;k < n; k++){
		b = (((uint8*)s)[k]);
		for(i = 0;i < 8; i++){
			c = ((b^c)&1)?(c>>1)^0xA001 : (c>>1);
			b >>= 1;
		}
	}
	return c;
}

/*
** modbus Pro Package
**	@param:
**		void*:����֡���
**		void*:������
**		uint16*:�������
*/
void modbus_ProPackage(void* pPara,void* pBuf,uint16* len){
	ModbusProFrame* modbusFrame = (ModbusProFrame*)pPara;
	uint16 index = 0;
	uint8* pbuf = pBuf;
	uint16 temp = 0;
	
	/*�豸��ַ*/
	pbuf[index] = modbusFrame->addr;
	index += sizeof(uint8);
	/*������*/
	pbuf[index] = modbusFrame->funId;
	index += sizeof(uint8);
	/*�Ĵ�����ַ*/
	temp = Common_Bytes_BigLittleChange((uint8*)&modbusFrame->regAddr);
	memcpy((uint8*)&pbuf[index],(uint8*)&temp,sizeof(uint16));
	index += sizeof(uint16);
	/*�Ĵ�������*/
	temp = Common_Bytes_BigLittleChange((uint8*)&modbusFrame->regNum);
	memcpy((uint8*)&pbuf[index],(uint8*)&temp,sizeof(uint16));
	index += sizeof(uint16);
	switch(modbusFrame->funId){
		case 0x10:/*���ò���֮��������*/
			/*�����ֽ�����*/
			pbuf[index] = modbusFrame->dataLen;
			index += sizeof(uint8);
			/*copy data*/
			memcpy((uint8*)&pbuf[index],(uint8*)&modbusFrame->data[0],modbusFrame->dataLen);
			index += modbusFrame->dataLen;			
			break;
	}
	/*CRCУ����*/
	temp = modBusCRC16((uint8*)&pbuf[0],index);
	memcpy((uint8*)&pbuf[index],(uint8*)&temp,sizeof(uint16));
	index += sizeof(uint16);
	/*֡����*/
	*len = index;
}

/*
** modbus Pro Package
**	@param:
**		void*:����֡���
**		void*:������
**		uint16:�������
**		handle:�ص�����
*/
bool modbus_ProParse(void* pPara,void* pBuf,uint16 len,ModBusParse_Handle handle){
	ModbusProFrame* modbusFrame = (ModbusProFrame*)pPara;
	uint8* pbuf = pBuf;
	uint16 index = 0;
	uint16 calc_CRC16 = 0;
	uint16 get_CRC16 = 0;
	uint16 regAddr = 0;
	uint16 regNum = 0;
	
	/*ָ���ݴ�У�� + �����������У��*/
	if(pPara == null || pbuf == null || len >= 6 || handle == null){
		return FALSE;
	}
	
	/*��ַУ��*/
	if(pbuf[index] != modbusFrame->addr){
		return FALSE;
	}
	index += sizeof(uint8);
	
	/*������У��*/
	if(pbuf[index] != modbusFrame->funId){
		/*Ԥ���������Ӧ�������--ʹ�ûص������߼�*/
		if(pbuf[index] == (modbusFrame->funId | 0x80)){
			
		}
		return FALSE;
	}
	index += sizeof(uint8);
	
	switch(modbusFrame->funId){
		case 0x10:
			/*�Ĵ�����ַ�ͼĴ�������У��*/
			regAddr = Common_Bytes_BigLittleChange((uint8*)&pbuf[index]);
			index += sizeof(uint16);
			regNum = Common_Bytes_BigLittleChange((uint8*)&pbuf[index]);
			index += sizeof(uint16);
			if(regAddr != modbusFrame->regAddr || regNum != modbusFrame->regNum){
				return FALSE;
			}
			break;
		default:
			/*У���������*/
			if((pbuf[index] + 5) != len){
				return FALSE;
			}
			index++;			
			break;
	}
	
	/*CRC16У��*/
	memcpy((uint8*)&get_CRC16,(uint8*)&pbuf[len-2],sizeof(uint16));
	calc_CRC16 = modBusCRC16((uint8*)&pbuf[0],len - 2);;
	if(get_CRC16 != calc_CRC16){
		return FALSE;
	}
	
	/*ִ�лص�����*/
	handle((uint8*)&pbuf[index],len - sizeof(uint8) - sizeof(uint8)- sizeof(uint8) - sizeof(uint16));
	
	return TRUE;
}
