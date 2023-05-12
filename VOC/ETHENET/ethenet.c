#include <REGX52.H>
#include <stdio.h>

#define READCMD 0x00  //Opcode Read Control Register
#define WRITECMD 0x40 //Opcode Write Control Register
#define BFSet 0x80 	  //Opcode Bit Field Set
#define BFClear 0xA0  //Opcode Bit Field Set
#define SoftReset 0xFF  //Opcode Systerm Reset
#define WRITEBUFF 0x7A//Opcode Write Buff Register
#define READBUFF 0x3A  //Opcode Read Buff Register
 //Bank 2
#define MACON1 0x00
#define MACON3 0x02
#define MABBIPG 0x04
#define MAIPGL 0x06
#define MAIPGH 0x07
#define MAMXFLL 0x0A
#define MAMXFLH 0x0B
#define MIREGADR 0x14
#define MIWRL 0x16
#define MIWRH 0x17
#define EIE 0x1B
#define EIR 0x1C
#define ECON2 0x1E
#define ECON1 0x1F
 //Bank 3
#define MAADR5 0x00
#define MAADR6 0x01
#define MAADR3 0x02
#define MAADR4 0x03
#define MAADR1 0x04
#define MAADR2 0x05
//PHY
#define PHCON2 0x10
#define PHCON1 0x00
#define PHLCON 0x14


sbit spi_SCK = P2^0;
sbit spi_MOSI = P2^1;
sbit spi_MISO = P2^2;
sbit spi_CS = P2^3;

unsigned int NextPacket;
unsigned char NextPacketL, NextPacketH;
unsigned char myaddr[6];
unsigned char arp[50]; /*= {
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, //board cat
0x10,0x0A,0x20,0x07,0x19,0x89, //source MAC
0x08,0x06, //Ethernet type - ARP
0x00,0x01, //HTPYE
0x08,0x00,
0x06,0x04,
0x00,0x01,
0x10,0x0A,0x20,0x07,0x19,0x89, //source MAC
192,168,1,197,
0x00,0x00,0x00,0x00,0x00,0x00,
192,168,1,10,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};*/ 

void UARTInit();
void SPIInit();
void delay_ms(int tg);
unsigned char SPIRead();
void SPIWrite(unsigned char b);
void WriteByte(unsigned char op, unsigned char add, unsigned char dl);
unsigned char ReadByte(unsigned char op, unsigned char add);
unsigned char ReadByteMII(unsigned char op, unsigned char add);
void SysReset();
void SetBank(unsigned char bank);
//void EnRX();
void ENCInit();
void Buff_TX_RX();
void ConfigMAC();
void ReadMAC();
void WritePHY(unsigned char add, unsigned char dlL, unsigned char dlH);
void ConfigPHY();
void ENC_MISTAT();
void Write_Buff(int len, unsigned char* dl);
void Send_Packet(int len, unsigned char* dl);

unsigned char Read_BuffByte();
void Read_Buff(unsigned int len, unsigned char* dl);
unsigned int Read_Packet(int bufflen, unsigned char* dl);

void CheckARP();
void ARPrequest(unsigned int bufflen, unsigned char* dl);

int ghepint(unsigned char a,unsigned char b);
void tachint(unsigned int c);



void main(){

SPIInit();
UARTInit();
ENCInit();
Buff_TX_RX();
ConfigMAC();
ConfigPHY();



while(1){
	  
	   CheckARP();
	 //SetBank(0x01);
	// if(ReadByte(READCMD, 0x19) > 0) 
	// P1_1 = 0;

	 //delay_ms(1000);
}

}

/* Khoi tao UART*/

void UARTInit(){

SCON =0x52; // Port noi tiep che do 1, REN =TI =1;	 {Chu y co TI khi khoi tao, code chay khong la do co TI}
TMOD = 0x20; //Timer 1 mode 2
TH1 = TL1 = -3; // Toc do baud la 9600, NHO SET TRONG PROTEUS 11.059
TR1 =1;

}

/* Khoi tao SPI*/

