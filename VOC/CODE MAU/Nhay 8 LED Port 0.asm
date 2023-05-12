#include <sfr52.inc>
        org     0000h ;Khai bao dia chi bat dau cua chuong trinh
        sjmp main

        org 0030h
main:   MOV     P1,#00h ;nap 00 vao port 0 de tat 8 Led
        LCALL   delay1s ;Goi chuong trinh con cua delay
        MOV     P1,#0ffh ;Nap ff vao port0 de sang 8 led
        LCALL   delay1s ;goi chuong trinh con delay
        SJMP    main ;nhay den de lam lai tu dau

;----------------------------------------------------------
;Chuong trinh con delay
;----------------------------------------------------------
DELAY1s:    MOV R3,#10
DL10:       MOV R2,#100            
DL9:        MOV R1,#250
DL8:        NOP
            NOP
            DJNZ R1,DL8
            DJNZ R2,DL9
            DJNZ R3,DL10
            RET

delay:  MOV     R6,#0ffh ;nap gia tri ff vao thanh ghi r6
de1:    MOV     R7,#0ffh ;nap gi tri ff vao thanh gi r7
        DJNZ    R7,$ ;Giam r7 di 1 va nhay khi r7 khac 00
        DJNZ    R6,de1 ;Giam r6 di 1 va nhay khi r6 khac 00
        RET             ;Thoat khoi chuong trinh con
        END
