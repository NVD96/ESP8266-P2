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
    sjmp $          ;khong lam gi
spisr:
HERE:
    JNB RI,HERE   ;    
    MOV A,SBUF
    CLR RI
    MOV P1,A
    MOV A,#20h
skip:
    mov SBUF,A
    inc A
    clr TI
    reti
    end