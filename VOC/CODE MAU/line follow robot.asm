*#cpu 8051 Tiny
*
* DDS MICRO-C 8031/51 Startup Code & Runtime library for TINY model
*
* Copyright 1991-1999 Dave Dunfield
* All rights reserved.
*
        ORG    $0000         $0800  CODE Starts here (Normally in ROM)
        LJMP   START

        ORG    $0003
        LJMP   SERVICE_EX0

        ORG    $000B
        LJMP   SERVICE_TIMER0_INTERRUPT


* Fixed memory locations for alternate access to the R0-R7 register bank.
* If you are NOT useing BANK 0, these equates must be adjusted.
?R0	EQU	0		Used for "POP" from stack
?R1	EQU	?R0+1		Used to load index indirectly
?R2	EQU	?R0+2		""		""		""		""
?R3	EQU	?R0+3		Used by some runtime lib functions
?R4	EQU	?R0+4
?R5	EQU	?R0+5
?R6	EQU	?R0+6
?R7	EQU	?R0+7
*
* Startup code entry point
*
* If you are NOT using interrupts, you can reclaim 50 bytes
* of code space by removing the following TWO lines.
*        AJMP    *+$0032         Skip interrupt vectors
*        DS      $0032-2         Reserve space for interrupt vectors
*
START   EQU     *
	MOV	SP,#?stk-1	Set up initial stack
        ORL  TMOD,#%00000001    set timer 0 to be counter 16 bit
        SETB    IE.7            $AF  EA
        SETB    IE.1            $A9  ET0 Enable timer 0 interrupt
        SETB    TCON.4          start timer 0


	LCALL	main		Execute program
        SJMP    *               JUMP HERE

* EXIT to MON51 by calling the 'timer1' interrupt vector ($001B).
* This causes MON51 to think that a single-step operation has just
* completed, and therefore it saves the user registers, and performs
* a context switch back to the monitor.
*
* When using 2K addressing (CC51: -Z option, ASM51: -A option) this LCALL
* may fail "Out of range" because it gets translated to ACALL, and $001B
* may not be in the same 2K block as your program. Since 2K devices cannot
* support a debugger, change the ORG to $0000, and ...<continue below>...
*
* If you are NOT using MON51 (or MONICA which works the same), you will
* need to change this to whatever action you desire when main() returns.
* Suggestions: 1:freeze (SJMP *) 2:Restart (SJMP *&$FF00)
exit	LCALL	$001B		Call Timer-1 interrupt
	SJMP	exit		Incase he go's again

**************************** My code *********************************

SERVICE_TIMER0_INTERRUPT   EQU *
   PUSH ACC
   PUSH PSW
   MOV  TH0,#$FF      reload timer 0 for ms
   MOV  TL0,#$00
   INC  tick

   MOV  A,tick
   CJNE A,#100,RIGHT
   MOV  tick,#0

RIGHT
   CLR  C
   SUBB A,speedright
   JC   ON_RIGHT
   CLR  P1.0
   SJMP LEFT

ON_RIGHT
   SETB P1.0

LEFT
   MOV  A,tick
   CLR  C
   SUBB A,speedleft
   JC   ON_LEFT
   CLR  P1.1
   SJMP EXIT_I

ON_LEFT
   SETB P1.1

EXIT_I
   POP  PSW
   POP  ACC
   RETI

SERVICE_EX0 EQU *
   INC  cputick
   RETI


$SE:1
*#map1 Segment 1, initialized variables
$SE:2
*#map2 Segment 2, internal "register" variables
	ORG	$0008		Internal ram ALWAYS starts here

tick         DS  1
speedright   DS  1                                                  
speedleft    DS  1   
cputick      DS  1
