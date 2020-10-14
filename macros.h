/**
 * File Name: macros.h
 * Description: A header file that contains useful macros
 * 
 * */
#ifndef MACROS_H
#define MACROS_H
#include "RTOS_Config.h"
#include <stdint.h>

/*Macro Description: A macro that converts the time in ms to ticks*/
#define MS_TO_TICKS(TimeInMS) (     ( (uint32_t)(TimeInMS) ) * (  (uint32_t)(TARGET_FREQUENCY/1000) )      )


#endif

