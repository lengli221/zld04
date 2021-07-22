#ifndef __USART_H_
#define __USART_H_

#include "type.h"

#define ARRAYNUM(arr_name)     (uint32_t)(sizeof(arr_name) / sizeof(*(arr_name)))

/*
** receive Data struct
*/
typedef struct{
	uint8 buf[256];/*���ݿռ�*/
	uint16 len;/*���ݳ���*/
}RecData;

/*
** clear RecData1
**	@param:
**		uint8:port
*/
void clear_RecData1(uint8 port);

/*
** fill RecData
**	@param:
**		uint8:port
**		uint8:data
*/
void fill_RecData(uint8 port,uint8 data);

/*
** set SendData
**	@param:
**		uint8:port
**		uint8*:������
**		uint16:�������ݳ���
**		uint16:�������ݳ���
*/
void set_SendData(uint8 port,uint8* data,uint16 txlen,uint16 rxlen);

/*
** com bsp
** 	@param:
**		uint8:port
**		uint32:������
**		uint32:����λ����
**		uint32:ֹͣλ
**		uint32:����
*/
void com_bsp(uint8 port,uint32 bd,uint32 wlen,uint32 stblen,uint32 paritycfg);

/*
** init usart Cfg
*/
void init_Usart_Cfg(void);

#endif
