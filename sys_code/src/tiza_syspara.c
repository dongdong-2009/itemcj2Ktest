#define SYS_PARA_STRUCT_GLOBAL

#include "tiza_include.h"

void SysPara_Init(void)
{
	//======================================//
	g_sysm_timeout_struct.SavePeri_count 	= 0;							///���ڱ����ʱ
	g_sysm_timeout_struct.NorUpPeri_count = 0;							///���������ϴ���Ϣ��ʱ
	g_sysm_timeout_struct.AlrUpPeri_count	= 0;							///�����ϴ������ʱ
	g_sysm_timeout_struct.upheart_count		= 0;							///ϵͳ�ϴ�������ʱ
//	sysm_timeout_struct.TAckTim_count		= 0;						///�ն�Ӧ��ʱʱ��		
	g_sysm_timeout_struct.PAckTim_count		= 0xFF;						///ƽ̨Ӧ��ʱʱ��		
	
	//======================================//
	g_sysm_on_off_struct.canrx_switch 				= SYSM_ON;
	g_sysm_on_off_struct.GPRSPacketTx_switch	= SYSM_ON;	
	g_sysm_on_off_struct.GPRSPeriodTx_switch 	= SYSM_ON;
	g_sysm_on_off_struct.ADC_switch 					= SYSM_ON;
	g_sysm_on_off_struct.extflash_w_switch		= SYSM_ON;
	g_sysm_on_off_struct.flash_w_switch				= SYSM_ON;
	
	//======================================//
	g_sysmiscrun_struct.have_sysAlarm_flag 	 = 0;						///ϵͳ������־ ͬʱsend_sysAlarm_count=0
	g_sysmiscrun_struct.stop_sysAlarm_flag 	 = 0;						//�ڷ��ͱ������ݹ����� ����ȡ����־  00��ʼ 01����ȡ��
	g_sysmiscrun_struct.need_check_time_flag = 3;//1;						///��ҪУ��ϵͳ����ʱ��     01��GPS   02��ƽ̨  00��Ч
	g_sysmiscrun_struct.have_sysAlarm_count  = 0;						///ϵͳ���ֱ�������ʱ
//	g_sysmiscrun_struct.save_sysAlarm_numb   = SYS_SAVEALARM_NUMB;		///δ���ֱ���ʱ ����INTFLAH�ļ�¼����  
	g_sysmiscrun_struct.assist_gps_flag      = 2;						///������λ��־		02��ʼ     00֮ǰ��λ��Ч   01֮ǰ��λ��Ч
	g_sysmiscrun_struct.GPRSreconnect2plat   = 0;						///ģ��Ͽ�������������������		00��ʼ 01��Ҫ�Ͽ� 02��Ҫ���� 03�ȴ�����OK 04����OK 05����ģ��
	g_sysmiscrun_struct.TCP_Disconnect_count = 0xFFFF;			///�����������涨ʱ����û�������ϣ�����ģ��  FFFF��Ч

}



void Tz1000_Init(void)
{
	ProPara_Init();
	SysPara_Init();
}




