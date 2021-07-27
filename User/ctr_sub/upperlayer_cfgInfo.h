#ifndef __UPPERLAYER_CFGINFO_H_
#define __UPPERLAYER_CFGINFO_H_

#include "type.h"

/*
** get SOC Limit
**	@return:
**		uint8:SOC�޶�ֵ
*/
uint8 get_SOCLimit(void);

#pragma pack(1)
/*
** Ctr Cfg
*/
typedef struct{
	uint8 chargerOT;/*���������*/
	uint8 batOT;/*��ع���*/
	uint8 socLimit;/*SOC������ֵ*/
	uint16 chgTimeLimit;/*���ʱ����ֵ*/
}CtrCfg;

/*
** Sub Cfg
*/
typedef union{
	uint8 flag;
	struct{
		uint8 chargerOTFail:1;/*���������ʧ��*/
		uint8 batOTFail:1;/*��ع�������ʧ��*/
		uint8 socLimitFail:1;/*SOC����ʧ��*/
		uint8 chgTimeLimitFail:1;/*���ʱ������ʧ��*/
		uint8 res:1;/*����*/
	}bits;
}SubCfg;

/*
** Cfg Info
*/
typedef struct{
	CtrCfg ctr;/*����������Ϣ*/
	SubCfg subCfg;/*�ֿ�������Ϣ*/
}CfgInfo;
#pragma pack()

/*
** parse Cfg Info
**	@param:
**		uint8:�������
**		uint8*:������
*/
void parse_CfgInfo(uint8 len,uint8* item);

#endif

