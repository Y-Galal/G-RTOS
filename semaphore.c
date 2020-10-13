#include "semaphore.h"

extern void TaskDelay(uint32_t TaskHandle ,uint32_t delayInTicks);

void SemaphoreCreate(int32_t startValue,int32_t *semaphoreHandle )
{
    *semaphoreHandle = startValue;
}

void SemaphoreTake(int32_t *semaphoreHandle,uint32_t TaskHandle, uint32_t delayInTicks)
{
    while(*semaphoreHandle <= 0 )
    {
        TaskDelay(TaskHandle,delayInTicks);
    }
    *semaphoreHandle = *semaphoreHandle -1;
}

void SemaphoreGive(int32_t *semaphoreHandle)
{
    INT_DISABLE();
    *semaphoreHandle = *semaphoreHandle + 1;
    INT_ENABLE();
}

