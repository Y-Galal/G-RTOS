/**
 * File Name: kernel.c
 * Description: A source file that contains kernel functions
 * 
 * */
#include "kernel.h"

#define IDLE_TASK 1

/********************Static functions prototypes********************/

static void StackInit(uint8_t i);
static void CreateIdleTask(uint8_t idleTask);
static void RequestContextSwitch();
extern void  osSchedulerLaunch();
void ContextSwitch();
static void CheckDelayedTasks();

volatile uint32_t tick=0;       /*tick counter for the systick timer*/

/**
 * Enum that has the possible status of a task
*/
typedef enum status
{
    READY,BLOCKED,DELAYED
}Status_Type;

/* defining the Task Control Block Struct*/
typedef struct tcb{
  int32_t *stackPtr;                    /*Variable that contains the stack pointer of a task*/   
  uint32_t delayTime;                   /*variable that contains the delay time if the task is delayed*/
  uint8_t priority;                     /*variable that contains the priority of the task*/
  Status_Type status;                   /*variable that contains the status of a task*/
}TCB_Type;

TCB_Type TCB[NUM_OF_TASKS+IDLE_TASK];           /*creating the TCBs for the task*/
TCB_Type *currentPtr;                           /*pointer that points to the running task's tcb*/

int32_t TCB_STACK[NUM_OF_TASKS+IDLE_TASK][STACK_SIZE];      /*creating the stack pointer for the tasks + idle task*/



/**
 * Function Name: osStartSystem
 * Description: This function configures the systick timer and starts the scheduler
 * */

void osStartSystem()
{
    STCTRL = 0;
	STCURRENT  = 0;
	STRELOAD = MS_TO_TICKS(TICK_RATE_MS)-1;

    SYSPRI3 = ( SYSPRI3&0x00FFFFFF ) | PRIORITY_7 ;       /* Lowest interrupt priority in the system (7) */

	STCTRL = (1<<INTEN) | (1<< ENABLE) | (1<<CLK_SRC);
	
	currentPtr = &TCB[0];                                   /*Loading the highest priority task*/
    osSchedulerLaunch();
}

/**
 * Function Name: osKernelInit
 * Description: This function is called before starting the system to avoid false interrupts
 * */
void osKernelInit(void)
{
     asm(" CPSID   I");
}

/**
 * Function Name: TaskCreate
 * Description: This function is called to create a task 
 * Inputs: the task function - the task priority and the task handle
 * !NOTE : You Must create the highest priority function first!!!
 * */
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
        CreateIdleTask(i);
    }
    return 1;
}

/**
 * Function Name: TaskDelay
 * Description: This fucntion takes the task handle that needs to be delayed for some time 
 * */
void TaskDelay(uint32_t TaskHandle ,uint32_t delayInTicks)
{
    if(delayInTicks == 0)   /*if the ticks are 0 this function doesn't do anything.*/
    {
        return;
    }
    TCB_Type *TaskToDelay = (TCB_Type*)TaskHandle;
    TaskToDelay->delayTime = tick + (delayInTicks/MS_TO_TICKS(TICK_RATE_MS));
    TaskToDelay->status = DELAYED;
    RequestContextSwitch();
}

/**
 * Function Name: TaskBlock 
 * Description: This function blocks a task and request a context switch, to unblock the task you should use TaskResume.
 * */
void TaskBlock(uint32_t TaskHandle)
{
    TCB_Type *TaskToBlock = (TCB_Type*)TaskHandle;
    TaskToBlock->status = BLOCKED;
    RequestContextSwitch();
}

/**
 * Function Name: TaskResume
 * Description: This function resumes a task by changing its status to "ready".
 * */
void TaskResume(uint32_t TaskHandle)
{
    TCB_Type *TaskToBlock = (TCB_Type*)TaskHandle;
    TaskToBlock->status = READY;
}



/************************Static functions***************************/

/**
 * Function Name: StackInit
 * Description: This function intializes the stack for a task
 * */
static void StackInit(uint8_t i)
{
  TCB[i].stackPtr = &TCB_STACK[i][STACK_SIZE-16]; 
  TCB_STACK[i][STACK_SIZE-1] = 0x01000000;          /*Declaring explicitly that we are using the thumb mode*/
}

/**
 * Function Name: IdleTask
 * Description: This function is the lowest priority task in the system (priority 0)
 * and it will be called when all of the tasks in the system are blocked/delayed.
 * */
void IdleTask()
{
    while(1)
    {

    }
}

/**
 * Function Name: CreateIdleTask
 * Description: This function is called after creating all the tasks in the system to 
 * create the idle task with the lowest priority.
 * */
static void CreateIdleTask(uint8_t idleTask)
{
    StackInit(idleTask);                                       /*Stack initialization*/
    TCB_STACK[idleTask][STACK_SIZE-2] = (int32_t)(IdleTask);   /*Adding the thread function*/
    TCB[idleTask].priority=0;
    TCB[idleTask].status =READY;
}

/**
 * Function Name: RequestContextSwitch
 * Description: This function resets the value in the counter and 
 * triggers a systick timer interrupt to switch the context.
 * */
static void RequestContextSwitch()
{
    STCURRENT =0;
    INTCTRL = 0x04000000 ;

}

/**
 * Function Name: ContextSwitch
 * Description: This function is called from the systick handler in kernel.asm file to perform the context switching
 * by making the currentPtr points to the task with highest priority and in the ready state.
 * */
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

/**
 * Function Name: checkDelayedTasks  
 * Description:This function is called to check if there's a delayed 
 * task that finished its delay time to make its status ready.
 * */
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
