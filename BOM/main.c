#include "stm8s.h"
#include "stm8s_conf.h"
#include "stm8s_delay.h"
#include "stm8s_gpio.h"
#include "stm8s_tim2.h"


#define CB GPIOD                //Input muc nuoc
#define CBMAX GPIO_PIN_5
#define CBMIN GPIO_PIN_6

#define RL GPIOC                //Chay Bom
#define RL1 GPIO_PIN_6
#define RL2 GPIO_PIN_7

#define OP GPIOB                // Bao he thong dang hoat dong
#define OP1 GPIO_PIN_5

 unsigned int count = 0;

//void Delay_stop();

void main( void )
{
    TIM2_DeInit();  //Reset Timer 2
    //TIM2_ClearFlag( TIM2_FLAG_UPDATE);                  // Xoa co ngat Tiner 2
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);   //Clock de MCU hoat dong 2MHz
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);  //Clock cho Timer 2 lay tu ngoai vi( = clock MCU)
    
    //enableInterrupts();					// enable global interrupt

    GPIO_Init(CB, CBMAX, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(CB, CBMIN, GPIO_MODE_IN_PU_NO_IT);
    
    GPIO_Init(RL, RL1, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(RL, RL2, GPIO_MODE_OUT_PP_LOW_FAST);
    
    GPIO_Init(OP, OP1, GPIO_MODE_OUT_PP_HIGH_FAST);
    
   // Delay_stop();
    
  while(1){
  
    if(GPIO_ReadInputPin(CB, CBMAX)){  ///1 dieu dien thi khong duoc so sanh ==,, nhieu dieu kien moi so sanh
    
    GPIO_WriteLow(RL, RL1);
    GPIO_WriteLow(RL, RL2);
    TIM2_Cmd(DISABLE);
    count = 0; 
    }
    
    
    if(TIM2_GetCounter() >= 18309){
  
    GPIO_WriteLow(RL, RL1);
    GPIO_WriteLow(RL, RL2);
    TIM2_Cmd(DISABLE);
    TIM2->CNTRH = 0;
    TIM2->CNTRL = 0;
    
    }
    
    
          if(GPIO_ReadInputPin(CB, CBMAX) == 0 && GPIO_ReadInputPin(CB, CBMIN) == 0 && count == 0){
            
           count = 1; 
           TIM2_GenerateEvent(TIM2_EVENTSOURCE_UPDATE);             //Phai co ham nay de no update bo dem Timer 2
           TIM2_TimeBaseInit(TIM2_PRESCALER_32768, 18310);           // Clock dem cua Tiner 2   f =  16MHz/1 = 0,0625us
           TIM2_Cmd(ENABLE); 
           GPIO_WriteHigh(RL, RL1);
           GPIO_WriteHigh(RL, RL2);
          }
    
  }
}

/*
void Delay_stop(){
  
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);  //Clock cho Timer 2 lay tu ngoai vi( = clock MCU)
    TIM2_TimeBaseInit(TIM2_PRESCALER_2048, 9760);           // Clock dem cua Tiner 2   f =  16MHz/1 = 0,0625us
                                                         // Clock dem cua Tiner 2   f =  16MHz/2 = 0,125us 
                                                         // Clock dem cua Tiner 2   f =  16MHz/4 = 0,25us 
    // Clock dem cua Tiner 2   f =  16MHz/8 = 0,5us
    // Clock dem cua Tiner 2   f =  16MHz/16 = 1us 
    // Clock dem cua Tiner 2   f =  16MHz/32 = 2us 
    // Clock dem cua Tiner 2   f =  16MHz/64 = 4us 
    // Clock dem cua Tiner 2   f =  16MHz/128 = 8us 
    // Clock dem cua Tiner 2   f =  16MHz/8192 = 512us 
    
    
    //TIM2_ARRPreloadConfig(ENABLE);                     //Reload Tmer 2
    //TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);              //Cho phep ngat Timer 2
    //TIM2_ClearFlag( TIM2_FLAG_UPDATE);                  // Xoa co ngat Tiner 2
    //TIM2_Cmd(ENABLE);                                   // Cho phep Tiner 2 hoat dong

}*/