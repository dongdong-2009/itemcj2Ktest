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
	#define SYS_ALARM_END_ADDR					FLASH_ALARM_END_ADDR		//与SYS_ALARM_MAXINDEX对应
	#define SYS_ALARM_INDEXPAGE					FLASH_ALARM_INDEXPAGE		
	#define SYS_SAVEPARA_INDEXPAGE			FLASH_SAVEPARA_INDEXPAGE	
	#define SYS_SAVEVAILDPOS_INDEXPAGE	FLASH_SAVEVAILDPOS_INDEXPAGE
	#define SYS_SAVEVIN_INDEXPAGE				FLASH_SAVEVIN_INDEXPAGE
	
	#define SYS_SAVEALARM_NUMB					3				//系统未出现报警时 保留INTFLAH的记录条数  
	#define SYS_HAVEALARM_COUNT					35//40			//出现报警最大倒计时时间 s
	typedef struct
	{
		uint16 beforeheadindex;							  //报警前的存储下标号 RAM
		uint16 beforetailindex;							  //报警前的存储下标号
		
		uint32 afterheadindex;							  //包号(每块16段，每段4Kbety)
		uint32 aftertailindex;								//包号(每块16段，每段4Kbety)
	}SYS_ALARM_STRUCT;
		
			
	typedef struct
	{					
		uint16 task1_feeddog_count;							///Uart3_task
		uint16 task2_feeddog_count;							///L218_task
		uint16 task3_feeddog_count;							///Period_task
		uint16 task4_feeddog_count;							///Test_task
		uint8  need_check_time_flag;						///需要校验系统日期时间     01从GPS   02从平台  03重启时间无效时 00无效
		uint8  have_sysAlarm_flag;							///系统出现报警             01有报警	02检测有无报警盲区 03无报警盲区	00无报警
		uint8  stop_sysAlarm_flag;							///在发送报警数据过程中 报警取消标志  00初始 01报警取消
		uint8  send_sysAlarm_numb;							///系统出现报警时 发送条数  
		PRO_TIME_UNION send_sysAlarm_time[2];		///系统出现报警时 发送使用时间
//		uint8  save_sysAlarm_numb;						///系统未出现报警时 保留INTFLAH的记录条数  
		uint8  have_sysAlarm_count;							///系统出现报警倒计时    

		union08 tasks_initOK_flag;					///各任务初始化OK标志  bit0 任务1 ... bit7 任务8
		uint8  main_power_statu;						///主电源状态     01上电1次  02断电  03再上电
//		uint8  assist_gps_flag;							///辅助定位标志		02初始     00之前定位有效   01之前定位无效
		
		uint8  ProgramUpgrade_flag;					///固件升级标志		00初始		 01有要更新升级    02执行固件下载   03写BOOT标志  04
		uint8  GPRSreconnect2plat;					///模块断开服务器连接再重连接		00初始 01需要断开 02需要重连 03等待重连OK 04重连OK 05重启模块
		
		uint8  ReceTCPClosed_flag;					///收到TCP断开信息(主动),		FF无效  其他表计数(若连续几次还能正常收发，认为是误判)
		uint16 TCP_Disconnect_count;				///断网计数，规定时间内没有连接上，重启模块  FFFF无效
	}SYS_MISC_RUN_STRUCT;///系统运行参数

	
	APP_EXTERN_SYSTASK SYS_ALARM_STRUCT 				g_sysalarm_struct;
	APP_EXTERN_SYSTASK SYS_MISC_RUN_STRUCT 		g_sysmiscrun_struct;		
	
	APP_EXTERN_SYSTASK	uint8 g_BeforeAlarmData[BEFORE_ALARM_MAXINDEX][APP_EF_EVERYLSNAL_SIZE1];	
			
			
	APP_EXTERN_SYSTASK void SetGPRSNetPara(void);
	APP_EXTERN_SYSTASK void AlarmHaveJudge(uint8 flag);
	APP_EXTERN_SYSTASK void ReadyBeforeUpgrade(void);
	
	APP_EXTERN_SYSTASK void GetCalendarTime(uint8 date_time[],uint8 flag);
	
	//------------------------------------------------------------------------------------------------------//
	
	#define SYSM_CANRX_TIMEOUT   60		//60个计数(1s调用就60s)	
	typedef struct{//系统程序块超时计时
		uint16 canrx_count;									///can长期收不到数据计数
		
//		uint16 SavePeri_count;							///周期保存计时
		uint16 NorUpPeri_count;							///正常周期上传信息计时
//		uint16 AlrUpPeri_count;							///报警上传间隔计时
//		uint16 TAckTim_count;								///终端应答超时时间					FF无效		
		uint16 PAckTim_count;								///平台应答超时时间					FF无效
		uint16 NLogTim_count;								///三次登录失败后重新登录时间间隔	
		uint8  upheart_count;								///系统上传心跳计时
	}SYSM_TIMEOUT_STRUCT;
	
	#define SYSM_ON							1
	#define SYSM_OFF						0
	typedef struct{//系统程序块是否启用 开关
		uint8 canrx_switch;										//接收中断
		uint8 GPRSPacketTx_switch;						//GPRS无需应答式发送 或 打包给周期发送
		uint8 GPRSPeriodTx_switch;						//GPRS需要应答式发送
		uint8 ADC_switch;											//ADC转换中断
		uint8 extflash_w_switch;							//片外flash擦写
		uint8 flash_w_switch;									//片内flash擦写
	}SYSM_ON_OFF_STRUCT;
	
	APP_EXTERN_SYSTASK SYSM_TIMEOUT_STRUCT g_sysm_timeout_struct;
	APP_EXTERN_SYSTASK SYSM_ON_OFF_STRUCT 	g_sysm_on_off_struct;
	
	
	APP_EXTERN_SYSTASK uint16 adc_result[2];
	
	
	APP_EXTERN_SYSTASK void SysPara_Init(void);
	
#endif
