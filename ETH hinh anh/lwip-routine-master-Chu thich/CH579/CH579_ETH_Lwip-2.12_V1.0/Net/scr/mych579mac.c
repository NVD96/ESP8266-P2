

/************************************************************
CH579 mac 通信

*************************************************************/

#include "mych579mac.h"

#include "parameter_setting.h"
static  RXBUFST   ETHRxMagPara;   //接收管理参数
/* UINT8  RecvEn;        Cho phep NHAN                        
    UINT8  RecvIndex;    Chi so NHAN        (Bien chay cho MAng)                  
    UINT8  ReadIndex;     CHi so DOC        (Bien chay cho MAng)                
    UINT8  RemainCout;     Bo dem chot                
    UINT8  RxBufStau[RX_QUEUE_NUM];    Mang trang thai Bo dem nhan      
    UINT16 RxBufLen[RX_QUEUE_NUM];     Mang do dai Bo dem nhan      
    UINT32 RxBufAddr[RX_QUEUE_NUM];*/	// Mang Bo dem Dia chi	

static  TXBUFST   ETHTxMagPara;   //发送管理参数
/* UINT8  SendEn;                Cho phep GUI      
    UINT8  SendIndex;            Chi so GUI  (Bien chay cho MAng)       
    UINT8  WriteIndex;           Chi so GHI  (Bien chay cho MAng)       
    UINT8  TxQueueCnt;           Bo dem hang doi Tx         
    UINT8  TxBufStau[TX_QUEUE_NUM];  Mang trang thai Bo dem      
    UINT16 TxBufLen[TX_QUEUE_NUM];   Mang Do dai Bo dem     
    UINT32 TxBufAddr[TX_QUEUE_NUM];*/	// Mang Bo dem Dia chi

static  __align(4) UINT8     MACRxBuf[RX_QUEUE_NUM][RX_BUF_SIZE];   /*RX_QUEUE_NUM = 3; RX_BUF_SIZE = 1064*/ 
static  __align(4) UINT8     MACTxBuf[TX_QUEUE_NUM][TX_BUF_SIZE];   /*TX_QUEUE_NUM = 2; TX_BUF_SIZE = 1064 */ 

UINT8   myCh579MAC[MACADDR_LEN] = {0x84,0xc2,0xe4,0x02,0x03,0x04};  // Dia chi MAC

u8 phy_flag=0; //FLAG vat ly


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
	
	R16_ETH_MIWR = reg_val; //MII Write Data Register: Gan gia tri vao thanh ghi 
	reg_op = (reg_add&RB_ETH_MIREGADR_MIRDL);
	R8_ETH_MIREGADR = reg_op;   //MII Address Register: Gan Dia chi can Write vaof thanh ghi
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
	
	reg_op = reg_add&RB_ETH_MIREGADR_MIRDL;  //The address of PHY register [4:0] 5 bit dau tien  reg_add & 0x1F
	R8_ETH_MIREGADR = RB_ETH_MIREGADR_MIIWR|reg_op;  //0x20 | reg_op
	read_reg_val = R16_ETH_MIRD; //MII Read Data Register: Du lieu doc duoc lu vao thanh ghi
	
	return read_reg_val;
}


