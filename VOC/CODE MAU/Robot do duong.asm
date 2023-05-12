#include <sfr52.inc>
        ORG 0000H
        LJMP MAIN

        ORG OO30H
MAIN:   MOV P3,#FFH
        MOV P1,#40H
         
HERE:   JNB P1.6,HERE           ;Kiem tra cong tac tai chan P1.6(Kiem tra logic 0)
LOOP:   JB P1.6,LOOP            ;Kiem tra logic 1 tai chan P1.6    

LOOP1:  MOV A,P3
R:      CJNE A,#15,FOW  ;Turn right
        MOV P1,#68H
        ACALL DELAY        
        SJMP LOOP1

FOW:    CJNE A,#9,L  ;Forward(high)
        MOV P1,#64H
        ACALL DELAY
        SJMP LOOP1

L:      CJNE A,#8,FL  ;Turn left
        MOV P1,#54H
        ACALL DELAY
        SJMP LOOP1

FL:     CJNE A,#11,EXIT  ;Forward(low)
        MOV P1,#64H
        ACALL DELAY
        SJMP LOOP1
EXIT:   SJMP LOOP1

DELAY:  MOV R2,#10
DL7:        MOV R1,#250            
DL6:        NOP
            NOP
            DJNZ R1,DL6
            DJNZ R2,DL7
            RET
END