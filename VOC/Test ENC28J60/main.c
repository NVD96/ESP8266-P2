#include <REGX52.H>
#include <stdio.h>
  sbit spi_SCK = P2^0;
  sbit spi_MOSI = P2^1;
  sbit spi_MISO = P2^2;
  sbit spi_CS = P2^3;	

unsigned char kqua;
void UARTInit();
void SPIInit();
void delay_ms(int tg);
unsigned char SPIRead();
void SPIWrite(unsigned char b);


void main(){

  SPIInit();
  UARTInit();
  printf("Khoi tao ENC\n");


  spi_CS = 0;
  SPIWrite(0xFF); //Reset chip
  SPIWrite(0x00);
  spi_CS = 1;
 

  spi_CS = 0;
  SPIWrite(0x1D);	//Doc thanh ghi ESTAT
  SPIRead();
  spi_CS = 1; 

  spi_CS = 0;
  SPIWrite(0xBF); //Set bank
  SPIWrite(0x03);
  spi_CS = 1;

 
  spi_CS = 0;
  SPIWrite(0x00);	//Doc thanh ghi ESTAT
  kqua = SPIRead();
  spi_CS = 1; 

  printf("%X\n", kqua);
   if(kqua == 0xFA) printf("Khoi tao ENC28J60 thanh cong\n");
 	else printf("Khoi tao ENC28J60 that bai\n");
  
  while(1){
  
 
  }

 }



 void UARTInit(){
 
    SCON =0x52; // Port noi tiep che do 1, REN =TI =1;	 {Chu y co TI khi khoi tao, code chay khong la do co TI}
	TMOD = 0x20; //Timer 1 mode 2
	TH1 = TL1 = -3; // Toc do baud la 9600
	TR1 =1;

 }


 void SPIInit(){
  
 	 spi_CS = 1;	  //set muc cao, chuan bi set muc thap de truyen data
 	 spi_SCK = 0; 	  // set muc thap chuan bi phat xung clock
	 spi_MISO = 1;   //set muc cao o chip master, san sang doc data tu slave
  }

 void delay_ms(int tg){
 
 	 int i, x;
	 for(i = 0; i < tg; i++){
	 	for(x = 0; x < 123; x++);
	 }
 
 
 }


 unsigned char SPIRead(){		  //Ham doc data
  
  	 unsigned char i, b = 0;
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
	 if( b & 0x80)			  // So sanh de lay gia tri bit dau tien (gui bit cao truoc [bit 7]--> bit thap sau [bit 1])
	 spi_MOSI = 1;			  // Neu bit =1 thi chan MOSI = 1
	 else 
	 spi_MOSI = 0;			 // Neu bit =0 thi chan MOSI = 0
	 spi_SCK = 1;			  // Bat dau ghi 1 bit
	 spi_SCK =0;			  // chuan bi de gui 1 bit tiep theo
	 b<<=1;					 // Dich trai 1 bit de lay bit tiep theo
	 }
  }



