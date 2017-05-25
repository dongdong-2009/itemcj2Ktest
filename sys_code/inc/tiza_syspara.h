#ifndef __SYS_PARA_H
#define __SYS_PARA_H

#include "stdint.h"

	#ifdef SYS_PARA_STRUCT_GLOBAL
		#define EXTERN_SYS_PARA
	#else
		#define EXTERN_SYS_PARA extern
	#endif
	
	
		
	#define SYSM_CANRX_TIMEOUT   60		//60������(1s���þ�60s)	
	typedef struct{//ϵͳ����鳬ʱ��ʱ
		uint16 canrx_count;									///can�����ղ������ݼ���
		
		uint16 SavePeri_count;							///���ڱ����ʱ
		uint16 NorUpPeri_count;							///���������ϴ���Ϣ��ʱ
		uint16 AlrUpPeri_count;							///�����ϴ������ʱ
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
	
	EXTERN_SYS_PARA SYSM_TIMEOUT_STRUCT g_sysm_timeout_struct;
	EXTERN_SYS_PARA SYSM_ON_OFF_STRUCT 	g_sysm_on_off_struct;
	
	EXTERN_SYS_PARA void Tz1000_Init(void);
	


#endif
