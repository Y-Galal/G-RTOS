#include "queues.h"
#include "queue_structs.h"

void QueueInit(QueueType_t *QueueHandle)
{
    uint8_t i=0;
    for(i=0;i<QUEUE_LENGTH;i++)
    {
        QueueHandle[i].status = EMPTY;
    }
}




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

