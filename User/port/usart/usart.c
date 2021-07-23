#include "includes.h"

/*
** clear RecData
**	@param:
**		uint8:port
*/
void clear_RecData(uint8 port){
	switch(port){
		case 1:
			clear_RecvData1();
			break;
		case 2:
			clear_RecvData2();
			break;
	}
}

/*
** get Recv Is Finsh Flag
**	@param:
**		uint8:port
**	@return:
**		bool:true-->接收完成 false-->接收失败
*/
bool get_RecvIsFinshFlag(uint8 port){
	switch(port){
		case 1:
			return get_Recv1IsFinshFlag();
		case 2:
			return get_Recv2IsFinshFlag();
	}
	return FALSE;
}

/*
** get Recv Data Ptr
**	@param:
**		uint8:port
**	@return:
**		uint8*:数据项首地址
*/
uint8* getRecvDataPtr(uint8 port){
	switch(port){
		case 1:
			return get_Recv1DataPrt();
		case 2:
			return get_Recv2DataPrt();
	}
	return NULL;
}

/*
** set SendData
**	@param:
**		uint8:port
**		uint8*:数据项
**		uint16:发送数据长度
**		uint16:接受数据长度
*/
void set_SendData(uint8 port,uint8* data,uint16 txlen,uint16 rxlen){
		switch(port){
			case 1:
				usart1_DmaSend(data,txlen,rxlen);
				break;
			case 2:
				usart2_DmaSend(data,txlen,rxlen);
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
	usart_enable(usrt);	
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
	** init Usart2 Cfg
	*/
	init_Usart2Cfg();
	/*
	** com bsp
	*/
	com_bsp(2,115200,USART_WL_8BIT,USART_STB_1BIT,USART_PM_NONE);	
}
