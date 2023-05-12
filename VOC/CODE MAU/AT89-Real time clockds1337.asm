#include <sfr52.inc>

sda bit P1.0
scl bit P1.1

i2c_ack_in equ 0       ; flag bit on RAM bit area at 20h
i2c_ack_out equ 1  
    
sec equ 21h
min equ 22h
hour equ 23h
dow equ 24h
date equ 25h
month equ 26h
year equ 27h
mode equ 28h
led1 equ 30h
led2 equ 31h
led3 equ 32h
led4 equ 33h
led5 equ 34h
led6 equ 35h

org 0
ljmp main

org 0030h
main:
    setb i2c_ack_in
    setb i2c_ack_out
    mov dptr,#maled7
    clr a
    acall init_time
    mov r5,#5
    mov mode,#0
loop:
    acall delay
    acall delay
    djnz r5,loop
    acall update_time
main_loop:
    acall read_time
    acall delay
    acall display
    acall keyscan
    sjmp main_loop

init_time:
    acall i2c_start        ;/* The following Enables the Oscillator */
    mov a,#0D0h
    acall i2c_write        ;  /* address the part to write */
    mov a,#00h
    acall i2c_write;  /* position the address pointer to 0 */
    mov a,#0
    acall   i2c_write;     /* write 0 to the seconds register, clear the CH bit */
    mov a,#55h
    acall i2c_write;  /* write slave address + write */
    mov a,#52h
    acall i2c_write;  /* hour = 101 0010  12h mode - time =12  */
    mov a,#07h
    acall i2c_write;  //Day of week. 1=sun, 2=mon...
    mov a,#03h
    acall i2c_write;
    mov a,#11h
    acall   i2c_write;
    mov a,#05h
    acall   i2c_write;
    acall i2c_stop;

    acall i2c_start;     // Write control and status register
    mov a,#0d0h
    acall i2c_write; // write slave address + write */
    mov a,#0eh
    acall i2c_write; // write register address, control register */
    mov a,#00000011b
    acall i2c_write; /* enable osc, bbsqi */
    mov a,#00h
    acall i2c_write;
    acall i2c_stop;
    ret
keyscan:
    mov p1,#0ffh    ; All are input
    nop
    mov a,P1
    jnb acc.0,mode_key
    jnb acc.1,change_key
    jnb acc.2,clear_key
    jnb acc.3,alarm_key
    nop
mode_key:
    inc mode
    ret
change_key:
    mov a,mode
    cjne a,#1,next1
    acall setmin
    ret
next1:
    cjne a,#2,next2
    acall sethour
    ret
next2:
    cjne a,#3,next3
    acall setdate
    ret
next3:
    cjne a,#4,exit
    acall setmonth
exit:
    cjne a,#5,thoat
    mov mode,#0
thoat:
    ret
clear_key:
    acall i2c_start;
    mov a,#0D0h
    acall i2c_write;  /* address the part to write */
    mov a,#0Fh
    acall i2c_write;  /* position the address pointer to 0 */
    mov a,#00h
    acall i2c_write;
    acall i2c_stop;
    ret
alarm_key:
    ret
; END of key scan

setmin: 
    inc min
    mov a,min
    cjne a,#0Ah,next4
    mov min,#10h
next4:
    cjne a,#1Ah,next5
    mov min,#20h
next5:
    cjne a,#2Ah,next6
    mov min,#30h
next6:
    cjne a,#3Ah,next7
    mov min,#40h
next7:
    cjne a,#4Ah,next8
    mov min,#50h
next8:
    cjne a,#5Ah,next9
    mov min,#00h
next9:
    mov led3,min
    mov led4,min
    anl led3,#0Fh
    anl led4,#0F0h
    mov a,led4
    swap a
    mov led4,a
    ret
sethour:
    inc hour
    mov a,hour
    cjne a,#0Ah,next10
    mov hour,#10h
next10:
    cjne a,#13h,next11
    mov hour,#01h
next11:
    mov led5,hour
    mov led6,hour
    anl led5,#0Fh
    anl led6,#0F0h
    mov a,led6
    swap a
    mov led6,a
    ret
setdate:
    inc date
    ret
setmonth:
    inc month
    ret
setyear:
    inc year
    ret
