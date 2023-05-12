#include <sfr51.inc>
;KIEM TRA CAC PHIM
KEYSCAN:MOV     R7,#20
        MOV  A,#FFH
        MOV  P2,A   
        JNB  P1.0,PRO1   ;GOI CT
        JNB  P1.1,PRO2
        JNB  P1.2,PRO3
        JNB  P1.3,PRO4
        JNB  P1.4,CALL1
        JNB  P1.5,CALL2
        JNB  P1.6,CALL3
        JNB  P1.7,CALL4
        SJMP KEYSCAN

CALL1:  LJMP    PRO5        ;Do PRO5 o trang 2K khac nen phai dung dia chi phu de goi
CALL2:  LJMP    PRO6
CALL3:  LJMP    PRO7
CALL4:  LJMP    PRO8

PRO1:   MOV     A0h,#00h    ;nap 00 vao port 0 de tat 8 Led
        LCALL   DELAY2     ;Goi chuong trinh con cua delay
        MOV     A0h,#0ffh   ;Nap ff vao port0 de sang 8 led
        LCALL   DELAY2     ;goi chuong trinh con delay
        DJNZ    R7,PRO1     
        LJMP    KEYSCAN     ;nhay den de lam lai tu dau

PRO2:   MOV     P2, #0FFh   ;Xac lap trang thi dau P2.x = 0
        CLR     P2.0
        LCALL   DELAY
        SETB    P2.0
        CLR     P2.1
        LCALL   DELAY
        SETB    P2.1
        CLR     P2.2
        LCALL   DELAY
        SETB    P2.2
        CLR     P2.3
        LCALL   DELAY
        SETB    P2.3
        CLR     P2.4
        LCALL   DELAY
        SETB    P2.4
        CLR     P2.5
        LCALL   DELAY
        SETB    P2.5
        CLR     P2.6
        LCALL   DELAY
        SETB    P2.6
        CLR     P2.7
        LCALL   DELAY
        SETB    P2.7
        DJNZ    R7,PRO2
        LJMP    KEYSCAN
        ;ret            ;Quay tro lai tu dau

;Nhay led xen ke nhau
PRO3:   MOV     A,#55H          ; A = 55h
        MOV     P2,A            ; P1 = 55h
        LCALL   DELAY2           ; 
        MOV     A,#0AAH         ; A = AAh
        MOV     P2,A            ; P1 = AAh
        LCALL   DELAY2    
        DJNZ    R7,PRO3
        LJMP    KEYSCAN
        ;ret          
PRO4:   MOV     A,#FEH  ; Nhap 11111110
        MOV     P2,A
        LCALL   DELAY
        MOV     A,#FCH  ;11111100
        MOV     P2,A
        LCALL   DELAY
        MOV     A,#F9H  ;11111001
        MOV     P2,A
        LCALL   DELAY
        MOV     A,#F3H  ;11110011
        MOV     P2,A
        LCALL   DELAY
        MOV     A,#E7H  ;11100111
        MOV     P2,A
        LCALL   DELAY
        MOV     A,#CFH  ;11001111
        MOV     P2,A
        LCALL   DELAY
        MOV     A,#9FH  ;10011111
        MOV     P2,A
        LCALL   DELAY
        MOV     A,#3FH  ;00111111
        MOV     P2,A
        LCALL   DELAY
        MOV     A,#7FH  ;01111111
        MOV     P2,A
        LCALL   DELAY
        MOV     A,#FFH  ;11111111
        MOV     P2,A
        LCALL   DELAY
        DJNZ    R7,PRO4
        LJMP    KEYSCAN
        ;RET
PRO5:   MOV     P2,#7EH          ;NHAP GIA TRI 01111110
        LCALL   DELAY
        MOV     P2,#BDH          ; 10111101
        LCALL   DELAY
        MOV     P2,#DBH          ; 11011011
        LCALL   DELAY
        MOV     P2,#E7H          ;11100111
        LCALL   DELAY
        MOV     P2,#DBH
        LCALL   DELAY
        MOV     P2,#BDH
        LCALL   DELAY
        MOV     P2,#7EH
        LCALL   DELAY
        djnz    r7,PRO5
        LJMP    KEYSCAN

PRO6:   MOV     A,#FEH  ; Nhap 11111110
        MOV     P2,A
        LCALL   DELAY2
        MOV     A,#FCH  ;11111100
        MOV     P2,A
        LCALL   DELAY2
        MOV     A,#F8H  ;11111000
        MOV     P2,A
        LCALL   DELAY2
        MOV     A,#F0H  ;11110000
        MOV     P2,A
        LCALL   DELAY2
        MOV     A,#E0H  ;11100000
        MOV     P2,A
        LCALL   DELAY2
        MOV     A,#C0H  ;11000000
        MOV     P2,A
        LCALL   DELAY2
        MOV     A,#80H  ;10000000
        MOV     P2,A
        LCALL   DELAY2
        MOV     A,#00H  ;00000000
        MOV     P2,A
        LCALL   DELAY2        
        DJNZ    R7,PRO6
        LJMP    KEYSCAN

        
        ;Chuong trinh dem xung o P3.4
