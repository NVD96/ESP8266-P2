

/************************************************************
CH579 mac 通信

*************************************************************/

#include "mych579mac.h"

#include "parameter_setting.h"
static  RXBUFST   ETHRxMagPara;   //接收管理参数
static  TXBUFST   ETHTxMagPara;   //发送管理参数


static  __align(4) UINT8     MACRxBuf[RX_QUEUE_NUM][RX_BUF_SIZE];   /* MAC接收缓冲区，4字节对齐 */ 
static  __align(4) UINT8     MACTxBuf[TX_QUEUE_NUM][TX_BUF_SIZE];   /* MAC发送缓冲区，4字节对齐 */ 

UINT8   RX_count;
UINT8   myCh579MAC[MACADDR_LEN] = {0x00,0x22,0x47,0x01,0x01,0x01};
UINT8   ARP_Buff[60];
UINT8   TCP_Buff[66];
UINT8   TCP_Buff1[200];

u8 phy_flag=0;


/*******************************************************************************
 * @fn          WritePHYReg
 *
 * @brief       写PHY寄存器
 *
 * input parameters
 *
 * @param       reg_add. 寄存器地址
 * @param       reg_val. 寄存器的值
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
static void WritePHYRegX(UINT8 reg_add,UINT16 reg_val)
{
	UINT8 reg_op=0;
	
	R16_ETH_MIWR = reg_val;
	reg_op = (reg_add&RB_ETH_MIREGADR_MIRDL);
	R8_ETH_MIREGADR = reg_op;   
}

/*******************************************************************************
 * @fn          ReadPHYReg
 *
 * @brief       读PHY寄存器
 *
 * input parameters
 *
 * @param       reg_add. 寄存器地址
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      寄存器读取的值.
 */
static UINT16 ReadPHYRegX(UINT8 reg_add)
{
	UINT8 reg_op=0;
	UINT16 read_reg_val = 0xffff;
	
	reg_op = reg_add&RB_ETH_MIREGADR_MIRDL;
	R8_ETH_MIREGADR = RB_ETH_MIREGADR_MIIWR|reg_op;
	read_reg_val = R16_ETH_MIRD;
	
	return read_reg_val;
}


//初始化网络参数
void ETHParaInitX(void)
{
	UINT8 i = 0;
	
	memset((char *)&MACRxBuf[0][0],0,sizeof(MACRxBuf));
	memset((char *)&MACTxBuf[0][0],0,sizeof(MACTxBuf));
	memset((char *)&ETHRxMagPara,0,sizeof(ETHRxMagPara));
	memset((char *)&ETHTxMagPara,0,sizeof(ETHTxMagPara));
	
	for(i=0; i<RX_QUEUE_NUM; i++) 
	{
		ETHRxMagPara.RxBufAddr[i] = (UINT32)(&MACRxBuf[i][0]);
	}
	
	for(i=0; i<TX_QUEUE_NUM; i++)
	{
		ETHTxMagPara.TxBufAddr[i] = (UINT32)(&MACTxBuf[i][0]);
	}

	//发送接收使能关闭
	ETHRxMagPara.RecvEn = 0;   
	ETHTxMagPara.SendEn = 0;


	//获取唯一MAC地址
//	GetMACAddress(myCh579MAC);
//	myCh579MAC[5]=myCh579MAC[0]+myCh579MAC[1]+myCh579MAC[2]+myCh579MAC[3]+myCh579MAC[4]+myCh579MAC[5];
//	myCh579MAC[0]=0x00; //确保MAC地址第1个字节为0
	

//	printf("ETHParaInitX mac: %x:%x:%x:%x:%x:%x\r\n",myCh579MAC[5], myCh579MAC[4],myCh579MAC[3],myCh579MAC[2],myCh579MAC[1],myCh579MAC[0]);
	

}





//网络接口初始化
void ETHInitX(void)
{

	R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;
	R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
	R8_SLP_CLK_OFF1 &= ~RB_SLP_CLK_ETH;	      //开启ETH控制器时钟
	R8_SLP_POWER_CTRL &= ~RB_SLP_ETH_PWR_DN;  //开启ETH控制器电源 
	R8_SAFE_ACCESS_SIG = 0;	

	R8_ETH_EIE = 0;
	R8_ETH_EIE |= RB_ETH_EIE_INTIE |
                  RB_ETH_EIE_RXIE  |
	              RB_ETH_EIE_LINKIE|
	              RB_ETH_EIE_TXIE  |
	              RB_ETH_EIE_TXERIE|
	              RB_ETH_EIE_RXERIE;   //开启所有中断
	
	
	R8_ETH_EIE |= RB_ETH_EIE_R_EN50;     //开启50欧上拉

	//写1清0
	R8_ETH_EIR = 0xff;               			//清除中断标志
	R8_ETH_ESTAT |= RB_ETH_ESTAT_INT | RB_ETH_ESTAT_BUFER;  //清除状态
	
	//收发模块复位
	R8_ETH_ECON1 |= (RB_ETH_ECON1_TXRST|RB_ETH_ECON1_RXRST);   
	R8_ETH_ECON1 &= ~(RB_ETH_ECON1_TXRST|RB_ETH_ECON1_RXRST);
	
	//接收使能在link中断里写

	//过滤模式，接收包类型
	R8_ETH_ERXFCON = 0;  //不过滤 接收所有包
	
	//过滤模式，限制包类型	
	R8_ETH_MACON1 |= RB_ETH_MACON1_MARXEN;       //MAC接收使能
	R8_ETH_MACON2 &= ~RB_ETH_MACON2_PADCFG;

	R8_ETH_MACON2 |= PADCFG_AUTO_3;               //所有短包自动填充到60
	
	R8_ETH_MACON2 |= RB_ETH_MACON2_TXCRCEN;          //硬件填充CRC
	R8_ETH_MACON2 &= ~RB_ETH_MACON2_HFRMEN;          //不接收巨型帧
	

	R8_ETH_MACON2 |= RB_ETH_MACON2_FULDPX;    //全双工
	R16_ETH_MAMXFL = MAC_MAX_LEN;            //MAC层最大接收包长度
	
	R8_ETH_MAADR1 = myCh579MAC[0];           //MAC赋值
	R8_ETH_MAADR2 = myCh579MAC[1];
	R8_ETH_MAADR3 = myCh579MAC[2];
	R8_ETH_MAADR4 = myCh579MAC[3];
	R8_ETH_MAADR5 = myCh579MAC[4];
	R8_ETH_MAADR6 = myCh579MAC[5];
	
	//使能ETH引脚
	R16_PIN_ANALOG_IE |= RB_PIN_ETH_IE;
	
	R16_ETH_ERXST = (UINT16)ETHRxMagPara.RxBufAddr[ETHRxMagPara.RecvIndex];     //当前接收缓存

	R8_ETH_ECON1 |= RB_ETH_ECON1_RXEN;                                      //接收使能

	NVIC_EnableIRQ(ETH_IRQn); 

	//printf("ETHInitX OK \r\n");
}


