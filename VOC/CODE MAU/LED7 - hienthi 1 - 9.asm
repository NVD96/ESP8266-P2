#include <sfr51.inc>
; a,b,c,d,e,f,g -> Port 2
; P3.0 -> LED1
        ORG     0H
        MOV     P3,#0FFH
        MOV     DPTR,#LED7SEG
BEGIN:  
        SETB    P2.7
        MOV     A,#0        ;so can hien thi tren 7-SEG la 5
        MOVC    A,@A+DPTR
        MOV     39H,A
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
        MOV     A,#6        ;so can hien thi tren 7-SEG la 6
        MOVC    A,@A+DPTR
        MOV     45H,A
        MOV     A,#7        ;so can hien thi tren 7-SEG la 7
        MOVC    A,@A+DPTR
        MOV     46H,A
        MOV     A,#8        ;so can hien thi tren 7-SEG la 8
        MOVC    A,@A+DPTR
        MOV     47H,A
        MOV     A,#9        ;so can hien thi tren 7-SEG la 9
        MOVC    A,@A+DPTR
        MOV     48H,A
        LCALL   DISPLAY
        SJMP    BEGIN
DISPLAY:
; LED 0
        MOV     P2,39H
        CLR     P3.0
        ACALL   DELAY_25
; LED1
        MOV     P2,40H
        CLR     P3.0
        ACALL   DELAY_25
; LED2
        MOV     P2,41H
        CLR     P3.0
        ACALL   DELAY_25
; LED 3
        MOV     P2,42H
        CLR     P3.0
        ACALL   DELAY_25
; LED 4
        MOV     P2,43H
        CLR     P3.0
        ACALL   DELAY_25
; LED 5
        MOV     P2,44H
        CLR     P3.0
        ACALL   DELAY_25
; LED 6
        MOV     P2,45H
        CLR     P3.0
        ACALL   DELAY_25
; LED 7
        MOV     P2,46H
        CLR     P3.0
        ACALL   DELAY_25
; LED 8
        MOV     P2,47H
        CLR     P3.0
        ACALL   DELAY_25
; LED 9
        MOV     P2,48H
        CLR     P3.0
        ACALL   DELAY_25
        RET
DELAY_25: PUSH 1
          PUSH 0  
        MOV     R1,#10
        MOV     R0,#0FFH
LOOP:   DJNZ    R0,LOOP
        DJNZ    R1,LOOP
        POP 0
        POP 1
        RET
;
LED7SEG:
        DB      0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H
        DB      88H,0C6H,86H,8EH,82H,89H,7FH,FFH
        END