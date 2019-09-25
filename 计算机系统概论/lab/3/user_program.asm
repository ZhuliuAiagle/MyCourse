	.ORIG 	x3000
	LD	R6,BASE
	LD	R3,ENTRYE
	STI	R3,ENTRYA
	LD	R3,A
	STI	R3,KBSR		;Set interrupt information
	LEA	R3,CHAR2
;------------------------	;Output String
LOOP1	AND	R1,R1,#0	
	ADD	R1,R1,#8	;R1 = 8, Which is the total times of outputing "**    "
LOOP11	LD	R0,CHAR2	;print "*"for 2 times
	TRAP	X21		
	TRAP	X21
	LD	R0,BLANK
	TRAP	X21		;print blank for 4 times
	TRAP	X21
	TRAP	X21
	TRAP	X21
	ADD	R1,R1,#-1	;R1--, when it equals 0, the program will go on to print next line
	BRp	LOOP11		;R1 doesn't equal 0, continue the loop
	
	
	LD	R0,ENTER	;print a '\n' to start a new line
	TRAP	X21	
	JSR	DELAY		;delay subroutines
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	LD	R0,BLANK	;
	TRAP	X21		;Print three blanks at the beginning of line
	TRAP	X21
	TRAP	X21                               
	AND	R1,R1,#0	;R1 = 7, Which is the total times of outputing "**    "
	ADD	R1,R1,#7	
LOOP12	LD	R0,CHAR2	;print "*"for 2 times
	TRAP	X21
	TRAP	X21
	LD	R0,BLANK	;print blank for 4 times
	TRAP	X21
	TRAP	X21
	TRAP	X21
	TRAP	X21
	ADD	R1,R1,#-1	;R1--, when it equals 0, the program will go on to print next line
	BRp	LOOP12		;R1 doesn't equal 0, continue the loop

	LD	R0,ENTER	;print a '\n' to start a new line
	TRAP	X21
	JSR	DELAY		;delay subroutines
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY
	JSR	DELAY                                                                   
	BR	LOOP1
;------------------------
DELAY	ST	R1,SaveR1	;Definition of delay subroutine
	LD	R1,COUNT
REP	ADD	R1,R1,#-1
	BRp	REP
	LD	R1,SaveR1
	RET
;------------------------	Some constants
CHAR2	.FILL	X002A		;'*'
BLANK	.FILL	X0020		;' '
ENTER	.FILL	#10		;'\N'

A	.FILL	x4000

COUNT	.FILL	#2500		;used to delay
SaveR1	.BLKW	1		;save R1
KBSR	.FILL	XFE00		;address of KBSR
BASE	.FILL	X3000		;the base of supervisor stack
ENTRYE	.FILL	X2000		;the entry address of interrupt service routine
ENTRYA	.FILL	X0180		;the address in the interrupt vector table that contains X2000
	.END