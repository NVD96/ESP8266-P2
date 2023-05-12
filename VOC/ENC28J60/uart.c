#include <REGX52.H>	 ///Thu vien truyen duoc UART bang ki tu
//#include <stdio.h>

char KYTU = 0X10;

void main(void)
{
SCON =0x52; // Port noi tiep che do 1, REN =TI =1;	 {Chu y co TI khi khoi tao, code chay khong la do co TI}
TMOD = 0x20; //Timer 1 mode 2
TH1 = TL1 = -3; // Toc do baud la 9600
TR1 =1;

while(!TI); // Cho TI =1;
	TI=0; //Xoa TI
	SBUF ='Z' ; // Truyen byte du lieu	
}