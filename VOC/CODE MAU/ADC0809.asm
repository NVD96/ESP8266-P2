#include <sfr51.inc>    ;Chen files chua cac dinh nghia ve thanh ghi trong 8051
;CT giao tiep voi ADC0809 va cam bien LM335
;Ket noi giua ADC0809 va MCU AT89S52
; P3.6 = Start
; P3.7 = Output_enable
; P1(0..7) = D0 -> D7
;********************************************  
;Ket noi cac den LED7 thanh voi AT89S52
;a...g = P2
;P0.0 = LED1 Chu C
;P0.1 = LED2 ky tu do
;P0.2 = LED3 Hang don vi
;P0.3 = LED4 hang chuc
;43H - Dia chi RAM chua so hang chuc
;42h - Dia chi RAM chua so hAng don vi
;40h,41h - Dia chi RAM chua ky hieu do C
;*******************************************
; Dinh nghia cac bien tai day
in_port equ     30h
count   equ     31h

Out_enable  bit P3.7
Start       bit P3.6

        ORG 0000h       ; Dia chi bat dau cua Rom
        LJMP MAIN

        ;ORG 0003h
        ;LJMP Read_ADC0809     ; Dia chi bat dau vecto ngat ngoai
        ;ORG 000BH       ; Vector ngat Timer0
        ;LJMP T0ISR      
        ;ORG 001BH       ; Vector ngat Timer1
        ;LJMP T1ISR

        ORG 0030H               ; Dia chi bat dau Chuong trinh chinh
MAIN:   ; Khoi tao cac gia tri ban dau
        ;SETB IT0                ; TAC DONG CANH AM (cho chan P3.2 la chan vao cua ngat ngoai)
                                ; Khi co xung tu cao xuong thap thi ngat xay ra
        ;MOV TMOD,#11H           ; CHE DO 16 BIT cho Timer0 va Timer1
        ;MOV TH0,#HIGH(-50000)   ;Nap gia tri ban dau cho TH0 va TL0
        ;MOV TL0,#LOW(-50000)
        ;MOV IE,#82H             ; Gan 81h vao thanh ghi IE la thanh ghi cho phep ngat
                                ; Cho phep cac ngat hoat dong (ngat ngoai,ngat do Timer0,1)
        MOV P1,#0FFH            ; Thiet lap P1 la chan vao du lieu
        MOV P3,#0FFH
        MOV DPTR,#LED7SEG       ; Dua DPTR tro toi bang giai ma hien thi cho LED7
        SETB P0.0               ; Tat het tat ca cac LED
        SETB P0.1
        SETB P0.2
        SETB P0.3
        CLR  Start
        CLR  Out_enable
        MOV  40H,#C6H           ; Chu C
        MOV  41H,#9CH           ; Ky tu do
        ;acall send_start
        ;acall Read_ADC0809
        mov R6,#0
        mov count,#10
        ;Setb TR0
        acall send_start
        acall read_0809
BACK:   
        acall send_start
        acall read_0809
next:                           ; Goi chuong trinh hien thi
        LCALL   DISPLAY         ; Goi chuong trinh con hien thi ra LED7
        SJMP    BACK            ; Chuan bi cho qua trinh chuyen doi tiep theo

read_0809:
        setb out_enable
        mov A,P1
        LCALL BIN2BCD
        clr out_enable
        ret

Read_ADC0809:
        nop
HERE:   JB     P3.2, HERE      ; Doi cho qua trinh chuyen doi xong(100us)
        setb    out_enable       ; Dua xung muc thap toi chan RD - cho phep doc du lieu tu ADC(Xuat ra D0..D7)
        nop
        nop
        MOV     A,P1            ; Dua du lieu 8bit tu P1 den thanh ghi A
        LCALL   BIN2BCD         ; tra bang, doi BCD -> LED 7 doan
        clr     out_enable
        ret

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
        clr     c       ; Tinh toan gia tri chuyen doi ve do C
        subb    a,#139  ; Tru di muc dien ap ban dau 2,73V de chuyen doi tu do K ve do C
        clr     c
        rlc     a       ;Dich trai A 1 vi tri = Nhan A voi 2
        mov     b,#10   ; Chia A cho 10 de tach so hang chuc va don vi
        div     ab      ; dua ra hien thi LED 7
        mov     42H,b   ; Bien luu so hang don vi
        mov     43H,a   ; Bien luu so hang chuc

        MOV     A,42h           ; Lay thong tin ma LED7 cua so don vi
        MOVC    A,@A+DPTR
        MOV     42h,A
        MOV     A,43H           ; Lay ma LED7 cua so hang chuc
        MOVC    A,@A+DPTR
        MOV     43H,A
        RET
Send_start:
        SETB Start
        nop
        nop
        nop
        CLR  Start
        acall delay120us
        ret
delay120us:
        MOV TH1,#HIGH(-120)
        MOV TL1,#LOW(-120)
        SETB TR1
        JNB TF1,$
        CLR TR1
        CLR TF1
        RET
;*********************************************
DELAY:                          ; Tao tre trong qua trinh quet cac LED7 Thanh
        MOV     R1,#2           ; Thoi gian quet moi LED la 2x255us
LOOP:   MOV     R0,#50        ; Nguyen tac quet LED la cac LED duoc quet lien tiep nhau trong thoi gian rat ngan
loop2:  DJNZ    R0,LOOP2         ; Do su luu anh tren vong mac nguoi nen ta thay cac LED nhu sang lien tuc
        DJNZ    R1,LOOP        ; Thoi gian quet tuy thuoc ham DELAY, gia tri R1 cang lon thi cac LED se nhap nhay
        RET                     ; Toi thieu phai dam bao du 24hinh/giay

T0ISR:                          ;Ngat do Timer0
        CLR TR0                 ; Xoa TR0, tam ngung, khong cho bo dinh thoi Tomer0 hoat dong
        DJNZ count,SKIP         ; Giam count di 1 va nhay nay count != 0
        ;CLR ET0
        inc in_port             ; Chuyen sang cong analog tiep theo de doc du lieu
        mov a,in_port
        cjne a,#8,next_port
        mov in_port,#0
next_port:
        mov P2,in_port
        acall send_start
        mov count,#10
        ;LJMP EXIT
SKIP:
        MOV TH0,#HIGH(-50000)   ;Nap gia tri ban dau cho TH0 va TL0
        MOV TL0,#LOW(-50000)
        SETB TR0
EXIT:   
        RETI 

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

