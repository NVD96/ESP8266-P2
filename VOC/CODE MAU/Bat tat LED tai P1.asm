#include <sfr52.inc>

mode equ 0

HUNDRED EQU     100             ;100 x 1000 = 1 giay
COUNT   EQU     1000

        ORG     0000h   
     

        mov p2,#0ffh
        mov p3,#0ffh
        mov p0,#0ffh
       
LOOP:   JB P3.7,test_mode       ;Cho logic 1 o P3.0
;LOOP:   JNB     P2.0,LOOP       ;Cho logic 0 o P3.0
;LOOP1:  JB      P2.0,test_mode  ;cho logic 1 o P3.0
        acall delay
        cpl mode
        setb p3.7
test_mode:
        jb mode,ko_nhay
nhay:
        MOV     P1,#00h    ; Xuat gia tri 00h ra cong 2
        ACALL   DELAY   ; Goi CT tao tre 1 giay
      
        MOV     P1,#ffh ; Xuat gia tri ffh ra cong 2
        ACALL   DELAY
ko_nhay:
        SJMP    LOOP ; Tro lai nhan co ten LOOP2

DELAY:  MOV     R7, #HUNDRED
AGAIN:  MOV     TH0,#HIGH(COUNT)
        MOV     TL0,#LOW(COUNT)
        SETB    TR0
WAIT:   JNB     TF0, WAIT
        CLR     TF0
        CLR     TR0
        DJNZ    R7,AGAIN
        RET
end
