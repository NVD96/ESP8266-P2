#include <sfr52.inc>
;Xua^'t ra LCD "Hello"
;P1=data pin
;P3.0 -> RS pin
;P3.1 -> R/W pin
;P3.2 -> E pin
LCD_RS      bit     P3.7
LCD_RW      bit     P3.6
LCD_E       bit     P3.5
LCD_DATA    equ     P1

        ORG     0000h

; KHoi tao LCD      
        MOV     A,#38H        ;init. LCD 2 do`ng, ma tra^.n 5x7 
        ACALL   CSTROBE 
        MOV     A,#0fH        ;LCD on, cursor on
        ACALL   CSTROBE
        MOV     A,#01H        ;clear LCD
        ACALL   CSTROBE
        MOV     A,#06H        ;cursor di.ch pha?i
        ACALL   CSTROBE 
        MOV     A,#86H        ;chuye^?n cursor dde^'n line 1, pos. 6
        ACALL   CSTROBE
; ket thuc khoi tao
        MOV     A,#'H'        
        ACALL   DSTROBE
        MOV     A,#'e'        
        ACALL   DSTROBE
        MOV     A,#'l'        
        ACALL   DSTROBE
        MOV     A,#'l'        
        ACALL   DSTROBE
        MOV     A,#'o'        
        ACALL   DSTROBE
        MOV     A,#C4H        ;chuye^?n cursor dde^'n line 2, pos. 6
        ACALL   CSTROBE
        MOV     A,#'D'        
        ACALL   DSTROBE
        MOV     A,#'H'        
        ACALL   DSTROBE
        MOV     A,#'B'        
        ACALL   DSTROBE
        MOV     A,#'K'        
        ACALL   DSTROBE
        MOV     A,#'H'        
        ACALL   DSTROBE
        MOV     A,#'N'        
        ACALL   DSTROBE
        
        mov a,#01h
        acall cstrobe
        mov dptr,#cau1
        acall string

        mov dptr,#cau2
        acall string

HERE:   SJMP    HERE    
;==============================================
string :              
        mov     a,#01h
        lcall   cstrobe
        mov     a,#80h
        lcall   cstrobe
xuatchuoi:      
        clr      a
        movc     a,@a+dptr
        jz       end_of_string
        lcall    dstrobe
        inc      dptr
        sjmp     xuatchuoi  
end_of_string :    
        ret
;==============================================
CSTROBE:                      ;command strobe
        ACALL   READY         ;is LCD ready?
        MOV     LCD_DATA,A          ;xua^'t ma~ le^.nh
        CLR     LCD_RS            ;RS=0: le^.nh
        CLR     LCD_RW            ;R/W=0 -> ghi ra LCD
        SETB    LCD_E             ;E=1 -> ta.o ca.nh xuo^'ng
        CLR     LCD_E             ;E=0 ,cho^'t
        RET
DSTROBE:                      ;data strobe
        ACALL   READY         ;is LCD ready?
        MOV     LCD_DATA,A          ;xua^'t du+~ lie^.u  
        SETB    LCD_RS            ;RS=1 for data
        CLR     LCD_RW            ;R/W=0 to write to LCD
        SETB    LCD_E             ;E=1 -> ta.o ca.nh xuo^'ng
        CLR     LCD_E             ;E=0, cho^'t 
        RET
; kie^?m tra co+` BF
READY:  SETB    P0.7          ;P1.7: input
        CLR     LCD_RS            ;RS=0: thanh ghi le^.nh
        SETB    LCD_RW      ;R/W=1: ddo.c
BACK:   CLR     LCD_E        ;E=0 -> ta.o ca.nh le^n
        SETB    LCD_E        ;E=1  
        JB      P0.7,BACK     ;cho+` busy flag=0
        RET
cau1: 
    DB "Nguyen Chi Linh",0
cau2:
    DB "Khoa DTVT",0
        END

