#ifndef __USART_H_
#define __USART_H_

#include "type.h"
#include "gd32f10x.h"

#define ARRAYNUM(arr_name)     (uint32_t)(sizeof(arr_name) / sizeof(*(arr_name)))

/*
** Receive Data struct
*/
typedef struct{
	uint8 buf[256];/*数据缓存*/
	bool recFinsh;/*接收完成标志*/
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
**		bool:true-->接收完成 false-->接收失败
*/
bool get_RecvIsFinshFlag(uint8 port);

/*
** get Recv Data Ptr
**	@param:
**		uint8:port
**	@return:
**		uint8*:数据项首地址
*/
uint8* getRecvDataPtr(uint8 port);

/*
** set SendData
**	@param:
**		uint8:port
**		uint8*:数据项
**		uint16:发送数据长度
**		uint16:接受数据长度
*/
void set_SendData(uint8 port,uint8* data,uint16 txlen,uint16 rxlen);

/*
** com bsp
** 	@param:
**		uint8:port
**		uint32:波特率
**		uint32:数据位长度
**		uint32:停止位
**		uint32:极性
*/
void com_bsp(uint8 port,uint32 bd,uint32 wlen,uint32 stblen,uint32 paritycfg);

/*
** init usart Cfg
*/
void init_Usart_Cfg(void);

#endif
