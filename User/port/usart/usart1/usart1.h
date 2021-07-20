#ifndef __USART1_H_
#define __USART1_H_

#include "type.h"

#define USART1_DATA_ADDRESS    ((uint32_t)&USART_DATA(USART1))

/*
** usart1 interrput cfg
*/
void usart1_Interrput_Cfg(void);

/*
** usart1 Dma Send
*/
void usart1_DmaSend(uint8* data,uint16 len);

/*
** init Usart1 Cfg
*/
void init_Usart1Cfg(void);
#endif
