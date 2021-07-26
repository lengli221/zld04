#include "includes.h"

uint8_t txbuf[256] = {0};//"\n\rUSART DMA receive and transmit example, please input 10 bytes:\n\r";
Rec_Data recData1 = {0};

/*
** clear Recv Data1
*/
void clear_RecvData1(void){
	memset((uint8*)&recData1.buf[0],0,ARRAYNUM(recData1.buf));
	recData1.recFinsh = FALSE;
}

/*
** get Recv1 Is Finsh Flag
**	@return:
**		bool:true-->接收完成 false-->接收失败
*/
bool get_Recv1IsFinshFlag(void){
	return recData1.recFinsh;
}

/*
** get Recv1 Data Ptr
**	@return:
**		uint8*:数据项首地址
*/
uint8* get_Recv1DataPrt(void){
	return (uint8*)&recData1.buf[0];
}

/*
** init Usart1 Gpio Cfg
*/
void init_Usart1GpioCfg(void){
	/* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOA);

	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART1);	
	
	/* connect port to USARTx_Tx */
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

	/* connect port to USARTx_Rx */
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);	
}

/*
** init usart1 DMA
*/
void init_usart1_DMA(void){
	dma_parameter_struct dma_init_struct;
	
	/* enable DMA0 */
	rcu_periph_clock_enable(RCU_DMA0);	

	/* deinitialize DMA channel6(USART1 tx) */
	dma_deinit(DMA0, DMA_CH6);	
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
	dma_init_struct.memory_addr = (uint32_t)txbuf;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.number = ARRAYNUM(txbuf);
	dma_init_struct.periph_addr = USART1_DATA_ADDRESS;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(DMA0, DMA_CH6, &dma_init_struct);	
	
	/* deinitialize DMA channel5 (USART1 rx) */
	dma_deinit(DMA0, DMA_CH5);	
	dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
	dma_init_struct.memory_addr = (uint32_t)recData1.buf;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.number = ARRAYNUM(recData1.buf);
	dma_init_struct.periph_addr = USART1_DATA_ADDRESS;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(DMA0, DMA_CH5, &dma_init_struct);		
	
	/* configure DMA mode -->tx*/
	dma_circulation_disable(DMA0, DMA_CH6);
	dma_memory_to_memory_disable(DMA0, DMA_CH6);
	/* enable/disable DMA0 channel6 transfer complete interrupt */
	dma_interrupt_enable(DMA0, DMA_CH6, DMA_INT_FTF);
	dma_interrupt_disable(DMA0,DMA_CH6,DMA_INT_HTF);
	dma_interrupt_disable(DMA0,DMA_CH6,DMA_INT_ERR);	

	/* configure DMA mode -->rx*/
	dma_circulation_disable(DMA0, DMA_CH5);	
	dma_memory_to_memory_disable(DMA0, DMA_CH5);
	/* enable/disable DMA0 channel2 transfer complete interrupt */
	dma_interrupt_enable(DMA0, DMA_CH5, DMA_INT_FTF);
	dma_interrupt_disable(DMA0,DMA_CH5,DMA_INT_HTF);
	dma_interrupt_disable(DMA0,DMA_CH5,DMA_INT_ERR);	
}

/*
** usart1 interrput cfg
*/
void usart1_Interrput_Cfg(void){
	nvic_irq_enable(USART1_IRQn,0,0);
	nvic_irq_enable(DMA0_Channel6_IRQn,0,1);//tx
	nvic_irq_enable(DMA0_Channel5_IRQn,0,2);//rx
}

