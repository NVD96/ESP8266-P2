#include "parameter_setting.h"


u8  gMyPeerMac[6];
u32  gSysTicket = 0;
__attribute__((aligned (4))) eth_uart_cfg_t  gEthUartCfgInfo;    //���紮��ת�Ӱ�������Ϣ


extern u32  g_lwip_localtime;

static  u32  TimingDelay = 0;

/* ��ӡ���� */
void DebugInit(void)		
{
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
}



/*  ���ڵ�IO��������*/

//��ʼ������LED GPIO
void NETLed_Init(void)
{

	//����PB4 PB7Ϊ���ģʽ ��ʼ��Ϊ�͵�ƽ
	GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeOut_PP_20mA);
	GPIOB_ModeCfg(GPIO_Pin_3, GPIO_ModeOut_PP_20mA);
	GPIOB_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_20mA);

	//��ʼ������״̬Ϊ�Ͽ�״̬
	SET_NET_LED_OFF();
	SET_NET_LEDX_OFF();
}



/*************************************************************************
 ��ʼ��Timer0    10ms����1���ж�
*************************************************************************/
void InitTimer0(void)
{
	TMR0_TimerInit((10000*32));  //10ms ����1���ж�
  TMR0_ITCfg(ENABLE, TMR0_3_IT_CYC_END);  // �����ж�
  NVIC_EnableIRQ(TMR0_IRQn);
}



//����Ĭ��������Ϣ
void  SetDefaultEthUartCfg(void)
{
	MY_ETH_IP = LED_CARD_DEFAULT_IP;		
	MY_ETH_GW  = LED_CARD_DEFAULT_GW;		
	MY_ETH_MASK = LED_CARD_DEFAULT_MASK;			
	MY_ETH_PORT = LED_CARD_DEFAULT_PORT;  			
}

/**********************************************
 ��ʼ���������ִ���
***********************************************/
void  InitAppCommon(void)
{	
	//��ʼ��phy ��·״̬Ϊ�Ͽ�����״̬
	g_eth_phy_link_state = 0; 

	init_my_buf_que(&g_eth_buf_que);  //����ӿڽ��ջ�����
}


void TMR0_IRQHandler( void )        // TMR0 ��ʱ�ж�
{
   TMR0_ClearITFlag( TMR0_3_IT_CYC_END );   // ����жϱ�־

		gSysTicket++;
		g_lwip_localtime+= 10;
		

		if (TimingDelay > 0)
		{ 
		 	TimingDelay--;
		}
}
