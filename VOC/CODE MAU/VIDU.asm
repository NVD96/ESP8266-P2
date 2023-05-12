#include <sfr51.inc>
        ORG     0000h
LOOP:   
        SETB    P2.0
        ACALL   DELAY
        CLR     P2.0
        ACALL   DELAY
        SJMP    LOOP
DELAY:  MOV     R6, #0FFh
LP2:    MOV     R7, #0FFh
LP1:    DJNZ    R7, LP1
        DJNZ    R6, LP2
        RET
END