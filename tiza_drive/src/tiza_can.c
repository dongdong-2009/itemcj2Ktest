/**
PA11  	RXCAN0
PA12  	TXCAN0

PB12  	RXCAN1
PB13  	TXCAN1

//CAN2 收发待需求明确后 调试
**/
#define TIZA_CAN_GLOBAL
//#define TIZA_CAN_DEBUG

#include "tiza_include.h"



static void Can1RecMessage(uint32 canid, uint8 candata[]);
static void Can2RecMessage(uint32 canid, uint8 candata[]);

/******************************************************
//CANID 转换函数

******************************************************/	
static uint32 CANID_2_RegValue(uint32 canid)
{
	uint32 tmp_u32_val;
	
	tmp_u32_val = canid;
	tmp_u32_val = tmp_u32_val << 3;
	tmp_u32_val |= CAN_Id_Extended;
	
	return tmp_u32_val;
}




///CAN1
/******************************************************
//CAN1 打开函数
//输入：
			bps  真实波特率
//输出：
			0成功 1波特率不对 2初始化失败
******************************************************/	
uint8 Can1OpenPort(uint32 bps)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	RCC_ClocksTypeDef RCC_ClocksStatus;
	NVIC_InitTypeDef NVIC_InitStructure;
	

	if(bps!=CAN_BPS_250K)// && bps!=CAN_BPS_500K && bps!=CAN_BPS_1M)
	{
		return 1;																		//波特率不对
	}

	ON_CAN_PWR();																	//开CAN 电源		
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	//Configure CAN1 pin: TX RX
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP; 							//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//CAN1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1); 		//GPIOA11复用为CAN1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1); 		//GPIOA12复用为CAN1

	RCC_GetClocksFreq(&RCC_ClocksStatus);
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	/// CAN1 cell init
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW	 = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1  = CAN_BS1_7tq;
	CAN_InitStructure.CAN_BS2  = CAN_BS2_6tq;
	CAN_InitStructure.CAN_Prescaler = RCC_ClocksStatus.PCLK1_Frequency/(14*bps);   ///波特率计算，42M/（1+7+6）/12=250k
	

	if(!CAN_Init(CAN1, &CAN_InitStructure))			{			return 2;			}			//CAN1初始化失败
	#if CAN1_NEED_INTER == 1
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);	
	NVIC_InitStructure.NVIC_IRQChannel 										= CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	3;		//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 				=	3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd 								= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	#endif
	
	//Can1FilterClear();
	g_cans_bssic_struct.can1_inited = TRUE;
	
	return 0;
	
}
/******************************************************
//CAN1 关闭函数

******************************************************/	
void Can1ClosePort(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);
	CAN_DeInit(CAN1);	
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP; 							//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	g_cans_bssic_struct.can1_inited = FALSE;

//	if(g_cans_bssic_struct.can1_inited == FALSE && g_cans_bssic_struct.can2_inited == FALSE)
//	{			
//		OFF_CAN_PWR();																//CAN1 CAN2都没有初始化就关闭电源
//	}
	
} 

/******************************************************
//CAN1 清滤波器

******************************************************/	
void Can1FilterClear(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	CAN_FilterInitStructure.CAN_FilterMode 				= CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale 			= CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh 			= 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow 			= 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh 	= 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow 	= 0x0000;
	CAN_FilterInitStructure.CAN_FilterActivation 	= ENABLE;
	
	CAN_FilterInitStructure.CAN_FilterNumber 			= 0;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;   //关联FIFO0
	
	CAN_FilterInit(&CAN_FilterInitStructure);
	
}

/******************************************************
//CAN1 设置滤波器
//输入：	
		filter_index：滤波器组号  0~13
		can_id_1		：本组1号寄存器   列表
		can_id_2		：本组2号寄存器		掩码
******************************************************/	
void Can1FilterSet(uint8 filter_index,uint32 can_id_1,uint32 can_id_2, uint8 mode)
{
	uint32 tmpu32;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	if(filter_index > 13)							{				return;				}				//组号错误

	CAN_FilterInitStructure.CAN_FilterNumber 			= filter_index;	
	CAN_FilterInitStructure.CAN_FilterMode 				= mode;//CAN_FilterMode_IdMask;//CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale				= CAN_FilterScale_32bit;	
	
	tmpu32 = CANID_2_RegValue(can_id_1);
	CAN_FilterInitStructure.CAN_FilterIdHigh 			= (tmpu32 >> 16) & 0xFFFF;			
	CAN_FilterInitStructure.CAN_FilterIdLow 			= tmpu32 & 0xFFFF;
	
	tmpu32 = CANID_2_RegValue(can_id_2);
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh 	= (tmpu32 >> 16) & 0xFFFF;	
	CAN_FilterInitStructure.CAN_FilterMaskIdLow 	= tmpu32 & 0xFFFF;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;	
	
	CAN_FilterInitStructure.CAN_FilterActivation		 = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

}


