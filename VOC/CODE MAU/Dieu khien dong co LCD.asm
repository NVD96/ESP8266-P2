#include <sfr52.inc>
ORG		0H
	
	LJMP 	SCANKEY 	; g‰i ch≠¨ng tr◊nh con qu–t bµn ph›m
;Ch≠¨ng tr◊nh con Æi“u khi”n ÆÃn LED bªng bµn ph›m
Start:
LJMP 	SCANKEY 	; g‰i ch≠¨ng tr◊nh con qu–t bµn ph›m
;Ch≠¨ng tr◊nh con Æi“u khi”n ÆÃn LED bªng bµn ph›m
SOSANH:	
	MOV	R0,#32H		;nπp thanh ghi R0 gi∏ trﬁ #30h ( m∑ ASCII cÒa ph›m 0) Æ” so s∏nh
	XRL	A,R0		;so s∏nh m∑ bµn ph›m
	JZ	QUAYPHAI 	; n’u ÆÛng th◊ A = 0, nh∂y tÌi ch≠¨ng tr◊nh con QUAYPHAI,n’u A #0 th˘c hi÷n l÷nh ti’p theo
	MOV	A,R5		; chuy”n  m∑ bµn ph›m (m∑ ASCII) vµo A
	MOV	R1,#33H		;nπp thanh ghi R1 gi∏ trﬁ #31h ( m∑ ASCII cÒa ph›m 1) Æ” so s∏nh
	XRL	A,R1		;so s∏nh m∑ bµn ph›m
	JZ	QUAYTRAI	; n’u ÆÛng th◊ A = 0, nh∂y tÌi ch≠¨ng tr◊nh con NHAY1,n’u A #0 th˘c hi÷n l÷nh ti’p theo	
	LJMP	SCANKEY
	RET
;Ch≠¨ng tr◊nh con gˆi li™n tÙc mÈt chuÁi xung tÌi cÊng P1, ÆÂng thÍi ki”m tra xem c„ ph›m kh∏c Æ≠Óc  n, n’u c„ nh∂y tÌi ch≠¨ng tr◊nh con kh∏c
QUAYPHAI:
MOV 	P2,#00h
acall	Lcd
Loop:
CLR	p2.4
SETB	p2.7
ACALL	Delay_1s
CLR	p2.7
SETB	p2.6
Acall	delay_1s
CLR	p2.6
SETB	p2.5
acall	delay_1s
clr	p2.5
setb	p2.4
acall	delay_1s
acall	test
SJMP	Loop
;
QUAYTRAI:
MOV 	P2,#00h
acall	Lcd
Loop1:
CLR	p2.6
SETB	p2.7
ACALL	Delay_1s
CLR	p2.7
SETB	p2.4
Acall	delay_1s
CLR	p2.4
SETB	p2.5
acall	delay_1s
clr	p2.5
setb	p2.6
acall	delay_1s
acall	test
SJMP	Loop1
;Ch≠¨ng tr◊nh con qu–t bµn ph›m theo b∂ng m∑ ASCII vµ gˆi m∑ ASCII tÌi thanh ghi t›ch lu¸ A (sˆ dÙng 4 ph›m Æi“u khi”n,cÊng P0 use lµm cÈt, P2 use lµm hµng) 
SCANKEY:
	MOV	P0,#0FFH		; Æ∆t cÊng P2 lµm cÊng vµo
K1:	
	CLR	P0.3
	CLR	P0.4
	MOV	A,P0		; Æ‰c cÈt P0D
	ANL	A,#00000111B	; che c∏c b›t ko dÔng
K2:	ACALL	DELAY20	; ÆÓi 20ms
	CJNE	A,#00000111B,K1	; ki”m tra nh∂ ph›m
	MOV	A,P0		; Æ‰c cÈt
	ANL	A,#00000111B	; che c∏c b›t ko dÔng
	CJNE	A,#00000111B,OVER	; x∏c nhÀn c„ ph›m  n
	SJMP	K2
OVER:	ACALL	DELAY20	; ÆÓi 20ms
	MOV	A,P0		; Æ‰c cÊng P2
	ANL	A,#00000111B	; che c∏c b›t ko dÔng
	CJNE	A,#00000111B,OVER1	; c„ ph›m nh n th◊ x∏c Æﬁnh hµng
	SJMP	K2
OVER1:	MOV	P0,#11110111B	; ti’p Æ t hµng 0
	MOV	A,P0		; Æ‰c c∏c cÈt
	ANL	A,#00000111B	; che c∏c b›t ko dÔng
	CJNE	A,#00000111B,HANG0	; x∏c nhÀn hµng 0 c„ ph›m nh n
	MOV	P0,#11101111B	; ti’p Æ t hµng 0
	MOV	A,P0		; Æ‰c c∏c cÈt
	ANL	A,#00000111B	; che c∏c b›t ko dÔng
	CJNE	A,#00000111B,HANG1	; x∏c nhÀn hµng 1 c„ ph›m nh n
	LJMP	K2
HANG0:	
	MOV 	DPTR,#KCODE0	; Æ∆t gi∏ trﬁ khÎi ÆÈng tı hµng 0
	SJMP	FIND		; t◊m cÈt c„ ph›m nh n
HANG1:	
	MOV 	DPTR,#KCODE1	; Æ∆t gi∏ trﬁ khÎi ÆÈng tı hµng 1