PRO7:   ORG     8100H
        MOV     DPTR,#LED7SEG   ; DPTR tro? dde^'n ba?ng ma~ LED
        MOV     TMOD,#06h       ; counter 0, mode 2
        MOV     TH0,#0
        SETB    P3.0            ; ta('t ta^'t ca? ca'c LED
        SETB    P3.4            ; P3.4: input
        SETB    TR0             ; cho phe'p counter 0 cha.y
BEGIN:  MOV     A,TL0
        LCALL   BIN2BCD         ; tra ba?ng, ddo^?i BCD -> LED 7 ddoa.n
        MOV     A,40h           
        MOVC    A,@A+DPTR
        MOV     40h,A
        LCALL   DISPLAY
        SJMP    BEGIN
DISPLAY:
        MOV     P2,40H          ; LED1
        CLR     P3.0            ; ba^.t LED1 sa'ng
        ACALL   DELAY3           ; delay
       ; ta('t LED1
        ;MOV     P2,41H          ; LED2
        ;CLR     P3.0            ; ba^.t LED2 sa'ng
        ;ACALL   DELAY           ; delay
       ; ta('t LED2

        RET
BIN2BCD:
        MOV     B,#10           ; B=10
        DIV     AB              ; chia cho 10
        MOV     40h,B           ; lu+u digit tha^'p
        MOV     41h,B           ;LUU DIGIT CAO VAO P
        RET
DELAY3:
        MOV     R1,#10
        MOV     R0,#0FFh
LOOP:   DJNZ    R0,LOOP
        DJNZ    R1,LOOP
        RET
;
LED7SEG:
        DB      0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H    ;90h LAM CHO SO 9 TREN SEG-7 LA 6 GACH
        DB      88H,0C6H,86H,8EH,82H,89H

        ;Chuong trinh dem nguoc xung vao tu P3.4
PRO8:   ORG     8100H
        MOV     DPTR,#LED7SEG1   ; DPTR tro? dde^'n ba?ng ma~ LED
        MOV     TMOD,#06h       ; counter 0, mode 2
        MOV     TH0,#0
        SETB    P3.0            ; ta('t ta^'t ca? ca'c LED
        SETB    P3.4            ; P3.4: input
        SETB    TR0             ; cho phe'p counter 0 cha.y
BEGIN1:  MOV     A,TL0
        LCALL   BIN2BCD1         ; tra ba?ng, ddo^?i BCD -> LED 7 ddoa.n
        MOV     A,40h           
        MOVC    A,@A+DPTR
        MOV     40h,A
        LCALL   DISPLAY1
        SJMP    BEGIN1
DISPLAY1:
        MOV     P2,40H          ; LED1
        CLR     P3.0            ; ba^.t LED1 sa'ng
        ACALL   DELAY4           ; delay
       ; ta('t LED1
        ;MOV     P2,41H          ; LED2
        ;CLR     P3.0            ; ba^.t LED2 sa'ng
        ;ACALL   DELAY           ; delay
       ; ta('t LED2
        RET
BIN2BCD1:
        MOV     B,#10           ; B=10
        DIV     AB              ; chia cho 10
        MOV     40h,B           ; lu+u digit tha^'p
        MOV     41h,B           ;LUU DIGIT CAO VAO P
        RET
DELAY4:
        MOV     R1,#10
        MOV     R0,#0FFh
LOOP:   DJNZ    R0,LOOP
        DJNZ    R1,LOOP
        RET

LED7SEG1:
        DB      0C0H,90H,80H,0F8H,82H,92H,99H,0B0H,0A4H,0F9H
        ;DB      0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H    ;90h LAM CHO SO 9 TREN SEG-7 LA 6 GACH
        DB      88H,0C6H,86H,8EH,82H,89H
        

        
DELAY:    MOV R2,#100     ;DELAY 100MS
DL5:      MOV R1,#250            
DL4:      NOP
          NOP
          DJNZ R1,DL4
          DJNZ R2,DL5
          RET
DELAY2:    MOV R2,#1000     ;DELAY 500MS
DL7:      MOV R1,#250            
DL6:      NOP
          NOP
          DJNZ R1,DL6
          DJNZ R2,DL7
          RET
        END
