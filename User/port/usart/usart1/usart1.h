#ifndef __USART1_H_
#define __USART1_H_

#include "type.h"
#include "gd32f10x.h"

#define USART1_DATA_ADDRESS    ((uint32_t)&USART_DATA(USART1))

/*
** clear Recv Data1
*/
void clear_RecvData1(void);

/*
** get Recv1 Is Finsh Flag
**	@return:
**		bool:true-->接收完成 false-->接收失败
*/
bool get_Recv1IsFinshFlag(void);

/*
** get Recv1 Data Ptr
**	@return:
**		uint8*:数据项首地址
*/
uint8* get_Recv1DataPrt(void);

/*
** usart1 interrput cfg
*/
void usart1_Interrput_Cfg(void);

/*
** usart1 Dma Send
*/
void usart1_DmaSend(uint8* data,uint16 txlen,uint16 rxlen);

/*
** init Usart1 Cfg
*/
void init_Usart1Cfg(void);
#endif
