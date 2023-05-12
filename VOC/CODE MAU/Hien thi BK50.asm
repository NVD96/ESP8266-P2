#include <sfr51.inc>

ORG 00H
MAIN: mov   r4,1000
MOV R3,#00H 
MOV R2,#01H 
MOV DPTR,#BANG 

;*************************************
CHUB: MOV R5,#150 
DD1: MOV R3,#00H 
DDD1: MOV A,R3 
MOVC A,@A+DPTR 
MOV 0b0h,A 
MOV 0a0h,R2 
ACALL DELAY1 
MOV 0a0h,#00H 
ACALL DELAY2 
MOV A,R2 
INC R3 
RL A 
MOV R2,A
CJNE R3,#08H,DDD1 
DJNZ R5,DD1 
;************************************
CHUK: MOV R5,#150 
DD2: MOV R3,#08H 
DDD2: MOV A,R3 
MOVC A,@A+DPTR 
MOV 0b0h,A 
MOV 0a0h,R2 
ACALL DELAY1 
MOV 0a0h,#00H 
ACALL DELAY2 
MOV A,R2 
INC R3 
RL A 
MOV R2,A 
CJNE R3,#16,DDD2 
DJNZ R5,DD2 

;*************************************
CHU5: MOV R5,#150 
DD3: MOV R3,#16 
DDD3: MOV A,R3 
MOVC A,@A+DPTR 
MOV 0b0h,A 
MOV 0a0h,R2 
ACALL DELAY1
MOV 0a0h,#00H 
ACALL DELAY2
MOV A,R2
INC R3
RL A
MOV R2,A
CJNE R3,#24,DDD3 
DJNZ R5,DD3

;************************************
CHU2: MOV R5,#150 
DD4: MOV R3,#24 
DDD4: MOV A,R3 
MOVC A,@A+DPTR                  ;00000
MOV 0b0h,A                      ;01111
MOV 0a0h,R2                     ;00001
ACALL DELAY1                    ;11110
MOV 0a0h,#00H                   ;11110
ACALL DELAY2                    ;01110    
MOV A,R2                        ;00001
INC R3                          ;11111
RL A
MOV R2,A
CJNE R3,#32,DDD4 
DJNZ R5,DD4

ACALL main

;***********************************
DELAY1: MOV R6,#5
LAP1: MOV R7,#250
DJNZ R7,$
DJNZ R6,LAP1
RET

;************************************
DELAY2: MOV R7,#4
DJNZ R7,$
RET
;********************************************************
BANG:   DB 01h,6dh,6dh,6dh,93h,ffh,ffh,ffh ;chu B
        DB 01h,efh,d7h,bbh,7dh,ffh,ffh,ffh ;Chu K
        DB 019h,5dh,5dh,5dh,63h,ffh,ffh,ffh ;so 5
        DB bdh,79h,75h,6dh,9dh,ffh,ffh,ffh  ;so 2
END
