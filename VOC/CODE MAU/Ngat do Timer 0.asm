#include <sfr52.inc>
        ORG 0000H   ;Diem nhap reset
        LJMP MAIN   ;nhay qua den main
        ORG 000BH   ;vector ngat cua Timer 0
TOISR:  CPL P1.0    ;lay bu P1.0
        RETI
        ORG 0030H   ;Diem nhap cua main
MAIN:   MOV TMOD,#02H   ;Timer 0, mode 2
        MOV TH0,#-50    ;tri hoan 50uS
        SETB TR0        ;cho Timer 0 hoat dong
        MOV IE,#82H     ;cho phep ngat do Timer 0
        SJMP $          ;Khong lam gi ca
        END

