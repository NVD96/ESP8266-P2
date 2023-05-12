#include <sfr52.inc>

mov dptr,#my_data
mov b,#0
loop:
    mov a,b
    movc a,@a+dptr
    inc b
    sjmp loop
my_data:
    db "â ô"
end
