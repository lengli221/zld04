#ifndef __UPPERLAYER_CFGINFO_H_
#define __UPPERLAYER_CFGINFO_H_

#include "type.h"

/*
** get SOC Limit
**	@return:
**		uint8:SOC限定值
*/
uint8 get_SOCLimit(void);

#pragma pack(1)
/*
** Ctr Cfg
*/
typedef struct{
	uint8 chargerOT;/*充电器过温*/
	uint8 batOT;/*电池过温*/
	uint8 socLimit;/*SOC满仓阈值*/
	uint16 chgTimeLimit;/*充电时间阈值*/
}CtrCfg;

/*
** Sub Cfg
*/
typedef union{
	uint8 flag;
	struct{
		uint8 chargerOTFail:1;/*充电器配置失败*/
		uint8 batOTFail:1;/*电池过温配置失败*/
		uint8 socLimitFail:1;/*SOC配置失败*/
		uint8 chgTimeLimitFail:1;/*充电时间配置失败*/
		uint8 res:1;/*保留*/
	}bits;
}SubCfg;

/*
** Cfg Info
*/
typedef struct{
	CtrCfg ctr;/*主控配置信息*/
	SubCfg subCfg;/*分控配置信息*/
}CfgInfo;
#pragma pack()

/*
** parse Cfg Info
**	@param:
**		uint8:数据项长度
**		uint8*:数据项
*/
void parse_CfgInfo(uint8 len,uint8* item);

#endif

