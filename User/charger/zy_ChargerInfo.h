#ifndef __ZY_CHARGERINFO_H_
#define __ZY_CHARGERINFO_H_

#include "type.h"

/*
** Charger Device Addr
*/
#define Charger_Device_Addr				(uint8)0x01

#pragma pack(1)
/*
** Charger Set
*/
typedef struct{
	uint16 vol;/*���õ�ѹ--0.1*/
	uint16 res0[2];/*����*/
	int16 cur;/*���õ���--0.01*/
	uint16 onoff;/*���ÿ��ػ�--0:�ػ� 1:����*/
}ChargerSet;

/*
** Charger Err
*/
typedef union{
	uint16 flag;
	struct{
		uint16 outputVolH:1;/*�����ѹ��*/
		uint16 outVolAvgH:1;/*�����ѹƽ��ֵ��*/
		uint16 batVolH:1;/*��ص�ѹ��*/
		uint16 batVolAvgH:1;/*��ص�ѹƽ��ֵ��*/
		uint16 batVolAvgL:1;/*��ص�ѹƽ��ֵ��*/
		uint16 batAT:1;/*��ط���*/
		uint16 batVolAb:1;/*��ص�ѹ�쳣*/
		uint16 outputOC:1;/*�������*/
		uint16 outputCurAvgH:1;/*�������ƽ��ֵ��*/
		uint16 chargerH:1;/*������¶ȹ���*/
		uint16 res:6;
	}bits;
}ChargerErr;

/*
** Charger Get
*/
typedef struct{
	uint16 chgCur;/*������*/
	uint16 batVol;/*��ص�ѹ*/
	uint16 outputVol;/*�����ѹ*/
	uint16 res0[2];/*�����ֶ�*/
	ChargerErr err;/*������Ϣ*/
	uint16 res1[3];/*�����ֶ�1*/
	uint16 state;/*״̬��Ϣ*/
}ChargerGet;

/*
** Charger Info
*/
typedef struct{
	ChargerSet set;
	ChargerGet get;
}ChargerInfo;
#pragma pack()

/*
** set Charger Vol Cur Limit
**	@param:
**		uint16:��ѹ--����--0.1
**		uint16:����--����--0.01
*/
void set_ChargerVolCurLimit(uint16 vol,int16 cur);

/*
** set Charger Vol Cur
** 	@return:
**		bool:true-->���ڱ�Ǵ������
*/
bool set_ChargerVolCur(void);

/*
** set Charger On Off Ctr
**	@param:
**		bool:true-->���� false-->�ػ�
*/
void set_ChargerOnOff_Ctr(bool flag);

/*
** set Charger On Off
** 	@return:
**		bool:true-->���ڱ�Ǵ������
*/
bool set_ChargerOnOff(void);

/*
** charger Frame Fun
**	@param:
**		uint8*:������
**		uint16:���ݳ���
**	note:
**		���ڷ�ֹ���Э��ָ��Ϊ��
*/
void charger_FrameFun(uint8* data,uint16 len);

/*
** get Charger Item
** 	@return:
**		bool:true-->���ڱ�Ǵ������
*/
bool get_ChargerItem(void);

/*
** set Charger Item
**	@param:
**		uint8*:������
**		uint16:���ݳ���
*/
void set_ChargerItem(uint8* data,uint16 len);

#endif