void SPIInit(){

 	 spi_CS = 1;	  //set muc cao, chuan bi set muc thap de truyen data
 	 spi_SCK = 0; 	  // set muc thap chuan bi phat xung clock
	 spi_MISO = 1;   //set muc cao o chip master, san sang doc data tu slave

}

/*Delay ms*/

void delay_ms(int tg){

		int x, i;
		for(i = 0; i < tg; i++){
			for(x = 0; x < 123; x++);
		}
}

/* Doc 1 Byte*/

unsigned char SPIRead(){
 	 
	 unsigned char i, b;
	 for (i = 0; i < 8; i++){	//doc ve 1 byte
	 b<<=1;			   // dich byte b sang trai 1 bit 
	 spi_SCK = 1;	   // nhan bit dau tien
	 if(spi_MISO) 	   //kiem tra muc tin hieu cua chan MISO 
	 b|=0x01;	   // dua tin hieu bit cua chan MISO vao byte b
	 spi_SCK = 0;	   // ket thuc qua trinh nhan 1 bit
	 }   
  	return b;	 // khi doc du 1 byte tra ve gias tri doc duoc
  
}

/* Ghi 1 Byte*/

void SPIWrite(unsigned char b){

     unsigned char i;
	 for(i = 0; i < 8; i++){  // Ghi 1 byte vao EEFROM
	 if( b & 0x80)			  // So sanh de lay gia tri bit dau tien (gui bit cao truoc [bit 7]--> bit thap sau [bit 1])
	 spi_MOSI = 1;			  // Neu bit =1 thi chan MOSI = 1
	 else 
	 spi_MOSI = 0;			 // Neu bit =0 thi chan MOSI = 0
	 spi_SCK = 1;			 // Bat dau ghi 1 bit
	 spi_SCK = 0;			  // Ket thuc qua trinh ghi bit dau tien
	 b<<=1;					 // Dich trai 1 bit de lay bit tiep theo
	 }
}

/*Ham ghi du lieu vao thanh ghi dia chi*/

void WriteByte(unsigned char op, unsigned char add, unsigned char dl){	  // Ham Write Control Regis ter

	 spi_CS = 0;		  			// Cho phep truyen data
	 SPIWrite(op|(add&0x1F));		  // Ghi 1 byte bao gom opcode va address
	 SPIWrite(dl);				  // Ghi byte data
	 spi_CS = 1;				  // Ngat ghi data

}

/*Ham doc du lieu thanh ghi*/

unsigned char ReadByte(unsigned char op, unsigned char add){

	  unsigned char b;				
	  spi_CS = 0;					 // Cho phep truyen data
	  SPIWrite(op|(add&0x1F));		 // Ghi 1 byte bao gom opcode va address
	  b = SPIRead();					 // Doc gia tri cua thanh ghi
	  spi_CS = 1;					 // Ngat doc data
	  return b;						 // Tra ve gia tri cho ham ReadByte

}
 
unsigned char ReadByteMII(unsigned char op, unsigned char add){

	  unsigned char b;				
	  spi_CS = 0;					 // Cho phep truyen data
	  SPIWrite(op|(add&0x1F));		 // Ghi 1 byte bao gom opcode va address
	  SPIRead();					 // Byte rac 
	  b = SPIRead();			     // Doc gia tri cua thanh ghi
	  spi_CS = 1;					 // Ngat doc data
	  return b;						 // Tra ve gia tri cho ham ReadByte

}


/* RESET chip ENC28J60*/

void SysReset(){

	 spi_CS = 0;		  			// Cho phep truyen data
	 SPIWrite(0xFF);		  // Ghi 1 byte soft reset 
	 SPIWrite(0x00);		 // Ghi byte data
	 spi_CS = 1;				  // Ngat ghi data
}

/*Chon Bank */

