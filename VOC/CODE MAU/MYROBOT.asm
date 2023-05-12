#include <sfr52.inc>
        ORG 0000H
        LJMP MAIN



        ORG OO30H
MAIN:   MOV P3,#FFH
        MOV P1,#40H
        MOV     DPTR,#LED7SEG   ; DPTR tro? dde^'n ba?ng ma~ LED
        MOV     TMOD,#02h       ; Timer 0, mode 2,C/T =0 che do dinh thoi
        MOV     TH0,#0
        SETB    P3.0            ; ta('t ta^'t ca? ca'c LED
        ;SETB    P3.4           ; P3.4: input
        SETB    TR0             ; cho phe'p counter 0 cha.y
        JNB P1.6,$              ;Kiem tra cong tac tai chan P1.6
LOOP1:  MOV     A,TL0
        LCALL   BIN2BCD         ; tra ba?ng, ddo^?i BCD -> LED 7 ddoa.n
        MOV     A,40h           
        MOVC    A,@A+DPTR
        MOV     40h,A
        LCALL   DISPLAY
        ;SJMP    BEGIN

LOOP2:  MOV A,P3
R:      CJNE A,#15,FOW  ;Turn right
        MOV P1,#68H
        ACALL DELAY        
        ;SJMP RIGHT
        SJMP LOOP1
FOW:    CJNE A,#9,L  ;Forward(high)
        MOV P1,#64H
        ACALL DELAY
        ;SJMP FORWH
        SJMP LOOP1
L:      CJNE A,#8,FL  ;Turn left
        MOV P1,#54H
        ACALL DELAY
        ;SJMP LEFT
        SJMP LOOP1
FL:     CJNE A,#11,EXIT  ;Forward(low)
        MOV P1,#64H
        ACALL DELAY
        ;SJMP FORWL
        SJMP LOOP1
EXIT:   SJMP LOOP1

DISPLAY:
        MOV     P2,40H          ; LED1
        CLR     P3.0            ; ba^.t LED1 sa'ng
        ACALL   DELAY1           ; delay
        RET
BIN2BCD:
        MOV     B,#10           ; B=10
        DIV     AB              ; chia cho 10
        MOV     40h,B           ; lu+u digit tha^'p
        MOV     41h,B           ;LUU DIGIT CAO VAO P
        RET
DELAY1: MOV     R1,#10
        MOV     R0,#0FFh
LOOP:   DJNZ    R0,LOOP
        DJNZ    R1,LOOP
        RET
LED7SEG:
        DB      0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H    ;90h LAM CHO SO 9 TREN SEG-7 LA 6 GACH
        DB      88H,0C6H,86H,8EH,82H,89H
        

DELAY:  MOV R2,#10
DL7:        MOV R1,#250            
DL6:        NOP
            NOP
            DJNZ R1,DL6
            DJNZ R2,DL7
            RET
END
