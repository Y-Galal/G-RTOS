/**
 * File Name: systick_registers.h
 * Description: A header file that contains the definitions for the systick registers in Cortex-M4 Processors
 * 
 * */
#ifndef SYSTICK_REGISTERS_H
#define SYSTICK_REGISTERS_H


#define SYSPRI3         (*((volatile uint32_t *)0xE000ED20))
#define STCTRL    (*((volatile uint32_t *)(0xE000E000 + 0x010)))
#define STCURRENT     (*((volatile uint32_t *)(0xE000E000 + 0x018)))
#define STRELOAD    (*((volatile uint32_t *)(0xE000E000 + 0x014)))

      



#define INTEN 1
#define ENABLE 0
#define CLK_SRC 2

#define PRIORITY_7 0xE0000000

#define INTCTRL (*((volatile uint32_t *)0xE000ED04))

#endif 


