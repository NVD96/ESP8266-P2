#include <sfr51.inc>
ORG     8100h        

        SETB    P3.0            ;P3.0: input
LOOP:   JNB     P3.0,LOOP       ;Cho logic 0 o P3.0
LOOP1:  JB      P3.0,LOOP1      ;cho logic 1 o P3.0
        CLR     P2.0
        LCALL   DELAY1000ms
        SETB    P2.0
        CLR     P2.1
        LCALL   DELAY1000ms
        SETB    P2.1
        CLR     P2.2
        LCALL   DELAY1000ms
        SETB    P2.2
        SJMP    LOOP
;CHUONG TRINH TRE 1MS
DELAY1ms:   MOV R1,#250
DL1:        NOP             ;1 chu ky may
            NOP             ;1 chu ky may (4 chu ky may = 4us x 250 = 1000us = 1ms)
            DJNZ R1, DL1    ;2 chu ky may 
            RET
;CHUONG TRINH TRE 5MS
DELAY5ms:  MOV R2,#5
DL3:        MOV R1,#250            
DL2:        NOP
            NOP
            DJNZ R1,DL2
            DJNZ R2,DL3
            RET
;CHUONG TRINH TRE 10MS
DELAY10ms:  MOV R2,#10
DL3:        MOV R1,#250            
DL2:        NOP
            NOP
            DJNZ R1,DL2
            DJNZ R2,DL3
            RET
DELAY100ms:  MOV R2,#100
DL5:        MOV R1,#250            
DL4:        NOP
            NOP
            DJNZ R1,DL4
            DJNZ R2,DL5
            RET
DELAY1000ms:  MOV R2,#1000
DL7:        MOV R1,#250            
DL6:        NOP
            NOP
            DJNZ R1,DL6
            DJNZ R2,DL7
            RET
DELAY1s:    MOV R3,#10
DL10:       MOV R2,#100            
DL9:        MOV R1,#250
DL8:        NOP
            NOP
            DJNZ R1,DL8
            DJNZ R2,DL9
            DJNZ R3,DL10
            RET
            END


