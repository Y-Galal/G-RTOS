#ifndef KERNEL_H
#define KERNEL_H
#include "systick_registers.h"
#include "RTOS_Config.h"
#include "macros.h"
#include <stdint.h>

void osStartSystem();

void osKernelInit(void);

uint8_t TaskCreate( void(*task)(), uint8_t priority , uint32_t *TaskHandle  );

void TaskDelay(uint32_t delayInTicks);

void TaskBlock(uint32_t *TaskHandle);

void TaskResume(uint32_t *TaskHandle);




#endif


