#include <sfr52.inc>
;P1 = data/command pin
;P3.0 -> RS pin
;P3.1 -> R/W pin
;P3.2 -> E pin
;P2 -> Keypad
        ORG     0000h
RS  EQU P3.0
RW  EQU P3.1
EN  EQU P3.2

        MOV     A,#38H        ;init. LCD 2 lines,5x7 matrix
        ACALL   CSTROBE
        MOV     A,#0EH        ;LCD on, cursor on
        ACALL   CSTROBE
        MOV     A,#01H        ;clear LCD
        ACALL   CSTROBE
        MOV     A,#06H        ;cursor di.ch pha?i
        ACALL   CSTROBE 
        MOV     A,#80H        ;cursor: line 1, pos. 0
        ACALL   CSTROBE
AGAIN:  LCALL   READKP
        ORL     A,#30h
        ACALL   DELAY
        ACALL   DSTROBE        
        SJMP    AGAIN         
;command strobe
CSTROBE:
        ACALL   READY         ;is LCD ready?
        MOV     P1,A          ;xua^'t ma~ le^.nh
        CLR     P3.0            ;RS=0: le^.nh
        CLR     P3.1            ;R/W=0: ghi ra LCD
        SETB    P3.2            ;EN=1 -> ta.o ca.nh xuo^'ng
        CLR     P3.2            ;EN=0 ,cho^'t
        RET
;data strobe
DSTROBE:
        ACALL   READY         ;is LCD ready?
        MOV     P1,A          ;xua^'t du+~ lie^.u ra P1
        SETB    P3.0            ;RS=1: du+~ lie^.u
        CLR     P3.1            ;R/W=0 ghi ra LCD
        SETB    P3.2            ;EN=1 -> ta.o ca.nh xuo^'ng
        CLR     P3.2            ;EN=0, cho^'t 
        RET
READY:  SETB    P1.7          ;P1.7: input
        CLR     P3.0            ;RS=0: le^.nh
        SETB    P3.1            ;R/W=1: ddo.c
BACK:   CLR     P3.2            ;EN=0 -> ta.o ca.nh le^n
        SETB    P3.2            ;EN=1  
        JB      P1.7,BACK     ;cho+` busy flag=0
        RET
; DDo.c ba`n phi'm
READKP: PUSH    7
SCAN:   MOV     A,#11111110B    ; col_0 -> GND
        MOV     R7,#0           ; R7 = i
CONT:   MOV     P2,A            ; no^'i col i -> GND
        MOV     A,P2            ; ddo.c row       
        JNB     ACC.4,ROW_0     ; xe't xem row na`o?       
        JNB     ACC.5,ROW_1
        JNB     ACC.6,ROW_2
        JNB     ACC.7,ROW_3
        RL      A               ; chua^?n bi. no^'i GND 
        INC     R7              ; co^.t tie^'p theo
        CJNE    R7,#4,CONT      ; la^`n luo+.t no^'i GND 4 co^.t
        SJMP    SCAN            ; quay la.i que't tu+` co^.t 0
ROW_0:  MOV     A,R7            ; Row=0, Col=R7
        ADD     A,#0            ; A = 0 + R7
        SJMP    EXIT
ROW_1:  MOV     A,R7            ; Row=1, Col=R7
        ADD     A,#4            ; A = 4 + R7
        SJMP    EXIT
ROW_2:  MOV     A,R7            ; Row=2, Col=R7
        ADD     A,#8            ; A = 8 + R7
        SJMP    EXIT
ROW_3:  MOV     A,R7            ; Row=3, Col=R7
        ADD     A,#12           ; A = 12 + R7
EXIT:   POP     7
        RET
DELAY:  PUSH    6
        PUSH    7
        MOV     R7,#0FFh
LP1:    MOV     R6,#0FFh
LP0:    DJNZ    R6,LP0
        DJNZ    R7,LP1
        POP     7
        POP     6
        RET
        END
