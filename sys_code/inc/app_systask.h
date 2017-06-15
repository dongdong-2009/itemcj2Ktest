#ifndef __APP_SYSTASK_H
#define __APP_SYSTASK_H

#include "stdint.h"

	#ifdef APP_SYSTASK_GLOBAL
		#define APP_EXTERN_SYSTASK
	#else
		#define APP_EXTERN_SYSTASK extern
	#endif

	
	#define BEFORE_ALARM_MAXINDEX				32	//40
	#define SYS_ALARM_MAXINDEX					80
	#define ALARM_PAGE_SIZE 						0x200						//512byte		
	#define SYS_ALARM_START_ADDR				FLASH_ALARM_START_ADDR	
	#define SYS_ALARM_END_ADDR					FLASH_ALARM_END_ADDR		//��SYS_ALARM_MAXINDEX��Ӧ
	#define SYS_ALARM_INDEXPAGE					FLASH_ALARM_INDEXPAGE		
	#define SYS_SAVEPARA_INDEXPAGE			FLASH_SAVEPARA_INDEXPAGE	
	#define SYS_SAVEVAILDPOS_INDEXPAGE	FLASH_SAVEVAILDPOS_INDEXPAGE
	#define SYS_SAVEVIN_INDEXPAGE				FLASH_SAVEVIN_INDEXPAGE
	
	#define SYS_SAVEALARM_NUMB					3				//ϵͳδ���ֱ���ʱ ����INTFLAH�ļ�¼����  
	#define SYS_HAVEALARM_COUNT					35//40			//���ֱ�����󵹼�ʱʱ�� s
	typedef struct
	{
		uint16 beforeheadindex;							  //����ǰ�Ĵ洢�±�� RAM
		uint16 beforetailindex;							  //����ǰ�Ĵ洢�±��
		
		uint32 afterheadindex;							  //����(ÿ��16�Σ�ÿ��4Kbety)
		uint32 aftertailindex;								//����(ÿ��16�Σ�ÿ��4Kbety)
	}SYS_ALARM_STRUCT;
		
			
	typedef struct
	{					
		uint16 task1_feeddog_count;							///Uart3_task
		uint16 task2_feeddog_count;							///L218_task
		uint16 task3_feeddog_count;							///Period_task
		uint16 task4_feeddog_count;							///Test_task
		uint8  need_check_time_flag;						///��ҪУ��ϵͳ����ʱ��     01��GPS   02��ƽ̨  03����ʱ����Чʱ 00��Ч
		uint8  have_sysAlarm_flag;							///ϵͳ���ֱ���             01�б���	02������ޱ���ä�� 03�ޱ���ä��	00�ޱ���
		uint8  stop_sysAlarm_flag;							///�ڷ��ͱ������ݹ����� ����ȡ����־  00��ʼ 01����ȡ��
		uint8  send_sysAlarm_numb;							///ϵͳ���ֱ���ʱ ��������  
		PRO_TIME_UNION send_sysAlarm_time[2];		///ϵͳ���ֱ���ʱ ����ʹ��ʱ��
//		uint8  save_sysAlarm_numb;						///ϵͳδ���ֱ���ʱ ����INTFLAH�ļ�¼����  
		uint8  have_sysAlarm_count;							///ϵͳ���ֱ�������ʱ    

		union08 tasks_initOK_flag;					///�������ʼ��OK��־  bit0 ����1 ... bit7 ����8
		uint8  main_power_statu;						///����Դ״̬     01�ϵ�1��  02�ϵ�  03���ϵ�
//		uint8  assist_gps_flag;							///������λ��־		02��ʼ     00֮ǰ��λ��Ч   01֮ǰ��λ��Ч
		
		uint8  ProgramUpgrade_flag;					///�̼�������־		00��ʼ		 01��Ҫ��������    02ִ�й̼�����   03дBOOT��־  04
		uint8  GPRSreconnect2plat;					///ģ��Ͽ�������������������		00��ʼ 01��Ҫ�Ͽ� 02��Ҫ���� 03�ȴ�����OK 04����OK 05����ģ��
		
		uint8  ReceTCPClosed_flag;					///�յ�TCP�Ͽ���Ϣ(����),		FF��Ч  ���������(���������λ��������շ�����Ϊ������)
		uint16 TCP_Disconnect_count;				///�����������涨ʱ����û�������ϣ�����ģ��  FFFF��Ч
	}SYS_MISC_RUN_STRUCT;///ϵͳ���в���

	
	APP_EXTERN_SYSTASK SYS_ALARM_STRUCT 				g_sysalarm_struct;
	APP_EXTERN_SYSTASK SYS_MISC_RUN_STRUCT 		g_sysmiscrun_struct;		
	
	APP_EXTERN_SYSTASK	uint8 g_BeforeAlarmData[BEFORE_ALARM_MAXINDEX][APP_EF_EVERYLSNAL_SIZE1];	
			
			
	APP_EXTERN_SYSTASK void SetGPRSNetPara(void);
	APP_EXTERN_SYSTASK void AlarmHaveJudge(uint8 flag);
	APP_EXTERN_SYSTASK void ReadyBeforeUpgrade(void);
	
	APP_EXTERN_SYSTASK void GetCalendarTime(uint8 date_time[],uint8 flag);
	
	//------------------------------------------------------------------------------------------------------//
	
	#define SYSM_CANRX_TIMEOUT   60		//60������(1s���þ�60s)	
	typedef struct{//ϵͳ����鳬ʱ��ʱ
		uint16 canrx_count;									///can�����ղ������ݼ���
		
//		uint16 SavePeri_count;							///���ڱ����ʱ
		uint16 NorUpPeri_count;							///���������ϴ���Ϣ��ʱ
//		uint16 AlrUpPeri_count;							///�����ϴ������ʱ
//		uint16 TAckTim_count;								///�ն�Ӧ��ʱʱ��					FF��Ч		
		uint16 PAckTim_count;								///ƽ̨Ӧ��ʱʱ��					FF��Ч
		uint16 NLogTim_count;								///���ε�¼ʧ�ܺ����µ�¼ʱ����	
		uint8  upheart_count;								///ϵͳ�ϴ�������ʱ
	}SYSM_TIMEOUT_STRUCT;
	
	#define SYSM_ON							1
	#define SYSM_OFF						0
	typedef struct{//ϵͳ������Ƿ����� ����
		uint8 canrx_switch;										//�����ж�
		uint8 GPRSPacketTx_switch;						//GPRS����Ӧ��ʽ���� �� ��������ڷ���
		uint8 GPRSPeriodTx_switch;						//GPRS��ҪӦ��ʽ����
		uint8 ADC_switch;											//ADCת���ж�
		uint8 extflash_w_switch;							//Ƭ��flash��д
		uint8 flash_w_switch;									//Ƭ��flash��д
	}SYSM_ON_OFF_STRUCT;
	
	APP_EXTERN_SYSTASK SYSM_TIMEOUT_STRUCT g_sysm_timeout_struct;
	APP_EXTERN_SYSTASK SYSM_ON_OFF_STRUCT 	g_sysm_on_off_struct;
	
	
	APP_EXTERN_SYSTASK uint16 adc_result[2];
	
	
	APP_EXTERN_SYSTASK void SysPara_Init(void);
	
#endif
