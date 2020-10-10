
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
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,7);
        TaskBlock(&TaskHandle1);
    }
}
void task2()
{

    while(1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x0E);

    }
}
void task3()
{

    while(1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,4);

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
    osKernelInit();
    osStartSystem();
    while(1)
    {

    }

}
