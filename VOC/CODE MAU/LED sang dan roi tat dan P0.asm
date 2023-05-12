#include <sfr51.inc>
;Chuong trinh dieu khien cac port0 sang dan len va tat dan het
;----------------------------------------------------------------------------------------
org 0000h

port0_022: 
MOV P0,#00000000b ;Tat port0
LCALL delay ;Goi chuong trinh con delay

MOV P0,#00000001b ;Sang 1 led
LCALL delay ;Goi chuong trinh con delay

MOV P0,#00000011b ;Sang led
LCALL delay ;Goi chuong trinh con delay

MOV P0,#00000111b ;Sang 3 led
LCALL delay ;Goi chuong trinh con delay

MOV P0,#00001111b ;Sang 4 led
LCALL delay ;Goi chuong trinh con delay

MOV P0,#00011111b ;Sang 5 led
LCALL delay ;Goi chuong trinh con delay

MOV P0,#00111111b ;Sang 6 led
LCALL delay ;Goi chuong trinh con delay

MOV P0,#01111111b ;Sang 7 led
LCALL delay ;Goi chuong trinh con delay

MOV P0,#11111111b ;Sang 8 led
LCALL delay ;Goi chuong trinh con delay

sjmp port0_022

;-------------------------------------------------------------------------------
;Chuong rinh con delay
;-------------------------------------------------------------------------------
delay:      MOV R2,#10
DL3:        MOV R1,#250            
DL2:        NOP
            NOP
            DJNZ R1,DL2
            DJNZ R2,DL3
            RET
;delay: MOV R6,#0ffh
;de1: MOV R7,0ffh
;DJNZ R7,$ 
;DJNZ R6,de1
;RET
END
