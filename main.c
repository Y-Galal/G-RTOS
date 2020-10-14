
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "kernel.h"
#include "queues.h"

uint32_t TaskHandle1,TaskHandle2,TaskHandle3;
QueueType_t queue1[QUEUE_LENGTH];

void task1()
{
    uint32_t valueFromQueue;
    while(1)
    {
        QueueReceive(TaskHandle1,queue1,&valueFromQueue,MS_TO_TICKS(1000));
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,valueFromQueue);
        TaskDelay(TaskHandle1, MS_TO_TICKS(1000));
        

    }
}
void task2()
{

        QueueSend(TaskHandle2,queue1,0x0E,MS_TO_TICKS(1000));
        TaskDelay(TaskHandle1, MS_TO_TICKS(2000));

        QueueSend(TaskHandle2,queue1,0x02,MS_TO_TICKS(1000));
        TaskDelay(TaskHandle1, MS_TO_TICKS(2000));
        
        QueueSend(TaskHandle2,queue1,0x04,MS_TO_TICKS(1000));
        TaskDelay(TaskHandle1, MS_TO_TICKS(2000));
       
        QueueSend(TaskHandle2,queue1,0x08,MS_TO_TICKS(1000));
        TaskDelay(TaskHandle1, MS_TO_TICKS(2000));

        QueueSend(TaskHandle2,queue1,0b00001100,0);
        TaskBlock(TaskHandle2);
    while(1)
    {
     

    }
}
void task3()
{

    while(1)
    {
        TaskBlock(TaskHandle3);

    }
}

int main(void)
{
    SysCtlClockSet(SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ|SYSCTL_SYSDIV_2_5);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    /*Enabling PORTF*/
 
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);    /*Enabling internal red  LEDs*/

    TaskCreate(task2,3,&TaskHandle2);
    TaskCreate( task1, 2 , &TaskHandle1  );
    TaskCreate(task3,1,&TaskHandle3);

    QueueInit(queue1);

    osKernelInit();
    osStartSystem();
    while(1)
    {

    }

}