display:
    mov a,led1
    movc a,@a+dptr
    mov p2,a
    clr p0.0
    acall delay
    setb p0.0

    mov a,led2
    movc a,@a+dptr
    mov p2,a
    clr p0.1
    acall delay
    setb p0.1

    mov a,led3
    movc a,@a+dptr
    mov p2,a
    clr p0.2
    acall delay
    setb p0.2

    mov a,led4
    movc a,@a+dptr
    mov p2,a
    clr p0.3
    acall delay
    setb p0.3

    mov a,led5
    movc a,@a+dptr
    mov p2,a
    clr p0.4
    acall delay
    setb p0.4

    mov a,led6
    movc a,@a+dptr
    mov p2,a
    clr p0.5
    acall delay
    setb p0.5
    ret
    
read_time:
    acall i2c_start        ;/* The following Enables the Oscillator */
    mov a,#0D0h
    acall i2c_write        ;  /* address the part to write */
    mov a,#00h
    acall i2c_write;  /* position the address pointer to 0 */
    acall i2c_start
    mov a,#0D1h
    acall   i2c_write;     /* write 0 to the seconds register, clear the CH bit */
    clr i2c_ack_out   ; no ACK
    acall i2c_read
    mov sec,a
    setb i2c_ack_out
    acall i2c_read
    mov min,a
    acall i2c_stop;
    mov a,sec
    jz update_time
next:
    mov led1,sec
    mov led2,sec
    anl led1,#0Fh
    anl led2,#0F0h
    mov a,led2
    swap a
    mov led2,a
    ret

update_time:
    acall i2c_start
    mov a,#0D0h
    acall i2c_write
    mov a,#0
    acall i2c_write
    acall i2c_start
    mov a,#0D1h
    acall i2c_write
    clr i2c_ack_out
    acall i2c_read
    mov sec,a
    acall i2c_read
    mov min,a
    setb i2c_ack_out
    acall i2c_read
    mov hour,a
    acall i2c_stop

    mov led1,sec
    mov led2,sec
    anl led1,#00001111b
    anl led2,#11110000b
    mov a,led2
    swap a
    mov led2,a

    mov led3,min
    mov led4,min
    anl led3,#0Fh
    anl led4,#0F0h
    mov a,led4
    swap a
    mov led4,a

    anl hour,#3Fh
    mov led5,hour
    mov led6,hour
    anl led5,#0Fh
    anl led6,#0F0h
    mov a,led6
    swap a
    mov led6,a
    ret

;i2c_start:
;    setb sda    ;sda =1
;    setb scl    ;sc=1
;   clr sda     ;sda=0
;    ret
;i2c_stop:
;    clr sda
;    setb scl
;   setb sda
;    ret
i2c_start:                 ; I2C Startbedingung
       setb sda
       nop
       setb scl
       acall i2c_wait;
       clr sda             ; Data auf 0 (Start)
       nop
       clr scl             ; Clock auf 0
       ret
i2c_stop:                  ; I2C Stopbedingung
      ;clr sda
       setb scl            ; Clock auf 1
       nop
       setb sda            ; Data auf 1 (Stop)
       ret
i2c_write:                 ; send 1 data byte (Accu) via I2C
       push 3
       mov r3,#8
i2c_sloop:
       rlc a
       mov sda,c
       nop
       setb scl
       ;acall i2c_wait
       clr scl
       djnz r3,i2c_sloop
       nop
       setb sda
       nop
       setb scl
       ;acall i2c_wait           
       mov c,sda            ; read Acknowledge-bit
       mov i2c_ack_in,c
       clr scl
       nop
       clr sda
       pop 3
       ret

i2c_read:                   ; receive 1 data byte via I2C (Accu)
       push 3
       mov r3,#8
       clr a
       setb sda
i2c_rloop:
       ;acall i2c_wait
       setb scl
       nop
       mov c,sda
       rlc a
       nop
       clr scl
       djnz r3,i2c_rloop

       mov c,i2c_ack_out   ; send Acknowledge-bit
       mov sda,c
       nop
       setb scl
       ;acall i2c_wait
       clr scl
       nop
       clr sda
       pop 3
       ret
delay:
    mov r7,#2
dl1:
    mov r6,#255
here:
    nop
    nop
    djnz r6,here
    djnz r7,dl1
    ret
i2c_wait:               ; Verzoegerung fuer I2C (CALL + RET -> 4us bei 12MHz)
       ret
maled7:
        DB     0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H    ;90h LAM CHO SO 9 TREN SEG-7 LA 6 GACH
        DB     88H,0C6H,86H,8EH,82H,89H
        END
end
