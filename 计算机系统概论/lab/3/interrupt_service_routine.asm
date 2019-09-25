	.ORIG	X2000
	ST	R0,SaveR0	;save the value of some key registers
	ST	R1,SaveR1
	ST	R2,SaveR2
	ST	R7,SaveR7

	LD	R4,SaveR0	;load the value of R0
	AND	R5,R5,#0	
	ADD	R5,R5,#10	;R5 = 10(the ASCII code of '\n')
	NOT	R5,R5		
	ADD	R5,R5,#1	;R5 = -R5
	ADD	R5,R5,R4	;R5 ?=	R4(R0)
	BRnp	FINISH1		;if not, Which means the printed line hasn't at an end, we can't interrupt it
				;if so, interrupt it
	
;-----------------------------------IF SO
	AND	R2,R2,#0	;R2 = 10, as the counter of characters.
	ADD	R2,R2,#10
ECHO1	LDI	R0,KBDR
	LDI	R1,DSR
	BRzp	ECHO1	
	STI	R0,DDR		;read the input character and echo it on the monitor for 10 times
	
	
	ADD	R2,R2,#-1	;R2-- When a new character is input.
	BRp	ECHO1		;R2 should be larger than 0
	
	
	
	LD	R0,ENTER
ECHO2	LDI	R1,DSR
	BRzp	ECHO2
	STI	R0,DDR		;print a '\n'
	
	LD	R0,FLAG		;load the flag with control the type of cahracter.
	BRp	CHAR		;flag ?> 0
	LD	R0,CHAR1	;flag<0,change to'*'
	STR	R0,R3,#0	;R3, which saves the address of CHAR2 in user_program.asm, let R1 change the value of CHAR2(IN THE OTHER PROGRAM).
	BR	NEXT		
CHAR	LD	R0,CHAR2	;flag>0, change to '#'
	STR	R0,R3,#0	;change the value of CHAR2 in user_program.asm
NEXT	LD	R0,FLAG		
	NOT	R0,R0
	ST	R0,FLAG		;inverse flag
	BR	FINISH2			
	
FINISH1	LDI	R0,KBDR		;if the interrupt service routine works before a line ends,we can't interrupt it
				;but we need to read the character stored in KBDR so as to get the next input
	
FINISH2	LD	R0,SaveR0	;reset registers
	LD	R1,SaveR1
	LD	R2,SaveR2
	LD	R7,SaveR7
	

	RTI

SaveR0	.BLKW	1
SaveR7	.BLKW	1
SaveR1	.BLKW	1
SaveR2	.BLKW	1

ENTER	.FILL	#10	;'\n'
KBSR	.FILL	XFE00	;KBSR
KBDR	.FILL	XFE02	;KBDR
DSR	.FILL	XFE04	;DSR
DDR	.FILL	XFE06	;DDR
FLAG	.FILL	X0001	;flag to determine which character('#'or'*')is to be output
CHAR2	.FILL	X0023	;'#'
CHAR1	.FILL	X002A	;'*'
.END