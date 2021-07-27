#ifndef __UPPERLAYER_BMSINFO_H_
#define __UPPERLAYER_BMSINFO_H_

#include "type.h"

/*
** parse Bms Info
**	@param:
**		uint8:�������
**		uint8*:������
*/
void parse_BmsInfo(uint8 len,uint8* item);

/*
** packect Bms Info
**	@param:
**		uint8:֡���
**		uint8:֡���ݳ���
**		uint8*:������ָ��
*/
void packect_BmsInfo(uint8 label,uint8 datalen,const uint8* data);

#endif
