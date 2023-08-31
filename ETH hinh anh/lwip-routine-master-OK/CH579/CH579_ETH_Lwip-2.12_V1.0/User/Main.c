#include "main.h"

//保存最新的网络通信方式,初始化为TCP通信
u8  g_cur_eth_comm_chan = PROT_DATA_FROM_TCP;

UINT8 u8Buff[66];

UINT32 checksum_IP; 
UINT32 checksum_ICMP;

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
	//printf("Test\n");
	//SetDefaultEthUartCfg();
	//InitAppCommon();
	ETHParaInitX();
	ETHInitX();
	
	//Goi ARP Broadcad
	//MAC Dist
	/*u8Buff[0] = 0xff;
	u8Buff[1] = 0xff;
	u8Buff[2] = 0xff;
	u8Buff[3] = 0xff;
	u8Buff[4] = 0xff;
	u8Buff[5] = 0xff;
	//MAC SOURE
	u8Buff[6] = 0x00;
	u8Buff[7] = 0x22;
	u8Buff[8] = 0x47;
	u8Buff[9] = 0x01;	
	u8Buff[10] = 0x01;
	u8Buff[11] = 0x01;
	//Ethenet Type ARP: 0x0806
	u8Buff[12] = 0x08;
	u8Buff[13] = 0x06;
	//Ethenet 10Mb
	u8Buff[14] = 0x00;
	u8Buff[15] = 0x01;
	//Protocol Type 0x0800
	u8Buff[16] = 0x08;
	u8Buff[17] = 0x00;
	//Size
	u8Buff[18] = 0x06;
	u8Buff[19] = 0x04;
	//Opcode Request 0x0001
	u8Buff[20] = 0x00;
	u8Buff[21] = 0x01;
	//MAC Soure
	u8Buff[22] = u8Buff[6];
	u8Buff[23] = u8Buff[7];
	u8Buff[24] = u8Buff[8];
	u8Buff[25] = u8Buff[9];
	u8Buff[26] = u8Buff[10];
	u8Buff[27] = u8Buff[11];
	//Soure IP
	u8Buff[28] = 0xc0;//Soure IP
	u8Buff[29] = 0xa8;
	u8Buff[30] = 0x01;
	u8Buff[31] = 0xc8;
	//MAC Dist
	u8Buff[32] = 0x00;
	u8Buff[33] = 0x00;
	u8Buff[34] = 0x00;
	u8Buff[35] = 0x00;
	u8Buff[36] = 0x00;
	u8Buff[37] = 0x00;
	//IP Dist
	u8Buff[38] = 0xc0;//Dist IP
	u8Buff[39] = 0xa8;
	u8Buff[40] = 0x01;
	u8Buff[41] = 0x64;
			for(int i = 42 ; i < 60 ; i++)  //Padding
		{
		 u8Buff[i] = 0x00;
		}*/


	
	/*Goi tin ICMP co 74 byte*/
	//MAC DIST
	/*u8Buff[0] = 0x00;
	u8Buff[1] = 0x0C;
	u8Buff[2] = 0x29;
	u8Buff[3] = 0xC9;
	u8Buff[4] = 0xD7;
	u8Buff[5] = 0x59;
	//MAC SOURE
	u8Buff[6] = 0x00;
	u8Buff[7] = 0x22;
	u8Buff[8] = 0x47;
	u8Buff[9] = 0xFF;	
	u8Buff[10] = 0xFF;
	u8Buff[11] = 0xFF;
	//Ethenet Type IP: 0x0800
	u8Buff[12] = 0x08;
	u8Buff[13] = 0x00;
	//20 Byte Header IP
	u8Buff[14] = 0x45;// Header Length
	u8Buff[15] = 0x00;// Services
	u8Buff[16] = 0x00;//Totol Length
	u8Buff[17] = 0x3C;// 60 byte
	u8Buff[18] = 0x07;//Identification
	u8Buff[19] = 0x15;
	u8Buff[20] = 0x00;//Flag
	u8Buff[21] = 0x00;
	u8Buff[22] = 0x80;//TimeToLive
	u8Buff[23] = 0x01;//Protocol
	//u8Buff[24] = 0xB1;//Checksum
	//u8Buff[25] = 0xF2;
	u8Buff[26] = 0xc0;//Soure IP
	u8Buff[27] = 0xa8;
	u8Buff[28] = 0x00;
	u8Buff[29] = 0x05;
	u8Buff[30] = 0xc0;//Dist IP
	u8Buff[31] = 0xa8;
	u8Buff[32] = 0x00;
	u8Buff[33] = 0x64;
	//ICMP
	u8Buff[34] = 0x08;//ICMP Type request
	u8Buff[35] = 0x00;//ICMP Code
	//u8Buff[36] = 0xE3;//ICMP Checksum
	//u8Buff[37] = 0xCA;
	u8Buff[38] = 0x00;//ICMP Identification
	u8Buff[39] = 0x01;
	u8Buff[40] = 0x00;//ICMP Squenquer
	u8Buff[41] = 0x20;*/
	
	//lwip_comm_init(); //网络接口初始化
		//初始化TCP SERVER 
	//TCP_server_init();
	
	//Goi TCP
	//MAC Dist
	/*u8Buff[0] = 0x00;
	u8Buff[1] = 0x0C;
	u8Buff[2] = 0x29;
	u8Buff[3] = 0xC9;
	u8Buff[4] = 0xD7;
	u8Buff[5] = 0x59;
	//MAC SOURE
	u8Buff[6] = 0x00;
	u8Buff[7] = 0x22;
	u8Buff[8] = 0x47;
	u8Buff[9] = 0x01;	
	u8Buff[10] = 0x01;
	u8Buff[11] = 0x01;
	//Ethenet Type IP: 0x0800
	u8Buff[12] = 0x08;
	u8Buff[13] = 0x00;
	//20 Byte Header IP
	u8Buff[14] = 0x45;// Header Length
	u8Buff[15] = 0x00;// Services
	u8Buff[16] = 0x00;//Totol Length
	u8Buff[17] = 0x34;// 60 byte
	u8Buff[18] = 0x03;//Identification
	u8Buff[19] = 0x09;
	u8Buff[20] = 0x40;//Flag
	u8Buff[21] = 0x00;
	u8Buff[22] = 0x80;//TimeToLive
	u8Buff[23] = 0x06;//Protocol
	u8Buff[24] = 0x00;//Checksum
	u8Buff[25] = 0x00;
	u8Buff[26] = 0xc0;//Soure IP
	u8Buff[27] = 0xa8;
	u8Buff[28] = 0x01;
	u8Buff[29] = 0xc8;
	u8Buff[30] = 0xc0;//Dist IP
	u8Buff[31] = 0xa8;
	u8Buff[32] = 0x00;
	u8Buff[33] = 0x64;
	//Port Soure
	u8Buff[34] = 0x20;
	u8Buff[35] = 0x00;
	//Port Dist
	u8Buff[36] = 0x03;
	u8Buff[37] = 0xe8;
	//Sequence
	u8Buff[38] = 0x01;
	u8Buff[39] = 0x01;
	u8Buff[40] = 0x01;
	u8Buff[41] = 0x20;
	//Ack Sequence
	u8Buff[42] = 0x00;
	u8Buff[43] = 0x00;
	u8Buff[44] = 0x00;
	u8Buff[45] = 0x00;	
	//SYN
	u8Buff[46] = 0x80;
	u8Buff[47] = 0x02;	
	//Windown
	u8Buff[48] = 0x20;
	u8Buff[49] = 0x00;
//Check sum
	u8Buff[50] = 0x00;
	u8Buff[51] = 0x00;
	//URG
	u8Buff[52] = 0x00;
	u8Buff[53] = 0x00;	
	//
	u8Buff[54] = 0x02;
	u8Buff[55] = 0x04;
	u8Buff[56] = 0x05;
	u8Buff[57] = 0xb4;
	u8Buff[58] = 0x01;
	u8Buff[59] = 0x03;
	u8Buff[60] = 0x03;
	u8Buff[61] = 0x08;
	u8Buff[62] = 0x01;
	u8Buff[63] = 0x01;	
	u8Buff[64] = 0x04;
	u8Buff[65] = 0x02;

		for(int k = 26; k < 66; k = k+2){
				checksum_ICMP = checksum_ICMP + (((UINT16)u8Buff[k]<<8)|u8Buff[k+1]);
				if(checksum_ICMP > 0x10000){
				checksum_ICMP = checksum_ICMP - 0x10000 + 1;
					}
				}
				checksum_ICMP = 0xFFFF - (UINT16)checksum_ICMP;
				u8Buff[50] = ((UINT16)checksum_ICMP>>8)&0xFF;
				u8Buff[51] = (UINT16)checksum_ICMP&0xFF;*/
	
	NETLed_Init();