/******************************************************
//CAN1 发送消息
//输入：
	can_id：	CANID
	id_type:	标准帧or扩展帧  STANDARD_CANID / EXTENDED_CANID
	data：		发送数据
	len:			发送数据长度
******************************************************/		
void Can1SendMessage(uint32 can_id, uint8 id_type, uint8* data, uint8 len)
{
	CanTxMsg TxMessage;

	if(g_cans_bssic_struct.can1_inited == FALSE){				return;				}				//CAN 未初始化
	if(len > 8)																	{				return;				}				//长度错误

	if(id_type == STANDARD_CANID){
		TxMessage.IDE = CAN_ID_STD;
		TxMessage.StdId = can_id;
	}
	else if(id_type == EXTENDED_CANID){
		TxMessage.IDE = CAN_ID_EXT;   
		TxMessage.ExtId = can_id;
	}
	else 																				{				return;				}				//类型错误
	
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = len;
	MemCpy(TxMessage.Data, data, len);
	
	CAN_Transmit(CAN1, &TxMessage);

}

/******************************************************
//CAN1 接收中断

******************************************************/		
void CAN1_RX0_IRQHandler()
{
	CanRxMsg 	can_msg;
	uint32 		tmp_id;
	
	CAN_Receive(CAN1, CAN_FIFO0, &can_msg);

	if(can_msg.IDE == CAN_ID_STD){			//标准帧
		tmp_id = can_msg.StdId;	
	}
	else{																//扩展帧
		tmp_id = can_msg.ExtId;	
	}
	
	Can1RecMessage(tmp_id, (uint8*)can_msg.Data);
}

//------------------------------------------------------------------------------//
///CAN2
/******************************************************
//CAN2 打开函数
//输入：
			bps  真实波特率
//输出：
			0成功 1波特率不对 2初始化失败
******************************************************/	
uint8 Can2OpenPort(uint32 bps)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	RCC_ClocksTypeDef RCC_ClocksStatus;
	NVIC_InitTypeDef NVIC_InitStructure;
	


	if(bps!=CAN_BPS_250K)// && bps!=CAN_BPS_500K && bps!=CAN_BPS_1M)
	{
		return 1;																		//波特率不对
	}

	ON_CAN_PWR();																	//开CAN 电源			
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/// if using can2 only, should enable can1 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2 | RCC_APB1Periph_CAN1, ENABLE);


	///Configure can2 pin: TX RX
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP; 							//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//CAN2对应引脚复用映射
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2); //GPIOA13复用为CAN2
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2); //GPIOA12复用为CAN2

	RCC_GetClocksFreq(&RCC_ClocksStatus);
	CAN_DeInit(CAN2);
	CAN_StructInit(&CAN_InitStructure);
	/// CAN2 cell init
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW	 = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1  = CAN_BS1_7tq;
	CAN_InitStructure.CAN_BS2  = CAN_BS2_6tq;
	CAN_InitStructure.CAN_Prescaler = RCC_ClocksStatus.PCLK1_Frequency/(14*bps);   ///波特率计算，42M/（1+7+6）/12=250k
	
	if(!CAN_Init(CAN2, &CAN_InitStructure))			{			return 2;			}			//CAN1初始化失败

	#if CAN2_NEED_INTER == 1
	CAN_ITConfig(CAN2, CAN_IT_FMP1, ENABLE);	
	NVIC_InitStructure.NVIC_IRQChannel 										= CAN2_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	3;		//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 				=	3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd 								= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	#endif

	//Can2FilterClear();
	g_cans_bssic_struct.can2_inited = TRUE;
	
	return 0;
}
/******************************************************
//CAN2 关闭函数

******************************************************/	
void Can2ClosePort(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	CAN_ITConfig(CAN2, CAN_IT_FMP1, DISABLE);
	CAN_DeInit(CAN2);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP; 							//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	g_cans_bssic_struct.can2_inited = FALSE;

//	if(g_cans_bssic_struct.can1_inited == FALSE && g_cans_bssic_struct.can2_inited == FALSE)
//	{			
//		OFF_CAN_PWR();																//CAN1 CAN2都没有初始化就关闭电源
//	}
	
} 

