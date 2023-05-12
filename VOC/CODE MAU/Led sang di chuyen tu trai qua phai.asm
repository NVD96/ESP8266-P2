;Mot led dang va di chuyen tu trai qua phai
#include <sfr51.inc>
;----------------------------------------------------------------------------------------
org 0000h

lb1:    MOV P0,#00h ;Tat port0
        mov R0,#8
lb11:   LCALL delay
        SETB C ;Lam cho bit cy = 1 cho viec quay trai co nho
        MOV A,P0 ;chuyen noi dung cua port0 vao thanh gi A
        RLC A ;Xoay noi dung thanh gi a sang tri
        MOV P0,A ;Tra lai noi dung cho port0
        JNC lb11 ;Nhay de thuc hien tiep 
        DJNZ R0,lb12
        
lb12:   ;clr p1.1
        MOV P1,#00h
        LCALL delay1s
        MOV p1,#FFh
        ;Nhay ve de thuc hien tiep khi c = 1
        SJMP lb1 ;Sau khi 8 led sang het thi quay lai tu dau
;-------------------------------------------------------------------------------
;Chuong rinh con delay
;-------------------------------------------------------------------------------

delay:  MOV R6,#0ffh
de1:    MOV R7,0ffh
        DJNZ R7,$ 
        DJNZ R6,de1
        RET
delay1s:     MOV R3,#10
DL6:        MOV R2,#100            
DL5:        MOV R1,#250
DL4:        DJNZ R4,DL4
            DJNZ R5,DL5
            DJNZ R6,DL6
            RET
        END
