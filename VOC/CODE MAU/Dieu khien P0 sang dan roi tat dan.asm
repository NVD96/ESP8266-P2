;Chuong trinh dieu khien cac port0 sang dan len va tat dan het
;----------------------------------------------------------------------------------------
        org 0000h

port0_022: 
    MOV     90h,#00000000b ;Tat port0
    LCALL   delay ;Goi chuong trinh con delay
    MOV     90h,#00000001b ;Sang 1 led
    LCALL   delay ;Goi chuong trinh con delay
    MOV     90h,#00000011b ;Sang led
    LCALL   delay ;Goi chuong trinh con delay
    MOV     90h,#00000111b ;Sang 3 led
    LCALL   delay ;Goi chuong trinh con delay
    MOV     90h,#00001111b ;Sang 4 led
    LCALL   delay ;Goi chuong trinh con delay
    MOV     90h,#00011111b ;Sang 5 led
    LCALL   delay ;Goi chuong trinh con delay
    MOV     90h,#00111111b ;Sang 6 led
    LCALL   delay ;Goi chuong trinh con delay
    MOV     90h,#01111111b ;Sang 7 led
    LCALL   delay ;Goi chuong trinh con delay
    MOV     90h,#11111111b ;Sang 8 led
    LCALL   delay ;Goi chuong trinh con delay
    SJMP    port0_022

;-------------------------------------------------------------------------------
;Chuong rinh con delay
;-------------------------------------------------------------------------------

delay:  MOV R6,#0ffh
de1:    MOV R7,0ffh
        DJNZ R7,$ 
        DJNZ R6,de1
        RET
END
