#include <sfr52.inc>
        ORG 0000H
        LJMP MAIN
        ORG 000BH
        LJMP T0ISR
        ORG OO1BH
        LJMP T1ISR
        ORG 0030H
MAIN:   MOV TMOD,#12    ;Timer1-mode 1,Timer0-mode 2
        MOV TH0,#-71    ;7
        SETB TR0
        SETB TF1
        MOV IE,#008AH
        SJMP $
T0ISR:  CPL P1.7
        RETI
T1ISR:  CLR TR1
        MOV TH1,#HIGH(-1000)
        MOV TL1,#LOW(-1000)
        SETB TR1
        CPL P1.6
        RETI

END
