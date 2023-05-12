#include <sfr51.inc>
org 0000h

lb1:    MOV P1,#00h ;Tat port0
lb11:   LCALL delay
        SETB C ;Lam cho bit cy = 1 cho viec quay trai co nho
        MOV A,P1 ;chuyen noi dung cua port0 vao thanh gi A
        RLC A ;Xoay noi dung thanh gi a sang tri
        MOV P1,A ;Tra lai noi dung cho port0
        JNC lb11 ;Nhay de thuc hien tiep 

lb12:   LCALL delay ;Goi chuong trinh con delay
        CLR C ;Lam cho bit c = 0
        MOV A,P1 ;Chuyen noi dung dung port0 vao thanh ghi A
        RLC A ;Xoay noi dung cua thanh ghi A sang phai
        MOV P1,A ;Tra noi dung port0
        JC lb12 ;Nhay ve de thuc hien tiep khi c = 1

        ;SJMP lb1 ;Sau khi 8 led sang het thi quay lai tu dau
;-------------------------------------------------------------------------------
;Chuong rinh con delay
;-------------------------------------------------------------------------------


;Chuong trinh dieu khien cac port0 sang dan len va tat dan het
;----------------------------------------------------------------------------------------
       
port0_022: 
    MOV     80h,#00000000b ;Tat port0
    LCALL   delay ;Goi chuong trinh con delay
    MOV     80h,#00000001b ;Sang 1 led
    LCALL   delay ;Goi chuong trinh con delay
    MOV     80h,#00000011b ;Sang led
    LCALL   delay ;Goi chuong trinh con delay
    MOV     80h,#00000111b ;Sang 3 led
    LCALL   delay ;Goi chuong trinh con delay
    MOV     80h,#00001111b ;Sang 4 led
    LCALL   delay ;Goi chuong trinh con delay
    MOV     80h,#00011111b ;Sang 5 led
    LCALL   delay ;Goi chuong trinh con delay
    MOV     80h,#00111111b ;Sang 6 led
    LCALL   delay ;Goi chuong trinh con delay
    MOV     80h,#01111111b ;Sang 7 led
    LCALL   delay ;Goi chuong trinh con delay
    MOV     80h,#11111111b ;Sang 8 led
    LCALL   delay ;Goi chuong trinh con delay
    SJMP    lb1;port0_022

;-------------------------------------------------------------------------------
;Chuong rinh con delay
;-------------------------------------------------------------------------------
delay:  MOV R6,#0ffh
de1:    MOV R7,0ffh
        DJNZ R7,$ 
        DJNZ R6,de1
        RET

END
