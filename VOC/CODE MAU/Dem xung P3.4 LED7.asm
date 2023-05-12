#include <sfr51.inc>
; Que't LED
; a,b,c,d,e,f,g -> Port 2
; P3.0 -> LED1
; P3.1 -> LED2
; P3.2 -> LED3
; P3.4(T0) -> Button
; 30h: ha`ng do+n vi.
; 31h: ha`ng chu.c
; 32h: ha`ng tra(m
;Mach dem chay den 255 thi reset lai( co su tran o TF0)
        ORG     0
        SJMP    MAIN

        
        ORG     0030H
MAIN:   MOV     DPTR,#LED7SEG   ; DPTR tro? dde^'n ba?ng ma~ LED
        MOV     TMOD,#06h       ; counter 0, mode 2, XUNG DEM TU BEN NGOAI
        
        MOV     TH0,#C4H       ;DEM 60 LAN
        SETB    P3.0            ; ta('t ta^'t ca? ca'c LED
        SETB    P3.1
        SETB    P3.2
        SETB    P3.4            ; P3.4: input(DAU VAO XUNG DEM)
        SETB    TR0             ; cho phe'p counter 0 cha.y

BEGIN:  
        MOV     A,TL0
        LCALL   BIN2BCD         ; tra ba?ng, ddo^?i BCD -> LED 7 ddoa.n
        MOV     A,30h
        CJNE    A,10,THOAT1
        MOV     A,#00H
THOAT1:
        MOVC    A,@A+DPTR
        MOV     30h,A
        MOV     A,31h
        CJNE    A,#6,THOAT2
        MOV     A,#00
THOAT2:
        MOVC    A,@A+DPTR
        MOV     31h,A
        MOV     A,32h
        CJNE    A,1,THOAT3
        MOV     A,#00H
THOAT3:
        MOVC    A,@A+DPTR
        MOV     32h,A
KET_THUC:
        LCALL   DISPLAY
        SJMP    BEGIN
DISPLAY:
        MOV     P2,30H          ; LED1
        CLR     P3.0            ; ba^.t LED1 sa'ng
        ACALL   DELAY           ; delay
        SETB    P3.0            ; ta('t LED1

        MOV     P2,31H          ; LED2
        CLR     P3.1            ; ba^.t LED2 sa'ng
        ACALL   DELAY           ; delay
        SETB    P3.1            ; ta('t LED2

        MOV     P2,32H          ; LED 3
        CLR     P3.2            ; ba^.t LED3 sa'ng
        ACALL   DELAY           ; delay
        SETB    P3.2            ; ta('t LED3
        RET
BIN2BCD:
        MOV     B,#10           ; B=10
        DIV     AB              ; chia cho 10
        MOV     30h,B           ; lu+u digit tha^'p VAO 30H
        MOV     B,#10           ;
        DIV     AB              ; chia cho 10
        MOV     31H,B
        MOV     32h,A           ; lu+u digit cuo^'i va`o 42h
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
;        DB      88H,0C6H,86H,8EH,82H,89H
        END