/*******************************************************************************
 * @fn          ETH_IRQ_ERR_Deal
 *
 * @brief       中断收发错误处理
 *
 * input parameters
 *
 * @param       err_sta. 当前错误状态
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
static void ETH_IRQ_ERR_Deal(UINT8 err_sta)
{
	if(err_sta&RB_ETH_ESTAT_BUFER)    printf("err:BUFER\r\n");
	if(err_sta&RB_ETH_ESTAT_RXCRCER)  printf("err:RXCRCER\r\n");
	if(err_sta&RB_ETH_ESTAT_RXNIBBLE) printf("err:RXNIBBLE\r\n");
	if(err_sta&RB_ETH_ESTAT_RXMORE)   printf("err:RXMORE\r\n");
	if(err_sta&RB_ETH_ESTAT_RXBUSY)   printf("err:RXBUSY\r\n");
	if(err_sta&RB_ETH_ESTAT_TXABRT)   printf("err:TXABRT\r\n");
}



/*************************************************************
将待发送的数据放入MAC发送队列

send_len <= MAC_MAX_LEN(576 字节)

成功返回 0，失败返回 非0值

*************************************************************/
UINT8 ETHSendX(UINT8 *pSendBuf, UINT16 send_len)
{
	UINT16 len;
	UINT8 *p_data,*p_tx_buf;

	len = send_len;
	p_data = pSendBuf;
	
	if(!ETHTxMagPara.SendEn)  
	{
		return 0xff;
	}

	
	if(ETHTxMagPara.TxQueueCnt>=TX_QUEUE_NUM)
	{
		//发送队列满
		return 0xfe;
	}


	//标记发送缓冲区中有数据及数据长度
	ETHTxMagPara.TxBufStau[ETHTxMagPara.WriteIndex] = 1;
	ETHTxMagPara.TxBufLen[ETHTxMagPara.WriteIndex] = len;
	p_tx_buf = (UINT8 *)ETHTxMagPara.TxBufAddr[ETHTxMagPara.WriteIndex];
	

	memcpy(p_tx_buf, p_data, len);

	
	ETHTxMagPara.WriteIndex++;
	if(ETHTxMagPara.WriteIndex>=TX_QUEUE_NUM)
	{
		ETHTxMagPara.WriteIndex = 0;
	}

	                                            
	ETHTxMagPara.TxQueueCnt++;
	if(ETHTxMagPara.TxQueueCnt==1)   //只有一个包的时候，启动发送
	{
		R16_ETH_ETXLN = ETHTxMagPara.TxBufLen[ETHTxMagPara.SendIndex];          //写入长度
		R16_ETH_ETXST = (UINT16)ETHTxMagPara.TxBufAddr[ETHTxMagPara.SendIndex]; //写入地址         
		ETHTxMagPara.TxBufStau[ETHTxMagPara.SendIndex] = 0;                     //空闲
		R8_ETH_ECON1 |= RB_ETH_ECON1_TXRTS;                                     //开始发送
		//printf("SendIndex: %x\r\n",ETHTxMagPara.SendIndex);
		ETHTxMagPara.SendIndex++;
		if(ETHTxMagPara.SendIndex>=TX_QUEUE_NUM) ETHTxMagPara.SendIndex = 0;
		ETHTxMagPara.TxQueueCnt--;
	}
	
	return 0;
}


/***************************************************************
接收以太网MAC层数据

pRecvBuf: 保存接收数据的缓冲区

pRecvBuf >= MAC缓冲区  MAC_MAX_LEN(576 字节)

RecvBufSize: 接收缓冲区大小

返回数据包的长度

1次调用只读取MAC接收缓冲区队列里面的一个接收缓冲区数据

//pRecvBuf  --> MAX512

****************************************************************/
UINT16 ETHRecX(UINT8     *pRecvBuf, UINT16 RecvBufSize)
{
	UINT16  recv_len = 0;
	UINT8  *p_data = NULL;
	UINT8  *p_rx_buf = NULL;

	
	if(!ETHRxMagPara.RemainCout) 
	{
		//ETHRxMagPara.RemainCout == 0 没有数据可接收
		return 0;  
	}
	
	
	p_data = pRecvBuf;
	p_rx_buf = (UINT8 *)ETHRxMagPara.RxBufAddr[ETHRxMagPara.ReadIndex];
	recv_len = (ETHRxMagPara.RxBufLen[ETHRxMagPara.ReadIndex]<RecvBufSize)?ETHRxMagPara.RxBufLen[ETHRxMagPara.ReadIndex]:RecvBufSize ;
	
	memcpy(p_data, p_rx_buf, recv_len);
	
	ETHRxMagPara.RxBufStau[ETHRxMagPara.ReadIndex] = 0;
	ETHRxMagPara.ReadIndex++;
	if(ETHRxMagPara.ReadIndex>=RX_QUEUE_NUM) ETHRxMagPara.ReadIndex = 0;

	ETHRxMagPara.RemainCout--;
	
	return recv_len;
	
}