/******************************************************
//CAN2 清滤波器

******************************************************/	
void Can2FilterClear(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	CAN_FilterInitStructure.CAN_FilterMode 				= CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale 			= CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh 			= 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow 			= 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh 	= 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow 	= 0x0000;
	CAN_FilterInitStructure.CAN_FilterActivation 	= ENABLE;	

	CAN_FilterInitStructure.CAN_FilterNumber 			= 14;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO1;   //关联FIFO1
	
	CAN_FilterInit(&CAN_FilterInitStructure);
	
}

/******************************************************
//CAN2 设置滤波器
//输入：	
		filter_index：滤波器组号  14~27
		can_id_1		：本组1号寄存器
		can_id_2		：本组2号寄存器
******************************************************/	
void Can2FilterSet(uint8 filter_index,uint32 can_id_1,uint32 can_id_2, uint8 mode)
{
	uint32 tmpu32;
	
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	if((filter_index < 14)||(filter_index > 27))	{				return;				}				//组号错误

	CAN_FilterInitStructure.CAN_FilterNumber 			= filter_index;	
	CAN_FilterInitStructure.CAN_FilterMode 				= CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale				= CAN_FilterScale_32bit;	

	tmpu32 = CANID_2_RegValue(can_id_1);
	CAN_FilterInitStructure.CAN_FilterIdHigh 			= (tmpu32 >> 16) & 0xFFFF;			
	CAN_FilterInitStructure.CAN_FilterIdLow 			= tmpu32 & 0xFFFF;
	
	tmpu32 = CANID_2_RegValue(can_id_2);
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh 	= (tmpu32 >> 16) & 0xFFFF;	
	CAN_FilterInitStructure.CAN_FilterMaskIdLow 	= tmpu32 & 0xFFFF;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO1;	

	CAN_FilterInitStructure.CAN_FilterActivation		 = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

}

/******************************************************
//CAN2 发送消息
//输入：
	can_id：	CANID
	id_type:	标准帧or扩展帧  STANDARD_CANID / EXTENDED_CANID
	data：		发送数据
	len:			发送数据长度
******************************************************/		
void Can2SendMessage(uint32 can_id, uint8 id_type, uint8* data, uint8 len)
{
	CanTxMsg TxMessage;

	if(g_cans_bssic_struct.can2_inited == FALSE){				return;				}				//CAN 未初始化
	if(len > 8)																	{				return;				}				//长度错误

	if(id_type == STANDARD_CANID){
		TxMessage.IDE = CAN_ID_STD;
		TxMessage.StdId = can_id;
	}
	else if(id_type == EXTENDED_CANID){
		TxMessage.IDE = CAN_ID_EXT;   
		TxMessage.ExtId = can_id;
	}
	else 																				{				return;				}				//类型错误
	
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = len;
	MemCpy(TxMessage.Data, data, len);
	
	CAN_Transmit(CAN2, &TxMessage);
	
}

/******************************************************
//CAN2 接收中断

******************************************************/		
void CAN2_RX1_IRQHandler()
{
	CanRxMsg	can_msg;
	uint32 		tmp_id;
	
	CAN_Receive(CAN2, CAN_FIFO1, &can_msg);

	if(can_msg.IDE == CAN_ID_STD){			//标准帧
		tmp_id = can_msg.StdId;	
	}
	else{																//扩展帧
		tmp_id = can_msg.ExtId;	
	}
	
	Can2RecMessage(tmp_id, (uint8*)can_msg.Data);
}


#if 0

/******************************************************
//CAN1 清发送缓存
//输出：
			0成功 1失败
******************************************************/		
uint8 Can1SendBufferEmpty(void)
{
	uint8 transmit_mailbox = 0;

	/* Select one empty transmit mailbox */
	if ((CAN1->TSR&CAN_TSR_TME0) == CAN_TSR_TME0){
		transmit_mailbox = 0;
	}
	else if ((CAN1->TSR&CAN_TSR_TME1) == CAN_TSR_TME1){
		transmit_mailbox = 1;
	}
	else if ((CAN1->TSR&CAN_TSR_TME2) == CAN_TSR_TME2){
		transmit_mailbox = 2;
	}
	else{
		transmit_mailbox = CAN_TxStatus_NoMailBox;
	}
		
	if(transmit_mailbox == CAN_TxStatus_NoMailBox){
		return 0;
	}
	else{
		return 1;
	}
}

