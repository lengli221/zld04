#include "includes.h"

/*
** packect Cfg Info
*/
void packect_CfgInfo(void);

CfgInfo cfgInfo = {0};

/*
** get SOC Limit
**	@return:
**		uint8:SOC限定值
*/
uint8 get_SOCLimit(void){
	return cfgInfo.ctr.socLimit;
}

/*
** parse Cfg Info
**	@param:
**		uint8:数据项长度
**		uint8*:数据项
*/
void parse_CfgInfo(uint8 len,uint8* item){
	uint8 index = 0;
	CtrCfg ctrCfg = {0};
	SubCfg subCfg = {0};
	
	if(get_CtrProtocol() >= 102){
		/*copy data*/
		memcpy((uint8*)&ctrCfg.chargerOT,(uint8*)&item[0],sizeof(CtrCfg));
		index += sizeof(CtrCfg);
		/*分析数据*/
		if(ctrCfg.chargerOT >= 100 || ctrCfg.chargerOT <= 40){
			subCfg.bits.chargerOTFail = 1;
		}else{
			cfgInfo.ctr.chargerOT = ctrCfg.chargerOT;
		}
		
		if(ctrCfg.batOT >= 100 || ctrCfg.batOT <= 40){
			subCfg.bits.batOTFail = 1;
		}else{
			cfgInfo.ctr.batOT = ctrCfg.batOT;
		}		
		
		if(ctrCfg.socLimit >= 100 || ctrCfg.socLimit <= 40){
			subCfg.bits.socLimitFail = 1;
		}else{
			cfgInfo.ctr.socLimit = ctrCfg.socLimit;
		}	

		if(ctrCfg.chgTimeLimit >= 720 || ctrCfg.chgTimeLimit <= 150){
			subCfg.bits.chgTimeLimitFail = 1;
		}else{
			cfgInfo.ctr.chgTimeLimit = ctrCfg.chgTimeLimit;
		}	
		
		/*赋值回复信息*/
		cfgInfo.subCfg.flag = subCfg.flag;
		
		/*
		** packect Cfg Info
		*/
		packect_CfgInfo();		
	}
}

/*
** packect Cfg Info
*/
void packect_CfgInfo(void){
	uint8 tx[8] = {0};
	
	/*copy data*/
	memcpy((uint8*)&tx[0],(uint8*)&cfgInfo.subCfg.flag,sizeof(SubCfg));
	
	can_TransitData(1,sizeof(SubCfg),(uint8*)&tx[0],0x02);
}
