#include <sfr51.inc>
;KIEM TRA CAC PHIM
        org 0
        MOV  A,#0FFH
        MOV  P3,A
        
KEYSCAN:MOV  R7,#20
        mov  r6,#10
        mov  p2,#0FFh
        JNB  P3.0,PRO1   ;GOI CT
        JNB  P3.1,PRO2   ;goi Ct con
        JNB  P3.2,PRO3
        JNB  P3.3,PRO4
        JNB  P3.4,CALL1
        JNB  P3.5,CALL2
        jnb  p3.6,call3
        jnb  p3.7,call4
        SJMP KEYSCAN

CALL1:  LJMP    PRO5        ;Do PRO5 o trang 2K khac nen phai dung dia chi phu de goi
CALL2:  LJMP    PRO6
call3:  ljmp    pro7
call4:  ljmp    pro8
PRO1:   MOV     A0h,#00h    ;nap 00 vao port 0 de tat 8 Led
        ACALL   DELAY2     ;Goi chuong trinh con cua delay
        MOV     A0h,#0ffh   ;Nap ff vao port0 de sang 8 led
        ACALL   DELAY2     ;goi chuong trinh con delay
        DJNZ    R7,PRO1     
        LJMP    KEYSCAN     ;nhay den de lam lai tu dau

PRO2:   MOV     P1, #0FFh   ;Xac lap trang thi dau P2.x = 0
        CLR     P1.0
        LCALL   DELAY
        SETB    P1.0
        CLR     P1.1
        LCALL   DELAY
        SETB    P1.1
        CLR     P1.2
        LCALL   DELAY
        SETB    P1.2
        CLR     P1.3
        LCALL   DELAY
        SETB    P1.3
        CLR     P1.4
        LCALL   DELAY
        SETB    P1.4
        CLR     P1.5
        LCALL   DELAY
        SETB    P1.5
        CLR     P1.6
        LCALL   DELAY
        SETB    P1.6
        CLR     P1.7
        LCALL   DELAY
        SETB    P1.7
        DJNZ    R7,PRO2
        LJMP    KEYSCAN
        ;ret            ;Quay tro lai tu dau

;Nhay led xen ke nhau
PRO3:   MOV     A,#55H          ; A = 55h
        MOV     P1,A            ; P1 = 55h
        LCALL   DELAY2           ; 
        MOV     A,#0AAH         ; A = AAh
        MOV     P1,A            ; P1 = AAh
        LCALL   DELAY2    
        DJNZ    R7,PRO3
        LJMP    KEYSCAN
        ;ret          
PRO4:   MOV     A,#FEH  ; Nhap 11111110
        MOV     P1,A
        LCALL   DELAY
        MOV     A,#FCH  ;11111100
        MOV     P1,A
        LCALL   DELAY
        MOV     A,#F9H  ;11111001
        MOV     P1,A
        LCALL   DELAY
        MOV     A,#F3H  ;11110011
        MOV     P1,A
        LCALL   DELAY
        MOV     A,#E7H  ;11100111
        MOV     P1,A
        LCALL   DELAY
        MOV     A,#CFH  ;11001111
        MOV     P1,A
        LCALL   DELAY
        MOV     A,#9FH  ;10011111
        MOV     P1,A
        LCALL   DELAY
        MOV     A,#3FH  ;00111111
        MOV     P1,A
        LCALL   DELAY
        MOV     A,#7FH  ;01111111
        MOV     P1,A
        LCALL   DELAY
        MOV     A,#FFH  ;11111111
        MOV     P1,A
        LCALL   DELAY
        DJNZ    R7,PRO4
        LJMP    KEYSCAN
        ;RET
PRO5:   MOV     P1,#7EH          ;NHAP GIA TRI 01111110
        LCALL   DELAY
        MOV     P1,#BDH          ; 10111101
        LCALL   DELAY
        MOV     P1,#DBH          ; 11011011
        LCALL   DELAY
        MOV     P1,#E7H          ;11100111
        LCALL   DELAY
        MOV     P1,#DBH
        LCALL   DELAY
        MOV     P1,#BDH
        LCALL   DELAY
        MOV     P1,#7EH
        LCALL   DELAY
        djnz    r7,PRO5
        LJMP    KEYSCAN

PRO6:   MOV     A,#FFH  ; Nhap 11111111
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#FEH  ; Nhap 11111110
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#FCH  ;11111100
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#F8H  ;11111000
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#F0H  ;11110000
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#E0H  ;11100000
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#C0H  ;11000000
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#80H  ;10000000
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#00H  ;00000000
        MOV     P1,A
        LCALL   DELAY2
        ;tat dan
        MOV     A,#01 ; Nhap 00000001
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#03H  ; Nhap 00000011
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#07H  ;00000111
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#0FH  ;00001111
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#1FH  ;00011111
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#3FH  ;00111111
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#7FH  ;01111111
        MOV     P1,A
        LCALL   DELAY2
        DJNZ    R7,PRO6
        LJMP    KEYSCAN

