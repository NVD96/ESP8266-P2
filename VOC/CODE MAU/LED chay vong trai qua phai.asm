#include <sfr51.inc>
       
LEDRUN1: MOV     A, FFH
LOOP1:   MOV     P0,A
         LCALL   DELAY
         RL      A
         SJMP    LOOP1

DELAY:      MOV R1,#250
DL1:        NOP
            NOP
            DJNZ R1, DL1
            RET
end
