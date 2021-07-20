#include "includes.h"

uint8_t txbuf[256] = {0};//"\n\rUSART DMA receive and transmit example, please input 10 bytes:\n\r";

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
	
	/* configure DMA mode */
	dma_circulation_disable(DMA0, DMA_CH6);
	dma_memory_to_memory_disable(DMA0, DMA_CH6);
	/* enable/disable DMA0 channel6 transfer complete interrupt */
	dma_interrupt_enable(DMA0, DMA_CH6, DMA_INT_FTF);
	dma_interrupt_disable(DMA0,DMA_CH6,DMA_INT_HTF);
	dma_interrupt_disable(DMA0,DMA_CH6,DMA_INT_ERR);	

	/* USART DMA0 enable for transmission */
	usart_dma_transmit_config(USART1, USART_DENT_ENABLE);	

	/* enable DMA channel6 */
	//dma_channel_enable(DMA0, DMA_CH6);
}

/*
** usart1 interrput cfg
*/
void usart1_Interrput_Cfg(void){
	nvic_irq_enable(USART1_IRQn,0,0);
	nvic_irq_enable(DMA0_Channel6_IRQn,0,1);
}

/*
** usart1 Dma Send
*/
void usart1_DmaSend(uint8* data,uint16 len){
	memcpy((uint8*)&txbuf[0],(uint8*)&data[0],len);
	DMA_CHMADDR(DMA0,DMA_CH6) = (uint32)txbuf;
	DMA_CHCNT(DMA0, DMA_CH6) = (len & DMA_CHANNEL_CNT_MASK);
	usart_interrupt_flag_clear(USART1,USART_INT_FLAG_TC);
	dma_interrupt_enable(DMA0, DMA_CH6, DMA_INT_FTF);
	dma_interrupt_disable(DMA0,DMA_CH6,DMA_INT_HTF);
	dma_interrupt_disable(DMA0,DMA_CH6,DMA_INT_ERR);
	usart_dma_transmit_config(USART1, USART_DENT_ENABLE);
	dma_channel_enable(DMA0, DMA_CH6);		
}

/*
** DMA0 Channel6 IRQHandler
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
** USART1 IRQHandler
*/
void USART1_IRQHandler(void){
	volatile uint8 dat = 0;
	
	if(usart_flag_get(USART1,USART_FLAG_FERR) != RESET
		|| usart_flag_get(USART1,USART_FLAG_NERR) != RESET
		|| usart_flag_get(USART1,USART_FLAG_ORERR) != RESET){
		usart_data_receive(USART1);
	}
		
	if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_RBNE) != RESET){
		usart_interrupt_disable(USART1,USART_INT_RBNE);
		dat = usart_data_receive(USART1);
		fill_RecData(1,dat);
		usart_interrupt_flag_clear(USART1,USART_INT_FLAG_RBNE);
		usart_interrupt_enable(USART1,USART_INT_RBNE);
	}
	
	if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_TC) != RESET){
		usart_interrupt_disable(USART1,USART_INT_TC);
		usart_interrupt_enable(USART1,USART_INT_RBNE);
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
