#include <sfr51.inc>
;Chuong trinh nhay LED
;P1 lam dau ra cua tin hieu dieu khien
;Cac led se sang tu p1.0 den p1.7 va nguoc lai
;Chip AT89S52  XTAL = 11.0592MHz
            
        ORG 0000H
        LJMP MAIN
        ORG 0030H
MAIN:
;        clr p2.0        ;P2.0 = 0
;        jnb p2.0,exit   ;Nhay den nhan Exit neu P2.0 = 0

        MOV R6,#7   ;Nap R6 = 7
        MOV R7,#7   ;Nap R7 = 7
        MOV A,#01H  ;Nap A = 1
LOOP:   
        MOV P1,A
        RL A        ;Quay trai thanh chua A
        ACALL DELAY ;Goi CT delay
        DJNZ R6,LOOP    ;Kiem tra R6, giam va nhay neu R6=0
        SJMP NEXT       ;Chuyen den CT tiep theo
NEXT:
        MOV P1,A
        ACALL DELAY
        RR A
        DJNZ R7,NEXT
        SJMP MAIN
exit:   sjmp exit
DELAY:
        MOV R2,#1000
DL5:    MOV R1,#250            
DL4:    NOP
        NOP
        DJNZ R1,DL4
        DJNZ R2,DL5
        RET

        END
        
    
