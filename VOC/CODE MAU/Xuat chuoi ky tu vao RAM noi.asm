#include <sfr51.inc>
ORG     0
        MOV     DPTR,#MYDATA    ; con tro? nguo^`n
        MOV     R0,#40H         ; con tro? ddi'ch 
BACK:   CLR     A               ; A=0
        MOVC    A,@A+DPTR       ; la^'y data tu+` bo^. nho+' CT
        JZ      HERE            ; thoa't ne^'u data = 0 (NULL) 
        MOV     @R0,A           ; lu+u va`o RAM
        INC     DPTR            ; ta(ng con tro? nguo^`n
        INC     R0              ; ta(ng con tro? ddi'ch
        SJMP    BACK            ; 
HERE:   SJMP    HERE
        ORG     250H
MYDATA: DB      "BACHKHOA",0      ; chuo^~i du+~ lie^.u 
        RET                      ; ke^'t thu'c la` 0 (NULL char)
        END

