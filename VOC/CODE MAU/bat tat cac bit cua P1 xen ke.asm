#include <sfr51.inc>
ORG     0
BACK:   MOV     A,#55H          ; A = 55h
        MOV     P1,A            ; P1 = 55h
        LCALL   DELAY           ; 
        MOV     A,#0AAH         ; A = AAh
        MOV     P1,A            ; P1 = AAh
        LCALL   DELAY    
        SJMP    BACK            ;this is the delay subroutine
        ORG 300H  
DELAY:  PUSH    4               ; PUSH R4
        PUSH    5               ; PUSH R5
        MOV     R4,#0FFH        ; R4=FFH
NEXT:   MOV     R5,#0FFH        ; R5=255
AGAIN:  DJNZ    R5,AGAIN   
        DJNZ    R4,NEXT
        POP     5               ; POP INTO R5
        POP     4               ; POP INTO R4
        RET                     ;  
        END         

