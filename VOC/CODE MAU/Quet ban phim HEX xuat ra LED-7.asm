#include <sfr51.inc>
; Ba`n phi'm hex no^'i va`o P1 
; Chuo+ng tri`nh hie^?n thi. phi'm nha^'n ra LED 7 ddoa.n
; P1.0-P1.3: columns
; P1.4-P1.7: rows
; DDi.a chi? LED: A000h)      
LOOP:   LCALL   READKB          ; tri. tra? ve^`: A = 0-15
        MOV     DPTR,#T7SEG
        MOVC    A,@A+DPTR
        MOV     DPTR,#A000h    ; A000h: LED 1 
        MOVX    @DPTR,A      
        SJMP    LOOP
READKB: PUSH    7
SCAN:   MOV     A,#11111110B    ; col_0 -> GND
        MOV     R7,#0           ; R7 = i
CONT:   MOV     P1,A            ; no^'i col i -> GND
        MOV     A,P1            ; ddo.c row       
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
T7SEG:  DB      40H,79H,24H,30H,19H,12H,02H,78H,00H,10H
        DB      08H,03H,46H,21H,04H,0EH
        END

