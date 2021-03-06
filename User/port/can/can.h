#ifndef __CNA_H_
#define __CNA_H_

#include "gd32f10x_can.h"
#include "type.h"

#define CAN_Rx_MaxNum													(uint8)20
#define CAN_Tx_MaxNum													(uint8)100

typedef struct{
	can_receive_message_struct msg[CAN_Rx_MaxNum];
	uint16 rx_wptr;
	uint16 rx_rptr;
}CAN_Rx_MsgInfo;

/*
** CAN Tx 处理结构体
*/
typedef struct{
	can_trasnmit_message_struct msg[CAN_Tx_MaxNum];
	uint16 wCnt;
	uint16 rCnt;
}CAN_Tx_MsgInfo;

/*
** init can
*/
void init_Can(void);

/*
** can Transit Data
**	@param:
**		uint8:port
**		uint8:数据长度
**		uint8:数据项
**		uint8:功能项
*/
void can_TransitData(uint8 port,uint8 len, uint8* tx,uint8 funId);

/*
** can Tx Data Task
*/
void can_TxDataTask(void);

/*
** can Recv Data
**	@param:
**		uint8:port
**		can_receive_message_struct*:接收邮箱数据
**	@return:
**		bool:true-->有数据 false-->无数据
*/
bool can_RecvData(uint8 port,can_receive_message_struct* msg);

#endif

