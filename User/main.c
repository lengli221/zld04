#include "includes.h"

int main(void){
	bool flag = FALSE;
	uint32 itick = 0;
	uint8 tx[8] = {1,2,3,4,5,6,7,8};
	uint8 funId = 12;
	
	/*
	** bsp init
	*/
	bsp_init();	
	
	while(1){	
		if(Tickout((uint32*)&itick,1000) == TRUE){
			Tickout((uint32*)&itick,0);
			flag = flag == FALSE?TRUE:FALSE;
			flag == TRUE?Led_Ctr_Off:Led_Ctr_On;
			printf("hello zld");
			set_SendData(1,(uint8*)&tx[0],8);
			funId++;
			can_TransitData(0,8,(uint8*)&tx[0],funId);
		}
		
		/*
		** can Tx Data Task
		*/
		can_TxDataTask();
	}
}