//	uint8_t dem = 6;
	while(1)
	{
		/*checksum_IP = 0; 
		checksum_ICMP = 0; 
		u8Buff[24] = 0x00;//Checksum
		u8Buff[25] = 0x00;
		u8Buff[36] = 0x00;//ICMP Checksum
		u8Buff[37] = 0x00;
		//tcp_Postback_test();//TCP回传测试
		
	//	lwip_pkt_handle();
		
		//lwip_periodic_handle();//轮询的东西
		mDelaymS(5);
		//Padding: DDem cho du 74 byte(DATA)
		for(int i = 42 ; i < 74 ; i++)  //32 Byte data
		{
		 u8Buff[i] = 0x41;
		}
		//Checksum IP
		for(int j = 14; j < 34; j = j+2){
		checksum_IP = checksum_IP + (((UINT16)u8Buff[j]<<8)|u8Buff[j+1]);
		if(checksum_IP > 0x10000){
		checksum_IP = checksum_IP - 0x10000 + 1;
			}		
		}
		checksum_IP = 0xFFFF - (UINT16)checksum_IP;
		u8Buff[24] = ((UINT16)checksum_IP>>8)&0xFF;
		u8Buff[25] = (UINT16)checksum_IP&0xFF;
		
		// ICMP Checksum
		for(int k = 34; k < 74; k = k+2){
		checksum_ICMP = checksum_ICMP + (((UINT16)u8Buff[k]<<8)|u8Buff[k+1]);
		if(checksum_ICMP > 0x10000){
		checksum_ICMP = checksum_ICMP - 0x10000 + 1;
			}
		}
		checksum_ICMP = 0xFFFF - (UINT16)checksum_ICMP;
		u8Buff[36] = ((UINT16)checksum_ICMP>>8)&0xFF;
		u8Buff[37] = (UINT16)checksum_ICMP&0xFF;
		
		
		//ETHSendX(u8Buff, 74);*/
		
		
		//Padding: DDem cho du 74 byte(DATA)
			/*if(dem > 0){
			ETHSendX(u8Buff, 66);
			mDelaymS(100);
				dem = dem -1;
			}*/
	}
}