void SetBank(unsigned char bank){		 //bank co gia tri 0 = 0x00, 1 = 0x01, 2 = 0x02, 3 = 0x03

	spi_CS = 0;				 // Cho phep truyen data
	SPIWrite(BFClear|0x1F);	 // Ghi lenh xoa bit theo truong cua thanh ghi ECON1 = 0x1F
	SPIWrite(0x03);			 // Xoa bit 0 va 1 cua truong thanh ghi ECON1
	spi_CS = 1;				 // Ngat ghi data

	delay_ms(1);

	spi_CS = 0;				 // Cho phep truyen data
	SPIWrite(BFSet|0x1F);	 // Ghi lenh set bit theo truong cua thanh ghi ECON1 = 0x1F
	SPIWrite(bank);			 // Set bit 0 va 1 cua truong thanh ghi ECON1  de chon bank
	spi_CS = 1;				  // Ngat ghi data

}

/*Khoi tao ENC28J60*/

void ENCInit(){

	printf("Dang khoi tao ENC28J60:\n");
	SysReset();						// Reset enc 
	delay_ms(10);
//	WriteByte(BFClear, ECON1, 0xFF);
	SetBank(0x00);				// Chon Bank 0
	while(!ReadByte(READCMD, 0x1D)&0x01);	 // kiem tra ENC da san sang chua?, bit CLKRDY cua thanh ghi ESTAT phai = 1
	if( ReadByte(READCMD, 0x00) == 0xFA)	printf("Khoi tao thanh cong!!!\n");	   // Kiiem tra thanh ghi ERDPTL = 0x00 da gan gia tri mac dinh la 0XFA chua?
	else printf("Khoi tao that bai....\n");

}

/*Phan vung dia chi truyen va nhan*/

void Buff_TX_RX(){
	// Set NextPacket = RX Start
	NextPacketL = 0x00;
	NextPacketH = 0x00;

	SetBank(0x00);
	//Dia chi TX bat dau la 0x0C00
	WriteByte(WRITECMD, 0x04, 0x00);  //Vi tri TX bat dau byte thap	   						ETXSTL
	WriteByte(WRITECMD, 0x05, 0x0C);  //Vi tri TX bat dau byte cao	  						ETXSTH
	//Dia chi TX ket thuc la 0x11FF
	WriteByte(WRITECMD, 0x06, 0xFF);  //Vi tri TX ket thuc byte thap   						ETXNDL
	WriteByte(WRITECMD, 0x07, 0x1F);  //Vi tri TX bat ket thuc byte cao						ETXNDH
	//Dia chi RX bat dau la 0x0000
	WriteByte(WRITECMD, 0x08, 0x00);  //Vi tri RX bat dau byte thap							ERXSTL
	WriteByte(WRITECMD, 0x09, 0x00);  //Vi tri RX bat dau byte cao							ERXSTH
	//Dia chi RX ket thuc la 0x0BFF
	WriteByte(WRITECMD, 0x0A, 0xFF);  //Vi tri RX ket thuc byte thap						ERXNDL
	WriteByte(WRITECMD, 0x0B, 0x0B);  //Vi tri RX bat ket thuc byte cao						ERXNDH
	//Dua con tro RX ve vi tri bat dau 	READ										
	WriteByte(WRITECMD, 0x0C, 0x00);  // Dua con tro RX ve vi tri ban dau byte thap			ERXRDPTL
	WriteByte(WRITECMD, 0x0D, 0x00);  // Dua con tro RX ve vi tri ban dau byte cao			ERXRDPTH
	//Dua con tro RX ve vi tri bat dau WRITE
	WriteByte(WRITECMD, 0x0E, 0x00);  // Dua con tro TX ve vi tri ban dau byte thap			ERXWRPTL
	WriteByte(WRITECMD, 0x0F, 0x00);  // Dua con tro TX ve vi tri ban dau byte cao			ERXWRPTH


}

/*cau hinh dia chi MAC*/

