#include <sfr51.inc>
        org 0000h            ;Khai bao dia chi bat dau cua chuong trinh
        
START:  JB P1.0,START  ;Cho logic 1 o P1.0

main:   MOV 80h,#00h    ;nap 00 vao port 0 de tat 8 Led
        LCALL delay     ;Goi chuong trinh con cua delay
        MOV 80h,#0ffh   ;Nap ff vao port0 de sang 8 led
        LCALL delay     ;goi chuong trinh con delay

STOP:  JNB P1.0,STOP  ;Cho logic 1 o P1.0
        SJMP main       ;nhay den de lam lai tu dau

;----------------------------------------------------------
;Chuong trinh con delay
;----------------------------------------------------------

delay:  MOV R6,#0ffh  ;nap gia tri ff vao thanh ghi r6
de1:    MOV R7,#0ffh  ;nap gi tri ff vao thanh gi r7
        DJNZ R7,$     ;Giam r7 di 1 va nhay khi r7 khac 00
        DJNZ R6,de1   ;Giam r6 di 1 va nhay khi r6 khac 00
        RET           ;Thoat khoi chuong trinh con
END
