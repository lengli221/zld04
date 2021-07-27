#ifndef __UPPERLAYER_OPENDOOR_H_
#define __UPPERLAYER_OPENDOOR_H_

#include "type.h"

/*
** parse Open Door
**	@param:
**		uint8:数据项长度
**		uint8*:数据项
*/
void parse_OpenDoor(uint8 len,uint8* item);

/*
** open Door Proc
*/
void openDoor_Proc(void);

#endif

