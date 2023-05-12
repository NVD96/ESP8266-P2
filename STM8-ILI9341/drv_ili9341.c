#include "drv_ili9341.h"
#include "stm8s.h"

#define PortC (GPIOC)
#define PortD (GPIOD)
#define CS (GPIO_PIN_3)// C3
#define DC (GPIO_PIN_4)//C4
#define RESET (GPIO_PIN_1)//D1
#define LED (GPIO_PIN_2)//D2

uint16_t POINT_COLOR = 0x0000;	
uint16_t BACK_COLOR = 0xFFFF; 

_lcd_dev lcddev;

void TFT_Init_GPIO(void){

        GPIO_Init(PortC, CS, GPIO_MODE_OUT_PP_LOW_FAST);
        GPIO_Init(PortC, DC, GPIO_MODE_OUT_PP_LOW_FAST);
        GPIO_Init(PortC, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
        GPIO_Init(PortC, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);
       
        GPIO_Init(PortD, RESET, GPIO_MODE_OUT_PP_LOW_FAST);
        GPIO_Init(PortD, LED, GPIO_MODE_OUT_PP_LOW_FAST);
        SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_16, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_1LINE_TX, SPI_NSS_SOFT, 0x07);
        SPI_BiDirectionalLineConfig(SPI_DIRECTION_TX);
        SPI_Cmd(ENABLE);
}

void TFT_Reset(void){
  
        GPIO_WriteHigh(PortD, RESET);
        delay_ms(50);
        GPIO_WriteLow(PortD, RESET);
        delay_ms(50);
        GPIO_WriteHigh(PortD, RESET);
        delay_ms(50);

}

void TFT_Init(void){

        TFT_Init_GPIO();
        TFT_Reset();
        TFT_WR_REG(0xCF);  //POWER CONTROL B
	TFT_WR_DATA(0x00); //default
	TFT_WR_DATA(0xC1); //C1 
	TFT_WR_DATA(0x30); 
	TFT_WR_REG(0xED);  //POWER ON SEQUENCE CONTROL
	TFT_WR_DATA(0x64); 
	TFT_WR_DATA(0x03); 
	TFT_WR_DATA(0x12); 
	TFT_WR_DATA(0x81); 
	TFT_WR_REG(0xE8);  //DRIVE TIMING CONTROL A
	TFT_WR_DATA(0x85); 
	TFT_WR_DATA(0x00); 
	TFT_WR_DATA(0x78); 
	TFT_WR_REG(0xCB);  //POWER CONTROL A
	TFT_WR_DATA(0x39); 
	TFT_WR_DATA(0x2C); 
	TFT_WR_DATA(0x00); 
	TFT_WR_DATA(0x34); 
	TFT_WR_DATA(0x02); 
	TFT_WR_REG(0xF7);  //PUMP RARIO CONTROL
	TFT_WR_DATA(0x20); 
	TFT_WR_REG(0xEA);  // DRIVE TIMING CONTROL B
	TFT_WR_DATA(0x00); 
	TFT_WR_DATA(0x00); 
	TFT_WR_REG(0xC0);    //Power control 1
	TFT_WR_DATA(0x23);   //VRH[5:0] 
	TFT_WR_REG(0xC1);    //Power control 2
	TFT_WR_DATA(0x10);   //SAP[2:0];BT[3:0] 01 
	TFT_WR_REG(0xC5);    //VCM control  1
	TFT_WR_DATA(0x3E); 	 //3F
	TFT_WR_DATA(0x28); 	 //3C
	TFT_WR_REG(0xC7);    //VCM control 2
	TFT_WR_DATA(0x86); 
	TFT_WR_REG(0x36);    // Memory Access Control  trang 127
	TFT_WR_DATA(0x08); //xoay màn hình
	TFT_WR_REG(0x3A);   //PIXEL FORMAT SET trang 134
	TFT_WR_DATA(0x55); //ch?n ch? d? 16bit/pixel
	TFT_WR_REG(0xB1);   //FRAME RATE CONTROL trang 155
	TFT_WR_DATA(0x00);   //cài d?t t?n s? quét
	TFT_WR_DATA(0x18); 
	TFT_WR_REG(0xB6);    // Display Function Control  trang 164 datasheet
	TFT_WR_DATA(0x08); 
	TFT_WR_DATA(0x82); 
        TFT_WR_DATA(0x27);
	TFT_WR_REG(0xF2);    // 3Gamma Function Disable 
	TFT_WR_DATA(0x00); 
	TFT_WR_REG(0x26);    //Gamma SET 
	TFT_WR_DATA(0x01); // Set gamma màu theo d? th? trang 107 datasheet
	TFT_WR_REG(0xE0);    //POSITIVE GAMMA CORRECTION 
	TFT_WR_DATA(0x0F); //Giá tr? d? tính toán GAMMA màu trang 221 datasheet
	TFT_WR_DATA(0x31); 
	TFT_WR_DATA(0x2B); 
	TFT_WR_DATA(0x0C); 
	TFT_WR_DATA(0x0E); 
	TFT_WR_DATA(0x08); 
	TFT_WR_DATA(0x4E); 
	TFT_WR_DATA(0xF1); 
	TFT_WR_DATA(0x37); 
	TFT_WR_DATA(0x07); 
	TFT_WR_DATA(0x10); 
	TFT_WR_DATA(0x03); 
	TFT_WR_DATA(0x0E); 
	TFT_WR_DATA(0x09); 
	TFT_WR_DATA(0x00); 		 
	TFT_WR_REG(0xE1);    //NAGATIVE GAMMA CORRECTION
	TFT_WR_DATA(0x00); //Giá tr? d? tính toán GAMMA màu trang 222 datasheet
	TFT_WR_DATA(0x0E); 
	TFT_WR_DATA(0x14); 
	TFT_WR_DATA(0x03); 
	TFT_WR_DATA(0x11); 
	TFT_WR_DATA(0x07); 
	TFT_WR_DATA(0x31); 
	TFT_WR_DATA(0xC1); 
	TFT_WR_DATA(0x48); 
	TFT_WR_DATA(0x08); 
	TFT_WR_DATA(0x0F); 
	TFT_WR_DATA(0x0C); 
	TFT_WR_DATA(0x31); 
	TFT_WR_DATA(0x36); 
	TFT_WR_DATA(0x0F); 
	TFT_WR_REG(0x2B); //PAGE ADDRESS SET
	TFT_WR_DATA(0x00);//Hàng start 0
	TFT_WR_DATA(0x00);
	TFT_WR_DATA(0x01); //Hàng stop 320
	TFT_WR_DATA(0x3F);
	TFT_WR_REG(0x2A); //COLUMN ADDRESS SET
	TFT_WR_DATA(0x00);// C?t start 0
	TFT_WR_DATA(0x00);
	TFT_WR_DATA(0x00); // C?t start 240
	TFT_WR_DATA(0xEF);	 
	TFT_WR_REG(0x11); //Exit Sleep
	delay_ms(120);
        TFT_WR_REG(0x29); //display on
        TFT_direction(0); // chon huong man hinh
        GPIO_WriteHigh(PortD, LED);

}
 