//初始化网络参数
void ETHParaInitX(void)
{
	UINT8 i = 0;
	
	memset((char *)&MACRxBuf[0][0],0,sizeof(MACRxBuf)); //Xoa toan bo vung nho MACRxBuf va ep kieu sang Pointer char
	memset((char *)&MACTxBuf[0][0],0,sizeof(MACTxBuf));	//Xoa toan bo vung nho MACTxBuf va ep kieu sang Pointer char
	memset((char *)&ETHRxMagPara,0,sizeof(ETHRxMagPara));	//Xoa toan bo vung nho ETHRxMagPara va ep kieu sang Pointer char
	memset((char *)&ETHTxMagPara,0,sizeof(ETHTxMagPara));	//Xoa toan bo vung nho ETHTxMagPara va ep kieu sang Pointer char
	
	for(i=0; i<RX_QUEUE_NUM; i++) 
	{
		ETHRxMagPara.RxBufAddr[i] = (UINT32)(&MACRxBuf[i][0]); // Gan dia chi MACRxBuf vao Pointer ETHRxMagPara.RxBufAddr[i] va ep kieu sang UINT32
	}
	
	for(i=0; i<TX_QUEUE_NUM; i++)
	{
		ETHTxMagPara.TxBufAddr[i] = (UINT32)(&MACTxBuf[i][0]);	// Gan dia chi MACTxBuf vao Pointer ETHRxMagPara.TxBufAddr[i] va ep kieu sang UINT32
	}

	//发送接收使能关闭
	ETHRxMagPara.RecvEn = 0;  //Ngat Nhan  
	ETHTxMagPara.SendEn = 0;	// Ngat truyen


	//获取唯一MAC地址
	GetMACAddress(myCh579MAC);
//	myCh579MAC[5]=myCh579MAC[0]+myCh579MAC[1]+myCh579MAC[2]+myCh579MAC[3]+myCh579MAC[4]+myCh579MAC[5];
//	myCh579MAC[0]=0x00; //确保MAC地址第1个字节为0
	

	printf("ETHParaInitX mac: %x:%x:%x:%x:%x:%x\r\n",myCh579MAC[5], myCh579MAC[4],myCh579MAC[3],myCh579MAC[2],myCh579MAC[1],myCh579MAC[0]);
	

}