/******************************************************
//CAN2 清发送缓存
//输出：
			0成功 1失败
******************************************************/		
uint8 Can2SendBufferEmpty(void)
{
	uint8 transmit_mailbox = 0;
	
	/* Select one empty transmit mailbox */
	if ((CAN2->TSR&CAN_TSR_TME0) == CAN_TSR_TME0){
		transmit_mailbox = 0;
	}
	else if ((CAN2->TSR&CAN_TSR_TME1) == CAN_TSR_TME1){
		transmit_mailbox = 1;
	}
	else if ((CAN2->TSR&CAN_TSR_TME2) == CAN_TSR_TME2){
		transmit_mailbox = 2;
	}
	else{
		transmit_mailbox = CAN_TxStatus_NoMailBox;
	}

	if(transmit_mailbox == CAN_TxStatus_NoMailBox){
		return 0;
	}
	else{
		return 1;
	}
	
}
#endif
	

//=====================================================================================//

///CAN1
/******************************************************
//CAN1 中断数据接收函数

******************************************************/		
static void Can1RecMessage(uint32 canid, uint8 candata[])
{
	uint8 i;
	
	#ifdef TIZA_CAN_DEBUG
	DPrint("\r\n Can1RecMessage CANID=%x 	 \n", canid);
	#endif
	
	#if CAN1_NEED_FILTER == 1
	///独立CANID
	for(i=APP_INDEP_INDEX_START; i < APP_CAN1ID_ALL_NUM; i++){	//从应用独立CANID中搜索接受项		
		if(canid == g_app_can1id_struct.Rec_canid[i]){
			memcpy(&g_app_can1id_struct.Rec_candata[i][0], candata, 8);
			g_app_can1id_struct.Rec_flag[i]  			= 1;							//置更新标志
			goto RETURN_LAB;
		}
	}
	///共享CANID
	for(i=0; i < CAN1_FILTER_SHARE_NUM; i++){										//从滤波器buff搜索接受项
		if(canid==g_can1_filter_sw_struct.regcanid[i] && g_can1_filter_sw_struct.count_flag[i]!=0xFE){
			memcpy(g_app_can1id_struct.Rec_candata[g_can1_filter_sw_struct.canid_index[i]], candata, 8);			
			g_app_can1id_struct.Rec_flag[i]  			= 1;							//置更新标志
			g_can1_filter_sw_struct.regcanid[i]   = 0xFFFFFFFF;			//从滤波器中释放该CANID
			g_can1_filter_sw_struct.count_flag[i] = 0xFE;
			g_can1_filter_sw_struct.free_num++;
			goto RETURN_LAB;
		}
	}

	#else 					
	// qlj  优化：使用屏蔽滤波器时候可以按canid分组索引进行搜索，来缩短时间
	for(i=0; i < APP_CAN1ID_ALL_NUM; i++){											//从应用CANID中搜索接受项
		if(canid == g_app_can1id_struct.Rec_canid[i]){
			#if 0                   			//VIN单独处理
			memcpy(g_app_can1id_struct.Rec_candata[i], candata, 8);
			
			#else
				if(canid == CAN1R_SIGLE1){ 		//VIN单独处理
					if(g_app_can1id_struct.Rec_candata[i][0] != candata[0]){
						memcpy(g_app_can1id_struct.Rec_candata[i], candata, 8);
						
						if(g_app_can1id_struct.Rec_candata[i][0] == 1)
							memcpy(g_Tmp_VIN_codes[0], g_app_can1id_struct.Rec_candata[i], 8);
						else if(g_app_can1id_struct.Rec_candata[i][0] == 2)
							memcpy(g_Tmp_VIN_codes[1], g_app_can1id_struct.Rec_candata[i], 8);
						else if(g_app_can1id_struct.Rec_candata[i][0] == 3)
							memcpy(g_Tmp_VIN_codes[2], g_app_can1id_struct.Rec_candata[i], 8);
					}
				}
				else{
					memcpy(g_app_can1id_struct.Rec_candata[i], candata, 8);
				}
			#endif
			
				
			g_app_can1id_struct.Rec_flag[i]  			= 1;							//置更新标志
			goto RETURN_LAB;
		}
	}
	#endif
	
	
	g_cans_bssic_struct.can1_rx_timeout				= 0;							//清CAN1接收超时计数
	
RETURN_LAB:
	return;
}
/******************************************************
//CAN1 动态切换滤波器参数初始化

******************************************************/		
void CAN1_Switch_FilterSetInit(void)
{
	#if CAN1_NEED_FILTER == 1
	uint8 i;
	
	g_can1_filter_sw_struct.free_num = CAN1_FILTER_SHARE_NUM;
	for(i=0; i < CAN1_FILTER_SHARE_NUM; i++){
		g_can1_filter_sw_struct.regcanid[i]		 	= 0;
		g_can1_filter_sw_struct.canid_index[i] 	= 0xFF;
		g_can1_filter_sw_struct.count_flag[i] 	= 0xFE;
	}
	#endif
	
	
}

