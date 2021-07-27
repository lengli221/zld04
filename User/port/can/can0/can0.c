#include "includes.h"

CAN_Rx_MsgInfo can0RxMsgInfo = {0};
CAN_Tx_MsgInfo can0TxMsgInfo = {0};

/*
** init can0 Gpio Cfg
*/
void init_Can0_Gpio_Cfg(void){
	/*enable CAN GPIO Clock*/
	rcu_periph_clock_enable(RCU_GPIOA);
	/* enable CAN clock */
	rcu_periph_clock_enable(RCU_CAN0);
	
	/* configure CAN0 GPIO */
	gpio_init(GPIOA,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ,GPIO_PIN_11);
	gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_12);
}

/*
** init Can0 Baudrate
*/
void init_Can0_Baudrate(void){
	can_parameter_struct can_parameter;

	can_struct_para_init(CAN_INIT_STRUCT,&can_parameter);
	
	/* initialize CAN register */
	can_deinit(CAN0);
	
	/* initialize CAN */
	can_parameter.time_triggered = DISABLE;
	can_parameter.auto_bus_off_recovery = ENABLE;
	can_parameter.auto_wake_up = DISABLE;
	can_parameter.no_auto_retrans = DISABLE;
	can_parameter.rec_fifo_overwrite = DISABLE;
	can_parameter.trans_fifo_order = DISABLE;
	can_parameter.working_mode = /*CAN_NORMAL_MODE*/CAN_LOOPBACK_MODE;
	/* configure baudrate to 125kbps */
	can_parameter.resync_jump_width = CAN_BT_SJW_1TQ;
	can_parameter.time_segment_1 = CAN_BT_BS1_5TQ;
	can_parameter.time_segment_2 = CAN_BT_BS2_3TQ;
	can_parameter.prescaler = 48;
	can_init(CAN0, &can_parameter);	
	
	can_init(CAN0,&can_parameter);
	
	/* enable CAN receive FIFO1 not empty interrupt  */
	can_interrupt_enable(CAN0, CAN_INT_RFNE1);
	/* bus-off interrupt enable */
	can_interrupt_enable(CAN0, CAN_INT_BO);
}

/*
** init Can0 Filter Cfg
*/
void init_Can0_Filter_Cfg(void){
	can_filter_parameter_struct can_filter;
	
	can_struct_para_init(CAN_FILTER_STRUCT, &can_filter);
	
	/* CAN0 filter number */
	can_filter.filter_number = 0;	
	
	can_filter.filter_mode = CAN_FILTERMODE_MASK;
	can_filter.filter_bits = CAN_FILTERBITS_32BIT;
	can_filter.filter_list_high = 0x0000;
	can_filter.filter_list_low = 0x0000;
	can_filter.filter_mask_high = 0x0000;
	can_filter.filter_mask_low = 0x0000;  
  can_filter.filter_fifo_number = CAN_FIFO1;
	can_filter.filter_enable=ENABLE;
	can_filter_init(&can_filter);	
}

/*
** can0 interrupt cfg
*/
void can0_Interrupt_Cfg(void){
	/* configure CAN0 NVIC */
	nvic_irq_enable(CAN0_RX1_IRQn,0,2);	
}

/*
** CAN0 RX1 IRQHandler
*/
void CAN0_RX1_IRQHandler(void){
	can_receive_message_struct receive_message = {0};
	uint16 i = can0RxMsgInfo.rx_wptr;
	
	if(can_interrupt_flag_get(CAN0,CAN_INT_FLAG_BOERR) != RESET){
		/*init Can0*/
		init_Can0();		
	}else{
		/*备注:读取数据时会释放邮箱*/
		can_message_receive(CAN0,CAN_FIFO1,&receive_message);
		can0RxMsgInfo.msg[i] = receive_message;
		can0RxMsgInfo.rx_wptr++;
		if(can0RxMsgInfo.rx_wptr == CAN_Rx_MaxNum){
			can0RxMsgInfo.rx_wptr = 0;
		}
	}
}

/*
** CAN0 Recv Data
**	@param:
**		can_receive_message_struct*:接收邮箱数据
**	@return:
**		bool:true-->有数据 false-->无数据
*/
bool CAN0_RecvData(can_receive_message_struct* msg){
	bool ret = FALSE;
	uint16 i = 0;
	
	if(can0RxMsgInfo.rx_rptr == can0RxMsgInfo.rx_wptr){
		ret = FALSE;
	}else{
		i = can0RxMsgInfo.rx_rptr;
		*msg = can0RxMsgInfo.msg[i];
		memset((uint8*)&can0RxMsgInfo.msg[0].rx_sfid,0,sizeof(can_receive_message_struct));
		can0RxMsgInfo.rx_rptr++;
		if(can0RxMsgInfo.rx_rptr == CAN_Rx_MaxNum){
			can0RxMsgInfo.rx_rptr = 0;
		}
		ret = TRUE;
	}
	return ret;
}

/*
** CAN0 Transmit Data
**	@param:
**		uint8:数据长度
**		uint8*:数据项
**		uint8:功能吗
*/
void CAN0_TransmitData(uint8 len, uint8* tx,uint8 funId){
	can_trasnmit_message_struct transmit_message = {0};
	
	/* initialize transmit message */
	transmit_message.tx_sfid = 0;
	transmit_message.tx_efid |= (uint32)((funId<<16));
	transmit_message.tx_ff = CAN_FF_EXTENDED;
	transmit_message.tx_ft = CAN_FT_DATA;
	transmit_message.tx_dlen = len;	
	/* copy data*/
	memcpy((uint8*)&transmit_message.tx_data[0],(uint8*)&tx[0],len);
	
	/* transmit a message */
	can0TxMsgInfo.msg[can0TxMsgInfo.wCnt++] = transmit_message;
	if(can0TxMsgInfo.wCnt == CAN_Tx_MaxNum){
		can0TxMsgInfo.wCnt = 0;
	}	
}

/*
** CAN0 Tx Data Task
*/
void CAN0_TxDataTask(void){
	can_trasnmit_message_struct TxMessage = {0};
	static uint32 itick = 0;
	uint8 circ = 0;
	uint8 i = 0;
	uint8 ret = CAN_NOMAILBOX;
	
	if(Tickout((uint32*)&itick,2) == TRUE){
		for(circ=0;circ<3;){
			if(can0TxMsgInfo.rCnt != can0TxMsgInfo.wCnt){
				Tickout((uint32*)&itick,0);
				i = can0TxMsgInfo.rCnt;
				TxMessage = can0TxMsgInfo.msg[i];
				ret = can_message_transmit(CAN0, (can_trasnmit_message_struct *)&TxMessage);
				if(ret != CAN_NOMAILBOX){
					can0TxMsgInfo.rCnt++;
					if(can0TxMsgInfo.rCnt == CAN_Tx_MaxNum){
						can0TxMsgInfo.rCnt = 0;
					}
					circ++;					
				}else{
					break;
				}
			}else{
				break;
			}
		}
	}
}

/*
** init Can0
*/
void init_Can0(void){
	/*
	** init can0 Gpio Cfg
	*/
	init_Can0_Gpio_Cfg();	
	/*
	** init Can0 Baudrate
	*/
	init_Can0_Baudrate();	
	/*
	** init Can0 Filter Cfg
	*/
	init_Can0_Filter_Cfg();	
}