//网络接口初始化
void ETHInitX(void)
{

	R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;		// 0x57 Default
	R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;		// 0xA8 Default
	R8_SLP_CLK_OFF1 &= ~RB_SLP_CLK_ETH;	      //Sleep Clock Control Register 1 (ETH controller Clock Soure)
	R8_SLP_POWER_CTRL &= ~RB_SLP_ETH_PWR_DN;  //Peripheral Sleep Power Control Register (Ethernet transceiver ETH PHY power down enable) 
	R8_SAFE_ACCESS_SIG = 0;	// WO, safe accessing sign register, must write SAFE_ACCESS_SIG1 then SAFE_ACCESS_SIG2 to enter safe accessing mode
	R8_ETH_EIE = 0;  // OFF All Interrupt Ethenet
	// ON All Interrupt Ethenet
	R8_ETH_EIE |= RB_ETH_EIE_INTIE |  //Ethernet interrupt enable
                  RB_ETH_EIE_RXIE  |	  //Reception completion interrupt enable
	              RB_ETH_EIE_LINKIE|  //Link change interrupt enable
	              RB_ETH_EIE_TXIE  |  // Transmission completion interrupt enable
	              RB_ETH_EIE_TXERIE|  // Transmission error interrupt enable
	              RB_ETH_EIE_RXERIE;   //Reception error interrupt enable
	
	
	R8_ETH_EIE |= RB_ETH_EIE_R_EN50;     //Built-in 50 ohm impedance matching resistor enable

	//Interrupt Flag Register
	R8_ETH_EIR = 0xff;               			//ON ALL Flag Interrupt(Reception completion flag, Link change flag, Transmission completion flag, Transmission error flag, Reception error flag)
	R8_ETH_ESTAT |= RB_ETH_ESTAT_INT | RB_ETH_ESTAT_BUFER;  //Status Register (Interrupted, Buffer error)
	
	//Transceiver Control Register 
	R8_ETH_ECON1 |= (RB_ETH_ECON1_TXRST|RB_ETH_ECON1_RXRST); //RESET Transmission module va Reception module 
	R8_ETH_ECON1 &= ~(RB_ETH_ECON1_TXRST|RB_ETH_ECON1_RXRST); // Xoa bo lenh RESET

	//Chua biet thanh ghi 0x40009018 la gi
	R8_ETH_ERXFCON = 0;  //不过滤 接收所有包
	
	//Mac Layer Flow Control Register
	R8_ETH_MACON1 |= RB_ETH_MACON1_MARXEN;       //MAC layer receive enable: Cho phep nhan du lieu qua Dia chi MAC

	// Mac Layer Packet Control Register: Cau hinh che do cho ETH
	R8_ETH_MACON2 &= ~RB_ETH_MACON2_PADCFG;	//(0xE0): Full duplex,  Allow reception,Hardware is filled with CRC, Short packets are not filled, Special 4 bytes do not participate in the CRC check
		
	R8_ETH_MACON2 |= PADCFG_AUTO_3;               //Short packets are not filled
	R8_ETH_MACON2 |= RB_ETH_MACON2_TXCRCEN;          //Transmit and add CRC Control: Hardware is filled with CRC
	R8_ETH_MACON2 &= ~RB_ETH_MACON2_HFRMEN;          //Allow receiving jumbo frames: Allow reception
	R8_ETH_MACON2 |= RB_ETH_MACON2_FULDPX;    //Mode of Ethernet communication: Full duplex.

	R16_ETH_MAMXFL = MAC_MAX_LEN;            //Maximum Receive Packet Length Register: 1064 Do dai goi tin MAX
	
	//Dia chi MAC
	R8_ETH_MAADR1 = myCh579MAC[5];          // 0x04
	R8_ETH_MAADR2 = myCh579MAC[4];		// 0x03
	R8_ETH_MAADR3 = myCh579MAC[3];		// 0x02
	R8_ETH_MAADR4 = myCh579MAC[2];		// 0xE4
	R8_ETH_MAADR5 = myCh579MAC[1];		// 0xC2
	R8_ETH_MAADR6 = myCh579MAC[0];		// 0x84
	
	//Peripheral analog pin configuration register Chan vat ly TX-, TX+, RX-, RX+
	R16_PIN_ANALOG_IE |= RB_PIN_ETH_IE;	//ETH Pin Enabled: PB12-15 are the ETH communication pins. If LCD is not enabled entirely, RB_PIN_SEG12_15_IE can be set to 1 to turn off the digital input, which can save power.
	
	R16_ETH_ERXST = (UINT16)ETHRxMagPara.RxBufAddr[ETHRxMagPara.RecvIndex];     //Receive DMA Buffer Start Address Register:Dia chi bat dau Bo dem nhan

	R8_ETH_ECON1 |= RB_ETH_ECON1_RXEN;                                      //Reception enable control: Cho phep nhaan goi tin

	NVIC_EnableIRQ(ETH_IRQn);  // Cho phep ngat ETH

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
static void ETH_IRQ_ERR_Deal(UINT8 err_sta) // Canh bao ERROR
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
	
	if(!ETHTxMagPara.SendEn)  //Neu SendEn = 0 thi tra ve 0xFF
	{
		return 0xff;
	}

	
	if(ETHTxMagPara.TxQueueCnt>=TX_QUEUE_NUM) // Neu bo dem Hang doi >=2 thi tra ve 0xFE
	{
		//发送队列满
		return 0xfe;
	}


	//标记发送缓冲区中有数据及数据长度
	ETHTxMagPara.TxBufStau[ETHTxMagPara.WriteIndex] = 1; // Gan .TxBufStau[0] = 1
	ETHTxMagPara.TxBufLen[ETHTxMagPara.WriteIndex] = len; // Gan .TxBufLen[0] = send_len 
	p_tx_buf = (UINT8 *)ETHTxMagPara.TxBufAddr[ETHTxMagPara.WriteIndex]; //Do .TxBufAddr kieu UINT32 ma p_tx_buf kieu Pointer UINT8 nen phai ep kieu sang UINT8
	

	memcpy(p_tx_buf, p_data, len);  // Sao chep pSendBuf sang p_tx_buf

	
	ETHTxMagPara.WriteIndex++; // Tang ETHTxMagPara.WriteIndex len 1 don vi
	if(ETHTxMagPara.WriteIndex>=TX_QUEUE_NUM) // Neu ETHTxMagPara.WriteIndex >= 2 thi  ETHTxMagPara.WriteIndex = 0
	{
		ETHTxMagPara.WriteIndex = 0;
	}

	                                            
	ETHTxMagPara.TxQueueCnt++; // Tang ETHTxMagPara.TxQueueCnt len 1 don vi 
	if(ETHTxMagPara.TxQueueCnt==1)   // Neu ETHTxMagPara.TxQueueCnt==1 thi
	{	//Transmit Length Register
		R16_ETH_ETXLN = ETHTxMagPara.TxBufLen[ETHTxMagPara.SendIndex];          //Gan Do dai GUI R16_ETH_ETXLN = ETHTxMagPara.TxBufLen[0]
		// Transmit DMA Buffer Start Address Register
		R16_ETH_ETXST = (UINT16)ETHTxMagPara.TxBufAddr[ETHTxMagPara.SendIndex]; //Gan Dia chi bat dau bo dem DMA     R16_ETH_ETXST = (UINT16)ETHTxMagPara.TxBufAddr[0]     
		ETHTxMagPara.TxBufStau[ETHTxMagPara.SendIndex] = 0;                     //Gan trang thai bo dem gui = 0 .TxBufStau[0] = 0
		// Transceiver Control Register; Transmission starts and is automatically reset to zero after the transmission is completed = 1: Start transmission
		R8_ETH_ECON1 |= RB_ETH_ECON1_TXRTS;                                     //Dieu khien GUI goi tin
		ETHTxMagPara.SendIndex++;	//Tang ETHTxMagPara.SendIndex++ len 1 don vi
		if(ETHTxMagPara.SendIndex>=TX_QUEUE_NUM) ETHTxMagPara.SendIndex = 0; //Neu ETHTxMagPara.SendIndex>= 2 thi ETHTxMagPara.SendIndex = 0
		ETHTxMagPara.TxQueueCnt--;		// Giam ETHTxMagPara.TxQueueCnt xuong 1 don vi
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
		//接收完成中断	

		//硬件自动覆盖
		rec_len = R16_ETH_ERXLN;
		if(ETHRxMagPara.RemainCout<(RX_QUEUE_NUM-1))  
		{
			 //有队列空闲，保留最后一个随便覆盖，也使用这个队列里的数据
			ETHRxMagPara.RxBufStau[ETHRxMagPara.RecvIndex] = 1;
			ETHRxMagPara.RxBufLen[ETHRxMagPara.RecvIndex] = rec_len;
			ETHRxMagPara.RemainCout++;
			ETHRxMagPara.RecvIndex++;
			if(ETHRxMagPara.RecvIndex>=RX_QUEUE_NUM) ETHRxMagPara.RecvIndex = 0;
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
	
		}

		//printf("recv finish:%d\r\n", rec_len);
		//lwip_pkt_handle();
		R8_ETH_EIR = RB_ETH_EIR_RXIF; //清除接收完成中断标志
	}	
	if(eth_irq_flag&RB_ETH_EIR_LINKIF)                                         
	{
		 //Link 变化中断
		phy_reg = ReadPHYRegX(PHY_BMSR);                                         //读取PHY状态寄存器
		if(phy_reg&0x04) 
		{
			//printf("link connect ok....\r\n");
			ETHTxMagPara.SendEn = 1;
			
		}
		else 
		{
			//printf("link disconnect ok....\r\n");
			ETHTxMagPara.SendEn = 0;
		}

		R8_ETH_EIR = RB_ETH_EIR_LINKIF;
	}
	if(eth_irq_flag&RB_ETH_EIR_TXIF)                                           
	{
		//printf("send finish interrupt\r\n");
		 //发送完成中断
		if(ETHTxMagPara.TxQueueCnt)   //队列里还有数据包没有发送
		{
			R16_ETH_ETXLN = ETHTxMagPara.TxBufLen[ETHTxMagPara.SendIndex];
			R16_ETH_ETXST = ETHTxMagPara.TxBufAddr[ETHTxMagPara.SendIndex];
			ETHTxMagPara.TxBufStau[ETHTxMagPara.SendIndex] = 0;  //空闲
			
			R8_ETH_ECON1 |= RB_ETH_ECON1_TXRTS;        //开始发送
			ETHTxMagPara.SendIndex++;
			if(ETHTxMagPara.SendIndex>=TX_QUEUE_NUM) ETHTxMagPara.SendIndex = 0;
			ETHTxMagPara.TxQueueCnt--;
		}                                                  //发送处理
		R8_ETH_EIR = RB_ETH_EIR_TXIF;
	}

	
	if(eth_irq_flag&(RB_ETH_EIR_TXERIF|RB_ETH_EIR_RXERIF))   //收发出错中断
	{
		eth_irq_status = R8_ETH_ESTAT;
		ETH_IRQ_ERR_Deal(eth_irq_status);  //出错处理
		//清除错误中断标志
		R8_ETH_EIR = (RB_ETH_EIR_TXERIF|RB_ETH_EIR_RXERIF);

		//发送接收出错后，重启CH579 
		NVIC_SystemReset();
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
//		printf("link connect ok....\r\n");
		phy_flag = 1;
		return 1;
	}
	//printf("link disconnect ok....\r\n");
	return 0;
	
}

