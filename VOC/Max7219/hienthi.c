#include <REGX52.H>
#include<stdio.h>

#define No_OP 0x00
#define Decode 0x09
#define Intensity 0x0A
#define Scan 0x0B
#define Shutdown 0x0C
#define DislayTest 0x0F

LED[32]={0xFF,0xFF,0xC0,0xFE,0xFF,0x03,0xFF,0xFE,0xFF,0xFF,0x18,0x18,0x18,0x18,0x18,0x18,0xC3,0xC3,0xC3,0x66,0x66,0x24,0x18,0x18,0xFF,0xFF,0x18,0x18,0x18,0x18,0x18,0x18};
sbit spi_SCK = P2^0;
sbit spi_DI = P2^1;
sbit spi_CS = P2^2;

unsigned char chay;

void SPI_Int();
void delay_ms(unsigned int tg);
void SPI_Write(unsigned char b);
void WriteByte(unsigned char op, unsigned char dl);
void Shutdown_Max(unsigned char status);
void Scan_Max(unsigned char status);
void Intensity_Max(unsigned char status);
void Decode_Max(unsigned char status);
void DislayTest_Max(unsigned char status);
void Max_Int();
void Del_Reg();
void Del_LedMax();
void Quet1LedMax(unsigned char n);
void LedMax();



void main(){

SPI_Int();
Max_Int();
Del_LedMax();


while(1){

	  LedMax();
	  delay_ms(1000); 
	} 
}

void SPI_Int(){

	 spi_SCK = 0;
	 spi_CS = 1;
}
void delay_ms(unsigned int tg){
	int i,j;
	for(i = 0; i<tg; i++){
		for(j=0; j<123; j++);
		}
}

void SPI_Write(unsigned char b){
	  unsigned char i;
	  for(i = 0; i<8; i++){
	  	spi_SCK = 0;
		if(b&0x80) spi_DI = 1;
		else spi_DI = 0;
		spi_SCK = 1;
		b<<=1;
	  }
}


void WriteByte(unsigned char op, unsigned char dl){

	  spi_CS = 0;
	  SPI_Write(op);
	  SPI_Write(dl);
	  spi_CS = 1;
}



void Shutdown_Max(unsigned char status){

	  unsigned char i;
	  spi_CS = 0;
	  for(i = 0; i < 4; i++){
	  SPI_Write(Shutdown);
	  SPI_Write(status);
	  }
	  spi_CS = 1;

}

void Scan_Max(unsigned char status){

  	  unsigned char i;
	  spi_CS = 0;
	  for(i = 0; i < 4; i++){
	  SPI_Write(Scan);
	  SPI_Write(status);
	  }
	  spi_CS = 1;
}


void Intensity_Max(unsigned char status){

  	  unsigned char i;
	  spi_CS = 0;
	  for(i = 0; i < 4; i++){
	  SPI_Write(Intensity);
	  SPI_Write(status);
	  }
	  spi_CS = 1;
}


void Decode_Max(unsigned char status){

  	  unsigned char i;
	  spi_CS = 0;
	  for(i = 0; i < 4; i++){
	  SPI_Write(Decode);
	  SPI_Write(status);
	  }
	  spi_CS = 1;
}

void DislayTest_Max(unsigned char status){

  	  unsigned char i;
	  spi_CS = 0;
	  for(i = 0; i < 4; i++){
	  SPI_Write(DislayTest);
	  SPI_Write(status);
	  }
	  spi_CS = 1;
}

void Max_Int(){

Shutdown_Max(0x00);
delay_ms(100);
Shutdown_Max(0x01);
DislayTest_Max(0x00);
Decode_Max(0x00);
Intensity_Max(0x00);
Scan_Max(0x07);

}

void Del_LedMax(){
	unsigned char i;
	for(i = 0; i < 11; i++){
	WriteByte((i%8) + 1, 0x00);
	}
}

void Quet1LedMax(unsigned char n){
	unsigned char i;

	spi_CS = 0;
	SPI_Write((chay%8) + 1);
	SPI_Write(LED[chay]);
	for(i = 0; i < 2*n; i++){
	   SPI_Write(No_OP);
	}
	spi_CS = 1;
	Del_Reg();
}

void Del_Reg(){

	unsigned char i; 
	spi_CS = 0;
	for(i = 0; i <8; i++){
	   SPI_Write(No_OP);
	}
	spi_CS = 1;
}

void LedMax(){
	
	  for(chay = 0; chay < 32; chay++){
	  Quet1LedMax(chay/8);
		}
}














