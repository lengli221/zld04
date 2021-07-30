#ifndef __UPPERLAYER_DOORRESULT_H_
#define __UPPERLAYER_DOORRESULT_H_

#include "type.h"
#include "gd32f10x.h"

/*
** Open Door Result
*/
typedef struct{
	bool flag;/*true-->�ϱ���ɽ��,FALSE-->�ȴ�*/
	uint8 result;/*���:0->���ֳɹ� 2-->���Ź���*/
}OpenDoorResult;

/*
** set OpenDoorResult 
**	@param:
**		uint8:�趨���ֽ��
*/
void set_OpenDoorResult(uint8 result);

/*
** parse Open Door Result
**	@param:
**		uint8:�������
**		uint8*:������
*/
void parse_OpenDoorResult(uint8 len,uint8* item);

/*
** openDoorResult Proc
*/
void openDoorResult_Proc(void);

#endif

