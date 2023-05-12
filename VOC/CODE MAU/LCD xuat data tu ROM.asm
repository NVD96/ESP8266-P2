#include <sfr52.inc>
;Xua^'t ra LCD "Hello"
;P1=data pin
;P3.0 -> RS pin
;P3.1 -> R/W pin
;P3.2 -> E pin 
        ORG     0000h
;RS      EQU     P3.0
;RW      EQU     P3.1
;E       EQU     P3.2
      
        MOV     A,#38H        ;init. LCD 2 do`ng, ma tra^.n 5x7 
        ACALL   CSTROBE 
        MOV     A,#0EH        ;LCD on, cursor on
        ACALL   CSTROBE
        MOV     A,#01H        ;clear LCD
        ACALL   CSTROBE
        MOV     A,#06H        ;cursor di.ch pha?i
        ACALL   CSTROBE 
        MOV     A,#80H        ;chuye^?n cursor dde^'n line 1, pos. 6
        ACALL   CSTROBE
        MOV     DPTR,#MYDATA    ;Dua con tro du lieu toi bang
DONG1:  CLR     A
        MOVC    A,@A+DPTR
        JZ      XUONGDONG    ;Neu A=0 thi thi xuong dong tiep theo
        ACALL   DSTROBE
        INC     DPTR
        SJMP    DONG1
XUONGDONG:  
        MOV     A,#C0H      ;Chuyen con tro den dong 2 cot 1
        ACALL   CSTROBE     ;Gui lenh den LCD
        INC     DPTR        ;Tang con tro du lieu
DONG2:  CLR     A           ;xoa A
        MOVC    A,@A+DPTR
        JZ      HERE       ;Neu A=0 thoat den HERE
        ACALL   DSTROBE
        INC     DPTR
        SJMP    DONG2
HERE:   MOV     A,#0FH
        ACALL   CSTROBE
        SJMP    $  
CSTROBE:                      ;command strobe
        ACALL   READY         ;is LCD ready?
        MOV     P1,A          ;xua^'t ma~ le^.nh
        CLR     P3.0            ;RS=0: le^.nh
        CLR     P3.1            ;R/W=0 -> ghi ra LCD
        SETB    P3.2             ;E=1 -> ta.o ca.nh xuo^'ng
        CLR     P3.2             ;E=0 ,cho^'t
        RET
DSTROBE:                      ;data strobe
        ACALL   READY         ;is LCD ready?
        MOV     P1,A          ;xua^'t du+~ lie^.u  
        SETB    P3.0            ;RS=1 for data
        CLR     P3.1            ;R/W=0 to write to LCD
        SETB    P3.2             ;E=1 -> ta.o ca.nh xuo^'ng
        CLR     P3.2             ;E=0, cho^'t 
        RET
; kie^?m tra co+` BF
READY:  SETB    P1.7          ;P1.7: input
        CLR     P3.0            ;RS=0: thanh ghi le^.nh
        SETB    P3.1            ;R/W=1: ddo.c
BACK:   CLR     P3.2             ;E=0 -> ta.o ca.nh le^n
        SETB    P3.2             ;E=1  
        JB      P1.7,BACK     ;cho+` busy flag=0
        RET


        ORG 250H
MYDATA:
        DB  "KHOA DIEN TU - VIEN THONG",0,"DAI HOC BACH KHOA HA NOI",0

        END

