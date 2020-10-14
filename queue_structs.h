#ifndef QUEUE_STRUCTS_H
#define QUEUE_STRUCTS_H

#include <stdint.h>


typedef enum
{
    EMPTY,FULL
}Status_Type;

typedef struct
{
    uint32_t data;
    Status_Type status;
    
}QueueType_t;







#endif

