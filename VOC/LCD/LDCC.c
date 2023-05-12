#include<regx52.h>

sbit LCD_RS = P3^5;
sbit LCD_EN = P3^7;
sbit LCD_RW = P3^6;	
sbit spi_SCK = P2^0;
sbit spi_MOSI = P2^1;
sbit spi_MISO = P2^2;
sbit spi_CS = P2^3; 
#define LCD_DATA P1	 // Khai báo chân data.

unsigned char kqua;


void Delay_ms(unsigned int t) //Hàm delay
{
	unsigned int x, y ;
	for(x=0;x<t;x++)
	{
	 for(y = 0; y<123;y++);
	}
}

void SPIInit(){
  
 	 spi_CS = 1;	  //set muc cao, chuan bi set muc thap de truyen data
 	 spi_SCK = 0; 	  // set muc thap chuan bi phat xung clock
	 spi_MISO = 1;   //set muc cao o chip master, san sang doc data tu slave
  }


unsigned char SPIRead(){		  //Ham doc data
  
  	 unsigned char i, b;
	 for (i = 0; i < 8; i++){	//doc ve 1 byte
	 b<<=1;			   // dich byte b sang trai 1 bit 
	 spi_SCK = 1;	   // bat dau nhan bit dau tien
	 if(spi_MISO) 	   //kiem tra muc tin hieu cua chan MISO 
	 b|=0x01;		   // dua tin hieu bit cua chan MISO vao byte b
	 spi_SCK = 0;	   // ket thuc qua trinh nhan 1 bit
	 
	 }   
  	return b;	 // khi doc du 1 byte tra ve gias tri doc duoc
  }

void SPIWrite(unsigned char b){
  
  	 unsigned char i;
	 for(i = 0; i < 8; i++){  // Ghi 1 byte vao EEFROM
	 spi_SCK = 0;			  // chuan bi de gui 1 bit
	 if( b &0x80)			  // So sanh de lay gia tri bit dau tien (gui bit cao truoc [bit 7]--> bit thap sau [bit 1])
	 spi_MOSI = 1;			  // Neu bit =1 thi chan MOSI = 1
	 else 
	 spi_MOSI = 0;			 // Neu bit =0 thi chan MOSI = 0
	 spi_SCK =1;			 // Bat dau ghi 1 bit
	 b<<=1;					 // Dich trai 1 bit de lay bit tiep theo
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
SPIInit();

//RESET
spi_CS = 0;
SPIWrite(0xFF);
SPIWrite(0x00);
spi_CS = 1;
Delay_ms(100);

// SET BANK
spi_CS = 0;
Delay_ms(1);
SPIWrite(0x9F);
SPIWrite(0x03);
spi_CS = 1;
Delay_ms(1);

spi_CS = 0;
SPIWrite(0xBF);
SPIWrite(0x00);
spi_CS = 1;
Delay_ms(1);

spi_SCK = 0;
spi_CS = 0;
SPIWrite(0x00);
kqua = SPIRead();
spi_CS = 1;
if(kqua == 0xFA)
	Lcd_Out(1, 2, "DUNG");
else 
    Lcd_Out(1, 2, "SAI");
while(1)
{
}
}