void TFT_Clear(uint16_t Color){
  
        uint16_t i,j;
        TFT_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	
        for(i=0;i<lcddev.width;i++)
                 {
                  for (j=0;j<lcddev.height;j++)
                        {
                         TFT_WR_DATA_16Bit(Color);
                    }

                  }

}

void TFT_WR_DATA(uint8_t Data){

        GPIO_WriteHigh(PortC, DC);
        GPIO_WriteLow(PortC, CS);
        SPI_SendData(Data);
        GPIO_WriteHigh(PortC, CS);

}

void TFT_WR_REG(uint8_t Reg){

        GPIO_WriteLow(PortC, DC);
        GPIO_WriteLow(PortC, CS);
        SPI_SendData(Reg);
        GPIO_WriteHigh(PortC, CS);

}

void TFT_SetCursor(uint16_t Xpos, uint16_t Ypos){ //chon diem

         TFT_SetWindows(Xpos,Ypos,Xpos,Ypos);

}


void TFT_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd){
    
         uint8_t xStarh,xStarl,yStarh,yStarl,xEndh,xEndl,yEndh,yEndl;
         xStarl = xStar;
         xEndl = xEnd;
         yStarl = yStar;
         yEndl = yEnd;
         
         xStarh = xStar>>8;
         xEndh= xEnd>>8;
         yStarh = yStar>>8;
         yEndh = yEnd>>8;
           
  
        TFT_WR_REG(lcddev.setxcmd);	
	TFT_WR_DATA(xStarh);
	TFT_WR_DATA(xStarl);		
	TFT_WR_DATA(xEndh);
	TFT_WR_DATA(xEndl);
	TFT_WR_REG(lcddev.setycmd);	
	TFT_WR_DATA(yStarh);
	TFT_WR_DATA(yStarl);		
	TFT_WR_DATA(yEndh);
	TFT_WR_DATA(yEndl);
        
        TFT_WR_REG(lcddev.wramcmd);// cho phep day du lieu

}

void TFT_DrawPoint(uint16_t x,uint16_t y){

}

void TFT_WriteRAM_Prepare(void){

}

void TFT_direction(uint8_t direction ){

              lcddev.setxcmd=0x2A;
              lcddev.setycmd=0x2B;
              lcddev.wramcmd=0x2C;
              lcddev.width=LCD_W;
              lcddev.height=LCD_H;	
              TFT_WR_REG(0x36);  
              TFT_WR_DATA(0x08); ;//BGR==1,MY==0,MX==0,MV==0
}

void TFT_WR_DATA_16Bit(uint16_t Data){
  
              uint8_t convetH,convetL;
              convetL = Data;
              convetH = Data>>8;
              
              GPIO_WriteHigh(PortC, DC);
              GPIO_WriteLow(PortC, CS);
              SPI_SendData(convetH);
              SPI_SendData(convetL);
              GPIO_WriteHigh(PortC, CS);

}