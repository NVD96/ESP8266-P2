#include <sfr51.inc>
; a,b,c,d,e,f,g -> Port 2
; P3.0 -> LED1
; P3.1 -> LED2
; P3.2 -> LED3
; P3.3  -> LED4
        ORG     0H
        MOV     P3,#0FFH
        MOV     DPTR,#LED7SEG
BEGIN:  
        MOV     A,#1        ;so can hien thi tren 7-SEG la 1
        MOVC    A,@A+DPTR
        MOV     40H,A
        MOV     A,#2        ;so can hien thi la 2
        MOVC    A,@A+DPTR
        MOV     41H,A
        MOV     A,#3        ;so can heint hi la 3
        MOVC    A,@A+DPTR
        MOV     42H,A
        MOV     A,#4        ;so can hient hi la 4
        MOVC    A,@A+DPTR
        MOV     43H,A
        MOV     A,#5        ;so can hien thi tren 7-SEG la 5
        MOVC    A,@A+DPTR
        MOV     44H,A
        LCALL   DISPLAY
        SJMP    BEGIN
DISPLAY:
; LED1
        MOV     P2,40H
        CLR     P3.0
        ACALL   DELAY_25
        SETB    P3.0
; LED2
        MOV     P2,41H
        CLR     P3.1
        ACALL   DELAY_25
        SETB    P3.1
; LED 3
        MOV     P2,42H
        CLR     P3.2
        ACALL   DELAY_25
        SETB    P3.2
; LED 4
        MOV     P2,43H
        CLR     P3.3
        ACALL   DELAY_25
        SETB    P3.3
; LED 5
        MOV     P2,44H
        CLR     P3.4
        ACALL   DELAY_25
        SETB    P3.4
        RET
DELAY_25: PUSH 1
          PUSH 0  
        MOV     R1,#10
        MOV     R0,#0
LOOP:   DJNZ    R0,LOOP
        DJNZ    R1,LOOP
        POP 0
        POP 1
        RET
;
LED7SEG:
        DB      0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,98H
        DB      88H,0C6H,86H,8EH,82H,89H
        END
