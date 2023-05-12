ORG 00H
MOV DPTR,#LUT // moves starting address of LUT to DPTR
MOV A,#11111111B // loads A with all 1's
MOV P2,#00000000B // initializes P0 as output port

BACK:MOV P1,#11111111B // loads P1 with all 1's
     CLR P1.0  // makes row 1 low
     JB P1.4,NEXT1  // checks whether column 1 is low and jumps to NEXT1 if not low
     MOV A,#0   // loads a with 0D if column is low (that means key 1 is pressed)
     ACALL DISPLAY  // calls DISPLAY subroutine
NEXT1:JB P1.5,NEXT2 // checks whether column 2 is low and so on...
      MOV A,#1
      ACALL DISPLAY
NEXT2:JB P1.6,NEXT3
      MOV A,#2
      ACALL DISPLAY
NEXT3:JB P1.7,NEXT4
      MOV A,#3
      ACALL DISPLAY
NEXT4:SETB P1.0
      CLR P1.1
      JB P1.4,NEXT5
      MOV A,#4
      ACALL DISPLAY
NEXT5:JB P1.5,NEXT6
      MOV A,#5
      ACALL DISPLAY
NEXT6:JB P1.6,NEXT7
      MOV A,#6
      ACALL DISPLAY
NEXT7:JB P1.7,NEXT8
      MOV A,#7
      ACALL DISPLAY
NEXT8:SETB P1.1
      CLR P1.2
      JB P1.4,NEXT9
      MOV A,#8
      ACALL DISPLAY
NEXT9:JB P1.5,NEXT10
      MOV A,#9
      ACALL DISPLAY
NEXT10:JB P1.6,NEXT11
       MOV A,#10
       ACALL DISPLAY
NEXT11:JB P1.7,NEXT12
       MOV A,#11
       ACALL DISPLAY
NEXT12:SETB P1.2
       CLR P1.3
       JB P1.4,NEXT13
       MOV A,#12
       ACALL DISPLAY
NEXT13:JB P1.5,NEXT14
       MOV A,#13
       ACALL DISPLAY
NEXT14:JB P1.6,NEXT15
       MOV A,#14
       ACALL DISPLAY
NEXT15:JB P1.7,BACK
       MOV A,#15
       ACALL DISPLAY
       LJMP BACK

DISPLAY:MOVC A,@A+DPTR // gets digit drive pattern for the current key from LUT
        MOV P2,A       // puts corresponding digit drive pattern into P0
        RET

LUT: DB 3FH, 06H, 5BH, 4FH, 66H, 6DH, 7DH, 07H, 7FH, 6FH, 77H, 7CH, 39H, 5EH, 79H, 71H, 40H  	;Look up table starts here

     END

