#include "includes.h"

const static UpperlayerPasreFun ulPasreFunTable[] = {
	{0x01,parse_StateInfo}
};
const static uint8 ulPasreFunTableNum = sizeof(ulPasreFunTable)/sizeof(UpperlayerPasreFun);

/*
** upperlayer Parse Fun
*/
void upperlayer_ParseFun(void){
	can_receive_message_struct msg = {0};
	uint8 funId = 0;
	uint8 i = 9;
	
	if(can_RecvData(0,(can_receive_message_struct*)&msg) == TRUE){
		funId = (uint8)((msg.rx_efid>>16)&0x000000FF);
		for(i=0;i<ulPasreFunTableNum;i++){
			if(funId == ulPasreFunTable[i].funId){
				ulPasreFunTable[i].handle(msg.rx_dlen,(uint8*)&msg.rx_data[0]);
				break;
			}
		}
	}
}

/*
** upperLayer Fun
*/
void upperLayer_Fun(void){
	static uint8 step = 0;
	
	switch(step){
		case 0:/*初始化参数*/
			
			step++;
			break;
		case 1:/*运行流程*/
			/*
			** upperlayer Parse Fun
			*/
			upperlayer_ParseFun();			
			break;
	}
}

