/**
 * File Name: semaphores.c
 * Description: A source file that contains the routines needed for the semaphores implemenation
 * 
 * */
#include "semaphore.h"

/*Externing the taskdelay function to block the task for some time if the semaphore is taken*/
extern void TaskDelay(uint32_t TaskHandle ,uint32_t delayInTicks);

/**
 * Function Name: SemaphoreCreate
 * Description: This function initializes the semaphore with a startValue passed into this function. 0 Means taken.
 * */
void SemaphoreCreate(int32_t startValue,int32_t *semaphoreHandle )
{
    *semaphoreHandle = startValue;
}

/**
 * Function Name: SemaphoreTake
 * Description: This function attempts to take the semaphore and if it's taken it will block untill the semaphore is given.
 * */
void SemaphoreTake(int32_t *semaphoreHandle,uint32_t TaskHandle, uint32_t delayInTicks)
{
    while(*semaphoreHandle <= 0 )
    {
        if (delayInTicks == 0)
        {
            return;
        }
        TaskDelay(TaskHandle,delayInTicks);
    }
    *semaphoreHandle = *semaphoreHandle -1;
}

/**
 * Function Name: SemaphoreGive
 * Description: This function gives a semaphore.
 * */
void SemaphoreGive(int32_t *semaphoreHandle)
{
    INT_DISABLE();
    *semaphoreHandle = *semaphoreHandle + 1;
    INT_ENABLE();
}

