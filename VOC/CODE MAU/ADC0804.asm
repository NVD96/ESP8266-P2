;CT giao tiep voi ADC0804 va cam bien LM35D
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

#include <sfr51.inc>    ;Chen files chua cac dinh nghia ve thanh ghi trong 8051

        ORG 0000h       ; Dia chi bat dau cua Rom
        LJMP MAIN
        ;LJMP EX0ISR     ; Dia chi bat dau vecto ngat ngoai
        ;ORG 000BH       ; Vector ngat Timer0
        ;LJMP T0ISR      
        ;ORG 001BH       ; Vector ngat Timer1
        ;LJMP T1ISR



        ORG 0030H               ; Dia chi bat dau Chuong trinh chinh
MAIN:   ; Khoi tao cac gia tri ban dau
        ;SETB IT0                ; TAC DONG CANH AM (cho chan P3.2 la chan vao cua ngat ngoai)
                                ; Khi co xung tu cao xuong thap thi ngat xay ra
        ;MOV TMOD,#11H           ; CHE DO 16 BIT cho Timer0 va Timer1
        ;MOV IE,#81H             ; Gan 81h vao thanh ghi IE la thanh ghi cho phep ngat
                                ; Cho phep cac ngat hoat dong (ngat ngoai,ngat do Timer0,1)
        MOV P1,#0FFH            ; Thiet lap P1 la chan vao du lieu
        MOV DPTR,#LED7SEG       ; Dua DPTR tro toi bang giai ma hien thi cho LED7
        SETB P0.0               ; Tat het tat ca cac LED
        SETB P0.1
        SETB P0.2
        SETB P0.3
        SETB P0.7               ;TAT CHUONG BAO DONG
        MOV  40H,#C6H           ; Chu C
        MOV  41H,#9CH           ; Ky tu do
        acall laymau
        mov a,44h
        mov 42h,a
        mov a,45h
        mov 43h,a

BACK:   
        acall laymau
        inc r6
        cjne r6,#200,next
;        inc r5
;        cjne r5,#100,next
        mov r6,#0
;        mov r5,#0
        mov a,44h
        mov 42h,a
        mov a,45h
        mov 43h,a
next:   LCALL   DISPLAY1        ; Goi chuong trinh hien thi
        LCALL   DISPLAY         ; Goi chuong trinh con hien thi ra LED7
        SETB    P3.5            ; Dua chan RD len muc cao ket thuc qua trinh lay du lieu
        SJMP    BACK            ; Chuan bi cho qua trinh chuyen doi tiep theo

laymau:
        CLR     P3.6            ; Tao xung tu cao xuong thap tai chan P3.6(Tuc W/R cua ADC)
        SETB    P3.6            ; Cho phep ADC0804 bat dau qua trinh chuyen doi tu tuong tu sang so
HERE:   JB      P3.7, HERE      ; Doi cho qua trinh chuyen doi xong(100us)
        CLR     P3.5           ; Dua xung muc thap toi chan RD - cho phep doc du lieu tu ADC(Xuat ra D0..D7)
        MOV     A,P1            ; Dua du lieu 8bit tu P1 den thanh ghi A
        LCALL   BIN2BCD         ; tra bang, doi BCD -> LED 7 doan
        ret

DISPLAY1:
        MOV     A,42h           ; Lay thong tin LED7 cua so don vi
        MOVC    A,@A+DPTR
        MOV     42h,A
        MOV     A,43H           ; Lay ma LED7 cua so hang chuc
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

BIN2BCD:;Chuong trinh chuyen doi tu so hex sang BCD
; gia tri doc tu ADC lu trong thanh ghi A
        clr     c
        subb    a,#139
        clr     c
        rlc     a
        mov     b,#10
        div     ab
        mov     44H,b
        mov     45H,a
        RET
DELAY:                          ; Tao tre trong qua trinh quet cac LED7 Thanh
        MOV     R1,#2           ; Thoi gian quet moi LED la 2x255us
LOOP:   MOV     R0,#50        ; Nguyen tac quet LED la cac LED duoc quet lien tiep nhau trong thoi gian rat ngan
loop2:  DJNZ    R0,LOOP2         ; Do su luu anh tren vong mac nguoi nen ta thay cac LED nhu sang lien tuc
        DJNZ    R1,LOOP        ; Thoi gian quet tuy thuoc ham DELAY, gia tri R1 cang lon thi cac LED se nhap nhay
        RET                     ; Toi thieu phai dam bao du 24hinh/giay

EX0ISR:                         ;Bat dau chuong trinh ngat tao am hieu 500Hz tai chan P0.6
        MOV R7,#20              ;Khi ngat ngoai xay ra thi chuong trinh EX0ISR duoc goi, NAp 20 vao thanh ghi R7
        SETB TF0                ; Bat co ngat TF0 
        SETB TF1                ; Bat co ngat TF1 
        SETB ET0                ; Cho phep ngat boi Timer0 hoat dong
        SETB ET1                ; Cho phep ngat boi Timer1 hoat dong
        RETI                    ; Tro ve chuong trinh chinh
; De su dung ngat noi chan P0.7 voi chan P3.2, con chan P0.6 noi ra loa ben ngoai
; se tao ra am hieu 500Hz trong 1 giay
T0ISR: ;Ngat do Timer0
        CLR TR0                 ; Xoa TR0, tam ngung, khong cho bo dinh thoi Tomer0 hoat dong
        DJNZ R7,SKIP            ; Giam R7 di 1 va nhay nay R7 =0
        CLR ET0
        CLR ET1
        LJMP EXIT
SKIP:
        MOV TH0,#HIGH(-50000)   ;Nap gia tri ban dau cho TH0 va TL0
        MOV TL0,#LOW(-50000)
        SETB TR0
EXIT:   RETI 

T1ISR: ;Ngat do Timer1
        CLR TR1
        MOV TH1,#HIGH(-1250)
        MOV TL1,#LOW(-1250)
        CPL P0.6
        SETB TR1
        RETI

LED7SEG:
        DB      0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H
        DB      88H,0C6H,86H,8EH,82H,89H,7FH,FFH
        END        

