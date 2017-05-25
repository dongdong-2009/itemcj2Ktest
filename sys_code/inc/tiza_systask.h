#ifndef __SYS_TASK_H
#define __SYS_TASK_H

#include "stdint.h"

	#ifdef SYS_TASK_GLOBAL
		#define EXTERN_SYS_TASK
	#else
		#define EXTERN_SYS_TASK extern
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
		uint8  assist_gps_flag;							///������λ��־		02��ʼ     00֮ǰ��λ��Ч   01֮ǰ��λ��Ч
		
		uint8  ProgramUpgrade_flag;					///�̼�������־		00��ʼ		 01��Ҫ��������    02ִ�й̼�����   03дBOOT��־  04
		uint8  GPRSreconnect2plat;					///ģ��Ͽ�������������������		00��ʼ 01��Ҫ�Ͽ� 02��Ҫ���� 03�ȴ�����OK 04����OK 05����ģ��
		
		uint8  ReceTCPClosed_flag;					///�յ�TCP�Ͽ���Ϣ(����),		FF��Ч  ���������(���������λ��������շ�����Ϊ������)
		uint16 TCP_Disconnect_count;				///�����������涨ʱ����û�������ϣ�����ģ��  FFFF��Ч
	}SYS_MISC_RUN_STRUCT;///ϵͳ���в���

	
	EXTERN_SYS_TASK SYS_ALARM_STRUCT 				g_sysalarm_struct;
	EXTERN_SYS_TASK SYS_MISC_RUN_STRUCT 		g_sysmiscrun_struct;		
	
	EXTERN_SYS_TASK	uint8 g_BeforeAlarmData[BEFORE_ALARM_MAXINDEX][512];	
			
			
	EXTERN_SYS_TASK void SetGPRSNetPara(void);
	EXTERN_SYS_TASK void ProPutIntoAlarm(uint8 data[], uint16 len, uint8 cmd);
	EXTERN_SYS_TASK void ReadWriteOldVaildPostion(uint8 rw);
	EXTERN_SYS_TASK void AlarmHaveJudge(uint8 flag);
	EXTERN_SYS_TASK void ReadyBeforeUpgrade(void);
#endif
