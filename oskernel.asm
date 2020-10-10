
		.thumb

		.ref currentPtr              ;getting the current pointer from kernel.c


		.def SysTick_Handler        ;defining the functions
		.def osSchedulerLaunch
		.ref ContextSwitch
current:					.word	currentPtr       ;declaring a variable that has the same value of currentpt

; Function name: SysTick_Handler
; Description: This function will be called when a systick interrupt occures to do the context switching
; NOTE: This function must be in the startup code in the systick interrupt section.

	.align 4
SysTick_Handler: .asmfunc          ;save r0,r1,r2,r3,r12,lr,pc,psr
    CPSID   I               ; disabling the interrupts            
    PUSH    {R4-R11}        ; save r4,r5,r6,r7,r8,r9,r10,r11   
    LDR     R0, current     ; r0 points to currentPt
    LDR     R1, [R0]        ; r1= currentPt   
    STR     SP, [R1]        ; these four lines above save the context
    
    PUSH	{R0,LR}
	BL      ContextSwitch
	POP		{R0,LR} 
	LDR		R1,[R0]          ;R1= currentPt i.e. New thread
    LDR     SP, [R1]        ; SP= currentPt->stackPt   
    POP     {R4-R11}        ; loading the new context from the saved stack frame   
    CPSIE   I                  
    BX      LR 
	.endasmfunc

; Function Name: osSchedulerLaunch
; Description: This function loads the first context of the first task to start the scheduler.

	.align 4
osSchedulerLaunch: .asmfunc
    LDR     R0, current
    LDR     R2, [R0]       ; R2 =currentPt       
    LDR     SP, [R2]       ; SP = currentPt->stackPt  pointing to the stack frame
    POP     {R4-R11}       ; loading the data saved in the stack to the cortex-M4 register file   
    POP     {R0-R3}            
    POP     {R12}
    ADD     SP,SP,#4           
    POP     {LR}               
    ADD     SP,SP,#4         
    CPSIE   I                 
    BX      LR                 
	.endasmfunc

	.end
