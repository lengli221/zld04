#ifndef __USART2_H_
#define __USART2_H_

#include "type.h"

#define USART2_DATA_ADDRESS    ((uint32_t)&USART_DATA(USART2))

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
