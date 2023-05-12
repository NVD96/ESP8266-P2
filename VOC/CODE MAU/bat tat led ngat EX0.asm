#include <sfr52.inc>

    org 0h
    ljmp main
    org 0003h
ex0_isr:
    cpl P1.0
    cpl P2.0
    cpl P0.0
    reti         ;thoat ngat

    org 0030h
main:
    setb EX0    ; Cho phep ngat ngoai chan P3.2 EX0 = 1
    setb IT0    ; Thiet lap ngat theo suon am   IT0 = 1
    setb EA     ; Cho phep ngat toan cuc(Global) EA = 1
    clr P2.0
    sjmp $
end