void ConfigMAC(){
	// Set cac thanh ghi bank 2 theo datasheet
	SetBank(0x02);						    //Chon Bank 2					       
	WriteByte(WRITECMD, MACON1, 0x0D); 	   //Set bit 0, 2, 3 thanh ghi MACON1
	WriteByte(WRITECMD, MACON3, 0x32); 	   //Set bit 1, 4, 5 thanh ghi MACON3
	WriteByte(WRITECMD, MABBIPG, 0x12);
	WriteByte(WRITECMD, MAIPGL, 0x12);
	WriteByte(WRITECMD, MAIPGH, 0x0C);
	WriteByte(WRITECMD, MAMXFLL, 0xDC);	  // Packet 1500(0x05DC) byte
	WriteByte(WRITECMD, MAMXFLH, 0x05);

	//Set dia chi MAC o Bank 3
	SetBank(0x03);			        //Chon Bank 3
	WriteByte(WRITECMD, MAADR6, 0x10);		//Dia chi MAC 10:0A:20:07:19:98
	WriteByte(WRITECMD, MAADR5, 0x0A);
	WriteByte(WRITECMD, MAADR4, 0x20);
	WriteByte(WRITECMD, MAADR3, 0x07);
	WriteByte(WRITECMD, MAADR2, 0x19);
	WriteByte(WRITECMD, MAADR1, 0x89);

}

/*Ham docj dia chi MAC*/

void ReadMAC(){

    SetBank(0x03);
    myaddr[0] = ReadByteMII(READCMD, MAADR6);
  	myaddr[1] = ReadByteMII(READCMD, MAADR5);
    myaddr[2] = ReadByteMII(READCMD, MAADR4);
	myaddr[3] = ReadByteMII(READCMD, MAADR3);
	myaddr[4] = ReadByteMII(READCMD, MAADR2);
	myaddr[5] = ReadByteMII(READCMD, MAADR1);

}

/*Ham ghi cau hinh PHY*/

void WritePHY(unsigned char add, unsigned char dlL, unsigned char dlH){

	 WriteByte(WRITECMD, MIREGADR, add);
	 WriteByte(WRITECMD, MIWRL, dlL);
	 WriteByte(WRITECMD, MIWRH, dlH);
	 ENC_MISTAT();
}

/*Cau hinh cac thanh ghi PHY PHYCON2, PHYCON*/

void ConfigPHY(){

	SetBank(0x02);
	WritePHY(PHCON1, 0x00, 0x00);	 //Set bit PDPXMP
	WritePHY(PHCON2, 0x00, 0x01);	 //Set bit HDLDIS
    WritePHY(PHLCON, 0x26, 0x34);	 //Cau hinh LED A va B; 0x04 = LED A hien thi trang thai Link, 0x76 = LED B hien thi trang thai TX va RX voi thoi gian 70ms 
	SetBank(0x03);
	WriteByte(WRITECMD, 0x15, 0x02);	//ECOCON
	delay_ms(1);
	WriteByte(BFSet, EIE, 0xC0);	   
	WriteByte(BFSet, ECON1, 0x04);	   //ECON1	  Cho phen nhan

}


void ENC_MISTAT(){
   SetBank(0x03);
   while(ReadByteMII(READCMD, 0x0A)&0x01);
   SetBank(0x02);

}

void Write_Buff(int len, unsigned char* dl){	//Gui goi ARP

	 spi_CS = 0;
	 SPIWrite(WRITEBUFF);
	 while(len--)
	 SPIWrite(*dl++);
	 spi_CS = 1;
}

void Send_Packet(int len, unsigned char* dl ){

	SetBank(0x00);
	while(ReadByte(READCMD, ECON1)&0x08);

	WriteByte(WRITECMD, 0x02, 0x00);  // 						EWRPTL
	WriteByte(WRITECMD, 0x03, 0x0C);  //   					    EWRPTH

	WriteByte(WRITECMD, 0x06, 0x3E);  //Vi tri TX ket thuc byte thap   						ETXNDL
	WriteByte(WRITECMD, 0x07, 0x0C);  //Vi tri TX bat ket thuc byte cao						ETXNDH
	//Gui buyte 0x00 vao bo dem TX
	SPIWrite(WRITEBUFF);
	SPIWrite(0x00);
	//Gui goi tin vao bo dem TX
	Write_Buff(len, dl);
	//set bit TXRTS de gui goi tin
    WriteByte(BFSet, ECON1, 0x08);
}

