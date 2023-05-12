#include <sfr52.inc>
;Xua^'t ra LCD "Hello"
;P1=data pin
;P3.0 -> RS pin
;P3.1 -> R/W pin
;P3.2 -> E pin
LCD_RS      bit     P3.5
LCD_RW      bit     P3.6
LCD_E       bit     P3.7
LCD_DATA    equ     P1
flag equ    1
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
        MOV R1,#5 
        mov a,#01h
        acall cstrobe
        mov a,#80h
        acall cstrobe
        mov dptr,#no_key
        acall string
main_loop: 
        acall keyscan
        mov a,r1
        cjne a,#1,next_key1
        mov a,#C0h
        acall cstrobe
        mov dptr,#key1
        acall string
        sjmp exit
next_key1:
        cjne a,#2,next_key2
       
        mov a,#C0h
        acall cstrobe
        mov dptr,#key2
        acall string
        sjmp exit
next_key2:
        cjne a,#3,next_key3
       
        mov a,#C0h
        acall cstrobe
        mov dptr,#key3
        acall string
        sjmp exit
next_key3:
        cjne a,#4,exit
        
        mov a,#C0h
        acall cstrobe
        mov dptr,#key4
        acall string
        sjmp exit
exit:   
        mov r1,#5
       ljmp main_loop

;--------------------------------------
keyscan:
            mov P0,#0ffh
            nop
            jnb p0.0,chongrung
            jnb p0.1,chongrung
            jnb p0.2,chongrung
            jnb p0.3,chongrung
            ret
chongrung:
            lcall delay10ms
            jnb p0.0,phim1
            jnb p0.1,phim2
            jnb p0.2,phim3
            jnb p0.3,phim4
            ret
phim1:
            jnb p0.0,$
            mov r1,#1
            ret
phim2:
            jnb p0.1,$
            mov r1,#2
            ret
phim3:
            jnb p0.2,$
            mov r1,#3
            ret
phim4:
            jnb p0.3,$
            mov r1,#4
            ret

delay10ms:
            lcall   delay
            lcall   delay
            lcall   delay
            lcall   delay
            lcall   delay
            lcall   delay
            lcall   delay
            lcall   delay
            lcall   delay
            lcall   delay
            ret
delay:
        mov r2,#100
        delay_loop:
              nop
              nop
              nop
              nop
              nop
              nop
              nop
              nop
              djnz r2,delay_loop
              ret
;==============================================
string :              
        ;mov     a,#01h
        ;lcall   cstrobe
        ;mov     a,#80h
        ;lcall   cstrobe
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
READY:  SETB    P1.7          ;P1.7: input
        CLR     LCD_RS            ;RS=0: thanh ghi le^.nh
        SETB    LCD_RW      ;R/W=1: ddo.c
BACK:   CLR     LCD_E        ;E=0 -> ta.o ca.nh le^n
        SETB    LCD_E        ;E=1  
        JB      P1.7,BACK     ;cho+` busy flag=0
        RET

key1: DB "Quay thuan",0
key2: DB "Quay nguoc",0
key3: DB "Tang toc  ",0
key4: DB "Giam toc  ",0
no_key: DB "DC motor control",0
  
        END

