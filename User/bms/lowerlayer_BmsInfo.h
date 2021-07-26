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
	uint8 idLen;/*id长度--铁塔电池ID存在两种电池*/
	uint8 id[28];/*ID*/	
}BmsId;

/*
** Bms Err Detail
*/
typedef union{
	uint16 flag;
	struct{
		uint16 res0:1;/*保留*/
		uint16 batCoreDiffPressP:1;/*电芯压差过大保护*/
		uint16 chgOCP:1;/*充电过流保护*/
		uint16 disOCP:1;/*放电过流保护*/
		uint16 shortP:1;/*短路保护*/
		uint16 chgHTP:1;/*充电高温保护*/
		uint16 disHTP:1;/*放电高温保护*/
		uint16 chgLTP:1;/*充电低温保护*/
		uint16 disLTP:1;/*放电低温保护*/
		uint16 chgMosE:1;/*充电MOS损坏*/
		uint16 disMosE:1;/*放电Mos损坏*/
		uint16 innerComE:1;/*内部通讯异常*/
		uint16 oChgVP:4;/*过充电压保护1-4*/
	}bits;
}BmsErrDetail;

/*
** Bms Err
*/
typedef struct{
	BmsErrDetail errDetail;
	uint8 err[5];/*过充电压保护5-20,过放电压保护1-20*/ 
}BmsErr;

/*
** Bms Item
*/
typedef struct{
	uint16 batVol;/*电池包实际总电压--0.01*/
	uint16 batCoreNum;/*电芯数量*/
	uint16 soc;/*soc*/
	uint16 surplueCap;/*剩余容量--0.01 AH*/
	uint16 soh;/*soh*/
	int16 chgCur;/*充电电流--0.01*/
	uint16 envTemp;/*环境温度*/
	uint16 batCoreTemp;/*电芯温度*/
	uint16 mosTemp;/*MOS管温度*/
	uint16 batCoreVol[20];/*电芯x电压--0.001*/
	uint16 res[20];/*预留*/
	uint16 reqChgCur;/*请求充电电流--0.01*/
	uint16 maxChgVol;/*最高充电电压--0.01*/
	uint16 chgIsPermit;/*充电允许/禁止--0xAA55:允许充电 其余参数均不允许*/	
}BmsItem;

/*
** Bms Info
*/
typedef struct{
	BmsId id;/*电池ID信息*/
	BmsItem item;/*bms数据项信息*/
	BmsErr err;/*电池故障信息*/
}BmsInfo;
#pragma pack()

/*
** get Bms Id
** 	@return:
**		bool:true-->用于标记打包数据
*/
bool get_BmsId(void);

/*
** set Bms ID
**	@param:
**		uint8*:数据项
**		uint16:数据长度
*/
void set_BmsId(uint8* data,uint16 len);

/*
** get Bms Item
** 	@return:
**		bool:true-->用于标记打包数据
*/
bool get_BmsItem(void);

/*
** set Bms Item
**	@param:
**		uint8*:数据项
**		uint16:数据长度
*/
void set_BmsItem(uint8* data,uint16 len);

/*
** get Bms Err
** 	@return:
**		bool:true-->用于标记打包数据
*/
bool get_BmsErr(void);

/*
** set Bms Err
**	@param:
**		uint8*:数据项
**		uint16:数据长度
*/
void set_BmsErr(uint8* data,uint16 len);

#endif

