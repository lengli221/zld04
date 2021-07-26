#include "includes.h"

/*
** ModBus CRC16
**	@param:
**		const void*s:数据项
**		int:数据项长度
**	@return:
**		uint16:CRC16码
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
**		void*:数据帧框架
**		void*:数据项
**		uint16*:数据项长度
*/
void modbus_ProPackage(void* pPara,void* pBuf,uint16* len){
	ModbusProFrame* modbusFrame = (ModbusProFrame*)pPara;
	uint16 index = 0;
	uint8* pbuf = pBuf;
	uint16 temp = 0;
	
	/*设备地址*/
	pbuf[index] = modbusFrame->addr;
	index += sizeof(uint8);
	/*功能吗*/
	pbuf[index] = modbusFrame->funId;
	index += sizeof(uint8);
	/*寄存器地址*/
	temp = Common_Bytes_BigLittleChange((uint8*)&modbusFrame->regAddr);
	memcpy((uint8*)&pbuf[index],(uint8*)&temp,sizeof(uint16));
	index += sizeof(uint16);
	/*寄存器数量*/
	temp = Common_Bytes_BigLittleChange((uint8*)&modbusFrame->regNum);
	memcpy((uint8*)&pbuf[index],(uint8*)&temp,sizeof(uint16));
	index += sizeof(uint16);
	switch(modbusFrame->funId){
		case 0x10:/*设置参数之批量设置*/
			/*数据字节数量*/
			pbuf[index] = modbusFrame->dataLen;
			index += sizeof(uint8);
			/*copy data*/
			memcpy((uint8*)&pbuf[index],(uint8*)&modbusFrame->data[0],modbusFrame->dataLen);
			index += modbusFrame->dataLen;			
			break;
	}
	/*CRC校验码*/
	temp = modBusCRC16((uint8*)&pbuf[0],index);
	memcpy((uint8*)&pbuf[index],(uint8*)&temp,sizeof(uint16));
	index += sizeof(uint16);
	/*帧长度*/
	*len = index;
}

/*
** modbus Pro Package
**	@param:
**		void*:数据帧框架
**		void*:数据项
**		uint16:数据项长度
**		handle:回调函数
*/
bool modbus_ProParse(void* pPara,void* pBuf,uint16 len,ModBusParse_Handle handle){
	ModbusProFrame* modbusFrame = (ModbusProFrame*)pPara;
	uint8* pbuf = pBuf;
	uint16 index = 0;
	uint16 calc_CRC16 = 0;
	uint16 get_CRC16 = 0;
	uint16 regAddr = 0;
	uint16 regNum = 0;
	
	/*指针容错校验 + 接收数据项长度校验*/
	if(pPara == null || pbuf == null || len >= 6 || handle == null){
		return FALSE;
	}
	
	/*地址校验*/
	if(pbuf[index] != modbusFrame->addr){
		return FALSE;
	}
	index += sizeof(uint8);
	
	/*功能码校验*/
	if(pbuf[index] != modbusFrame->funId){
		/*预留错误码回应处理机制--使用回调函数逻辑*/
		if(pbuf[index] == (modbusFrame->funId | 0x80)){
			
		}
		return FALSE;
	}
	index += sizeof(uint8);
	
	switch(modbusFrame->funId){
		case 0x10:
			/*寄存器地址和寄存器数量校验*/
			regAddr = Common_Bytes_BigLittleChange((uint8*)&pbuf[index]);
			index += sizeof(uint16);
			regNum = Common_Bytes_BigLittleChange((uint8*)&pbuf[index]);
			index += sizeof(uint16);
			if(regAddr != modbusFrame->regAddr || regNum != modbusFrame->regNum){
				return FALSE;
			}
			break;
		default:
			/*校验数据项长度*/
			if((pbuf[index] + 5) != len){
				return FALSE;
			}
			index++;			
			break;
	}
	
	/*CRC16校验*/
	memcpy((uint8*)&get_CRC16,(uint8*)&pbuf[len-2],sizeof(uint16));
	calc_CRC16 = modBusCRC16((uint8*)&pbuf[0],len - 2);;
	if(get_CRC16 != calc_CRC16){
		return FALSE;
	}
	
	/*执行回调函数*/
	handle((uint8*)&pbuf[index],len - sizeof(uint8) - sizeof(uint8)- sizeof(uint8) - sizeof(uint16));
	
	return TRUE;
}
