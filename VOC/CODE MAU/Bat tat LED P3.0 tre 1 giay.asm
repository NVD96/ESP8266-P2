#include <sfr52.inc>
mode    equ 1

HUNDRED EQU     100             ;100 x 1000 = 1 giay
COUNT   EQU     1000

        ORG     0000h   
        sjmp main

        org 0003h
ex0_isr:
        cpl mode
        reti

        org 0030h
main:
    setb EX0    ; Cho phep ngat ngoai chan P3.2 EX0 = 1
    setb IT0    ; Thiet lap ngat theo suon am   IT0 = 1
    setb EA     ; Cho phep ngat toan cuc(Global) EA = 1
    setb mode
main_loop:
        jb mode,no_blink
LOOP2:  
        MOV     P1,#00h    ; Xuat gia tri 00h ra cong 2
        ACALL   DELAY   ; Goi CT tao tre 1 giay
      
        MOV     P1,#ffh ; Xuat gia tri ffh ra cong 2
        ACALL   DELAY
        ;SJMP    LOOP2 ; Tro lai nhan co ten LOOP2
no_blink:
        sjmp main_loop

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
