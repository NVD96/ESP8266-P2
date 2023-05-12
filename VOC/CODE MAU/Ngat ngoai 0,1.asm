#include <sfr52.inc>
        ORG 0
        LJMP MAIN
EX0ISR: CLR P1.7    ;Tat lo dot
        RETI
        ORG 0013H
EX1ISR: SETB P1.7   ;Mo lo
        RETI
        ORG 0030H
MAIN:   MOV IE,#85H ;Cho phep ngat ngoai
        SETB IT0    ;kich boi canh am
        SETB IT1    
        SETB P1.7   ;mo lo
        JB P2.3,SKIP ;Neu T >21C
        CLR P1.7    ;Tat lo
        SJMP $      ;Khong lam gi
SKIP:   SJMP $
        END
