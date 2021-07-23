#ifndef __USART2_H_
#define __USART2_H_

#include "type.h"
#include "gd32f10x.h"


#define USART2_DATA_ADDRESS    ((uint32_t)&USART_DATA(USART2))

/*
** clear Recv Data2
*/
void clear_RecvData2(void);

/*
** get Recv2 Is Finsh Flag
**	@return:
**		bool:true-->接收完成 false-->接收失败
*/
bool get_Recv2IsFinshFlag(void);

/*
** get Recv2 Data Ptr
**	@return:
**		uint8*:数据项首地址
*/
uint8* get_Recv2DataPrt(void);

/*
** usart2 interrput cfg
*/
void usart2_Interrupt_Cfg(void);

/*
** usart2 Dma Send
*/
void usart2_DmaSend(uint8* data,uint16 txlen,uint16 rxlen);

/*
** init Usart2 Cfg
*/
void init_Usart2Cfg(void);

#endif
