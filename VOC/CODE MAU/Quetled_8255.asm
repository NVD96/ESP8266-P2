#include <sfr51.inc>
;       quetled_8255
ORG     0H
        MOV     DPTR,#4003H
        MOV     A,#80H
        MOVX    @DPTR,A
        MOV     P3,#0FFH
        MOV     DPTR,#LED7SEG
BEGIN:  
        MOV     A,#4
        MOVC    A,@A+DPTR
        MOV     40H,A
        MOV     A,#3
        MOVC    A,@A+DPTR
        MOV     41H,A
        MOV     A,#2
        MOVC    A,@A+DPTR
        MOV     42H,A
        MOV     A,#1
        MOVC    A,@A+DPTR
        MOV     43H,A
        LCALL   DISPLAY
        SJMP    BEGIN
DISPLAY:
        PUSH    DPH
        PUSH    DPL
        MOV     A,40H
        MOV     DPTR,#4000H
        MOVX    @DPTR,A
        MOV     DPTR,#4001H     ; chon LED o PB
        MOV     A,#0FEH
        MOVX    @DPTR,A
        ACALL   DELAY_25
        MOV     A,41H           ; xuat nd o nho 41h ra PA
        MOV     DPTR,#4000H
        MOVX    @DPTR,A
        MOV     DPTR,#4001H     ; cho.n
        MOV     A,#0FDH
        MOVX    @DPTR,A
        ACALL   DELAY_25
        MOV     A,42H
        MOV     DPTR,#4000H
        MOVX    @DPTR,A
        MOV     DPTR,#4001H
        MOV     A,#0FBH
        MOVX    @DPTR,A
        ACALL   DELAY_25
        MOV     A,43H
        MOV     DPTR,#4000H
        MOVX    @DPTR,A
        MOV     DPTR,#4001H
        MOV     A,#0F7H
        MOVX    @DPTR,A
        ACALL   DELAY_25
        POP     DPL
        POP     DPH
        RET
DELAY_25:
        MOV     R1,#10
        MOV     R0,#0
LOOP:   DJNZ    R0,LOOP
        DJNZ    R1,LOOP
        RET
;
LED7SEG:
        DB      0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,98H
        DB      88H,0C6H,86H,8EH,82H,89H
        END

