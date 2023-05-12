#include <sfr51.inc>
        ORG O000h
        SETB    P3.1
LOOP:   JNB     P3.1, LOOP
LOOP1:  
        JB      P3.1, LOOP1
        CLR     P2.0
        ACALL   DELAY
        SETB    P2.0
        SJMP    LOOP
DELAY:   MOV     R6, #0FFh
LP2:     MOV     R7, #0FFh
LP1:     DJNZ    R7, LP1
         DJNZ    R6, LP2
         RET
END
