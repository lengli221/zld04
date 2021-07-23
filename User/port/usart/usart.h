#ifndef __USART_H_
#define __USART_H_

#include "type.h"
#include "gd32f10x.h"

#define ARRAYNUM(arr_name)     (uint32_t)(sizeof(arr_name) / sizeof(*(arr_name)))

/*
** Receive Data struct
*/
typedef struct{
	uint8 buf[256];/*���ݻ���*/
	bool recFinsh;/*������ɱ�־*/
}Rec_Data;

/*
** clear RecData
**	@param:
**		uint8:port
*/
void clear_RecData(uint8 port);

/*
** get Recv Is Finsh Flag
**	@param:
**		uint8:port
**	@return:
**		bool:true-->������� false-->����ʧ��
*/
bool get_RecvIsFinshFlag(uint8 port);

/*
** get Recv Data Ptr
**	@param:
**		uint8:port
**	@return:
**		uint8*:�������׵�ַ
*/
uint8* getRecvDataPtr(uint8 port);

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
