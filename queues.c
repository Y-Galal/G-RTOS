/**
 * File Name: queues.c
 * Description: A source file that contains queues' routines
 * 
 * */

#include "queues.h"
#include "queue_structs.h"

/**
 * Function Name: QueueInit
 * Description: This function initializes the queue by making its elements empty by default
 * !NOTE: You can only create one queue, Define QUEUE_LENGTH with your desired length in the RTOS_Config.h
 * */
void QueueInit(QueueType_t *QueueHandle)
{
    uint8_t i=0;
    for(i=0;i<QUEUE_LENGTH;i++)
    {
        QueueHandle[i].status = EMPTY;
    }
}



/**
 * Function Name: QueueSend
 * Description: this function sends the data to the queue and blocks if the queue is full
 * Inputs: (TaskHandle to block) - (QueueHandle) - (data to send) - (delay time if it's full )
 * */
void QueueSend(uint32_t TaskHandle,QueueType_t* QueueHandle, uint32_t data,uint32_t delayTime)
{
    static uint8_t i=0;
    uint8_t blockFlag =1;
    while(blockFlag == 1)
    {
        if(QueueHandle[i].status == EMPTY)
        {
            QueueHandle[i].status = FULL;
            QueueHandle[i].data = data;
            i = (i + 1) % QUEUE_LENGTH;
            blockFlag=0;
        }
        else
        {
            if(delayTime==0)
            {
                return;
            }
            TaskDelay(TaskHandle,delayTime);

        }
        
    }
}

/**
 * Function Name:QueueReceive
 * Description: This function receives the data saved in the queue and blocks if the queue was empty
 * Inputs: (TaskHandle to block) - (QueueHandle) - (address to save the data in it) - (the delay time in ticks)
 * */
void QueueReceive(uint32_t TaskHandle,QueueType_t* QueueHandle, uint32_t*data, uint32_t delayTime)
{
    static uint8_t i=0;
    uint8_t blockFlag =1;
    while(blockFlag == 1)
    {
        if(QueueHandle[i].status == FULL)
        {
            QueueHandle[i].status = EMPTY;
            *data = QueueHandle[i].data;
            i = (i + 1) % QUEUE_LENGTH;
            blockFlag=0;
        }
        else
        {
            if(delayTime==0)
            {
                return;
            }
            TaskDelay(TaskHandle,delayTime);
        }
        
    }
}

