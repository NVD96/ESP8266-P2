#include <sfr51.inc>
; a,b,c,d,e,f,g -> Port 2
; P3.0 -> LED1
; P3.1 -> LED2
; P3.1 -> LED3
        ORG     0H
        MOV     P3, #0FFh      ;Xac lap trang thi dau P3.x = 0
START:  JB P1.0,START  ;Cho logic 1 o P1.0
BEGIN:  CLR    P3.0
        LCALL   DELAY
        SETB    P3.0
        
        CLR    P3.1
        LCALL   DELAY
        SETB    P3.1

        CLR    P3.2
        LCALL   DELAY
        SETB    P3.2
        
        CLR    P3.3
        LCALL   DELAY
        SETB    P3.3
        
        CLR    P3.4
        LCALL   DELAY
        SETB    P3.4
        
        CLR    P3.5
        LCALL   DELAY
        SETB    P3.5
       
        CLR    P3.6
        LCALL   DELAY
        SETB    P3.6
        
        CLR    P3.7
        LCALL   DELAY
        SETB    P3.7

        SJMP    BEGIN           ;Quay tro lai tu dau
DELAY:  MOV     R1,#10
        MOV     R0,#0FFh
LOOP:   DJNZ    R0,LOOP
        DJNZ    R1,LOOP
        RET
        END