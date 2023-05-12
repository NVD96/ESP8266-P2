#include <sfr52.inc>
;Bo bao gio RTC: hh-mm-ss
        ORG 0
        ljmp main

        org 002Bh
T2ISR:  CPL p1.0
        Acall delay
        ;CLR TF2
        RETI
        
        ORG 0030H
main:   
        MOV IE,#A0H
        MOV T2MOD,#00000010B
        MOV RCAP2L,#LOW(-16)
        MOV RCAP2H,HIGH(-16)
        SETB P1.0
        SETB TR2
        SJMP $
delay: mov r0,#7fh
dl1:   mov r1,#ffh
dl2:   djnz r1,dl2
       djnz r0,dl1
       ret
        END
