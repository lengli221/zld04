#include "includes.h"

/*
** ��ˡ�--��С��--uint16
**	@param:
**		uint8*:�ֽ���
**	@return:
**		uint16:������
*/
uint16 Common_Bytes_BigLittleChange(uint8* pData){
	uint16 tmp = 0;

	tmp = *(pData);
	tmp <<= 8;
	tmp |= (*(pData+1))&0xFF;

	return tmp;
}

