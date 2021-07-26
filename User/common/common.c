#include "includes.h"

/*
** 大端《--》小端--uint16
**	@param:
**		uint8*:字节码
**	@return:
**		uint16:数据项
*/
uint16 Common_Bytes_BigLittleChange(uint8* pData){
	uint16 tmp = 0;

	tmp = *(pData);
	tmp <<= 8;
	tmp |= (*(pData+1))&0xFF;

	return tmp;
}

