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
		uint8 oneErr:1;/*一级故障*/
		uint8 res:7;/*保留*/
	}bits;
}CtrErr;

/*
** Ctr State
*/
typedef struct{
	uint8 protocol;/*协议号*/
	uint8 softVer;/*软件版本号*/
	CtrErr err;/*故障*/
}CtrState;

/*
** Sub Err
*/
typedef union{
	uint8 flag;
	struct{
		uint8 batIsOnline:1;/*电池是否在线*/
		uint8 batIsErr:1;/*电池是否故障*/
		uint8 subSelfChkErr:1;/*分控自检故障*/
		uint8 chargerOnline:1;/*充电器是否在线*/
		uint8 chargerErr:1;/*充电器故障*/
		uint8 res:3;/*保留*/
	}bits;
}SubErr;

/*
** Sub State
*/
typedef struct{
	uint8 protocl;/*协议号*/
	uint8 softVer;/*软件版本号*/
	uint8 doorState;/*仓门状态:0x00(仓门关)/0x01(仓门开)/0x02(仓门故障)*/
	uint8 doorLoc;/*仓定位:0x00(空仓)/0x01(充电中)/0x02(满电)/0x03(故障)*/
	SubErr err;/*故障状态*/
	uint8 batErrLabel;/*电池故障标识*/
	uint8 subErrLabel;/*分控故障标识*/
	uint8 ChargerErrLabel;/*充电器故障标识*/
}SubState;

/*
** State Info
*/
typedef struct{
	CtrState ctr;/*控制板*/
	SubState sub;/*分控*/
}StateInfo;
#pragma pack()

/*
** get Ctr Protocol
**	@return:
**		uint8:协议号
*/
uint8 get_CtrProtocol(void);

/*
** parse State Info
**	@param:
**		uint8:数据项长度
**		uint8*:数据项
*/
void parse_StateInfo(uint8 len,uint8* item);

#endif

