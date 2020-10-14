#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include <stdint.h>


#define INT_DISABLE() asm(" CPSID I")
#define INT_ENABLE() asm(" CPSIE I")


void SemaphoreCreate(int32_t startValue,int32_t *semaphoreHandle );

void SemaphoreTake(int32_t *semaphoreHandle,uint32_t TaskHandle, uint32_t delayInTicks);

void SemaphoreGive(int32_t *semaphoreHandle);


#endif


