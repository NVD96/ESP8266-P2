;Dem tin hieu vao o P3.4
;Khi dem duoc 10 xung thi dung
#include <sfr52.inc>
        
count1: mov R7,#10
        mov P1,#FFh     ;Tat P1
        mov TMOD,#05h   ;Timer 0, Cuonter, Mode 1(16-bit)
        mov TL0,#F6h    ;Thiet lap TL0 de dem 10
        mov TH0,#0FFh   ;Set TH0 la FFh
        SETB P3.4       ;P3.4 Input
        SETB TR0        ;Cho phep Timer0 hoat dong
WAIT:   JNB TF0,WAIT    ;Kiem tra co tran xem co tran xay ra ko
        MOV P1,#00h     ;bat  P1
LOOP1:  ACALL DELAY     ;Goi CT tre 1ms
        DJNZ R7,LOOP1
        CLR TR0        ;Dung bo dinh thoi
        CLR TF0         ;Xoa co tran TF0
       ;SJMP BEGIN      ;Quay tro lai tu dau

count2: CLR P2.0       ;LEFT=0
        CLR  P2.7      ;RIGHT=0  Di thang
        mov TMOD,#05h   
        mov TL0,#F9h   
        mov TH0,#0FFh   
        SETB P3.4      
        SETB TR0       
WAIT1:  JNB TF0,WAIT1
        NOP
        ACALL DELAY
        SETB P2.0      ;RE TRAI
        ACALL DELAY 
WAIT2:  JB P3.4,WAIT2  
        ;CLR TR0     
        CLR TF0        
        SJMP COUNT2      


        

        
DELAY:      MOV R3,#10
DL10:       MOV R2,#100            
DL9:        MOV R1,#250
DL8:        NOP
            NOP
            DJNZ R1,DL8
            DJNZ R2,DL9
            DJNZ R3,DL10
            RET
END
        

