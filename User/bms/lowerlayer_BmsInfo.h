#ifndef __LOWERLAYER_BMSINFO_H_
#define __LOWERLAYER_BMSINFO_H_

/*
** BMS Device Addr
*/
#define Bms_DeviceAddr									(uint8)0x01

#pragma pack(1)
/*
** Bms Id
*/
typedef struct{
	uint8 idLen;/*id����--�������ID�������ֵ��*/
	uint8 id[28];/*ID*/	
}BmsId;

/*
** Bms Err Detail
*/
typedef union{
	uint16 flag;
	struct{
		uint16 res0:1;/*����*/
		uint16 batCoreDiffPressP:1;/*��оѹ����󱣻�*/
		uint16 chgOCP:1;/*����������*/
		uint16 disOCP:1;/*�ŵ��������*/
		uint16 shortP:1;/*��·����*/
		uint16 chgHTP:1;/*�����±���*/
		uint16 disHTP:1;/*�ŵ���±���*/
		uint16 chgLTP:1;/*�����±���*/
		uint16 disLTP:1;/*�ŵ���±���*/
		uint16 chgMosE:1;/*���MOS��*/
		uint16 disMosE:1;/*�ŵ�Mos��*/
		uint16 innerComE:1;/*�ڲ�ͨѶ�쳣*/
		uint16 oChgVP:4;/*�����ѹ����1-4*/
	}bits;
}BmsErrDetail;

/*
** Bms Err
*/
typedef struct{
	BmsErrDetail errDetail;
	uint8 err[5];/*�����ѹ����5-20,���ŵ�ѹ����1-20*/ 
}BmsErr;

/*
** Bms Item
*/
typedef struct{
	uint16 batVol;/*��ذ�ʵ���ܵ�ѹ--0.01*/
	uint16 batCoreNum;/*��о����*/
	uint16 soc;/*soc*/
	uint16 surplueCap;/*ʣ������--0.01 AH*/
	uint16 soh;/*soh*/
	int16 chgCur;/*������--0.01*/
	uint16 envTemp;/*�����¶�*/
	uint16 batCoreTemp;/*��о�¶�*/
	uint16 mosTemp;/*MOS���¶�*/
	uint16 batCoreVol[20];/*��оx��ѹ--0.001*/
	uint16 res[20];/*Ԥ��*/
	uint16 reqChgCur;/*���������--0.01*/
	uint16 maxChgVol;/*��߳���ѹ--0.01*/
	uint16 chgIsPermit;/*�������/��ֹ--0xAA55:������ ���������������*/	
}BmsItem;

/*
** Bms Info
*/
typedef struct{
	BmsId id;/*���ID��Ϣ*/
	BmsItem item;/*bms��������Ϣ*/
	BmsErr err;/*��ع�����Ϣ*/
}BmsInfo;
#pragma pack()

/*
** get Bms Id
** 	@return:
**		bool:true-->���ڱ�Ǵ������
*/
bool get_BmsId(void);

/*
** set Bms ID
**	@param:
**		uint8*:������
**		uint16:���ݳ���
*/
void set_BmsId(uint8* data,uint16 len);

/*
** get Bms Item
** 	@return:
**		bool:true-->���ڱ�Ǵ������
*/
bool get_BmsItem(void);

/*
** set Bms Item
**	@param:
**		uint8*:������
**		uint16:���ݳ���
*/
void set_BmsItem(uint8* data,uint16 len);

/*
** get Bms Err
** 	@return:
**		bool:true-->���ڱ�Ǵ������
*/
bool get_BmsErr(void);

/*
** set Bms Err
**	@param:
**		uint8*:������
**		uint16:���ݳ���
*/
void set_BmsErr(uint8* data,uint16 len);

#endif

