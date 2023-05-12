#include <sfr51.inc>
    ORG     0000h
    LJMP    main
    ORG     0023h
    LJMP    spisr
    ORG     0030h
main:
    mov TMOD,#20h   ;bo dinh thoi 1 che do 2
    mov TH1,#-26    ;gia tri nap lai 1200 baud
    setb TR1        ;timer hoat dong
    mov SCON,#40    ;che do 1 set TI=1 de buoc co ngat dau tien
                    ;gui ky tu thu nhat
    mov A,#20h      ;ky tu thu nhat
    mov IE,#90h     ;cho phep ngat do port noi tiep
    mov DPTR,#LED7SEG
    sjmp $          ;khong lam gi
spisr:
    MOV A,SBUF
    CLR RI
    MOV 30h,A
    inc A
    mov SBUF,A
    clr TI
    MOV A,30h
    ANL A,#0FH
    MOVC A,@A+DPTR
    MOV P2,A
    RETI
LED7SEG:
        DB      0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H
    end