unsigned char Read_BuffByte(){

	 unsigned char b;				
	  spi_CS = 0;					 // Cho phep truyen data
	  SPIWrite(READBUFF);		 // Ghi 1 byte opcode
	  b = SPIRead();					 // Doc gia tri cua thanh ghi
	  spi_CS = 1;					 // Ngat doc data
	  return b;						 // Tra ve gia tri cho ham ReadByte

}

void Read_Buff(unsigned int len, unsigned char* dl){

	  spi_CS = 0;
	  SPIWrite(READBUFF);
	  while(len){
	   len--;
	   *dl = SPIRead();
	   dl++;
	  }

	 spi_CS = 1;
}

unsigned int Read_Packet(int bufflen, unsigned char* dl){

  unsigned int len = 0;
  unsigned int status = 0;
  unsigned char lenL, lenH, statusL, statusH;
  SetBank(0x01);
  if(ReadByte(READCMD, 0x19)>0){
  	 printf("Da nhan 1 goi tin \n");
	 // Set ERDPT ve vi tri NextPaxket
  	SetBank(0x00);
	WriteByte(WRITECMD, 0x00, NextPacketL);  // 						ERDPTL
	WriteByte(WRITECMD, 0x01, NextPacketH);  //   					    ERDPTH
	//Doc gia tri con tro cua goi tin tiep theo
	NextPacketL = Read_BuffByte();
	NextPacketH = Read_BuffByte();
	NextPacket = ghepint(NextPacketH, NextPacketL);
	// Doc do dai goi tin
	lenL = 	Read_BuffByte();
	lenH = 	Read_BuffByte();
	//Tru di 4 byte CRC
	len = ghepint(lenH, lenL) - 4;
	if(len > bufflen) len = bufflen;
	
	//Doc trang thai cua bo nhan
	statusL = 	Read_BuffByte();
	statusH = 	Read_BuffByte();
  	status = ghepint(statusH, statusL);
	//Kieem tra bit  Received OK, neu Error	 thi khong doc goi tin
	if((status&0x80)==0) len = 0;
	else {
	Read_Buff(len, dl);		 // Doc du lieu goi tin
	}
	//Chuyen con tro nhan du lieu toi goi tin tiep theo
	if(NextPacket - 1 > 0xBFF) {
	
	WriteByte(WRITECMD, 0x0C, 0xFF);  // Dua con tro RX ve vi tri ban dau byte thap			ERXRDPTL
	WriteByte(WRITECMD, 0x0D, 0x0B);  // Dua con tro RX ve vi tri ban dau byte cao			ERXRDPTH
	
	}
	else{
	
	WriteByte(WRITECMD, 0x0C, NextPacketL);  // Dua con tro RX ve vi tri ban dau byte thap			ERXRDPTL
	WriteByte(WRITECMD, 0x0D, NextPacketH);  // Dua con tro RX ve vi tri ban dau byte cao			ERXRDPTH
	
	}

	// Set bit PKTDEC cua thanh ghi ECON2
	WriteByte(BFSet, ECON2, 0x40);

  }
 return len;
}

//Kiem tra co phai goi tin ARP?
void CheckARP(){
  
   unsigned int len;
   while(1){

   	len = Read_Packet(50, arp);
   	if(len == 0) return;
	else ARPrequest(len, arp);
   
   }
}

void ARPrequest(unsigned int bufflen, unsigned char* dl){

	 

	 if(bufflen > 41){
	 
	 if(dl[12] == 0x08 && dl[13] == 0x06){
	 	 printf("Da nhan goi tin co do dai = %i \n", bufflen);
	 	 printf("Day la goi tin ARP \n");
	 	}
	 
	 }

	 
}


int ghepint(unsigned char a,unsigned char b)	{
	   int c;
	 c= a&0xFF;
	 c = (c<<8)|b;
	 return c;
}

void tachint(unsigned int c){
	 unsigned char aH, aL;
	 aL =  c&0xFF;
	 aH = (c>>8)&0xFF;
	 printf("%X \n", aH, aL);

}

