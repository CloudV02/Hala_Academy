			AREA	|.text|, CODE, READONLY, ALIGN = 2
			THUMB
			PRESERVE8
			IMPORT currentPt
			IMPORT osKernelSW
			EXPORT PendSV_Handler
			EXPORT osSchedularLauch

PendSV_Handler
	CPSID		I
	; save stackPt current
	PUSH {R4-R11}
	LDR R0, =currentPt
	LDR R1, [R0]
	STR SP, [R1]
	
	PUSH {R0,LR}
	; switch new SP
	BL osKernelSW
	
	POP {R0,LR}
	LDR R1, [R0]
	LDR SP, [R1]
	
	POP {R4-R11}
	CPSIE		I
	BX LR
	

osSchedularLauch	; Manual push for all register
	CPSID		I
	LDR	R0, =currentPt
	LDR R1, [R0]	; take address first task tcb[0]
	LDR SP, [R1]
	POP {R4-R11}
	POP {R0-R3}
	POP {R12}
	
	ADD SP,SP,#4
	POP {LR}
	ADD SP,SP,#4
	
	CPSIE		I
	BX 	LR
	ALIGN
	END
