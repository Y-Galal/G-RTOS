
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "kernel.h"

uint32_t TaskHandle1,TaskHandle2,TaskHandle3;

void task1()
{

    while(1)
    {
        GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_4,GPIO_PIN_4);
        TaskDelay(TaskHandle1,MS_TO_TICKS(3000));
        GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_4,!GPIO_PIN_4);
        TaskDelay(TaskHandle1,MS_TO_TICKS(3000));

    }
}
void task2()
{

    while(1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_PIN_1);
        TaskDelay(TaskHandle2,MS_TO_TICKS(2000));
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,!GPIO_PIN_1);
        TaskDelay(TaskHandle2,MS_TO_TICKS(2000));

    }
}
void task3()
{

    while(1)
    {
        GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,GPIO_PIN_5);
        TaskDelay(TaskHandle3,MS_TO_TICKS(1000));
        GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,!GPIO_PIN_5);
        TaskDelay(TaskHandle3,MS_TO_TICKS(1000));

    }
}

int main(void)
{
    SysCtlClockSet(SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ|SYSCTL_SYSDIV_2_5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    /*Enabling PORTF*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);    /*Enabling internal red  LEDs*/
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_4|GPIO_PIN_5);
    TaskCreate(task2,3,&TaskHandle2);
    TaskCreate( task1, 2 , &TaskHandle1  );

    TaskCreate(task3,1,&TaskHandle3);
    osKernelInit();
    osStartSystem();
    while(1)
    {

    }

}
