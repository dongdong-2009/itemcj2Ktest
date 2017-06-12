#ifndef __TIZA_CAN_H
#define __TIZA_CAN_H
	
	#ifdef TIZA_CAN_GLOBAL
		#define TIZA_CAN_EXTERN
	#else
		#define TIZA_CAN_EXTERN extern
	#endif
	
	#include "stm32f4xx.h"
	
	///使能 CAN中断
	#define CAN1_NEED_INTER			1
	#define CAN2_NEED_INTER			1	
	#define CAN1_NEED_FILTER		0//1
	#define CAN2_NEED_FILTER		1	
	
	///波特率
	#define CAN_BPS_250K				250000
	#define CAN_BPS_500K				500000
	#define CAN_BPS_1M					1000000
	///CAN 帧类型    标准帧、扩展帧
	#define STANDARD_CANID			1				
	#define EXTENDED_CANID			2
	///CAN 接收数据超时
	#define CAN1_RX_TIMEOUT  		120
	#define CAN2_RX_TIMEOUT  		120
	
	typedef struct 
	{
		uint8  can1_inited;						//can1初始化状态
		uint8  can2_inited;						//can2初始化状态
		uint16 can1_rx_timeout;				//can1接收数据超时计数
		uint16 can2_rx_timeout;				//can2接收数据超时计数

	}CANS_BASIC_STRUCT;

	TIZA_CAN_EXTERN CANS_BASIC_STRUCT g_cans_bssic_struct;
	

//=====================================================================================//
	
	#if CAN1_NEED_FILTER == 1
		///滤波器总条数 、 共享滤波器条数 、 独立滤波器条数   *设置需都为偶数
		///需对应修改 1.App_Can1Init()独立滤波器设置   2.g_app_can1id_struct赋值   
		#define CAN1_FILTER_ALL_NUM				28
		#define CAN1_FILTER_SHARE_NUM			20		//从0开始
		#define CAN1_FILTER_INDEP_NUM			CAN1_FILTER_ALL_NUM-CAN1_FILTER_SHARE_NUM
		#define CAN1_INDEP_INDEX_START		CAN1_FILTER_SHARE_NUM

		#define CAN1_FILTER_TIMEOUT				100 	//0~250s  CANID超时未收到数据

		typedef struct 
		{
			uint8  free_num;																//共享滤波器空闲个数（ID收到过数据就置空闲）
			uint8  canid_index[CAN1_FILTER_SHARE_NUM];			//应用层CANID索引   					0xFF无效
			uint8  count_flag[CAN1_FILTER_SHARE_NUM];				//共享滤波器空闲标志与计时    0x00~0xFA计时 0xFE空闲 0xFF无效  
			uint32 regcanid[CAN1_FILTER_SHARE_NUM];					//设置上CANID （转换过的寄存器的值）
		}CAN1_FILTER_SW_STRUCT;
		
		TIZA_CAN_EXTERN CAN1_FILTER_SW_STRUCT g_can1_filter_sw_struct;
	#endif
	
	///APP_CAN1ID_IDEP_NUM 不得大于 CAN1_FILTER_INDEP_NUM ，否则有CANID收不到数据
	#define APP_CAN1ID_ALL_NUM				52																			//应用层CANID总数 0~250
		
	#if CAN1_NEED_FILTER == 1	
		#define APP_CAN1ID_SHARE_NUM			44																			//应用层共享CANID数量	
		#define APP_CAN1ID_IDEP_NUM				APP_CAN1ID_ALL_NUM-APP_CAN1ID_SHARE_NUM	//应用层独立CANID数量  
		#define APP_INDEP_INDEX_START			APP_CAN1ID_SHARE_NUM
	#endif
		
	typedef struct 
	{
		uint8  canid_index;															//循环设置共享到滤波器中，指示当前设置的CANID索引   	0x00初始 <APP_CAN1ID_SHARE_NUM
		uint8  Rec_flag[APP_CAN1ID_ALL_NUM];						//应用层CANDID有数据更新	0xFF无效   0x01有更新
		uint8  Rec_candata[APP_CAN1ID_ALL_NUM][8];			//应用层CANDID对应数据
		uint32 Rec_canid[APP_CAN1ID_ALL_NUM];						//应用层CANDID  				0xFFFFFFFF无效
	}APP_CAN1ID_STRUCT;

	extern APP_CAN1ID_STRUCT 		g_app_can1id_struct;
	extern uint8 g_Tmp_VIN_codes[3][8];
	
	
	
	
	TIZA_CAN_EXTERN uint8 Can1OpenPort(uint32 bps);
	TIZA_CAN_EXTERN void Can1ClosePort(void);
	TIZA_CAN_EXTERN void Can1FilterClear(void);
	TIZA_CAN_EXTERN void Can1FilterSet(uint8 filter_index,uint32 can_id_1,uint32 can_id_2, uint8 mode);
	TIZA_CAN_EXTERN void CAN1_Switch_FilterSetInit(void);
	TIZA_CAN_EXTERN void CAN1_Switch_FilterSet(void);
	TIZA_CAN_EXTERN void Can1SendMessage(uint32 can_id, uint8 id_type, uint8* data, uint8 len);
	TIZA_CAN_EXTERN void CAN1_Switch_Filter_Overtime(uint8 second);
	
	
	
	TIZA_CAN_EXTERN uint8 Can2OpenPort(uint32 bps);
	TIZA_CAN_EXTERN void Can2ClosePort(void);
	TIZA_CAN_EXTERN void Can2FilterClear(void);
	TIZA_CAN_EXTERN void Can2FilterSet(uint8 filter_index,uint32 can_id_1,uint32 can_id_2, uint8 mode);
	TIZA_CAN_EXTERN void CAN2_Switch_FilterSetInit(void);
	TIZA_CAN_EXTERN void CAN2_Switch_FilterSet(void);
	TIZA_CAN_EXTERN void Can2SendMessage(uint32 can_id, uint8 id_type, uint8* data, uint8 len);
	TIZA_CAN_EXTERN void CAN2_Switch_Filter_Overtime(uint8 second);
	
#endif

