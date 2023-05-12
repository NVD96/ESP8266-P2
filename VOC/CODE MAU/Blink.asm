#include <sfr51.inc>

ORG 0000h
sjmp loop

    org 0050h

loop:   cpl p2.0
        acall delay
        sjmp loop

delay: mov r0,#7fh
dl1:   mov r1,#ffh
dl2:   djnz r1,dl2
       djnz r0,dl1
       ret
end

