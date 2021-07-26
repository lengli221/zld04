#ifndef __COMMON_H_
#define __COMMON_H_

#include "includes.h"

/*
** 大端《--》小端--uint16
**	@param:
**		uint8*:字节码
**	@return:
**		uint16:数据项
*/
uint16 Common_Bytes_BigLittleChange(uint8* pData);

#endif
