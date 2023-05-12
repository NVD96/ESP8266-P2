#ifndef __ili9341_H
#define __ili9341_H	
#include "stm8s.h"
#include "stm8s_conf.h"
#include "stm8s_delay.h"


#define LCD_W 240
#define LCD_H 320

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 		 0XFFE0
#define GBLUE		 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 		 0XBC40 
#define BRRED 		 0XFC07 
#define GRAY  		 0X8430 
#define DARKBLUE      	 0X01CF	
#define LIGHTBLUE      	 0X7D7C	 
#define GRAYBLUE       	 0X5458 
#define LIGHTGREEN     	 0X841F 
#define LGRAY 	         0XC618 
#define LGRAYBLUE        0XA651 
#define LBBLUE           0X2B12 

extern uint16_t  POINT_COLOR;   
extern uint16_t  BACK_COLOR; 

typedef struct  
{										    
	uint16_t width;			
	uint16_t height;			
	uint16_t id;				
	uint8_t dir;				
	uint16_t wramcmd;		
	uint16_t setxcmd;		
	uint16_t setycmd;		
}_lcd_dev;

extern _lcd_dev lcddev;

void TFT_Init(void); 
void TFT_Clear(uint16_t Color);
void TFT_WR_DATA(uint8_t Data); 
void TFT_WR_REG(uint8_t Reg);
void TFT_SetCursor(uint16_t Xpos, uint16_t Ypos);
void TFT_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
void TFT_DrawPoint(uint16_t x,uint16_t y);
void TFT_WriteRAM_Prepare(void);
void TFT_direction(uint8_t direction );
void TFT_WR_DATA_16Bit(uint16_t Data);
void TFT_Init_GPIO();
void TFT_Reset();


#endif  /* __ili9341_H */