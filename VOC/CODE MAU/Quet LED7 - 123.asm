#include <sfr51.inc>
; a,b,c,d,e,f,g -> Port 2
; P3.0 -> LED1
; P3.1 -> LED2
; P3.1 -> LED3
        ORG     0H
        MOV     P3,#0FFh        ; ta('t ta^'t ca? ca'c LED
BEGIN:  MOV     P2,#F9H        ; xua^'t ra P2 ma~ cu?a '3'
        CLR     P3.0            ; ba^.t LED1
        ACALL   DELAY           ; delay
        SETB    P3.0            ; ta('t LED1
        MOV     P2,#A4h        ; xua^'t ra P2 ma~ cu?a '2'
        CLR     P3.1            ; ba^.t LED2
        ACALL   DELAY           ; delay
        SETB    P3.1            ; ta('t LED2
        MOV     P2,#B0h        ; xua^'t ra P2 ma~ cu?a '1'
        CLR     P3.2            ; ba^.t LED3
        ACALL   DELAY           ; delay
        SETB    P3.2            ; ta('t LED3
        SJMP    BEGIN
DELAY:  MOV     R1,#10
        MOV     R0,#0FFh
LOOP:   DJNZ    R0,LOOP
        DJNZ    R1,LOOP
        RET
        END
