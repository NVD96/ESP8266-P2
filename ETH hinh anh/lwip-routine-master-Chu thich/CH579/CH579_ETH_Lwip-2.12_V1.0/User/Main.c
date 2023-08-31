#include "main.h"

//保存最新的网络通信方式,初始化为TCP通信
u8  g_cur_eth_comm_chan = PROT_DATA_FROM_TCP;


//全局变量 保存PHY LINK 状态  1：PHY 链路连接状态  0： PHY 链路断开状态
u8 g_eth_phy_link_state = 0;
/*
  转发网络接收缓冲区队列到串口
*/

void tcp_Postback_test(void)
{
	 e_buf_t *p_cur_ebuf = NULL;
	 
	 p_cur_ebuf = get_valid_e_from_que_head(&g_eth_buf_que);

	 if(p_cur_ebuf!=NULL)
	 {
			MyTcpSendData(p_cur_ebuf->buf, p_cur_ebuf->buf_len);
			p_cur_ebuf->buf_len = 0; //发送完毕后，该缓冲区清0
			e_buf_num_sub_one(&g_eth_buf_que);
	 }
}
int main()
{
	PWR_UnitModCfg(ENABLE, (UNIT_SYS_PLL|UNIT_ETH_PHY)); 
  DelayMs(3); 

	//初始化系统使用外部32MHZ时钟
  SetSysClock(CLK_SOURCE_HSE_32MHz); 
	InitTimer0();
	DebugInit();
	printf("Test\n");
	SetDefaultEthUartCfg();
	InitAppCommon();


	lwip_comm_init(); //网络接口初始化
		//初始化TCP SERVER 
	TCP_server_init();
	NETLed_Init();
	while(1)
	{
		tcp_Postback_test();//TCP回传测试
		
		lwip_pkt_handle();

		lwip_periodic_handle();//轮询的东西
	}
}



