#include <sfr51.inc>
ORG     0000H        
HUND    EQU     100             ;100 x 1000 = 1 giay
COUNT   EQU     1000
        SETB    P2.0            ;P3.0: input
LOOP:   JNB     P2.0,LOOP       ;Cho logic 0 o P3.0
LOOP1:  JB      P2.0,LOOP1      ;cho logic 1 o P3.0
        CLR     P1.0
        ACALL   DELAY
        SETB    P1.0
        SJMP     LOOP

DELAY:  MOV     R7,#HUND
AGAIN:  MOV     TH0,#HIGH(COUNT)
        MOV     TL0,#LOW(COUNT)
        SETB    TR0
WAIT:   JNB     TF0,WAIT
        CLR     TF0
        CLR     TR0
        DJNZ    R7,AGAIN
        RET
end
