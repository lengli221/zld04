#include "includes.h"

/*
** init can
*/
void init_Can(void){
	/*
	** init Can0
	*/
	init_Can0();	
}

/*
** can Transit Data
**	@param:
**		uint8:port
**		uint8:数据长度
**		uint8:数据项
**		uint8:功能项
*/
void can_TransitData(uint8 port,uint8 len, uint8* tx,uint8 funId){
	switch(port){
		case 0:
			CAN0_TransmitData(len,tx,funId);
			break;
		case 1:
			
			break;
	}
}

/*
** can Tx Data Task
*/
void can_TxDataTask(void){
	/*
	** CAN0 Tx Data Task
	*/
	CAN0_TxDataTask();
}


/*
** can Recv Data
**	@param:
**		uint8:port
**		can_receive_message_struct*:接收邮箱数据
**	@return:
**		bool:true-->有数据 false-->无数据
*/
bool can_RecvData(uint8 port,can_receive_message_struct* msg){
		switch(port){
			case 0:
				return CAN0_RecvData(msg);
			case 1:
				return FALSE;
		}
		return FALSE;
}

