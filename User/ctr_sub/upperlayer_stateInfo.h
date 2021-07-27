#ifndef __UPPERLAYER_STATEINFO_H_
#define __UPPERLAYER_STATEINFO_H_

#include "type.h"

#pragma pack(1)
/*
** Ctr Err
*/
typedef union{
	uint8 flag;
	struct{
		uint8 oneErr:1;/*һ������*/
		uint8 res:7;/*����*/
	}bits;
}CtrErr;

/*
** Ctr State
*/
typedef struct{
	uint8 protocol;/*Э���*/
	uint8 softVer;/*����汾��*/
	CtrErr err;/*����*/
}CtrState;

/*
** Sub Err
*/
typedef union{
	uint8 flag;
	struct{
		uint8 batIsOnline:1;/*����Ƿ�����*/
		uint8 batIsErr:1;/*����Ƿ����*/
		uint8 subSelfChkErr:1;/*�ֿ��Լ����*/
		uint8 chargerOnline:1;/*������Ƿ�����*/
		uint8 chargerErr:1;/*���������*/
		uint8 res:3;/*����*/
	}bits;
}SubErr;

/*
** Sub State
*/
typedef struct{
	uint8 protocl;/*Э���*/
	uint8 softVer;/*����汾��*/
	uint8 doorState;/*����״̬:0x00(���Ź�)/0x01(���ſ�)/0x02(���Ź���)*/
	uint8 doorLoc;/*�ֶ�λ:0x00(�ղ�)/0x01(�����)/0x02(����)/0x03(����)*/
	SubErr err;/*����״̬*/
	uint8 batErrLabel;/*��ع��ϱ�ʶ*/
	uint8 subErrLabel;/*�ֿع��ϱ�ʶ*/
	uint8 ChargerErrLabel;/*��������ϱ�ʶ*/
}SubState;

/*
** State Info
*/
typedef struct{
	CtrState ctr;/*���ư�*/
	SubState sub;/*�ֿ�*/
}StateInfo;
#pragma pack()

/*
** get Ctr Protocol
**	@return:
**		uint8:Э���
*/
uint8 get_CtrProtocol(void);

/*
** parse State Info
**	@param:
**		uint8:�������
**		uint8*:������
*/
void parse_StateInfo(uint8 len,uint8* item);

#endif

