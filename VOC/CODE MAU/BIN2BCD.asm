; ddo^?i Binary (P1) -> BCD (R5 R6 R7)
        MOV     A,#0FFH
        MOV     P1,A    ; P1: input port
        MOV     A,P1    ; ddo.c P1
        MOV     B,#10   ; B=0A hex (10 dec)
        DIV     AB      ; chia cho 10
        MOV     R7,B    ; lu+u digit tha^'p
        MOV     B,#10   ;
        DIV     AB      ; chia cho 10
        MOV     R6,B    ; lu+u digit tie^'p theo va`o R6
        MOV     R5,A    ; lu+u digit cuo^'i va`o R6
        SJMP    $
; Ba.n ha~y vie^'t la.i ddoa.n chuo+ng tri`nh tre^n
; tha`nh mo^.t chuo+ng tri`nh con, dda(.t te^n la` BIN2BCD

