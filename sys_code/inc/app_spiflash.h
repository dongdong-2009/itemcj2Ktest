#ifndef __APP_SPIFLASH_H
#define __APP_SPIFLASH_H

#include <stdint.h>

#ifdef APP_SPIFLASH_GLOBAL
	#define EXTERN_APP_SPIFLASH
#else
	#define EXTERN_APP_SPIFLASH extern
#endif

	
	#define APP_EF_SECTOR_SIZE							EXTEFLASH_SECTOR_SIZE														//Ƭ��flash��ÿ�δ�С
	#define APP_EF_EVERYLSNAL_SIZE0					507//795																							//ÿ��ä����С<1k����ʵʱ����
	#define APP_EF_EVERYLSNAL_SIZE1					512//(APP_EF_EVERYLSNAL_SIZE0 + 5)										//ÿ��ä����С<1k����ʵʱ����+�洢����
	#define APP_EF_EVERYSECTOR_NUM					8//(APP_EF_SECTOR_SIZE/APP_EF_EVERYLSNAL_SIZE1)		//ÿ�δ��ä������
	#define APP_EF_LSNALSECTOR_NUM					2047																						//���ڴ�ä���Ķ���(0~2046)
	//Ƭ��flash�ɴ�ä�����������	(��Ч������ APP_EF_LSNAL_NUM-APP_EF_EVERYSECTOR_NUM)
	#define APP_EF_LSNAL_NUM								16376  //APP_EF_EVERYSECTOR_NUM * APP_EF_LSNALSECTOR_NUM				
	
	
	#define APP_EF_LSNAL_START_ADD					0x00000000												//ä����ʼ�洢��ַ
	#define APP_EF_LSNAL_END_ADD						0X007FEFFF												//ä���洢������ַ
	#define APP_EF_LSNAL_INDEXADD						0X007FF000												//ҳ���±걣����ʼ��ַ
	#define APP_EF_LSNAL_INDEXSECTOR				2047															//ҳ���±걣����ʼ�κ�
	
	
	//---------------------------------------------------------------------------//
		
	#define SYS_LSNAL_SPIMAXINDEX			(APP_EF_LSNALSECTOR_NUM*APP_EF_EVERYSECTOR_NUM)				

	
	
	
	typedef struct
	{
		uint8  data[APP_EF_EVERYLSNAL_SIZE1];						//һ��ä�����ݴ�С
		uint32 headindex;							  								//����(ÿ��16�Σ�ÿ��4Kbety)
		uint32 tailindex;																//����(ÿ��16�Σ�ÿ��4Kbety)
	}SYS_LSNAL_STRUCT;
		
	
	EXTERN_APP_SPIFLASH SYS_LSNAL_STRUCT g_syslsnal_struct;
	
	
	
	EXTERN_APP_SPIFLASH void ExtFlashLsnalHeadTailSave(void);	
	EXTERN_APP_SPIFLASH void PutIntoExtFlashLsnal(uint8 data[],uint16 len,uint8 cmd);			     
	EXTERN_APP_SPIFLASH void Read_Lsnal_Index(void);


#endif
