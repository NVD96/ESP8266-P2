#include <sfr52.inc>

SCL BIT P3.1
SDA BIT P3.0
RTCW EQU 0D0H
RTCR EQU 0D1H
FLAGS DATA 20H
LASTREAD BIT FLAGS.0
BUSFAULT BIT FLAGS.6
_2WBUSY BIT FLAGS.7
BITCOUNT DATA 21H
BYTECOUNT DATA 22H
BYTE DATA 23H
i2c_ack_in equ 0       ; flag bit on RAM bit area at 20h
i2c_ack_out equ 1  

ORG 0000H
LJMP MAIN
ORG 0003H
LJMP EX0ISR
ORG 0030H

MAIN:
    MOV DPTR,#LED7SEG
    SETB EX0
    SETB IT0
    MOV R6,#40      ;R6 chua so thu tu cac giay (giay 1, giay 2,...)
    mov p3,#0ffh
    ;SETB P3.2		; THIET LAP P3.3 LA DAU VAO
    SETB EA
    LCALL SETCLOCK
    
LOOP: 
    CJNE R6,#10,LOOP1
LAP1:
    CJNE R6,#4,LOOP2
LAP2:
    CJNE R6,#0,LOOP3
    MOV R6,#40
    LJMP LOOP
LOOP1:
    CLR P1.0
    LCALL DELAY1MS
    SETB P1.0
    CLR P1.3
    LCALL DELAY1MS
    SETB P1.3
    LCALL SHOWLED7
    LJMP LOOP

LOOP2:
    CLR P1.0
    LCALL DELAY1MS
    SETB P1.0
    LCALL SHOWLED7
    LJMP LAP1

LOOP3: 
    CLR P1.0
    LCALL DELAY1MS
    SETB P1.0
    LCALL SHOWLED7
    LJMP LAP2

SHOWLED7:
    LCALL BIN2BCD
    MOVC A,@A+DPTR
    CLR P0.1
    MOV P2,A
    LCALL DELAY1MS
    SETB P0.1
    MOV A,B
    MOVC A,@A+DPTR
    CLR P0.0
    MOV P2,A
    LCALL DELAY1MS
    SETB P0.0
RET

BIN2BCD:
        MOV A,R6
        MOV B,#10           ; B=10
        DIV AB              ; chia cho 10
        RET

EX0ISR:
      DEC R6    ;tang R0 de nhan biet giay thu bao nhieu
RETI

DELAY1MS:
    MOV 33H,#10
    LOOP4: MOV 34H,#50
    WAIT: DJNZ 34H,WAIT
    DJNZ 33H,LOOP4
RET

SETCLOCK:
    ACALL SENDSTART
    MOV A,#0D0h  ;RTCW
    ACALL SENDBYTE
    MOV A,#00H
    ACALL SENDBYTE
    MOV A,#00H    ;00 GIAY
    ACALL SENDBYTE
    MOV A,#15H    ;15 PHUT
    aCALL SENDBYTE
    MOV A,#12H    ;12 GIO
    aCALL SENDBYTE
    ;LCALL SENDSTOP
    acall sendstart;     // Write control and status register
    mov a,#0d0h
    acall sendbyte; // write slave address + write */
    mov a,#0eh
    acall sendbyte; // write register address, control register */
    mov a,#00000011b
    acall sendbyte; /* enable osc, bbsqi */
    mov a,#00h
    acall sendbyte;
    acall sendstop;
RET 

DELAY: 
    NOP
RET

sendstart:                 ; I2C Startbedingung
       setb sda
       nop
       setb scl
       acall i2c_wait;
       clr sda             ; Data auf 0 (Start)
       nop
       clr scl             ; Clock auf 0
       ret
sendstop:                  ; I2C Stopbedingung
      ;clr sda
       setb scl            ; Clock auf 1
       nop
       setb sda            ; Data auf 1 (Stop)
       ret
sendbyte:                 ; send 1 data byte (Accu) via I2C
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

readbyte:                   ; receive 1 data byte via I2C (Accu)
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
i2c_wait:               ; Verzoegerung fuer I2C (CALL + RET -> 4us bei 12MHz)
       ret

LED7SEG: DB 0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H
END