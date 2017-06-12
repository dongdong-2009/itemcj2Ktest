#ifndef __TIZA_CAN_H
#define __TIZA_CAN_H
	
	#ifdef TIZA_CAN_GLOBAL
		#define TIZA_CAN_EXTERN
	#else
		#define TIZA_CAN_EXTERN extern
	#endif
	
	#include "stm32f4xx.h"
	
	///ʹ�� CAN�ж�
	#define CAN1_NEED_INTER			1
	#define CAN2_NEED_INTER			1	
	#define CAN1_NEED_FILTER		0//1
	#define CAN2_NEED_FILTER		1	
	
	///������
	#define CAN_BPS_250K				250000
	#define CAN_BPS_500K				500000
	#define CAN_BPS_1M					1000000
	///CAN ֡����    ��׼֡����չ֡
	#define STANDARD_CANID			1				
	#define EXTENDED_CANID			2
	///CAN �������ݳ�ʱ
	#define CAN1_RX_TIMEOUT  		120
	#define CAN2_RX_TIMEOUT  		120
	
	typedef struct 
	{
		uint8  can1_inited;						//can1��ʼ��״̬
		uint8  can2_inited;						//can2��ʼ��״̬
		uint16 can1_rx_timeout;				//can1�������ݳ�ʱ����
		uint16 can2_rx_timeout;				//can2�������ݳ�ʱ����

	}CANS_BASIC_STRUCT;

	TIZA_CAN_EXTERN CANS_BASIC_STRUCT g_cans_bssic_struct;
	

//=====================================================================================//
	
	#if CAN1_NEED_FILTER == 1
		///�˲��������� �� �����˲������� �� �����˲�������   *�����趼Ϊż��
		///���Ӧ�޸� 1.App_Can1Init()�����˲�������   2.g_app_can1id_struct��ֵ   
		#define CAN1_FILTER_ALL_NUM				28
		#define CAN1_FILTER_SHARE_NUM			20		//��0��ʼ
		#define CAN1_FILTER_INDEP_NUM			CAN1_FILTER_ALL_NUM-CAN1_FILTER_SHARE_NUM
		#define CAN1_INDEP_INDEX_START		CAN1_FILTER_SHARE_NUM

		#define CAN1_FILTER_TIMEOUT				100 	//0~250s  CANID��ʱδ�յ�����

		typedef struct 
		{
			uint8  free_num;																//�����˲������и�����ID�յ������ݾ��ÿ��У�
			uint8  canid_index[CAN1_FILTER_SHARE_NUM];			//Ӧ�ò�CANID����   					0xFF��Ч
			uint8  count_flag[CAN1_FILTER_SHARE_NUM];				//�����˲������б�־���ʱ    0x00~0xFA��ʱ 0xFE���� 0xFF��Ч  
			uint32 regcanid[CAN1_FILTER_SHARE_NUM];					//������CANID ��ת�����ļĴ�����ֵ��
		}CAN1_FILTER_SW_STRUCT;
		
		TIZA_CAN_EXTERN CAN1_FILTER_SW_STRUCT g_can1_filter_sw_struct;
	#endif
	
	///APP_CAN1ID_IDEP_NUM ���ô��� CAN1_FILTER_INDEP_NUM ��������CANID�ղ�������
	#define APP_CAN1ID_ALL_NUM				52																			//Ӧ�ò�CANID���� 0~250
		
	#if CAN1_NEED_FILTER == 1	
		#define APP_CAN1ID_SHARE_NUM			44																			//Ӧ�ò㹲��CANID����	
		#define APP_CAN1ID_IDEP_NUM				APP_CAN1ID_ALL_NUM-APP_CAN1ID_SHARE_NUM	//Ӧ�ò����CANID����  
		#define APP_INDEP_INDEX_START			APP_CAN1ID_SHARE_NUM
	#endif
		
	typedef struct 
	{
		uint8  canid_index;															//ѭ�����ù����˲����У�ָʾ��ǰ���õ�CANID����   	0x00��ʼ <APP_CAN1ID_SHARE_NUM
		uint8  Rec_flag[APP_CAN1ID_ALL_NUM];						//Ӧ�ò�CANDID�����ݸ���	0xFF��Ч   0x01�и���
		uint8  Rec_candata[APP_CAN1ID_ALL_NUM][8];			//Ӧ�ò�CANDID��Ӧ����
		uint32 Rec_canid[APP_CAN1ID_ALL_NUM];						//Ӧ�ò�CANDID  				0xFFFFFFFF��Ч
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