FIND:	
	RRC	A		; ki”m tra cÍ CY c„ Î m¯c th p kh´ng
	JNC	MATCH		; n’u bªng 0, l y m∑ ASCII
	INC	DPTR		; n’u kh´ng tr· Æ’n cÈt ti’p theo
	SJMP	FIND		; ti’p tÙc t◊m
MATCH:	CLR	A		; xo∏ thanh ghi A
	MOVC	A,@A+DPTR	; x∏c Æﬁnh m∑ ASCII tı b∂ng vµ gˆi vµo thanh ghi A
	MOV 	R5,A
	LJMP	SOSANH
	ret
;Ch≠¨ng tr◊nh con ki”m tra bµn ph›m c„ Æ≠Óc nh n kh´ng
TEST:
	MOV	P0,#0FFH
	CLR	P0.3
	CLR	P0.4
	MOV	A,P0
	ANL	A,#00011111B
	CJNE	A,#00000111B,KT
	SJMP	EXIT
KT:	LJMP	OVER1
	RET
; Ch≠¨ng tr◊nh con tho∏t 
EXIT:
	RET
;

Delay_1s:
   	mov    r0,#2
loop4:
   	mov    r1,#250
loop5:
   	nop
   	nop
   	nop
   	nop
   	nop
 	nop
   	nop
   	nop
   	djnz   r1,loop5
   	djnz   r0,loop4
ret
;Ch≠¨ng tr◊nh con tπo tr‘ 20ms
DELAY20:
	MOV 	R4,#9
HERE:	DJNZ	R4,HERE
	RET
;
;Chuong trinh hien thi

HIENTHI:
        ACALL	DELAY_50MS
	MOV 	P3,#0FFH
	MOV	P3,#00H
	MOV	P3,#0FFH
LAP:
	ACALL	DELAY_50MS
	MOV     A,#38H        ;init. LCD 2 do`ng, ma tra^.n 5x7 
       	ACALL   CSTROBE 
      	ACALL	DELAY_50MS
	MOV     A,#0EH        ;LCD on, cursor on
   	ACALL   CSTROBE
	ACALL	DELAY_50MS
   	MOV     A,#01H        ;clear LCD
        ACALL   CSTROBE
	ACALL	DELAY_50MS
       	MOV	A,#82H
        ACALL   CSTROBE
	ACALL	DELAY_50MS
        MOV     DPTR,#MYDATA0    ;Dua con tro du lieu toi bang
DONG1:  CLR     A
        MOVC    A,@A+DPTR
        JZ      XUONGDONG    ;Neu A=0 thi thi xuong dong tiep theo
        ACALL   DSTROBE
        ACALL	Delay_1s
        INC     DPTR
XUONGDONG:  
        MOV     A,#0C0H      ;Chuyen con tro den dong 2 cot 1
        ACALL   CSTROBE     ;Gui lenh den LCD
        INC     DPTR        ;Tang con tro du lieu
DONG2:  CLR     A           ;xoa A
        MOVC    A,@A+DPTR
        ACALL   DSTROBE
	ACALL	Delay_1s
        INC     DPTR	
CSTROBE:                      ;command strobe
        ACALL   READY         ;is LCD ready?
        MOV     P1,A          ;xua^'t ma~ le^.nh
        CLR     P3.0            ;RS=0: le^.nh
        CLR     P3.1            ;R/W=0 -> ghi ra LCD
        SETB    P3.2             ;E=1 -> ta.o ca.nh xuo^'ng
 
        CLR     P3.2             ;E=0 ,cho^'t
        RET
DSTROBE:                      ;data strobe
        ACALL   READY         ;is LCD ready?
        MOV     P1,A          ;xua^'t du+~ lie^.u  
        SETB    P3.0            ;RS=1 for data
        CLR     P3.1            ;R/W=0 to write to LCD
        SETB    P3.2             ;E=1 -> ta.o ca.nh xuo^'ng
     
	CLR     P3.2             ;E=0, cho^'t 
        RET
; kie^?m tra co+` BF
READY:  SETB    P1.7          ;P1.7: input
        CLR     P3.0            ;RS=0: thanh ghi le^.nh
        SETB    P3.1            ;R/W=1: ddo.c
BACK:   CLR     P3.2             ;E=0 -> ta.o ca.nh le^n
        SETB    P3.2             ;E=1  
        JB      P1.7,BACK     ;cho+` busy flag=0
        RET

Delay_50ms:
   	mov    r0,#1
loop:
   	mov    r1,#1
loop1:
	nop
     	djnz   r1,loop1
   	djnz   r0,loop
ret

Delay_1s:
   	mov    r0,#250
loop4:
   	mov    r1,#250
loop5:
   	nop
   	nop
   	nop
   	nop
   	nop
 	nop
   	nop
   	nop
   	djnz   r1,loop5
   	djnz   r0,loop4
ret
;---------------------------------------------------------------------------------------
;B∂ng m∑ ASCII tham chi’u qu–t bµn ph›m bæt Æ«u tı Æﬁa chÿ 300H
ORG	300H
	KCODE0:	 DB	'0','1','2'
	KCODE1:	 DB	'3','4','5'
	MYDATA0: DB  	'QUAY THUAN'
	MYDATA1: DB  	'QUAY NGUOC'
	MYDATA2: DB  	'TANG TOC'
	MYDATA3: DB  	'GIAM TOC'
	MYDATA4: DB  	'STOP'
	MYDATA5: DB  	'NOCOMAND'
END