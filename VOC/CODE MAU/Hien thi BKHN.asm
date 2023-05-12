#include <sfr52.inc>
; HANG = P1
; COT = P2

ORG 0000H
ljmp main

org 0030h
MAIN:   MOV R3,#00H 
        MOV R2,#01H 
        MOV DPTR,#BANG 

;*************************************
CHUB:   MOV R5,#50 
DD1:    MOV R3,#00H 
DDD1:   MOV A,R3 
        MOVC A,@A+DPTR 
        MOV P1,A 
        MOV P2,R2 
        ACALL DELAY1 
        MOV 0a0h,#00H 
        ACALL DELAY2 
        MOV A,R2 
        INC R3 
        RL A 
        MOV R2,A
        CJNE R3,#08H,DDD1 
        DJNZ R5,DD1
        ret
;************************************
CHUK:   MOV R5,#50 
DD2:    MOV R3,#08H 
DDD2:   MOV A,R3 
        MOVC A,@A+DPTR 
        MOV P1,A 
        MOV P2,R2 
        ACALL DELAY1 
        MOV P2,#00H 
        ACALL DELAY2 
        MOV A,R2 
        INC R3 
        RL A 
        MOV R2,A 
        CJNE R3,#16,DDD2 
        DJNZ R5,DD2 

;*************************************
CHU5:   MOV R5,#50 
DD3:    MOV R3,#16 
DDD3:   MOV A,R3 
        MOVC A,@A+DPTR 
        MOV P1,A 
        MOV P2,R2 
        ACALL DELAY1
        MOV P2,#00H 
        ACALL DELAY2
        MOV A,R2
        INC R3
        RL A
        MOV R2,A
        CJNE R3,#24,DDD3 
        DJNZ R5,DD3
        RET
;************************************
CHUO:   MOV R5,#50 
DD4:    MOV R3,#24 
DDD4:   MOV A,R3 
        MOVC A,@A+DPTR 
        MOV P1,A 
        MOV P2,R2 
        ACALL DELAY1
        MOV P2,#00H 
        ACALL DELAY2
        MOV A,R2
        INC R3
        RL A
        MOV R2,A
        CJNE R3,#32,DDD4 
        DJNZ R5,DD4
        RET

;***********************************
DELAY1: MOV R6,#10
LAP1: MOV R7,#250
DJNZ R7,$
DJNZ R6,LAP1
RET

;************************************
DELAY2: MOV R7,#4
DJNZ R7,$
RET
;********************************************************
BANG:   DB 0ffh,91h,91h,0aah,44h,00h,00h,00h    ;Chu B
        DB 0ffh,10h,28h,44h,00h,00h,00h,00h     ;Chu K
        DB 0efh,91h,91h,1eh,00h,00h,00h,00h     ;Chu 5
        DB 3ch,42h,81h,81h,42h,3ch,00h,00h      ;Chu 2
END
