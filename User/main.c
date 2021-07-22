#include "includes.h"

int main(void){
	bool flag = FALSE;
	uint32 itick = 0;
	uint8 tx[8] = {1,2,3,4,5,6,7,8};
	uint8 tx1[8] = {11,12,13,14,15,16,17,18};
	uint8 funId = 12;
	uint8 step = 0;
	
	/*
	** bsp init
	*/
	bsp_init();	
	
	while(1){	
		if(Tickout((uint32*)&itick,5000) == TRUE){
			Tickout((uint32*)&itick,0);
			flag = flag == FALSE?TRUE:FALSE;
			flag == TRUE?Led_Ctr_Off:Led_Ctr_On;
			//printf("hello zld");
			set_SendData(1,(uint8*)&tx[0],8,0);
			switch(step){
				case 0:
					set_SendData(2,(uint8*)&tx[0],8,8);
					step++;
					break;
				case 1:
					set_SendData(2,(uint8*)&tx1[0],8,10);
					step = 0;
					break;
			}
			
			funId++;
			can_TransitData(0,8,(uint8*)&tx[0],funId);
		}
		
		/*
		** can Tx Data Task
		*/
		can_TxDataTask();
	}
}
