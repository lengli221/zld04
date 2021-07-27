#ifndef __UPPERLAYER_BMSINFO_H_
#define __UPPERLAYER_BMSINFO_H_

#include "type.h"

/*
** parse Bms Info
**	@param:
**		uint8:数据项长度
**		uint8*:数据项
*/
void parse_BmsInfo(uint8 len,uint8* item);

/*
** packect Bms Info
**	@param:
**		uint8:帧标号
**		uint8:帧数据长度
**		uint8*:数据项指针
*/
void packect_BmsInfo(uint8 label,uint8 datalen,const uint8* data);

#endif