/*
** usart1 Dma Send
*/
void usart1_DmaSend(uint8* data,uint16 txlen,uint16 rxlen){
	memcpy((uint8*)&txbuf[0],(uint8*)&data[0],txlen);
	dma_transfer_number_config(DMA0,DMA_CH6,txlen);//tx
	dma_memory_address_config(DMA0,DMA_CH6,(uint32)txbuf);//memory addr(tx)
	/*备注:后三条语句非常重要--》解决回复丢数据,重发数据之后,重定位接受数据地址*/
	dma_channel_disable(DMA0, DMA_CH5);
	dma_memory_address_config(DMA0,DMA_CH5,(uint32)recData1.buf);//memory addr(rx)	
	dma_transfer_number_config(DMA0,DMA_CH5,rxlen);//rx
	/*clear Recv Data1*/
	clear_RecvData1();	
	usart_interrupt_flag_clear(USART1,USART_INT_FLAG_TC);
	dma_interrupt_enable(DMA0, DMA_CH6, DMA_INT_FTF);
	dma_interrupt_disable(DMA0,DMA_CH6,DMA_INT_HTF);
	dma_interrupt_disable(DMA0,DMA_CH6,DMA_INT_ERR);
	usart_dma_transmit_config(USART1, USART_DENT_ENABLE);
	dma_channel_enable(DMA0, DMA_CH6);		
}

/*
** DMA0 Channel6 IRQHandler -->tx
*/
void DMA0_Channel6_IRQHandler(void){
	dma_flag_clear(DMA0,DMA_CH6,DMA_FLAG_HTF);
	dma_flag_clear(DMA0,DMA_CH6,DMA_FLAG_ERR);
	if(dma_interrupt_flag_get(DMA0,DMA_CH6,DMA_INT_FLAG_FTF) != RESET){
		dma_channel_disable(DMA0,DMA_CH6);
		usart_interrupt_enable(USART1,USART_INT_TC);
		dma_interrupt_disable(DMA0, DMA_CH6, DMA_INT_FTF);
		dma_interrupt_flag_clear(DMA0,DMA_CH6,DMA_INT_FLAG_FTF);
	}
}

/*
** DMA Channel5 IRQHandler-->rx
*/
void DMA0_Channel5_IRQHandler(void){
	dma_flag_clear(DMA0,DMA_CH5,DMA_FLAG_HTF);
	dma_flag_clear(DMA0,DMA_CH5,DMA_FLAG_ERR);
	if(dma_interrupt_flag_get(DMA0,DMA_CH5,DMA_INT_FLAG_FTF) != RESET){
		dma_interrupt_flag_clear(DMA0,DMA_CH5,DMA_INT_FLAG_FTF);
		/*
		** 备注:本条语句非常重要:限制数据超DMA限制字节数据,
		**	在下次开启DMA接受数据时,会存在之前一个字节数据进入DMA接受地址,
		**	导致内存地址偏移
		**	--》故而:在接受完当次数据之后,直接关闭usart和dma通道之间的联系
		***/
		usart_dma_receive_config(USART1, USART_DENR_DISABLE);
		dma_channel_disable(DMA0, DMA_CH5);
		usart_interrupt_enable(USART1,USART_INT_RBNE);
		/*提供标志给App层 -- 接收完成*/
		recData1.recFinsh = TRUE;
	}
}

/*
** USART1 IRQHandler
*/
void USART1_IRQHandler(void){
	
	if(usart_flag_get(USART1,USART_FLAG_FERR) != RESET
		|| usart_flag_get(USART1,USART_FLAG_NERR) != RESET
		|| usart_flag_get(USART1,USART_FLAG_ORERR) != RESET){
		usart_data_receive(USART1);
	}
	
	/*关于接受中断在此处目的:超过DMA接受限制字节部分,会出发接受中断,用于排除处理*/	
	if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_RBNE) != RESET){
		usart_interrupt_disable(USART1,USART_INT_RBNE);
		dma_channel_disable(DMA0, DMA_CH5);
		usart_interrupt_flag_clear(USART1,USART_INT_FLAG_RBNE);
	}
	
	if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_TC) != RESET){
		usart_interrupt_disable(USART1,USART_INT_TC);
		dma_channel_enable(DMA0, DMA_CH5);
		usart_dma_receive_config(USART1, USART_DENR_ENABLE);
		usart_interrupt_flag_clear(USART1,USART_INT_FLAG_TC);
	}
}

/*
** init Usart1 Cfg
*/
void init_Usart1Cfg(void){
	/*
	** init Usart1 Gpio Cfg
	*/
	init_Usart1GpioCfg();	
	/*
	** init usart1 DMA
	*/
	init_usart1_DMA();	
}
