;CT giao tiep voi ADC0804 va cam bien LM335
;Ket noi giua ADC0804 va MCU AT89S52
; P3.5 = RD
; P3.6 = W/R
; P3.7 = INTR
; P1(0..7) = D0 -> D7
;Chan CS(chon chip) cua ADC noi voi GND (luon luon cho phep ADC hoat dong)
;********************************************  
;Ket noi cac den LED7 thanh voi AT89S52
;a...g = P2
;P0.0 = LED1 Chu C
;P0.1 = LED2 ky tu do
;P0.3 = LED3 Hang don vi
;P0.4 = LED4 hang chuc
;43H - Dia chi RAM chua so hang chuc
;42h - Dia chi RAM chua so hAng don vi
;40h,41h - Dia chi RAM chua ky hieu do C
;*******************************************
; Dinh nghia cac bien tai day

;RDbit bit P2.5
;WRbit bit P2.6
;INTRbit bit P2.7

#include <sfr51.inc>       ;Chen files chua cac dinh nghia ve thanh ghi trong 8051

        ORG    0000h        ; Dia chi bat dau cua Rom
        LJMP   MAIN        
                                    ; Khong can lenh ORG  0003h vi lenh LJMP da la lenh 3 bytes
        ORG     0030H                     ; Dia chi bat dau Chuong trinh chinh
MAIN:                                          ; Khoi tao cac gia tri ban dau
        MOV     P1,#0FFH                ; Thiet lap P1 la chan vao du lieu
        MOV     DPTR,#LED7SEG     ; Dua DPTR tro toi bang giai ma hien thi cho LED7
        SETB    P0.0                        ; Tat het tat ca cac LED
        SETB    P0.1
        SETB    P0.2
        SETB    P0.3
        SETB    P0.7             ;Tat chuong bao dong
        MOV     40H,#C6H         ; Chu C
        MOV     41H,#9CH         ; Ky tu do
        ACALL   laymau
        MOV     A,44h
        MOV     42h,A
        MOV     A,45h
        MOV     43h,A
BACK:   ACALL   laymau
        INC     R6
        CJNE    R6,#100,next
        MOV     R6,#0
        MOV     A,44h
        MOV     42h,A
        MOV     A,45h
        MOV     43h,A
next:   LCALL   getdata          ; Goi chuong trinh hien thi
        LCALL   DISPLAY            ; Goi chuong trinh con hien thi ra LED7
        SETB    P3.5                   ; Dua chan RD len muc cao ket thuc qua trinh lay du lieu
        SJMP    BACK                 ; Chuan bi cho qua trinh chuyen doi tiep theo

laymau:
             CLR     P3.6                ; Tao xung tu cao xuong thap tai chan P3.6(Tuc W/R cua ADC)
             SETB    P3.6               ; Cho phep ADC0804 bat dau qua trinh chuyen doi tu tuong tu sang so
HERE:        JB      P3.7, HERE       ; Doi cho qua trinh chuyen doi xong(100us)
             CLR     P3.5                 ; Dua xung muc thap toi chan RD - cho phep doc du lieu tu ADC(Xuat ra D0..D7)
             MOV     A,P1               ; Dua du lieu 8bit tu P1 den thanh ghi A
             setb   p3.5
             LCALL   BIN2BCD        ; tra bang, doi BCD -> LED 7 doan
             RET

getdata:
        MOV     A,42h                    ; Lay thong tin LED7 cua so don vi
        MOVC    A,@A+DPTR
        MOV     42h,A
        MOV     A,43H                   ; Lay ma LED7 cua so hang chuc
        MOVC    A,@A+DPTR
        MOV     43H,A
        RET
DISPLAY:
        MOV     P2,40H
        CLR     P0.0            ;Bat LED1
        ACALL   DELAY
        SETB    P0.0            ;TAT LED 1
        MOV     P2,41H
        CLR     P0.1
        ACALL   DELAY
        SETB    P0.1            ;TAT LED 2
        MOV     P2,42H          ; LED1 Hang don vi
        CLR     P0.2            ; ba^.t LED1 sa'ng
        ACALL   DELAY           ; delay
        SETB    P0.2            ; ta('t LED1
        MOV     P2,43H          ; LED2  hang chuc
        CLR     P0.3            ; ba^.t LED2 sa'ng
        ACALL   DELAY           ; delay
        SETB    P0.3            ; ta('t LED2
        RET

;Chuong trinh chuyen doi tu so hex sang BCD
; gia tri doc tu ADC lu trong thanh ghi A
BIN2BCD:
        CLR     c
        SUBB    a,#139
        CLR     c
        RLC     a
        MOV     b,#10
        DIV     ab
        MOV     44H,b
        MOV     45H,a
        RET
DELAY:                                  ; Tao tre trong qua trinh quet cac LED7 Thanh
             MOV     R1,#2                 ; Thoi gian quet moi LED la 2x255us
LOOP:   MOV     R0,#50          ; Nguyen tac quet LED la cac LED duoc quet lien tiep nhau trong thoi gian rat ngan
LOOP2: 
             DJNZ    R0,LOOP2        ; Do su luu anh tren vong mac nguoi nen ta thay cac LED nhu sang lien tuc
             DJNZ    R1,LOOP             ; Thoi gian quet tuy thuoc ham DELAY, gia tri R1 cang lon thi cac LED se nhap nhay
             RET                                 ; Toi thieu phai dam bao du 24hinh/giay


LED7SEG:
        DB      0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H
        DB      88H,0C6H,86H,8EH,82H,89H,7FH,FFH
        END        

