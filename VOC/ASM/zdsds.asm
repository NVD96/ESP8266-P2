MAPHIM        EQU    30H
MACOT          EQU    31H
CHONGDOI    EQU    32H
TEMP            EQU    33H

;****************************************************************
        ORG    000H
        MOV     A,#0                             ;GIA TRI HIEN THI TREN LED 7 DOAN BAN DAU
X1:   CALL    GIAIMA_HIENTHI
X2:   CALL    QUETPHIM
        CJNE    A,#0FFH,X1
        JMP      X2
;****************************************************************
GIAIMA_HIENTHI:
    MOV      DPTR,#MA7DOAN
    MOVC    A,@A+DPTR
    MOV      P1,A
RET

;****************************************************************
QUETPHIM:
         MOV    CHONGDOI,#50            ;CHONG DOI KHI NHAN PHIM
X3:    CALL    DOPHIM              
        JC         EXIT                             ;C = 1 LA KO CO PHIM NHAN
        DJNZ     CHONGDOI,X3
        MOV     TEMP,A                        ;LUU TAM MA PHIM
  
X4:    MOV    CHONGDOI,#50            ;CHONG DOI KHI NHA PHIM
X5:    CALL    DOPHIM
        JNC       X4                               ;C = 0 LA CO PHIM NHAN
        DJNZ     CHONGDOI,X5
EXIT:
        MOV     A,TEMP
RET

;****************************************************************
DOPHIM:
         MOV    MAPHIM,#0
         MOV    MACOT,#0EFH            ;MA COT 1
X6:    MOV    P3,MACOT
         NOP                                       ;CHO NHAN PHIM
         NOP
         NOP
         NOP
         NOP
         NOP
         NOP
         NOP
         MOV     A,P3
         ANL      A,#0FH                      ;LAY MA HANG
         CJNE    A,#0FH,LAPMA          ;KIEM TRA CO NHAN PHIM KO(Chay CT con LAPMA roi tra ve cho nao)
;*****ko co phim nhan
        MOV     A,MAPHIM
        ADD      A,#4
        MOV     MAPHIM,A                  ;MAPHIM = MAPHIM + 4
        MOV     A,MACOT          
        RL         A                               ;DICH SANG COT KE
        MOV     MACOT,A
        CJNE    A,#0FEH,X6                ;KIEM TRA DA DICH DEN COT THU 4 CHUA ?
        SETB    C                               ;KO CO PHIM BAM
        MOV     A,#0FFH
RET
;*****co phim nhan
LAPMA:
       RRC     A
       JNC      X7  ;(chay CT con X7 xong roi tra ve cho nao)
       INC      MAPHIM                      ;MA PHIM = MA PHIM + 1
       JMP     LAPMA
X7:   MOV   A,MAPHIM                   ;DA CO PHIM AN
       CLR     C
RET
;XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
MA7DOAN:
	DB 3FH, 06H, 5BH, 4FH, 66H, 6DH, 7DH, 07H, 7FH, 6FH, 77H, 7CH, 39H, 5EH, 79H, 71H, 40H

END 
