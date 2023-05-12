#include <sfr52.inc>
; Que't LED
; a,b,c,d,e,f,g -> Port 2
; P3.0 -> LED1
; P3.1 -> LED2
; P3.2 -> LED3
; P3.4(T0) -> Button
; 40h: ha`ng do+n vi.
; 41h: ha`ng chu.c
; 42h: ha`ng tra(m
        ORG     0000H
        ljmp main

        org 0030h
main:
        MOV     DPTR,#LED7SEG   ; DPTR tro? dde^'n ba?ng ma~ LED
        MOV     TMOD,#06h       ; counter 0, mode 2
        MOV     TH0,#-60
        SETB    P3.0            ; ta('t ta^'t ca? ca'c LED
        SETB    P3.1
        SETB    P3.2
        SETB    P3.4            ; P3.4: input
        SETB    TR0             ; cho phe'p counter 0 cha.y
BEGIN:  MOV     A,TL0
        LCALL   BIN2BCD
        ; tra ba?ng, ddo^?i BCD -> LED 7 ddoa.n
        MOV     A,40h           
        MOVC    A,@A+DPTR
        MOV     40h,A
        MOV     A,41h
        MOVC    A,@A+DPTR
        MOV     41h,A
        MOV     A,42h
        MOVC    A,@A+DPTR
        MOV     42h,A
        LCALL   DISPLAY
        SJMP    BEGIN
DISPLAY:
        MOV     P2,40H          ; LED1
        CLR     P3.0            ; ba^.t LED1 sa'ng
        ACALL   DELAY           ; delay
        SETB    P3.0            ; ta('t LED1
        MOV     P2,41H          ; LED2
        CLR     P3.1            ; ba^.t LED2 sa'ng
        ACALL   DELAY           ; delay
        SETB    P3.1            ; ta('t LED2
        MOV     P2,42H          ; LED 3
        CLR     P3.2            ; ba^.t LED3 sa'ng
        ACALL   DELAY           ; delay
        SETB    P3.2            ; ta('t LED3
        RET
BIN2BCD:
        MOV     B,#10           ; B=10
        DIV     AB              ; chia cho 10
        MOV     40h,B           ; lu+u digit tha^'p
        MOV     B,#10           ;
        DIV     AB              ; chia cho 10
        MOV     41h,B           ; lu+u digit tie^'p theo va`o 41h
        MOV     42h,A           ; lu+u digit cuo^'i va`o 42h
        RET
DELAY:
        MOV     R1,#10
        MOV     R0,#0FFh
LOOP:   DJNZ    R0,LOOP
        DJNZ    R1,LOOP
        RET
;
LED7SEG:
        DB      0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H
        DB      88H,0C6H,86H,8EH,82H,89H
        END

