#ifndef __INCLUDE_H_
#define __INCLUDE_H_

#include <stdio.h>
#include  <stdlib.h>
#include  <string.h>

#include "gd32f10x.h"

#include "type.h"
#include "common.h"

#include "bsp.h"

#include "gpio.h"
#include "sysTick.h"
#include "debug_printf.h"
#include "usart1.h"
#include "usart2.h"
#include "usart.h"
#include "can0.h"
#include "can.h"

/*pro*/
#include "modbus_Pro.h"

/*bms*/
#include "lowerlayer_Bms.h"
#include "lowerlayer_BmsInfo.h"

/*charger*/
#include "zy_Charger.h"
#include "zy_ChargerInfo.h"

/*ctr sub*/
#include "upperlayer_stateInfo.h"
#include "upperlayer_cfgInfo.h"
#include "upperlayer_bmsInfo.h"
#include "upperlayer_chargerInfo.h"
#include "upperlayer_openDoor.h"
#include "upperlayer.h"

#endif
