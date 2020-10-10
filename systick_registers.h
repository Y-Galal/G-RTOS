#ifndef SYSTICK_REGISTERS_H
#define SYSTICK_REGISTERS_H


#define SYSPRI3         (*((volatile uint32_t *)0xE000ED20))
#define SYSTICK_CTRL    (*((volatile uint32_t *)(0xE000E000 + 0x010)))
#define SYSTICK_VAL     (*((volatile uint32_t *)(0xE000E000 + 0x018)))
#define SYSTICK_LOAD    (*((volatile uint32_t *)(0xE000E000 + 0x014)))

#define INTEN 1
#define ENABLE 0
#define CLK_SRC 2

#define PRIORITY_7 0xE0000000

#define INTCTRL (*((volatile uint32_t *)0xE000ED04))

#endif 


