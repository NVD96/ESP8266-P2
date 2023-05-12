#include<regx52.h>

sbit LCD_RS = P2^6;
sbit LCD_EN = P2^7;
sbit LCD_RW = P2^5;	 
#define LCD_DATA P0	 // Khai báo chân data.

void Delay_ms(unsigned int t) //Hàm delay
{
	unsigned int x, y ;
	for(x=0;x<t;x++)
	{
	 for(y = 0; y<120;y++)
	}
}

void Lcd_cmd(unsigned char cmd)	  //Hàm g?i l?nh ra LCD

{ 

   LCD_RW =0;// Ch?n ch? d? ghi
   LCD_RS =0;// Ch?n ch? d? ghi l?nh
   LCD_DATA =cmd;// cmd là l?nh c?n g?i ra màn hình
   LCD_EN = 0; 						  
   LCD_EN =1;
   if(cmd<=0x02){
   Delay_ms(2);
   }
   else{
   Delay_ms(1);}

}
void Lcd_Char_Cp(char c)//Hàm hi?n th? 1 kí t? t?i v? trí con tr?
{
   LCD_RW =0;
   LCD_RS =1; //Ch?n ch? d? g?i d? li?u
   LCD_DATA =c;	 //Kí t? c?n g?i ra màn hình
   LCD_EN = 0;
   LCD_EN =1;
   Delay_ms(1);

}
void Lcd_Out_Cp(char *str)// Hàm g?i chu?i kí t? t?i v? trí con tr?

{

unsigned char i=0;
while(str[i]!=0)  //G?i t?ng kí t? c?a chu?i d?n khi g?p kí t? null
{
	 Lcd_Char_Cp(str[i]);
	 i++;
}

}


void Lcd_Out(unsigned char row, unsigned char col, char *str)		//Hàm Di chuy?n v? trí con tr? d?n v? trí b?t kì và in chu?i ra màn hình
{
 	unsigned char cmd;

	// Di chuy?n con tr? d?n v? trí c?n thi?t
	cmd = (row==1?0x80:0xC0) + col - 1;
	Lcd_cmd(cmd);
	Lcd_Out_Cp(str);

}


void Lcd_Init() //Hàm kh?i t?o LCD
{

 Lcd_cmd(0x30);
 Delay_ms(5);
 Lcd_cmd(0x30);
 Delay_ms(1);
 Lcd_cmd(0x30);
 Lcd_cmd(0x38);	  // S? dòng hi?n th? là 2, c? ch? 5x8
 Lcd_cmd(0x01);	  // xóa màn hình
 Lcd_cmd(0x0C);	  //B?t hi?n th? và t?t con tr? các b?n có th? dùng l?nh 0xE0 d? b?t hi?n th? và b?t con tr?



}


main()

{
 
Lcd_Init();
Lcd_Out(1,4,"Dien Tu 3M")	;   //In dòng "Dien Tu 3M" ra hàng 1 và c?t 4 trên màn hình
Lcd_Out(2,3,"Chotroihn.Vn")	;  //In dòng "Chotroihn.vn" ra hàng 2 và c?t 3 trên màn hình
while(1)
{
}
}