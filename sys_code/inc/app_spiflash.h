#ifndef __APP_SPIFLASH_H
#define __APP_SPIFLASH_H

#include <stdint.h>

#ifdef APP_SPIFLASH_GLOBAL
	#define EXTERN_APP_SPIFLASH
#else
	#define EXTERN_APP_SPIFLASH extern
#endif

	
	#define APP_EF_SECTOR_SIZE							EXTEFLASH_SECTOR_SIZE														//片外flash，每段大小
	#define APP_EF_EVERYLSNAL_SIZE0					507//795																							//每条盲区大小<1k，纯实时数据
	#define APP_EF_EVERYLSNAL_SIZE1					512//(APP_EF_EVERYLSNAL_SIZE0 + 5)										//每条盲区大小<1k，纯实时数据+存储冗余
	#define APP_EF_EVERYSECTOR_NUM					8//(APP_EF_SECTOR_SIZE/APP_EF_EVERYLSNAL_SIZE1)		//每段存的盲区条数
	#define APP_EF_LSNALSECTOR_NUM					2047																						//用于存盲区的段数(0~2046)
	//片外flash可存盲区最大总条数	(有效总条数 APP_EF_LSNAL_NUM-APP_EF_EVERYSECTOR_NUM)
	#define APP_EF_LSNAL_NUM								16376  //APP_EF_EVERYSECTOR_NUM * APP_EF_LSNALSECTOR_NUM				
	
	
	#define APP_EF_LSNAL_START_ADD					0x00000000												//盲区开始存储地址
	#define APP_EF_LSNAL_END_ADD						0X007FEFFF												//盲区存储结束地址
	#define APP_EF_LSNAL_INDEXADD						0X007FF000												//页码下标保存起始地址
	#define APP_EF_LSNAL_INDEXSECTOR				2047															//页码下标保存起始段号
	
	
	//---------------------------------------------------------------------------//
		
	#define SYS_LSNAL_SPIMAXINDEX			(APP_EF_LSNALSECTOR_NUM*APP_EF_EVERYSECTOR_NUM)				

	
	
	
	typedef struct
	{
		uint8  data[APP_EF_EVERYLSNAL_SIZE1];						//一包盲区数据大小
		uint32 headindex;							  								//包号(每块16段，每段4Kbety)
		uint32 tailindex;																//包号(每块16段，每段4Kbety)
	}SYS_LSNAL_STRUCT;
		
	
	EXTERN_APP_SPIFLASH SYS_LSNAL_STRUCT g_syslsnal_struct;
	
	
	
	EXTERN_APP_SPIFLASH void ExtFlashLsnalHeadTailSave(void);	
	EXTERN_APP_SPIFLASH void PutIntoExtFlashLsnal(uint8 data[],uint16 len,uint8 cmd);			     
	EXTERN_APP_SPIFLASH void Read_Lsnal_Index(void);


#endif