/*
 获取MAC层接收到的数据的长度

*/
UINT16 GetRecvMacDataLen(void)
{
	UINT16  recv_len = 0;

	if(!ETHRxMagPara.RemainCout) 
	{
		//ETHRxMagPara.RemainCout == 0 没有数据可接收
		return 0;  
	}

	recv_len = ETHRxMagPara.RxBufLen[ETHRxMagPara.ReadIndex];

	return recv_len;
}


extern void  lwip_pkt_handle(void);

void ETH_IRQHandler(void)
{
	UINT8   eth_irq_flag = 0;
	UINT8   eth_irq_status = 0;
	UINT16  phy_reg = 0;
	UINT16  rec_len = 0;

	eth_irq_flag = R8_ETH_EIR;
	
	if(eth_irq_flag&RB_ETH_EIR_RXIF)                                           
	{	
		SET_NET_LEDX_FLASH();
		//接收完成中断	

		//硬件自动覆盖
		rec_len = R16_ETH_ERXLN;
		if(ETHRxMagPara.RemainCout < (RX_QUEUE_NUM-1))  
		{
			 //有队列空闲，保留最后一个随便覆盖，也使用这个队列里的数据
			ETHRxMagPara.RxBufStau[ETHRxMagPara.RecvIndex] = 1;
			ETHRxMagPara.RxBufLen[ETHRxMagPara.RecvIndex] = rec_len;
			RX_count = ETHRxMagPara.RecvIndex;
			ETHRxMagPara.RemainCout++;
			ETHRxMagPara.RecvIndex++;
			//if(ETHRxMagPara.RecvIndex>=RX_QUEUE_NUM) ETHRxMagPara.RecvIndex = 0;
			R16_ETH_ERXST = (UINT16)ETHRxMagPara.RxBufAddr[ETHRxMagPara.RecvIndex]; //更新下一个接收地址
		}
		else                                                                        
		{
			//覆盖最新的包
			/************************************************************************
				所有队列都有数据时，最后一个队列是不用的，直到一个新的空闲队列产生后，
				并接收一个新的数据包才会读取这个数据。
			*************************************************************************/
			ETHRxMagPara.RxBufStau[ETHRxMagPara.RecvIndex] = 1;
			ETHRxMagPara.RxBufLen[ETHRxMagPara.RecvIndex] = rec_len;
			ETHRxMagPara.RemainCout = 0;
			ETHRxMagPara.RecvIndex = 0;
			RX_count = ETHRxMagPara.RecvIndex;
			R16_ETH_ERXST = (UINT16)ETHRxMagPara.RxBufAddr[ETHRxMagPara.RecvIndex];
		}
		//printf("Bo dem:%d\r\n", ETHRxMagPara.RemainCout);
		//printf("recv finish:%d\r\n", rec_len);
		//lwip_pkt_handle();
		
		/*printf("MAC: ");
		for(int i = 22; i < 28; i++){
			printf("%x-", MACRxBuf[RX_count][i]);
		}
		printf("\r\n");*/
		R8_ETH_EIR = RB_ETH_EIR_RXIF; //清除接收完成中断标志
		
		//ARP_Respone
		if(MACRxBuf[RX_count][12] ==0x08 && MACRxBuf[RX_count][13] == 0x06 && MACRxBuf[RX_count][41] == 0xc8 && MACRxBuf[RX_count][21] == 0x01){
		ARP_Respone();
		}
		//UDP Revcive
		/*if(MACRxBuf[RX_count][12] ==0x08 && MACRxBuf[RX_count][13] == 0x00 && MACRxBuf[RX_count][23] == 0x11){
			 
			//LED_FLASH();
			if(MACRxBuf[RX_count][42] == 'O' && MACRxBuf[RX_count][43] == 'N') LED_ON();
			if(MACRxBuf[RX_count][42] == 'O' && MACRxBuf[RX_count][43] == 'F' && MACRxBuf[RX_count][44] == 'F') LED_OFF();
		}*/
		//TCP Handle
		if(MACRxBuf[RX_count][12] ==0x08 && MACRxBuf[RX_count][13] == 0x00 && MACRxBuf[RX_count][23] == 0x06){
			TCP_Handle();
		}
		
	}	
	if(eth_irq_flag&RB_ETH_EIR_LINKIF)                                         
	{
		 //Link 变化中断
		phy_reg = ReadPHYRegX(PHY_BMSR);                                         //读取PHY状态寄存器
		if(phy_reg&0x04) 
		{
			//printf("link connect ok....\r\n");
			ETHTxMagPara.SendEn = 1;
			SET_NET_LED_ON();
			
		}
		else 
		{
			//printf("link disconnect ok....\r\n");
			ETHTxMagPara.SendEn = 0;
			SET_NET_LED_OFF();
		}

		R8_ETH_EIR = RB_ETH_EIR_LINKIF;
	}
	if(eth_irq_flag&RB_ETH_EIR_TXIF)                                           
	{
		//SET_NET_LEDX_FLASH();
		//printf("send finish interrupt\r\n");
		 //发送完成中断
		/*if(ETHTxMagPara.TxQueueCnt)   //队列里还有数据包没有发送
		{
			R16_ETH_ETXLN = ETHTxMagPara.TxBufLen[ETHTxMagPara.SendIndex];
			R16_ETH_ETXST = ETHTxMagPara.TxBufAddr[ETHTxMagPara.SendIndex];
			ETHTxMagPara.TxBufStau[ETHTxMagPara.SendIndex] = 0;  //空闲
			
			R8_ETH_ECON1 |= RB_ETH_ECON1_TXRTS;        //开始发送
			ETHTxMagPara.SendIndex++;
			if(ETHTxMagPara.SendIndex>=TX_QUEUE_NUM) ETHTxMagPara.SendIndex = 0;
			ETHTxMagPara.TxQueueCnt--;
		}  */                                                //发送处理
		R8_ETH_EIR = RB_ETH_EIR_TXIF;
	}

	
	if(eth_irq_flag&(RB_ETH_EIR_TXERIF|RB_ETH_EIR_RXERIF))   //收发出错中断
	{
		eth_irq_status = R8_ETH_ESTAT;
		ETH_IRQ_ERR_Deal(eth_irq_status);  //出错处理
		//清除错误中断标志
		R8_ETH_EIR = (RB_ETH_EIR_TXERIF|RB_ETH_EIR_RXERIF);

		//发送接收出错后，重启CH579 
		//NVIC_SystemReset();
	}
	
}

