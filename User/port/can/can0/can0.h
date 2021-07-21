#ifndef __CAN0_H_
#define __CAN0_H_

#include "type.h"
#include "gd32f10x_can.h"

/*
** can0 interrupt cfg
*/
void can0_Interrupt_Cfg(void);

/*
** CAN0 Transmit Data
**	@param:
**		uint8:数据长度
**		uint8*:数据项
**		uint8:功能吗
*/
void CAN0_TransmitData(uint8 len, uint8* tx,uint8 funId);

/*
** CAN0 Tx Data Task
*/
void CAN0_TxDataTask(void);

/*
** CAN0 Recv Data
**	@param:
**		can_receive_message_struct*:接收邮箱数据
**	@return:
**		bool:true-->有数据 false-->无数据
*/
bool CAN0_RecvData(can_receive_message_struct* msg);

/*
** init Can0
*/
void init_Can0(void);

#endif

