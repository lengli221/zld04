#include "includes.h"

uint8 txbuf2[256] = {0};
Rec_Data recData2 = {0};

/*
** clear Recv Data2
*/
void clear_RecvData2(void){
	memset((uint8*)&recData2.buf[0],0,ARRAYNUM(recData2.buf));
	recData2.recFinsh = FALSE;
}

/*
** get Recv2 Is Finsh Flag
**	@return:
**		bool:true-->接收完成 false-->接收失败
*/
bool get_Recv2IsFinshFlag(void){
	return recData2.recFinsh;
}

/*
** get Recv2 Data Ptr
**	@return:
**		uint8*:数据项首地址
*/
uint8* get_Recv2DataPrt(void){
	return (uint8*)&recData2.buf[0];
}

/*
** init Usart2 Gpio Cfg
*/
void init_Usart2GpioCfg(void){
	/* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOB);	
	
	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART2);

	/* connect port to USARTx_Tx */
	gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);	
	
	/* connect port to USARTx_Rx */
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);		
}

/*
** init usart2 DMA
*/
void init_usart2_DMA(void){
	dma_parameter_struct dma_init_struct;

	/* enable DMA0 */
	rcu_periph_clock_enable(RCU_DMA0);
	
	/* deinitialize DMA channel1 (USART2 tx) */
	dma_deinit(DMA0, DMA_CH1);	
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
	dma_init_struct.memory_addr = (uint32_t)txbuf2;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.number = ARRAYNUM(txbuf2);
	dma_init_struct.periph_addr = USART2_DATA_ADDRESS;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(DMA0, DMA_CH1, &dma_init_struct);		
	
	/* deinitialize DMA channel2 (USART2 rx) */
	dma_deinit(DMA0, DMA_CH2);	
	dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
	dma_init_struct.memory_addr = (uint32_t)recData2.buf;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.number = ARRAYNUM(recData2.buf);
	dma_init_struct.periph_addr = USART2_DATA_ADDRESS;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(DMA0, DMA_CH2, &dma_init_struct);	
	
	/* configure DMA mode -->tx*/
	dma_circulation_enable(DMA0, DMA_CH1);
	dma_memory_to_memory_disable(DMA0, DMA_CH1);
	/* enable/disable DMA0 channel6 transfer complete interrupt */
	dma_interrupt_enable(DMA0, DMA_CH1, DMA_INT_FTF);
	dma_interrupt_disable(DMA0,DMA_CH1,DMA_INT_HTF);
	dma_interrupt_disable(DMA0,DMA_CH1,DMA_INT_ERR);		
	
	/* configure DMA mode -->rx*/
	dma_circulation_enable(DMA0, DMA_CH2);	
	dma_memory_to_memory_disable(DMA0, DMA_CH2);
	/* enable/disable DMA0 channel2 transfer complete interrupt */
	dma_interrupt_enable(DMA0, DMA_CH2, DMA_INT_FTF);
	dma_interrupt_disable(DMA0,DMA_CH2,DMA_INT_HTF);
	dma_interrupt_disable(DMA0,DMA_CH2,DMA_INT_ERR);			
}

/*
** usart2 interrput cfg
*/
void usart2_Interrupt_Cfg(void){
	nvic_irq_enable(USART2_IRQn,1,0);
	nvic_irq_enable(DMA0_Channel1_IRQn,1,1);//tx
	nvic_irq_enable(DMA0_Channel2_IRQn,1,2);//rx
}

/*
** usart2 Dma Send
*/
void usart2_DmaSend(uint8* data,uint16 txlen,uint16 rxlen){
	memcpy((uint8*)&txbuf2[0],(uint8*)&data[0],txlen);
	dma_transfer_number_config(DMA0,DMA_CH1,txlen);//tx
	/*备注:后三条语句非常重要--》解决回复丢数据,重发数据之后,重定位接受数据地址*/
	dma_channel_disable(DMA0, DMA_CH2);
	dma_memory_address_config(DMA0,DMA_CH2,(uint32)recData2.buf);//memory addr(rx)
	dma_transfer_number_config(DMA0,DMA_CH2,rxlen);//rx
	/*clear Recv Data2*/
	clear_RecvData2();
	usart_interrupt_flag_clear(USART2,USART_INT_FLAG_TC);
	dma_interrupt_enable(DMA0, DMA_CH1, DMA_INT_FTF);
	dma_interrupt_disable(DMA0,DMA_CH1,DMA_INT_HTF);
	dma_interrupt_disable(DMA0,DMA_CH1,DMA_INT_ERR);
	usart_dma_transmit_config(USART2, USART_DENT_ENABLE);
	dma_channel_enable(DMA0, DMA_CH1);		
}