void phy_change(UINT8 flag)
{
	if((flag%2)==0)
		R32_ETH_MIWR=0x0004011e;
	else if(flag%2)
   	R32_ETH_MIWR=0x0000011e; 	
}
/*
 获取物理层链路状态

	1--》链路连接
	0--》链路断开

*/
UINT8 GetPhyLinkState(void)
{
	UINT16  phy_reg = 0;
	phy_reg = ReadPHYRegX(PHY_BMSR);                                         //读取PHY状态寄存器
	if(phy_reg&0x04) 
	{
		//printf("link connect ok....\r\n");
		phy_flag = 1;
		return 1;
	}
	 //printf("link disconnect ok....\r\n");
	return 0;
	
}

void ARP_Respone(void){
		/*Goi tin ARP Respone co 42 byte*/
	//MAC DIST
	ARP_Buff[0] = MACRxBuf[RX_count][6];
	ARP_Buff[1] = MACRxBuf[RX_count][7];
	ARP_Buff[2] = MACRxBuf[RX_count][8];
	ARP_Buff[3] = MACRxBuf[RX_count][9];
	ARP_Buff[4] = MACRxBuf[RX_count][10];
	ARP_Buff[5] = MACRxBuf[RX_count][11];
	//MAC SOURE
	ARP_Buff[6] = 0x00;
	ARP_Buff[7] = 0x22;
	ARP_Buff[8] = 0x47;
	ARP_Buff[9] = 0x01;	
	ARP_Buff[10] = 0x01;
	ARP_Buff[11] = 0x01;
	//Ethenet Type ARP: 0x0806
	ARP_Buff[12] = 0x08;
	ARP_Buff[13] = 0x06;
	//Hardware type  10Mb: 0x0001
	ARP_Buff[14] = 0x00;
	ARP_Buff[15] = 0x01;
	//Protocol type  0x0800: ARP
	ARP_Buff[16] = 0x08;
	ARP_Buff[17] = 0x00;
	//Size  0x0604
	ARP_Buff[18] = 0x06;
	ARP_Buff[19] = 0x04;
	//Opcode ARP Reply: 0x0002 
	ARP_Buff[20] = 0x00;
	ARP_Buff[21] = 0x02;
	//MAC Soure
	ARP_Buff[22] = ARP_Buff[6];
	ARP_Buff[23] = ARP_Buff[7];
	ARP_Buff[24] = ARP_Buff[8];
	ARP_Buff[25] = ARP_Buff[9];
	ARP_Buff[26] = ARP_Buff[10];
	ARP_Buff[27] = ARP_Buff[11];
	//Soure IP
	ARP_Buff[28] = 0xc0; //192.168.1.200
	ARP_Buff[29] = 0xa8;
	ARP_Buff[30] = 0x01;
	ARP_Buff[31] = 0xc8;
	//MAC Dist
	ARP_Buff[32] = ARP_Buff[0];
	ARP_Buff[33] = ARP_Buff[1];
	ARP_Buff[34] = ARP_Buff[2];
	ARP_Buff[35] = ARP_Buff[3];
	ARP_Buff[36] = ARP_Buff[4];
	ARP_Buff[37] = ARP_Buff[5];
	//Dist IP
	ARP_Buff[38] = MACRxBuf[RX_count][28];//192.168.1.100
	ARP_Buff[39] = MACRxBuf[RX_count][29];
	ARP_Buff[40] = MACRxBuf[RX_count][30];
	ARP_Buff[41] = MACRxBuf[RX_count][31];
	
	for(int i = 42 ; i < 60 ; i++)  //Padding
		{
		 ARP_Buff[i] = 0x00;
		}
		
	ETHSendX(ARP_Buff, 60);
}

