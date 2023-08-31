
#ifndef 	__PARAMETER_SETTING_H__
#define   __PARAMETER_SETTING_H__

#include "CH57x_common.h"
#include "core_cm0.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mybufque.h"



typedef struct  _eth_uart_cfg
{
	u16   save_flag;     //保存标志 0xAA55 表示有效数据
	u16   eth_port; //端口号 UDP,TCP使用同一个端口号
	u32   eth_ip;
	u32   eth_gw;
	u32   eth_mask;
}eth_uart_cfg_t;


extern  eth_uart_cfg_t   gEthUartCfgInfo;

extern u8  g_eth_phy_link_state;


#define  MY_ETH_IP  			gEthUartCfgInfo.eth_ip
#define  MY_ETH_GW  			gEthUartCfgInfo.eth_gw
#define  MY_ETH_MASK  			gEthUartCfgInfo.eth_mask
#define  MY_ETH_PORT  			gEthUartCfgInfo.eth_port
//定义网络通信通道
#define   	PROT_DATA_FROM_TCP			1
#define   	PROT_DATA_FROM_UDP			2
#define   	PROT_DATA_FROM_MAC			3
#define   	PROT_DATA_FROM_UDP3W	  4   //udp 30000 广播端口
#define   	PROT_DATA_FROM_TCP3W		5   //tcp 30000 广播端口



extern  u8  g_cur_eth_comm_chan;   //保存最新通信方式


//设置网络链路指示灯亮
#define  SET_NET_LED_ON()		 GPIOB_ResetBits(GPIO_Pin_4)
#define  SET_NET_LEDX_ON()		 GPIOB_ResetBits(GPIO_Pin_7)
#define  LED_OFF()		 GPIOB_ResetBits(GPIO_Pin_3)

//设置网络链路指示灭
#define  SET_NET_LED_OFF()		 GPIOB_SetBits(GPIO_Pin_4)
#define  SET_NET_LEDX_OFF()		 GPIOB_SetBits(GPIO_Pin_7)
#define  LED_ON()		 GPIOB_SetBits(GPIO_Pin_3)

//网络LED灯闪烁
#define  SET_NET_LEDX_FLASH()	 GPIOB_InverseBits(GPIO_Pin_7)
#define  LED_FLASH()	 GPIOB_InverseBits(GPIO_Pin_3)

#define  	LED_CARD_DEFAULT_PORT 				8800
#define  	LED_CARD_DEFAULT_IP       		0xC0A801EB   //默认IP地址 192.168.0.235      0xC0A800EB   0xEB00A8C0
#define  	LED_CARD_DEFAULT_GW       		0xC0A80101    //默认IP地址 192.168.0.1      C0A80001        0x0100A8C0
#define  	LED_CARD_DEFAULT_DNS       		0xC0A80101      //C0A80001
#define  	LED_CARD_DEFAULT_MASK       	0xFFFFFF00     //FFFFFF00 
								
#define  	GET_SYS_TICK()			gSysTicket

//该全局变量会有异步操作，应用要保证不会发生冲突
extern 	 u8  			gMyPeerMac[];
extern   u32   			gSysTicket ;


extern void  InitTimer0(void);
extern void  InitAppCommon(void);
extern void  NETLed_Init(void);
extern void DebugInit(void);

extern void  SetDefaultEthUartCfg(void);

#endif 
