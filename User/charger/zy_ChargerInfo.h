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
	uint16 vol;/*设置电压--0.1*/
	uint16 res0[2];/*保留*/
	int16 cur;/*设置电流--0.01*/
	uint16 onoff;/*设置开关机--0:关机 1:开机*/
}ChargerSet;

/*
** Charger Err
*/
typedef union{
	uint16 flag;
	struct{
		uint16 outputVolH:1;/*输出电压高*/
		uint16 outVolAvgH:1;/*输出电压平均值高*/
		uint16 batVolH:1;/*电池电压高*/
		uint16 batVolAvgH:1;/*电池电压平均值高*/
		uint16 batVolAvgL:1;/*电池电压平均值低*/
		uint16 batAT:1;/*电池反接*/
		uint16 batVolAb:1;/*电池电压异常*/
		uint16 outputOC:1;/*输出过流*/
		uint16 outputCurAvgH:1;/*输出电流平均值高*/
		uint16 chargerH:1;/*充电器温度过高*/
		uint16 res:6;
	}bits;
}ChargerErr;

/*
** Charger Get
*/
typedef struct{
	uint16 chgCur;/*充电电流*/
	uint16 batVol;/*电池电压*/
	uint16 outputVol;/*输出电压*/
	uint16 res0[2];/*保留字段*/
	ChargerErr err;/*故障信息*/
	uint16 res1[3];/*保留字段1*/
	uint16 state;/*状态信息*/
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
**		uint16:电压--倍率--0.1
**		uint16:电流--倍率--0.01
*/
void set_ChargerVolCurLimit(uint16 vol,int16 cur);

/*
** set Charger Vol Cur
** 	@return:
**		bool:true-->用于标记打包数据
*/
bool set_ChargerVolCur(void);

/*
** set Charger On Off Ctr
**	@param:
**		bool:true-->开机 false-->关机
*/
void set_ChargerOnOff_Ctr(bool flag);

/*
** set Charger On Off
** 	@return:
**		bool:true-->用于标记打包数据
*/
bool set_ChargerOnOff(void);

/*
** charger Frame Fun
**	@param:
**		uint8*:数据项
**		uint16:数据长度
**	note:
**		用于防止框架协议指针为空
*/
void charger_FrameFun(uint8* data,uint16 len);

/*
** get Charger Item
** 	@return:
**		bool:true-->用于标记打包数据
*/
bool get_ChargerItem(void);

/*
** set Charger Item
**	@param:
**		uint8*:数据项
**		uint16:数据长度
*/
void set_ChargerItem(uint8* data,uint16 len);

#endif
