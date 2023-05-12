#include <sfr51.inc>
; pha't xung o+? P1.0
        ORG     0
        MOV     TMOD,#2H        ;Timer 0,mode 2                              
                                ;(8-bit,auto reload)          
        MOV     TH0,#0          ;TH0=0
AGAIN:  MOV     R5,#250         ;dde^'m so^' la^`n tra`n (250 la^`n)
        ACALL   DELAY          
        CPL     P1.0          
        SJMP    AGAIN
DELAY:  SETB    TR0             ;cho phe'p Timer0 cha.y
BACK:   JNB     TF0,BACK        ;cho+` tra`n
        CLR     TR0             ;du+`ng timer 0          
        CLR     TF0             ;xo'a TF0
        DJNZ    R5,DELAY          
        RET
end
