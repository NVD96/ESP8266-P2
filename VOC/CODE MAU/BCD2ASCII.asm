ORG     0
        MOV     A,#29H    ; A = 29H, packed BCD
        MOV     R2,A      ; sao lu+u A va`o R2
        ANL     A,#0FH    ; che nibble cao (A=09)
        ORL     A,#30H    ; chuye^?n tha`nh ma~ ASCII, A=39H (`9')
        MOV     R6,A      ; lu+u ke^'t qua? va`o R6 (R6=39H ASCII char)
        MOV     A,R2      ; la^'y la.i gia' tri. A ban dda^`u
        ANL     A,#0F0H   ; che nibble tha^'p (A=20)
        RR      A         ; quay pha?i 4 la^`n
        RR      A         ;
        RR      A         ;
        RR      A         ; -> A=02
        ORL     A,#30H    ; chuye^? tha`nh ma~ ASCII
        MOV     R2,A      ; lu+u va`o R2
        SJMP    $