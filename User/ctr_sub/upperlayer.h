#ifndef __UPPERLOWER_H_
#define __UPPERLOWER_H_

#include "type.h"

/*
** upperlayer Parse Fun
*/
typedef struct{
	uint8 funId;/*������*/
	void (*handle)(uint8,uint8*);/*���ܺ��� 1.���ݳ��� 2.������*/
}UpperlayerPasreFun;

/*
** upperLayer Fun
*/
void upperLayer_Fun(void);

#endif