/******************************************************
//CAN1 动态切换滤波器

******************************************************/		
void CAN1_Switch_FilterSet(void)
{	
	#if CAN1_NEED_FILTER == 1
	uint8 i;
	uint8  group, canid1, canid2;
	
	if(g_can1_filter_sw_struct.free_num) {					//有空闲
		for(i=0; i < CAN1_FILTER_SHARE_NUM; i++){			//搜索空闲项
			if(g_can1_filter_sw_struct.count_flag[i] == 0xFE){
				if(g_app_can1id_struct.Rec_canid[i+g_app_can1id_struct.canid_index] == 0xFFFFFFFF){	//应用层CANID无效跳过
					g_app_can1id_struct.canid_index 				= (g_app_can1id_struct.canid_index+1) % APP_CAN1ID_SHARE_NUM;
					return;			
				}
				
				g_can1_filter_sw_struct.regcanid[i] 		= g_app_can1id_struct.Rec_canid[i+g_app_can1id_struct.canid_index];
				g_can1_filter_sw_struct.canid_index[i] 	= g_app_can1id_struct.canid_index;					//添加应用层CANID索引
				g_app_can1id_struct.canid_index 				= (g_app_can1id_struct.canid_index+1) % APP_CAN1ID_SHARE_NUM;
				group  = i/2;															//绿波寄存器组号 can2要+14偏移
				canid1 = group * 2;
				canid2 = canid1 + 1;
				Can1FilterSet(group, g_can1_filter_sw_struct.regcanid[canid1], g_can1_filter_sw_struct.regcanid[canid2], CAN_FilterMode_IdList); // 
				
				g_can1_filter_sw_struct.count_flag[i] 	= 0x00;																			//置赋值标志并开始计时
				if(g_can1_filter_sw_struct.free_num){
					g_can1_filter_sw_struct.free_num--;
				}
					
			}
		}
		
	}
	#endif
	
}

/******************************************************
//CAN1 动态滤波器超时

******************************************************/		
void CAN1_Switch_Filter_Overtime(uint8 second)
{	
	#if CAN1_NEED_FILTER == 1
	uint8 i;	
	
	for(i=0; i < CAN1_FILTER_SHARE_NUM; i++){
		if(g_can1_filter_sw_struct.count_flag[i] < CAN1_FILTER_TIMEOUT){					//计时
			if((g_can1_filter_sw_struct.count_flag[i]+second) > CAN1_FILTER_TIMEOUT)
				g_can1_filter_sw_struct.count_flag[i] = CAN1_FILTER_TIMEOUT;
			else
				g_can1_filter_sw_struct.count_flag[i] += second;
		}
		else if(g_can1_filter_sw_struct.count_flag[i]<=250 && g_can1_filter_sw_struct.count_flag[i]>=CAN1_FILTER_TIMEOUT){		//超时  从滤波器中释放该CANID
			g_can1_filter_sw_struct.regcanid[i]   = 0xFFFFFFFF;
			g_can1_filter_sw_struct.count_flag[i] = 0xFE;
			g_can1_filter_sw_struct.free_num++;
		}
	
	}

	#endif	
}


//------------------------------------------------------------------------------//
///CAN2
/******************************************************
//CAN2 中断数据接收函数

******************************************************/		
static void Can2RecMessage(uint32 canid, uint8 candata[])
{
	
}








