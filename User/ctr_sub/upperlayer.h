#ifndef __UPPERLOWER_H_
#define __UPPERLOWER_H_

#include "type.h"

/*
** upperlayer Parse Fun
*/
typedef struct{
	uint8 funId;/*功能吗*/
	void (*handle)(uint8,uint8*);/*功能函数 1.数据长度 2.数据项*/
}UpperlayerPasreFun;

/*
** upperLayer Fun
*/
void upperLayer_Fun(void);

#endif
