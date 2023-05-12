#include <sfr51.inc>
; Button no^'i vo+'i /INT1
; Nha^'n button -> LED (P1.3) sa'ng mo^.t lu'c ro^`i ta('t 
        ORG  0000H
        LJMP MAIN               ;nha?y qua vu`ng vector nga('t
; ISR cu?a INT1
        ORG  0013H              ;INT1 ISR
        SETB P1.3               ;ba^.t LED sa'ng (1 byte)
        MOV  R3,#255            ;(2 byte)
BACK:   DJNZ R3,BACK            ;delay 1 chu't (2 byte)
        CLR  P1.3               ;ta('t LED (1 byte)
        RETI                    ;(1 byte)
; MAIN program for initialization
        ORG  30H
MAIN:   MOV  IE,#0BF7h;10000100B      ;cho phe'p nga('t ngoa`i 1 (/INT1)
HERE:   SJMP HERE               ;cho+` nha^.n nga('t 
        END

