#include "stm8s_conf.h"
#include "stm8s.h"
#include "drv_ili9341.h"
#include "stm8s_delay.h"

void main( void )
{

  TFT_Init();

  
  while(1){
  TFT_Clear(BLUE);
  delay_ms(2000);
    TFT_Clear(WHITE);
  delay_ms(2000);
    TFT_Clear(BLUE);
  delay_ms(2000);
    TFT_Clear(RED);
  delay_ms(2000);
    TFT_Clear(YELLOW);
  delay_ms(2000);
  }
  
}