/*
** DMA Channel1 IRQHandler -->tx
*/
void DMA0_Channel1_IRQHandler(void){
	dma_flag_clear(DMA0,DMA_CH1,DMA_FLAG_HTF);
	dma_flag_clear(DMA0,DMA_CH1,DMA_FLAG_ERR);
	if(dma_interrupt_flag_get(DMA0,DMA_CH1,DMA_INT_FLAG_FTF) != RESET){
		dma_channel_disable(DMA0,DMA_CH1);
		usart_interrupt_enable(USART2,USART_INT_TC);
		dma_interrupt_disable(DMA0, DMA_CH1, DMA_INT_FTF);
		dma_interrupt_flag_clear(DMA0,DMA_CH1,DMA_INT_FLAG_FTF);
	}	
}

/*
** DMA Channel2 IRQHandler-->rx
*/
void DMA0_Channel2_IRQHandler(void){
	dma_flag_clear(DMA0,DMA_CH2,DMA_FLAG_HTF);
	dma_flag_clear(DMA0,DMA_CH2,DMA_FLAG_ERR);
	if(dma_interrupt_flag_get(DMA0,DMA_CH2,DMA_INT_FLAG_FTF) != RESET){
		dma_interrupt_flag_clear(DMA0,DMA_CH2,DMA_INT_FLAG_FTF);
		/*
		** 备注:本条语句非常重要:限制数据超DMA限制字节数据,
		**	在下次开启DMA接受数据时,会存在之前一个字节数据进入DMA接受地址,
		**	导致内存地址偏移
		**	--》故而:在接受完当次数据之后,直接关闭usart和dma通道之间的联系
		***/
		usart_dma_receive_config(USART2, USART_DENR_DISABLE);
		dma_channel_disable(DMA0, DMA_CH2);
		usart_interrupt_enable(USART2,USART_INT_RBNE);
		/*提供标志给App层 -- 接收完成*/
		recData2.recFinsh = TRUE;
	}
}

/*
** USART2 IRQHandler
*/
void USART2_IRQHandler(void){
	
	if(usart_flag_get(USART2,USART_FLAG_FERR) != RESET
		|| usart_flag_get(USART2,USART_FLAG_NERR) != RESET
		|| usart_flag_get(USART2,USART_FLAG_ORERR) != RESET){
		usart_data_receive(USART2);
	}

	/*关于接受中断在此处目的:超过DMA接受限制字节部分,会出发接受中断,用于排除处理*/
	if(usart_interrupt_flag_get(USART2,USART_INT_FLAG_RBNE) != RESET){
		usart_interrupt_disable(USART2,USART_INT_RBNE);
		dma_channel_disable(DMA0, DMA_CH2);
		usart_interrupt_flag_clear(USART2,USART_INT_FLAG_RBNE);
	}
	
	if(usart_interrupt_flag_get(USART2,USART_INT_FLAG_TC) != RESET){
		usart_interrupt_disable(USART2,USART_INT_TC);
		dma_channel_enable(DMA0, DMA_CH2);
		usart_dma_receive_config(USART2, USART_DENR_ENABLE);
		usart_interrupt_flag_clear(USART2,USART_INT_FLAG_TC);
	}	
}

/*
** init Usart2 Cfg
*/
void init_Usart2Cfg(void){
	/*
	** init Usart1 Gpio Cfg
	*/
	init_Usart2GpioCfg();
	/*
	** init usart2 DMA
	*/
	init_usart2_DMA();	
}

