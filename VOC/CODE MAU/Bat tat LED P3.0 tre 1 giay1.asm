#include <sfr52.inc>
ORG     00h        
HUNDRED EQU     100             ;100 x 1000 = 1 giay
COUNT   EQU     1000
        SETB    0BFEH            ;OBFEH: input
LOOP:   JNB     0BFEH,LOOP       ;Cho logic 0 o OBFEH
LOOP1:  JB      0BFEH,LOOP1      ;cho logic 1 o OBFEH
        CLR     0AFEH
        ACALL   DELAY
        SETB    0AFEH
        SJMP    LOOP

DELAY:  MOV     R7, #HUNDRED
AGAIN:  MOV     8CH, #HIGH(COUNT)
        MOV     8AH, #LOW(COUNT)
        SETB    8CH
WAIT:   JNB     8DH, WAIT
        CLR     8DH
        CLR     8CH
        DJNZ    R7,AGAIN
        RET
end
