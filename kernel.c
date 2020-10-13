
#include "kernel.h"

#define IDLE_TASK 1

/*Static functions prototypes*/
static void StackInit(uint8_t i);
static void CreateIdleTask();
static void RequestContextSwitch();
extern void  osSchedulerLaunch();
void ContextSwitch();
static void CheckDelayedTasks();

volatile uint32_t tick=0;

typedef enum status
{
    READY,BLOCKED,DELAYED
}Status_Type;

typedef struct tcb{
  int32_t *stackPtr;       
  uint32_t delayTime; 
  uint8_t priority;
  Status_Type status;
}TCB_Type;

TCB_Type TCB[NUM_OF_TASKS+IDLE_TASK];
TCB_Type *currentPtr;

int32_t TCB_STACK[NUM_OF_TASKS+IDLE_TASK][STACK_SIZE];





void osStartSystem()
{
    STCTRL = 0;
	STCURRENT  = 0;
	STRELOAD = MS_TO_TICKS(TICK_RATE_MS)-1;

    SYSPRI3 = ( SYSPRI3&0x00FFFFFF ) | PRIORITY_7 ;       /* Lowest interrupt priority in the system (7) */

	STCTRL = (1<<INTEN) | (1<< ENABLE) | (1<<CLK_SRC);
	
	currentPtr = &TCB[0];
    osSchedulerLaunch();
}

void osKernelInit(void)
{
     asm(" CPSID   I");
}

uint8_t TaskCreate( void(*task)(), uint8_t priority  , uint32_t *TaskHandle  )
{
    asm(" CPSID   I");
    static uint8_t i = 0;
    StackInit(i);                                   /*Stack initialization*/
    TCB_STACK[i][STACK_SIZE-2] = (int32_t)(task);   /*Adding the thread function*/
    TCB[i].priority = priority;
    TCB[i].status = READY;
    *TaskHandle = (uint32_t)&TCB[i];
    i++;
    if(i==NUM_OF_TASKS)
    {
        CreateIdleTask();
    }
    return 1;
}

void TaskDelay(uint32_t TaskHandle ,uint32_t delayInTicks)
{
    TCB_Type *TaskToDelay = (TCB_Type*)TaskHandle;
    TaskToDelay->delayTime = tick + (delayInTicks/MS_TO_TICKS(TICK_RATE_MS));
    TaskToDelay->status = DELAYED;
    RequestContextSwitch();
}

void TaskBlock(uint32_t *TaskHandle)
{
    TCB_Type *TaskToBlock = (TCB_Type*)&TaskHandle;
    TaskToBlock->status = BLOCKED;
    RequestContextSwitch();
}

void TaskResume(uint32_t *TaskHandle)
{
    TCB_Type *TaskToBlock = (TCB_Type*)TaskHandle;
    TaskToBlock->status = READY;
}



/************************Static functions***************************/

static void StackInit(uint8_t i)
{
  TCB[i].stackPtr = &TCB_STACK[i][STACK_SIZE-16]; 
  TCB_STACK[i][STACK_SIZE-1] = 0x01000000;          /*Declaring exiplicitly that we are using the thumb mode*/
}

void IdleTask()
{
    while(1)
    {

    }
}

static void CreateIdleTask()
{
    StackInit(3);                                       /*Stack initialization*/
    TCB_STACK[3][STACK_SIZE-2] = (int32_t)(IdleTask);   /*Adding the thread function*/
    TCB[3].priority=0;
    TCB[3].status =READY;
}

static void RequestContextSwitch()
{
    STCURRENT =0;
    INTCTRL = 0x04000000 ;

}

void ContextSwitch()
{
    uint8_t i =0;

    CheckDelayedTasks();
    tick++;
    for(i=0;i<NUM_OF_TASKS+IDLE_TASK;i++)
    {
        if(TCB[i].status == READY)
        {
            currentPtr = &TCB[i];
            break;
        }
    }
}

static void CheckDelayedTasks()
{
    uint8_t i=0;
    for(i=0;i<NUM_OF_TASKS;i++)
    {
        if(TCB[i].status == DELAYED && TCB[i].delayTime == tick)
        {
            TCB[i].status = READY;
        }
    }
}