Pro7:   MOV     A,#FFH  ; Nhap 11111111
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#FEH  ; Nhap 11111110
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#FAH  ;11111010
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#EAH  ;11101010
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#AAH  ;10101010
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#FFH  ;11111111
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#7FH  ;01111111
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#5FH  ;01011111
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#57H  ;01010111
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#55H  ;01010101
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#FFH  ;11111111
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#00H  ;00000000
        MOV     P1,A
        LCALL   DELAY2
        ;tat dan
        MOV     A0h,#ffh    ;nap 00 vao port 0 de tat 8 Led
        LCALL   DELAY2      ;Goi chuong trinh con cua delay
        MOV     A0h,#00h   ;Nap ff vao port0 de sang 8 led
        LCALL   DELAY2      ;goi chuong trinh con delay
        MOV     A0h,#ffh    ;nap 00 vao port 0 de tat 8 Led
        LCALL   DELAY2     ;Goi chuong trinh con cua delay
        MOV     A0h,#00h   ;Nap ff vao port0 de sang 8 led
        LCALL   DELAY2     ;goi chuong trinh con delay
        MOV     A0h,#ffh    ;nap 00 vao port 0 de tat 8 Led
        LCALL   DELAY2     ;Goi chuong trinh con cua delay
        MOV     A0h,#00h   ;Nap ff vao port0 de sang 8 led
        LCALL   DELAY2     ;goi chuong trinh con delay
        MOV     A0h,#ffh    ;nap 00 vao port 0 de tat 8 Led
        LCALL   DELAY2     ;Goi chuong trinh con cua delay
        MOV     A0h,#00h   ;Nap ff vao port0 de sang 8 led
        LCALL   DELAY2     ;goi chuong trinh con delay
        djnz    R6,CALL5     
        LJMP    KEYSCAN 
CALL5:  LJMP    PRO7
pro8:   MOV     P1,#7EH          ;NHAP GIA TRI 01111110
        LCALL   DELAY
        MOV     P1,#BDH          ; 10111101
        LCALL   DELAY
        MOV     P1,#DBH          ; 11011011
        LCALL   DELAY
        MOV     P1,#E7H          ;11100111
        LCALL   DELAY
        MOV     P1,#DBH
        LCALL   DELAY
        MOV     P1,#BDH
        LCALL   DELAY
        MOV     P1,#7EH
        LCALL   DELAY
        djnz    r6,PRO8
        
        MOV     A,#FFH  ; Nhap 11111111
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#FEH  ; Nhap 11111110
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#FCH  ;11111100
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#F8H  ;11111000
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#F0H  ;11110000
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#E0H  ;11100000
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#C0H  ;11000000
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#80H  ;10000000
        MOV     P1,A
        LCALL   DELAY2
        MOV     A,#00H  ;00000000
        MOV     P1,A
        LCALL   DELAY2
        ;tat dan
        
        MOV     A0h,#ffh    ;nap 00 vao port 0 de tat 8 Led
        ACALL   DELAY2      ;Goi chuong trinh con cua delay
        MOV     A0h,#00h   ;Nap ff vao port0 de sang 8 led
        ACALL   DELAY2      ;goi chuong trinh con delay
        MOV     A0h,#ffh    ;nap 00 vao port 0 de tat 8 Led
        ACALL   DELAY2     ;Goi chuong trinh con cua delay
        MOV     A0h,#00h   ;Nap ff vao port0 de sang 8 led
        ACALL   DELAY2     ;goi chuong trinh con delay
        MOV     A0h,#ffh    ;nap 00 vao port 0 de tat 8 Led
        ACALL   DELAY2     ;Goi chuong trinh con cua delay
        MOV     A0h,#00h   ;Nap ff vao port0 de sang 8 led
        ACALL   DELAY2     ;goi chuong trinh con delay
        MOV     A0h,#ffh    ;nap 00 vao port 0 de tat 8 Led
        ACALL   DELAY2     ;Goi chuong trinh con cua delay
        MOV     A0h,#00h   ;Nap ff vao port0 de sang 8 led
        ACALL   DELAY2  

        MOV     A,#01 ; Nhap 00000001
        MOV     P1,A
        ACALL   DELAY2
        MOV     A,#03H  ; Nhap 00000011
        MOV     P1,A
        ACALL   DELAY2
        MOV     A,#07H  ;00000111
        MOV     P1,A
        ACALL   DELAY2
        MOV     A,#0FH  ;00001111
        MOV     P1,A
        ACALL   DELAY2
        MOV     A,#1FH  ;00011111
        MOV     P1,A
        ACALL   DELAY2
        MOV     A,#3FH  ;00111111
        MOV     P1,A
        ACALL   DELAY2
        MOV     A,#7FH  ;01111111
        MOV     P1,A
        ACALL   DELAY2
;        DJNZ    R6,pro8
        LJMP    KEYSCAN

         
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
