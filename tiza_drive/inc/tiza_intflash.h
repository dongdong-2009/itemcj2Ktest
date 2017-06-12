#ifndef __TIZA_FLASH_H__
#define __TIZA_FLASH_H__

#include "tiza_include.h"

	#ifdef TIZA_INTFLASH_GLOBAL
		#define TIZA_EXTERN_INTFLASH
	#else
		#define TIZA_EXTERN_INTFLASH extern
	#endif

    /***************** �ڲ�FLASH���򻮷� ********************/
    /********************************************************/
    /*                                                      */
    /*           BOOT����      ����0 (16K)                  */
    /*                                                      */
    /********************************************************/
    /*                                                      */
    /*           �û��Զ���1   ����1 - ����4 (112K)         */
    /*                                                      */
    /********************************************************/
    /*                                                      */
    /*           APP����       ����5 - ����6 (256K)         */
    /*                                                      */
    /********************************************************/
    /*                                                      */
    /*           ��������洢  ����7 - ����8 (256K)         */
    /*                                                      */
    /********************************************************/
    /*                                                      */
    /*           �û��Զ���2   ����9 - ����11 (384K)        */
    /*                                                      */
    /********************************************************/

/**************************** ע�� *****************************/
/*                                                             */	
/*  ��Ϊ�ڲ�FLASH�������Ƚϴ�RAMС���洢����ʱ���ʺ�������,  */
/*  ���Դ洢����ʱ��ѡ����ʴ�С�������洢���ݣ���д������ʱ�� */
/*  �Ὣ��ַ��������������Ȼ��д���µ�����                     */ 
/***************************************************************/
   

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
#define STM32_FLASH_SIZE 0x100000 		//STM32 FLASH�Ĵ�С1M
 

//FLASH ��������ʼ��ַ
#define ADDR_FLASH_SECTOR_0     ((uint32)0x08000000) 	//����0��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((uint32)0x08004000) 	//����1��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((uint32)0x08008000) 	//����2��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((uint32)0x0800C000) 	//����3��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((uint32)0x08010000) 	//����4��ʼ��ַ, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((uint32)0x08020000) 	//����5��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((uint32)0x08040000) 	//����6��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((uint32)0x08060000) 	//����7��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((uint32)0x08080000) 	//����8��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((uint32)0x080A0000) 	//����9��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((uint32)0x080C0000) 	//����10��ʼ��ַ,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((uint32)0x080E0000) 	//����11��ʼ��ַ,128 Kbytes  



#define BOOT_START_ADD									ADDR_FLASH_SECTOR_0
#define BOOT_RELAY_PARA_ADD							ADDR_FLASH_SECTOR_1
#define APP_START_ADD5									ADDR_FLASH_SECTOR_5
#define APP_START_ADD6									ADDR_FLASH_SECTOR_6		
#define APP_START_ADD7									ADDR_FLASH_SECTOR_7		
#define INTFlASH_STORE_START_ADDR				ADDR_FLASH_SECTOR_8
#define INTFLASH_STORE_END_ADDR					ADDR_FLASH_SECTOR_11 - 0x100		//ADDR_FLASH_SECTOR_10����
#define INTFLASH_STORE_VERSION_ADD			ADDR_FLASH_SECTOR_11						//�³�����Ϣ��
#define INTFLASH_APP_VERSION_ADD				ADDR_FLASH_SECTOR_8 - 0X100			//ADDR_FLASH_SECTOR_7����


///ƫ��flash�󣬲���ʱ�䳤��ע��ι��
//#define WRITEFLASH_FEEDWTD					0X20000				//д����ʱ��д����ʱι��







/* FLASH_Sector_0 - FLASH_Sector_11 �� stm32f4xx_flash.h ���ж��壬�����ţ����ݸ����� FlashEraseSector */
#if 0
TIZA_EXTERN_INTFLASH uint8 CpuFlashEraseSector(uint16 Sector);
#else
TIZA_EXTERN_INTFLASH uint8 CpuFlashEraseSector(uint32 Add);
#endif
TIZA_EXTERN_INTFLASH uint8 CpuFlashWrite(uint32 Addr, uint8 *Data, uint16 Len);
TIZA_EXTERN_INTFLASH uint8 CpuFlashRead(uint32 Addr, uint8 *Data, uint16 Len);
						   
#endif

















