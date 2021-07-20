#include "includes.h"

RecData recData1 = {0};

/*
** clear RecData1
**	@param:
**		uint8:port
*/
void clear_RecData1(uint8 port){
	switch(port){
		case 1:
			memset((uint8*)&recData1.buf[0],0,(uint16)ARRAYNUM(recData1.buf));
			break;
		case 2:
			
			break;
	}
}

/*
** fill RecData
**	@param:
**		uint8:port
**		uint8:data
*/
void fill_RecData(uint8 port,uint8 data){
	switch(port){
		case 1:
			recData1.buf[recData1.len++] = data;
			if((uint16)ARRAYNUM(recData1.buf) == recData1.len){
				memset((uint8*)&recData1.buf[0],0,(uint16)ARRAYNUM(recData1.buf));
			}
			break;
		case 2:
			
			break;
	}
}

/*
** get RecvData
**	@param:
**		uint8:port
**		uint8*:数据项
**		uint16:数据长度
*/
void get_RecvData(uint8 port,uint8* data,uint16* len){
	switch(port){
		case 1:
			memcpy((uint8*)&data[0],(uint8*)&recData1.buf[0],recData1.len);
			*len = recData1.len;
			break;
		case 2:
			
			break;
	}
}

/*
** set SendData
**	@param:
**		uint8:port
**		uint8*:数据项
**		uint16:数据长度
*/
void set_SendData(uint8 port,uint8* data,uint16 len){
		switch(port){
			case 1:
				usart1_DmaSend(data,len);
				break;
			case 2:
				
				break;
		}
}

/*
** com bsp
** 	@param:
**		uint8:port
**		uint32:波特率
**		uint32:数据位长度
**		uint32:停止位
**		uint32:极性
*/
void com_bsp(uint8 port,uint32 bd,uint32 wlen,uint32 stblen,uint32 paritycfg){
	__IO uint32 usrt = USART1;
	
	switch(port){
		case 0:
			
			break;
		case 1:
			usrt = USART1;
			break;
		case 2:
			usrt = USART2;
			break;
	}
	
	/* USART configure */
	usart_deinit(usrt);
	usart_baudrate_set(usrt, bd);
	usart_word_length_set(usrt, wlen);
	usart_stop_bit_set(usrt, stblen);
	usart_parity_config(usrt, paritycfg);
	usart_hardware_flow_rts_config(usrt, USART_RTS_DISABLE);
	usart_hardware_flow_cts_config(usrt, USART_CTS_DISABLE);
	usart_receive_config(usrt, USART_RECEIVE_ENABLE);
	usart_transmit_config(usrt, USART_TRANSMIT_ENABLE);
	usart_interrupt_enable(usrt,USART_INT_RBNE);
	usart_enable(usrt);	
	usart_dma_transmit_config(usrt, USART_DENT_ENABLE);
	//dma_channel_enable(DMA0, DMA_CH6);
}

/*
** init usart Cfg
*/
void init_Usart_Cfg(void){
	/*
	** init Usart1 Cfg
	*/
	init_Usart1Cfg();
	/*
	** com bsp
	*/
	com_bsp(1,115200,USART_WL_8BIT,USART_STB_1BIT,USART_PM_NONE);
	/*
	** init Usart2 Gpio Cfg
	*/
	init_Usart2GpioCfg();
	/*
	** com bsp
	*/
	com_bsp(2,115200,USART_WL_8BIT,USART_STB_1BIT,USART_PM_NONE);	
}