void TCP_Handle(void){
	UINT32 checksumIP_TCP = 0;
	UINT32 checksumTCP_TCP = 0;
	//printf("Co 1 Client yeu cau ket noi\r\n");
	
	if(MACRxBuf[RX_count][47] == TCP_SYN && MACRxBuf[RX_count][33] == 0xc8){//Goi SYN|ACK chi co 44 Byte phan hoi thoi! + 14 Byte MAC + 2Byte Padding cho du 60Byte
		//MAC Dist
		TCP_Buff[0] = MACRxBuf[RX_count][6];
		TCP_Buff[1] = MACRxBuf[RX_count][7];
		TCP_Buff[2] = MACRxBuf[RX_count][8];
		TCP_Buff[3] = MACRxBuf[RX_count][9];
		TCP_Buff[4] = MACRxBuf[RX_count][10];
		TCP_Buff[5] = MACRxBuf[RX_count][11];
		// MAC Soure
		TCP_Buff[6] = 0x00;
		TCP_Buff[7] = 0x22;
		TCP_Buff[8] = 0x47;
		TCP_Buff[9] = 0x01;
		TCP_Buff[10] = 0x01;
		TCP_Buff[11] = 0x01;
		//Ethenet Type IP: 0x0800
		TCP_Buff[12] = 0x08;
		TCP_Buff[13] = 0x00;
		//Header Length
		TCP_Buff[14] = 0x45;
		//Services: Rountine 0x00
		TCP_Buff[15] = 0x00;
		//Total Length  52 byte
		TCP_Buff[16] = 0x00;
		TCP_Buff[17] = 0x34;
	
		//Indenfitication
		TCP_Buff[18] = 0x00;
		TCP_Buff[19] = 0x0c;
		//Flag 0x00
		TCP_Buff[20] = 0x00;
		TCP_Buff[21] = 0x00;
		//TimetoLive
		TCP_Buff[22] = 0x80;
		//Protocol
		TCP_Buff[23] = 0x06;
		//Checksum
		TCP_Buff[24] = 0x00;
		TCP_Buff[25] = 0x00;
		//Soure IP
		TCP_Buff[26] = 0xc0; //192.168.1.200
		TCP_Buff[27] = 0xa8;
		TCP_Buff[28] = 0x01;
		TCP_Buff[29] = 0xC8;
		//Dist IP
		TCP_Buff[30] = MACRxBuf[RX_count][26];//192.168.1.100
		TCP_Buff[31] = MACRxBuf[RX_count][27];
		TCP_Buff[32] = MACRxBuf[RX_count][28];
		TCP_Buff[33] = MACRxBuf[RX_count][29];
		//Tinh Check sum IP
		for(int j = 14; j < 34; j = j+2){
		checksumIP_TCP = checksumIP_TCP + (((UINT16)TCP_Buff[j]<<8)|TCP_Buff[j+1]);
		if(checksumIP_TCP > 0x10000){
		checksumIP_TCP = checksumIP_TCP - 0x10000 + 1;
			}		
		}
		checksumIP_TCP = 0xFFFF - (UINT16)checksumIP_TCP;
		TCP_Buff[24] = ((UINT8)(checksumIP_TCP>>8))&0xFF;
		TCP_Buff[25] = (UINT8)checksumIP_TCP&0xFF;
		
		//Soure Port 2000
		TCP_Buff[34] = 0x07;
		TCP_Buff[35] = 0xD0;
		//Dist Port
		TCP_Buff[36] = MACRxBuf[RX_count][34];
		TCP_Buff[37] = MACRxBuf[RX_count][35];
		// Sequence
		TCP_Buff[38] = 0x00;
		TCP_Buff[39] = 0x00;
		TCP_Buff[40] = 0x00;
		TCP_Buff[41] = 0x00;
		//Ack Sequence
		TCP_Buff[42] = MACRxBuf[RX_count][38];
		TCP_Buff[43] = MACRxBuf[RX_count][39];
		TCP_Buff[44] = MACRxBuf[RX_count][40];
		TCP_Buff[45] = MACRxBuf[RX_count][41]+1;
		//Data Offset
		TCP_Buff[46] = 0x80;
		//TCP Flags
		TCP_Buff[47] = 0x12;
		//Windown 
		TCP_Buff[48] = 0x72;
		TCP_Buff[49] = 0x10;
		//TCP checksum
		TCP_Buff[50] = 0x00;
		TCP_Buff[51] = 0x00;
		//Urgent
		TCP_Buff[52] = 0x00;//MACRxBuf[RX_count][52];
		TCP_Buff[53] = 0x00;//MACRxBuf[RX_count][53];
		//Data
		TCP_Buff[54] = 0x02;
		TCP_Buff[55] = 0x04;
		TCP_Buff[56] = 0x05;//Windown Size
		TCP_Buff[57] = 0x84;
		TCP_Buff[58] = 0x01;
		TCP_Buff[59] = 0x03;//Windown Scaling
		TCP_Buff[60] = 0x03;
		TCP_Buff[61] = 0x07;
		TCP_Buff[62] = 0x01;
		TCP_Buff[63] = 0x01;
		TCP_Buff[64] = 0x04;//Selecttive ACK
		TCP_Buff[65] = 0x02;
		

		//Tinh Checksum TCP
		for(int k = 26; k < 66; k = k+2){
		checksumTCP_TCP = checksumTCP_TCP + (((UINT16)TCP_Buff[k]<<8)|TCP_Buff[k+1]);
		if(checksumTCP_TCP > 0x10000){
		checksumTCP_TCP = checksumTCP_TCP - 0x10000 + 1;
			}
		}
		checksumTCP_TCP = checksumTCP_TCP + 0x06 + 0x20;  //Checksum + them Protocol(0x06 -TCP) + Do dai TCP Segment Header(0x20 -32Byte)
		checksumTCP_TCP = 0xFFFF - (UINT16)checksumTCP_TCP;
		TCP_Buff[50] = ((UINT8)(checksumTCP_TCP>>8))&0xFF;
		TCP_Buff[51] = (UINT8)checksumTCP_TCP&0xFF;
		//TCP_Buff[50] = ((UINT16)checksumTCP_TCP>>8)&0xFF;
		//TCP_Buff[51] = (UINT16)checksumTCP_TCP&0xFF;
		
		//Send Packet
		ETHSendX(TCP_Buff, 66);
	}
	
	if(MACRxBuf[RX_count][47] == TCP_ACK && MACRxBuf[RX_count][33] == 0xc8){
		printf("ket noi THANH CONG\r\n");
	}
	
	if(MACRxBuf[RX_count][47] == (TCP_FIN|TCP_ACK) && MACRxBuf[RX_count][33] == 0xc8){
		//printf("ket noi THANH CONG\r\n");
		//MAC Dist
		TCP_Buff[0] = MACRxBuf[RX_count][6];
		TCP_Buff[1] = MACRxBuf[RX_count][7];
		TCP_Buff[2] = MACRxBuf[RX_count][8];
		TCP_Buff[3] = MACRxBuf[RX_count][9];
		TCP_Buff[4] = MACRxBuf[RX_count][10];
		TCP_Buff[5] = MACRxBuf[RX_count][11];
		// MAC Soure
		TCP_Buff[6] = 0x00;
		TCP_Buff[7] = 0x22;
		TCP_Buff[8] = 0x47;
		TCP_Buff[9] = 0x01;
		TCP_Buff[10] = 0x01;
		TCP_Buff[11] = 0x01;
		//Ethenet Type IP: 0x0800
		TCP_Buff[12] = 0x08;
		TCP_Buff[13] = 0x00;
		//Header Length
		TCP_Buff[14] = 0x45;
		//Services: Rountine 0x00
		TCP_Buff[15] = 0x00;
		//Total Length  40 byte
		TCP_Buff[16] = 0x00;
		TCP_Buff[17] = 0x28;
	
		//Indenfitication
		TCP_Buff[18] = 0x00;
		TCP_Buff[19] = 0x0c;
		//Flag 0x00
		TCP_Buff[20] = 0x00;
		TCP_Buff[21] = 0x00;
		//TimetoLive
		TCP_Buff[22] = 0x80;
		//Protocol
		TCP_Buff[23] = 0x06;
		//Checksum
		TCP_Buff[24] = 0x00;
		TCP_Buff[25] = 0x00;
		//Soure IP
		TCP_Buff[26] = 0xc0; //192.168.1.200
		TCP_Buff[27] = 0xa8;
		TCP_Buff[28] = 0x01;
		TCP_Buff[29] = 0xC8;
		//Dist IP
		TCP_Buff[30] = MACRxBuf[RX_count][26];//192.168.1.100
		TCP_Buff[31] = MACRxBuf[RX_count][27];
		TCP_Buff[32] = MACRxBuf[RX_count][28];
		TCP_Buff[33] = MACRxBuf[RX_count][29];
		//Tinh Check sum IP
		for(int j = 14; j < 34; j = j+2){
		checksumIP_TCP = checksumIP_TCP + (((UINT16)TCP_Buff[j]<<8)|TCP_Buff[j+1]);
		if(checksumIP_TCP > 0x10000){
		checksumIP_TCP = checksumIP_TCP - 0x10000 + 1;
			}		
		}
		checksumIP_TCP = 0xFFFF - (UINT16)checksumIP_TCP;
		TCP_Buff[24] = ((UINT8)(checksumIP_TCP>>8))&0xFF;
		TCP_Buff[25] = (UINT8)checksumIP_TCP&0xFF;
		
		//Soure Port 2000
		TCP_Buff[34] = 0x07;
		TCP_Buff[35] = 0xD0;
		//Dist Port
		TCP_Buff[36] = MACRxBuf[RX_count][34];
		TCP_Buff[37] = MACRxBuf[RX_count][35];
		// Sequence
		TCP_Buff[38] = MACRxBuf[RX_count][42];
		TCP_Buff[39] = MACRxBuf[RX_count][43];
		TCP_Buff[40] = MACRxBuf[RX_count][44];
		TCP_Buff[41] = MACRxBuf[RX_count][45];
		//Ack Sequence
		TCP_Buff[42] = MACRxBuf[RX_count][38];
		TCP_Buff[43] = MACRxBuf[RX_count][39];
		TCP_Buff[44] = MACRxBuf[RX_count][40];
		TCP_Buff[45] = MACRxBuf[RX_count][41]+1;
		//Data Offset
		TCP_Buff[46] = 0x50;
		//TCP Flags
		TCP_Buff[47] = 0x11;
		//Windown 
		TCP_Buff[48] = 0x72;
		TCP_Buff[49] = 0x10;
		//TCP checksum
		TCP_Buff[50] = 0x00;
		TCP_Buff[51] = 0x00;
		//Urgent
		TCP_Buff[52] = 0x00;//MACRxBuf[RX_count][52];
		TCP_Buff[53] = 0x00;//MACRxBuf[RX_count][53];
		//Data
		TCP_Buff[54] = 0x00;//Padding
		TCP_Buff[55] = 0x00;
		TCP_Buff[56] = 0x00;
		TCP_Buff[57] = 0x00;
		TCP_Buff[58] = 0x00;
		TCP_Buff[59] = 0x00;

		
		//Tinh Checksum TCP
		for(int k = 26; k < 60; k = k+2){
		checksumTCP_TCP = checksumTCP_TCP + (((UINT16)TCP_Buff[k]<<8)|TCP_Buff[k+1]);
		if(checksumTCP_TCP > 0x10000){
		checksumTCP_TCP = checksumTCP_TCP - 0x10000 + 1;
			}
		}
		checksumTCP_TCP = checksumTCP_TCP + 0x06 + 0x14;  //Checksum + them Protocol(0x06 -TCP) + Do dai TCP Segment Header(0x14 -20Byte)
		checksumTCP_TCP = 0xFFFF - (UINT16)checksumTCP_TCP;
		TCP_Buff[50] = ((UINT8)(checksumTCP_TCP>>8))&0xFF;
		TCP_Buff[51] = (UINT8)checksumTCP_TCP&0xFF;
		//TCP_Buff[50] = ((UINT16)checksumTCP_TCP>>8)&0xFF;
		//TCP_Buff[51] = (UINT16)checksumTCP_TCP&0xFF;
		
		//Send Packet
		ETHSendX(TCP_Buff, 60);
	}
	
	if(MACRxBuf[RX_count][47] == (TCP_PSH|TCP_ACK) && MACRxBuf[RX_count][33] == 0xc8){
			for(int i = 54; i < (MACRxBuf[RX_count][17] + 0x0E); i++){
			printf("%c", MACRxBuf[RX_count][i]);
			}
			printf("....\r\n");
		//ACK
		TCP_Buff[0] = MACRxBuf[RX_count][6];
		TCP_Buff[1] = MACRxBuf[RX_count][7];
		TCP_Buff[2] = MACRxBuf[RX_count][8];
		TCP_Buff[3] = MACRxBuf[RX_count][9];
		TCP_Buff[4] = MACRxBuf[RX_count][10];
		TCP_Buff[5] = MACRxBuf[RX_count][11];
		// MAC Soure
		TCP_Buff[6] = 0x00;
		TCP_Buff[7] = 0x22;
		TCP_Buff[8] = 0x47;
		TCP_Buff[9] = 0x01;
		TCP_Buff[10] = 0x01;
		TCP_Buff[11] = 0x01;
		//Ethenet Type IP: 0x0800
		TCP_Buff[12] = 0x08;
		TCP_Buff[13] = 0x00;
		//Header Length
		TCP_Buff[14] = 0x45;
		//Services: Rountine 0x00
		TCP_Buff[15] = 0x00;
		//Total Length  40 byte
		TCP_Buff[16] = 0x00;
		TCP_Buff[17] = 0x28;
	
		//Indenfitication
		TCP_Buff[18] = 0x00;
		TCP_Buff[19] = 0x0c;
		//Flag 0x00
		TCP_Buff[20] = 0x00;
		TCP_Buff[21] = 0x00;
		//TimetoLive
		TCP_Buff[22] = 0x80;
		//Protocol
		TCP_Buff[23] = 0x06;
		//Checksum
		TCP_Buff[24] = 0x00;
		TCP_Buff[25] = 0x00;
		//Soure IP
		TCP_Buff[26] = 0xc0; //192.168.1.200
		TCP_Buff[27] = 0xa8;
		TCP_Buff[28] = 0x01;
		TCP_Buff[29] = 0xC8;
		//Dist IP
		TCP_Buff[30] = MACRxBuf[RX_count][26];//192.168.1.100
		TCP_Buff[31] = MACRxBuf[RX_count][27];
		TCP_Buff[32] = MACRxBuf[RX_count][28];
		TCP_Buff[33] = MACRxBuf[RX_count][29];
		//Tinh Check sum IP
		for(int j = 14; j < 34; j = j+2){
		checksumIP_TCP = checksumIP_TCP + (((UINT16)TCP_Buff[j]<<8)|TCP_Buff[j+1]);
		if(checksumIP_TCP > 0x10000){
		checksumIP_TCP = checksumIP_TCP - 0x10000 + 1;
			}		
		}
		checksumIP_TCP = 0xFFFF - (UINT16)checksumIP_TCP;
		TCP_Buff[24] = ((UINT8)(checksumIP_TCP>>8))&0xFF;
		TCP_Buff[25] = (UINT8)checksumIP_TCP&0xFF;
		
		//Soure Port 2000
		TCP_Buff[34] = 0x07;
		TCP_Buff[35] = 0xD0;
		//Dist Port
		TCP_Buff[36] = MACRxBuf[RX_count][34];
		TCP_Buff[37] = MACRxBuf[RX_count][35];
		// Sequence
		TCP_Buff[38] = MACRxBuf[RX_count][42];
		TCP_Buff[39] = MACRxBuf[RX_count][43];
		TCP_Buff[40] = MACRxBuf[RX_count][44];
		TCP_Buff[41] = MACRxBuf[RX_count][45];
		//Ack Sequence
		TCP_Buff[42] = MACRxBuf[RX_count][38];
		TCP_Buff[43] = MACRxBuf[RX_count][39];
		TCP_Buff[44] = MACRxBuf[RX_count][40];
		TCP_Buff[45] = MACRxBuf[RX_count][41] + MACRxBuf[RX_count][17] + 14 - 54;
		//Data Offset
		TCP_Buff[46] = 0x50;
		//TCP Flags
		TCP_Buff[47] = TCP_ACK;
		//Windown 
		TCP_Buff[48] = 0x72;
		TCP_Buff[49] = 0x10;
		//TCP checksum
		TCP_Buff[50] = 0x00;
		TCP_Buff[51] = 0x00;
		//Urgent
		TCP_Buff[52] = 0x00;//MACRxBuf[RX_count][52];
		TCP_Buff[53] = 0x00;//MACRxBuf[RX_count][53];
		
		TCP_Buff[54] = 0x00;//Padding
		TCP_Buff[55] = 0x00;
		TCP_Buff[56] = 0x00;
		TCP_Buff[57] = 0x00;
		TCP_Buff[58] = 0x00;
		TCP_Buff[59] = 0x00;
	
		//Tinh Checksum TCP
		for(int k = 26; k < 60; k = k+2){
		checksumTCP_TCP = checksumTCP_TCP + (((UINT16)TCP_Buff[k]<<8)|TCP_Buff[k+1]);
		if(checksumTCP_TCP > 0x10000){
		checksumTCP_TCP = checksumTCP_TCP - 0x10000 + 1;
			}
		}
		checksumTCP_TCP = checksumTCP_TCP + 0x06 + 0x14;  //Checksum + them Protocol(0x06 -TCP) + Do dai TCP Segment Header(0x14 -20Byte)
		checksumTCP_TCP = 0xFFFF - (UINT16)checksumTCP_TCP;
		TCP_Buff[50] = ((UINT8)(checksumTCP_TCP>>8))&0xFF;
		TCP_Buff[51] = (UINT8)checksumTCP_TCP&0xFF;
		
		
		//PSH|ACK
		TCP_Buff1[0] = MACRxBuf[RX_count][6];
		TCP_Buff1[1] = MACRxBuf[RX_count][7];
		TCP_Buff1[2] = MACRxBuf[RX_count][8];
		TCP_Buff1[3] = MACRxBuf[RX_count][9];
		TCP_Buff1[4] = MACRxBuf[RX_count][10];
		TCP_Buff1[5] = MACRxBuf[RX_count][11];
		// MAC Soure
		TCP_Buff1[6] = 0x00;
		TCP_Buff1[7] = 0x22;
		TCP_Buff1[8] = 0x47;
		TCP_Buff1[9] = 0x01;
		TCP_Buff1[10] = 0x01;
		TCP_Buff1[11] = 0x01;
		//Ethenet Type IP: 0x0800
		TCP_Buff1[12] = 0x08;
		TCP_Buff1[13] = 0x00;
		//Header Length
		TCP_Buff1[14] = 0x45;
		//Services: Rountine 0x00
		TCP_Buff1[15] = 0x00;
		//Total Length  40 byte
		TCP_Buff1[16] = 0x00;
		TCP_Buff1[17] = MACRxBuf[RX_count][17];//0x36;
	
		//Indenfitication
		TCP_Buff1[18] = 0x00;
		TCP_Buff1[19] = 0x0c;
		//Flag 0x00
		TCP_Buff1[20] = 0x00;
		TCP_Buff1[21] = 0x00;
		//TimetoLive
		TCP_Buff1[22] = 0x80;
		//Protocol
		TCP_Buff1[23] = 0x06;
		//Checksum
		TCP_Buff1[24] = 0x00;
		TCP_Buff1[25] = 0x00;
		//Soure IP
		TCP_Buff1[26] = 0xc0; //192.168.1.200
		TCP_Buff1[27] = 0xa8;
		TCP_Buff1[28] = 0x01;
		TCP_Buff1[29] = 0xC8;
		//Dist IP
		TCP_Buff1[30] = MACRxBuf[RX_count][26];//192.168.1.100
		TCP_Buff1[31] = MACRxBuf[RX_count][27];
		TCP_Buff1[32] = MACRxBuf[RX_count][28];
		TCP_Buff1[33] = MACRxBuf[RX_count][29];
		//Tinh Check sum IP
		checksumIP_TCP = 0;
		
		for(int j = 14; j < 34; j = j+2){
		checksumIP_TCP = checksumIP_TCP + (((UINT16)TCP_Buff1[j]<<8)|TCP_Buff1[j+1]);
		if(checksumIP_TCP > 0x10000){
		checksumIP_TCP = checksumIP_TCP - 0x10000 + 1;
			}		
		}
		checksumIP_TCP = 0xFFFF - (UINT16)checksumIP_TCP;
		TCP_Buff1[24] = ((UINT8)(checksumIP_TCP>>8))&0xFF;
		TCP_Buff1[25] = (UINT8)checksumIP_TCP&0xFF;
		
		//Soure Port 2000
		TCP_Buff1[34] = 0x07;
		TCP_Buff1[35] = 0xD0;
		//Dist Port
		TCP_Buff1[36] = MACRxBuf[RX_count][34];
		TCP_Buff1[37] = MACRxBuf[RX_count][35];
		// Sequence
		TCP_Buff1[38] = MACRxBuf[RX_count][42];
		TCP_Buff1[39] = MACRxBuf[RX_count][43];
		TCP_Buff1[40] = MACRxBuf[RX_count][44];
		TCP_Buff1[41] = MACRxBuf[RX_count][45];
		//Ack Sequence
		TCP_Buff1[42] = MACRxBuf[RX_count][38];
		TCP_Buff1[43] = MACRxBuf[RX_count][39];
		TCP_Buff1[44] = MACRxBuf[RX_count][40];
		TCP_Buff1[45] = MACRxBuf[RX_count][41] + MACRxBuf[RX_count][17] + 14 - 54;
		//Data Offset
		TCP_Buff1[46] = 0x50;
		//TCP Flags
		TCP_Buff1[47] = 0x18;
		//Windown 
		TCP_Buff1[48] = 0x72;
		TCP_Buff1[49] = 0x10;
		//TCP checksum
		TCP_Buff1[50] = 0x00;
		TCP_Buff1[51] = 0x00;
		//Urgent
		TCP_Buff1[52] = 0x00;//MACRxBuf[RX_count][52];
		TCP_Buff1[53] = 0x00;//MACRxBuf[RX_count][53];
		
		for(int chay = 54; chay < (MACRxBuf[RX_count][17] + 14); chay++){
			TCP_Buff1[chay] = MACRxBuf[RX_count][chay];
		}
				
	  checksumTCP_TCP = 0;
		//Tinh Checksum TCP
		int bu = 0;
		if((MACRxBuf[RX_count][17] + 14) % 2 == 0) bu = 14;
		else {
			bu = 15;
			TCP_Buff1[MACRxBuf[RX_count][17] + 14]= 0x00;
		}
		
		for(int k = 26; k < (MACRxBuf[RX_count][17] + bu); k = k+2){
		checksumTCP_TCP = checksumTCP_TCP + (((UINT16)TCP_Buff1[k]<<8)|TCP_Buff1[k+1]);
		if(checksumTCP_TCP > 0x10000){
		checksumTCP_TCP = checksumTCP_TCP - 0x10000 + 1;
			}
		}
		checksumTCP_TCP = checksumTCP_TCP + 0x06 + MACRxBuf[RX_count][17] + 14 - 34;  //Checksum + them Protocol(0x06 -TCP) + Do dai TCP Segment Header(0x14 -20Byte) + Do dai DATA (0X0E - 14 byte)
		checksumTCP_TCP = 0xFFFF - (UINT16)checksumTCP_TCP;
		TCP_Buff1[50] = ((UINT8)(checksumTCP_TCP>>8))&0xFF;
		TCP_Buff1[51] = (UINT8)checksumTCP_TCP&0xFF;
		
		//Send Packet
		ETHSendX(TCP_Buff, 60);
		mDelaymS(10);
		//Send Packet
		ETHSendX(TCP_Buff1, (MACRxBuf[RX_count][17] + 14));
			
	}

}
