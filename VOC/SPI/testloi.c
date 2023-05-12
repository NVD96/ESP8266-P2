#include <REGX52.H>
#include <stdio.h>

#define READ 0x03		 // Doc data tu bo nho, chu y bit A8   (ADD[0-->7] A8 = 0 <==> 0x03, ADD[8-512] A8 = 1 <==> 0x0B])
#define WRITE 0x02		 //	Ghi data vao bo nho, chu y bit A8  (ADD[0-->7] A8 = 0 <==> 0x02, ADD[8-512] A8 = 1 <==> 0x0A])
#define WRDI 0x04		  // Khong cho phep ghi vao EEFROM
#define WREN 0x06		 // cho phep ghi vao EEFROM
#define RDSR 0x05		 // Doc trang thai cua qua trinh doc
#define WRSR 0x01		 // Doc trang thai cua qua trinh ghi

  sbit spi_SCK = P2^0;
  sbit spi_MOSI = P2^1;
  sbit spi_MISO = P2^2;
  sbit spi_CS = P2^3;	

char kq;

void SPIInit();
void UARTInit();
unsigned char SPIRead();
void SPIWrite(unsigned char b);
unsigned char ROMReadByte(unsigned char add);
void ROMWriteByte(unsigned char add, unsigned char dl);
bit ROMStatus();

void main(){

  SPIInit();
  UARTInit();
  ROMWriteByte(0x09, 0xFA);
  printf("Dang test SPI\n");
  kq = ROMReadByte(0x09);
  printf("%c", kq);

  while(1){
  
  }


}


void UARTInit(){
 SCON =0x52; // Port noi tiep che do 1, REN =TI =1;	 {Chu y co TI khi khoi tao, code chay khong la do co TI}
TMOD = 0x20; //Timer 1 mode 2
TH1 = TL1 = -3; // Toc do baud la 9600
TR1 =1;
}


 void SPIInit(void){
  
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
	 if( b &0x80)			  // So sanh de lay gia tri bit dau tien (gui bit cao truoc [bit 7]--> bit thap sau [bit 1])
	 spi_MOSI = 1;			  // Neu bit =1 thi chan MOSI = 1
	 else 
	 spi_MOSI = 0;			 // Neu bit =0 thi chan MOSI = 0
	 spi_SCK = 1;			  // Bat dau ghi 1 bit
	 spi_SCK =0;			 /// chuan bi de gui 1 bit tiep theo
	 b<<=1;					 // Dich trai 1 bit de lay bit tiep theo
	 }
  }

   unsigned char ROMReadByte(unsigned char add){

  unsigned char b;   // Dia chi can doc ve
  spi_CS = 0;		   // Cho phep bat dau doc EEFROM
  SPIWrite(READ);	   // Ghi xuong lenh doc data
  SPIWrite(add);	   // Ghi xuong dia chi muon doc
  b = SPIRead();	  //  Doc data va luu vao byte b
  spi_CS = 1;		  // Khong cho giao tiep EEFROM
  return b;			  // Tra ve data doc duoc
  
  }

   void ROMWriteByte(unsigned char add, unsigned char dl){

  spi_CS = 0; //Cho phep giao tiep EEFROM
  SPIWrite(WREN); // Cho phep ghi vao EEFROM
  spi_CS = 1; // Khoong cho phep giao tiep EEFROM
  spi_CS = 0; //Cho phep giao tiep EEFROM
  SPIWrite(WRITE); // Ghi xuong lenh ghi data
  SPIWrite(add);   // Ghi xuong dia chi muon ghi data
  SPIWrite(dl);   // Ghi xuong gia tri dl vao dia chi add
  spi_CS = 1;	 // Khoong cho phep giao tiep EEFROM
  while(ROMStatus()); // Kiem tra trang thai cua thanh (Neu EEFROM dang luu gia tri thi KHONG cho phep doc hay ghi du lieu)
  }
 

  bit ROMStatus(){

  unsigned char b;
  spi_CS = 0;	  //Cho phep giao tiep EEFROM
  SPIWrite(RDSR); // Ghi xuong lenh doc trang thai thanh ghi
  b = SPIRead();  // Doc ve gia tri trang thai thanh ghi va luu vao b
  spi_CS = 1;	  //Cho phep giao tiep EEFROM
  if (b & 0x01)  // So sanh voi 0x01
  return 1;		  // Neu b = 0x01 thi tra ve 1
  else
  return 0;		  // Neu b # 0x01 thi tra ve 0
 
  }