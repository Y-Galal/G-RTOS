#ifndef QUEUES_H
#define QUEUES_H
#include <stdint.h>
#include "kernel.h"

#include "queue_structs.h"

void QueueInit(QueueType_t *QueueHandle);

void QueueSend(uint32_t TaskHandle,QueueType_t* QueueHandle, uint32_t data,uint32_t delayTime);

void QueueReceive(uint32_t TaskHandle,QueueType_t* QueueHandle, uint32_t*data, uint32_t delayTime);

#endif

