#include "main.h"

//�������µ�����ͨ�ŷ�ʽ,��ʼ��ΪTCPͨ��
u8  g_cur_eth_comm_chan = PROT_DATA_FROM_TCP;


//ȫ�ֱ��� ����PHY LINK ״̬  1��PHY ��·����״̬  0�� PHY ��·�Ͽ�״̬
u8 g_eth_phy_link_state = 0;
/*
  ת��������ջ��������е�����
*/

void tcp_Postback_test(void)
{
	 e_buf_t *p_cur_ebuf = NULL;
	 
	 p_cur_ebuf = get_valid_e_from_que_head(&g_eth_buf_que);

	 if(p_cur_ebuf!=NULL)
	 {
			MyTcpSendData(p_cur_ebuf->buf, p_cur_ebuf->buf_len);
			p_cur_ebuf->buf_len = 0; //������Ϻ󣬸û�������0
			e_buf_num_sub_one(&g_eth_buf_que);
	 }
}
int main()
{
	PWR_UnitModCfg(ENABLE, (UNIT_SYS_PLL|UNIT_ETH_PHY)); 
  DelayMs(3); 

	//��ʼ��ϵͳʹ���ⲿ32MHZʱ��
  SetSysClock(CLK_SOURCE_HSE_32MHz); 
	InitTimer0();
	DebugInit();
	printf("Test\n");
	SetDefaultEthUartCfg();
	InitAppCommon();


	lwip_comm_init(); //����ӿڳ�ʼ��
		//��ʼ��TCP SERVER 
	TCP_server_init();
	NETLed_Init();
	while(1)
	{
		tcp_Postback_test();//TCP�ش�����
		
		lwip_pkt_handle();

		lwip_periodic_handle();//��ѯ�Ķ���
	}
}



