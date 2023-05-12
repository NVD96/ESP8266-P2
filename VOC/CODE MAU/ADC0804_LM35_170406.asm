;CT giao tiep voi ADC0804 va cam bien LM35D
;Ket noi giua ADC0804 va MCU AT89S52
; P3.5 = RD
; P3.6 = W/R
; P3.7 = INTR
; P1(0..7) = D0 -> D7
;Chan CS(chon chip) cua ADC noi voi GND (luon luon cho phep ADC hoat dong)
;********************************************  
#include <sfr51.inc>    ;Chen files chua cac dinh nghia ve thanh ghi trong 8051

do_F    equ 32h
canh_bao equ 1
count_l equ 30h
count_h equ 31h

        ORG 0000h       ; Dia chi bat dau cua Rom
        LJMP MAIN
        ORG 0003H       ;NGAT NGOAI 0
        LJMP EX0ISR

        ORG 0030H               ; Dia chi bat dau Chuong trinh chinh
MAIN:   ; Khoi tao cac gia tri ban dau
        MOV P1,#0FFH            ; Thiet lap P1 la chan vao du lieu
        MOV DPTR,#LED7SEG       ; Dua DPTR tro toi bang giai ma hien thi cho LED7
        MOV P2,#0h
        mov P0,#0h
        MOV  40H,#C6H           ; Chu C
        mov count_l,#0
        mov count_h,#0
; Khoi tao cho ngat ngoai 0
        setb ex0
        setb IT0
        setb ea
        mov do_F,#0h
        acall laymau
BACK:   
        cjne a,#10,next
        mov count_l,#0
        mov count_h,#0
        acall laymau
next:   lcall   DISPLAY        ; Goi chuong trinh hien thi LED 7
        inc count_l
        mov a,count_l
        cjne a,#0FFh,no_inc_count_h
        inc count_h
no_inc_count_h:
        jnb canh_bao,temp_ok
        setb p2.5
        clr p2.4
        sjmp temp_not_ok
temp_ok:
        setb p2.4
        clr p2.5
temp_not_ok:
        mov a,count_h
        SJMP    BACK            ; Chuan bi cho qua trinh chuyen doi tiep theo
;===========END MAIN FUNCTION ==================
laymau:
        CLR     P3.7            ; Tao xung tu cao xuong thap tai chan P3.7(Tuc W/R cua ADC)
        NOP
        SETB    P3.7            ; Cho phep ADC0804 bat dau qua trinh chuyen doi tu tuong tu sang so
;here:   JB      P3.5,here
        acall delay_100us
        CLR     P3.6           ; Dua xung muc thap toi chan RD - cho phep doc du lieu tu ADC(Xuat ra D0..D7)
        MOV     A,P1            ; Dua du lieu 8bit tu P1 den thanh ghi A
        acall   BIN2BCD         ; tra bang, doi BCD -> LED 7 doan
        ret

BIN2BCD:;Chuong trinh chuyen doi tu so hex sang BCD
; gia tri doc tu ADC lu trong thanh ghi A
; So sanh gia tri nhiet do voi MAX va MIN
     MOV     R4,A
     CLR     C
     SUBB    A,#30  ; Gia tri nhiet do MAX de so sanh
     JC      nho_hon_40
     setb     canh_bao
     SJMP    thoat_kt
nho_hon_40:
     MOV     A,R4
     CLR     C
     SUBB    A,#20  ; Gia tri nhiet do MIN de so sanh
     JNC     lon_hon_15
     setb     canh_bao
     SJMP    thoat_kt
lon_hon_15:
     clr     canh_bao
thoat_kt:
;===============================
     mov    a,do_F
     jz     nhiet_do_C
     mov    a,r4
     mov    b,#18
     mul    ab
     mov    b,#10
     div    ab
     add    a,#57
     sjmp nhiet_do_F
nhiet_do_C:
     mov    a,r4
nhiet_do_F:
     mov     b,#100
     div     ab
     mov     44H,a
     mov     a,b
     mov     b,#10
     div     ab
     mov     43h,a
     mov     42h,b
     RET
; Chuong trinh con phuc vu ngat ngoai
EX0ISR:
    PUSH    ACC
    cpl P3.4    ; Chan dieu khien bat tat LED
    MOV     A,do_F
    JNZ     C0007
    mov     do_F,#01h
    SJMP    C0009
C0007:
    mov     do_F,#0h
C0009:
    POP     ACC
   RETI    

DISPLAY:
        mov a,do_F
        jnz      hien_chu_F
        MOV     P0,40H
        sjmp    hien_chu_C
hien_chu_F:
        mov     p0,#8Eh ;Chu F
hien_chu_C:
        CLR     P2.0            ;Bat LED1
        ACALL   DELAY
        SETB    P2.0            ;TAT LED 1

        MOV     A,42h           ; Lay thong tin LED7 cua so don vi
        MOVC    A,@A+DPTR
        MOV     P0,A
        clr     P0.7
        CLR     P2.1
        ACALL   DELAY
        setb    P0.7
        SETB    P2.1            ;TAT LED 2

        MOV     A,43H           ; Lay ma LED7 cua so hang chuc
        MOVC    A,@A+DPTR
        MOV     P0,A          ; LED1 Hang don vi
        CLR     P2.2            ; ba^.t LED1 sa'ng
        ACALL   DELAY           ; delay
        SETB    P2.2            ; ta('t LED1

        MOV     A,44H           ; Lay ma LED7 cua so hang chuc
        MOVC    A,@A+DPTR
        MOV     P0,A          ; LED2  hang chuc
        CLR     P2.3            ; ba^.t LED2 sa'ng
        ACALL   DELAY           ; delay
        SETB    P2.3            ; ta('t LED2

        RET

DELAY:                          ; Tao tre trong qua trinh quet cac LED7 Thanh
        MOV     R6,#30          ; Nguyen tac quet LED la cac LED duoc quet lien tiep nhau trong thoi gian rat ngan
loop2:  DJNZ    R6,LOOP2        ; Do su luu anh tren vong mac nguoi nen ta thay cac LED nhu sang lien tuc
        RET                     ; Thoi gian quet tuy thuoc ham DELAY, gia tri R1 cang lon thi cac LED se nhap nhay
                                ; Toi thieu phai dam bao du 24hinh/giay

delay_100us:
        mov R7,50
loop3:  djnz R7,loop3           ; 2us x 60 = 120uS
        ret

LED7SEG:
        DB      0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H
        DB      88H,0C6H,86H,8EH,82H,89H,7FH,FFH
        END        

