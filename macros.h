#ifndef MACROS_H
#define MACROS_H
#include "RTOS_Config.h"
#include <stdint.h>

#define MS_TO_TICKS(TimeInMS) (     ( (uint32_t)(TimeInMS) ) * (  (uint32_t)(TARGET_FREQUENCY/1000) )      )


#endif


