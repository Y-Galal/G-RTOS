
#include "kernel.h"

#define IDLE_TASK 1

/*Static functions prototypes*/
static void StackInit(uint8_t i);
static void CreateIdleTask();
static void RequestContextSwitch();
extern void  osSchedulerLaunch();
void ContextSwitch();

typedef enum status
{
    READY,BLOCKED
}Status_Type;

typedef struct tcb{
  int32_t *stackPtr;        
  uint8_t priority;
  Status_Type status;
}TCB_Type;

TCB_Type TCB[NUM_OF_TASKS+IDLE_TASK];
TCB_Type *currentPtr;

int32_t TCB_STACK[NUM_OF_TASKS+IDLE_TASK][STACK_SIZE];





void osStartSystem()
{
    SYSTICK_CTRL = 0;
	SYSTICK_VAL  = 0;
	SYSTICK_LOAD = MS_TO_TICKS(TICK_RATE_MS)-1;

    SYSPRI3 = ( SYSPRI3&0x00FFFFFF ) | PRIORITY_7 ;       /* Lowest interrupt priority in the system (7) */

	SYSTICK_CTRL = (1<<INTEN) | (1<< ENABLE) | (1<<CLK_SRC);
	
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
    if(i==3)
    {
        CreateIdleTask();
    }
    return 1;
}

void TaskDelay(uint32_t delayInTicks)
{
    uint32_t TicksNow = SYSTICK_VAL;
    while( TicksNow - SYSTICK_VAL != delayInTicks)
    {
        RequestContextSwitch();
    }
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
    RequestContextSwitch();
}



/************************Static functions***************************/

static void StackInit(uint8_t i)
{
  TCB[i].stackPtr = &TCB_STACK[i][STACK_SIZE-16]; 
  TCB_STACK[i][STACK_SIZE-1] = 0x01000000;          /*Declaring exiplicitly that we are using the thumb mode*/
}

static void IdleTask()
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
    SYSTICK_VAL =0;
    INTCTRL = 0x04000000 ;

}

void ContextSwitch()
{
//    uint8_t i =0;
//    for(i=0;i<4;i++)
//    {
//        if(TCB[i].status == READY)
//        {
//            currentPtr = &TCB[i];
//            break;
//        }
//    }
    static uint8_t i =0;
    currentPtr = &TCB[i];
    i= (i+1)% 3;
}
