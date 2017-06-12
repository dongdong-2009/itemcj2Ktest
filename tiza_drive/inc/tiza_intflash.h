#ifndef __TIZA_FLASH_H__
#define __TIZA_FLASH_H__

#include "tiza_include.h"

	#ifdef TIZA_INTFLASH_GLOBAL
		#define TIZA_EXTERN_INTFLASH
	#else
		#define TIZA_EXTERN_INTFLASH extern
	#endif

    /***************** 内部FLASH区域划分 ********************/
    /********************************************************/
    /*                                                      */
    /*           BOOT程序      扇区0 (16K)                  */
    /*                                                      */
    /********************************************************/
    /*                                                      */
    /*           用户自定义1   扇区1 - 扇区4 (112K)         */
    /*                                                      */
    /********************************************************/
    /*                                                      */
    /*           APP程序       扇区5 - 扇区6 (256K)         */
    /*                                                      */
    /********************************************************/
    /*                                                      */
    /*           升级程序存储  扇区7 - 扇区8 (256K)         */
    /*                                                      */
    /********************************************************/
    /*                                                      */
    /*           用户自定义2   扇区9 - 扇区11 (384K)        */
    /*                                                      */
    /********************************************************/

/**************************** 注意 *****************************/
/*                                                             */	
/*  因为内部FLASH的扇区比较大，RAM小，存储数据时不适合作缓存,  */
/*  所以存储数据时，选择合适大小的扇区存储数据，在写入数据时， */
/*  会将地址所在扇区擦除，然后写入新的数据                     */ 
/***************************************************************/
   

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
#define STM32_FLASH_SIZE 0x100000 		//STM32 FLASH的大小1M
 

//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((uint32)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((uint32)0x08004000) 	//扇区1起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((uint32)0x08008000) 	//扇区2起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((uint32)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((uint32)0x08010000) 	//扇区4起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((uint32)0x08020000) 	//扇区5起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((uint32)0x08040000) 	//扇区6起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((uint32)0x08060000) 	//扇区7起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((uint32)0x08080000) 	//扇区8起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((uint32)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((uint32)0x080C0000) 	//扇区10起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((uint32)0x080E0000) 	//扇区11起始地址,128 Kbytes  



#define BOOT_START_ADD									ADDR_FLASH_SECTOR_0
#define BOOT_RELAY_PARA_ADD							ADDR_FLASH_SECTOR_1
#define APP_START_ADD5									ADDR_FLASH_SECTOR_5
#define APP_START_ADD6									ADDR_FLASH_SECTOR_6		
#define APP_START_ADD7									ADDR_FLASH_SECTOR_7		
#define INTFlASH_STORE_START_ADDR				ADDR_FLASH_SECTOR_8
#define INTFLASH_STORE_END_ADDR					ADDR_FLASH_SECTOR_11 - 0x100		//ADDR_FLASH_SECTOR_10结束
#define INTFLASH_STORE_VERSION_ADD			ADDR_FLASH_SECTOR_11						//新程序信息区
#define INTFLASH_APP_VERSION_ADD				ADDR_FLASH_SECTOR_8 - 0X100			//ADDR_FLASH_SECTOR_7结束


///偏内flash大，擦除时间长，注意喂狗
//#define WRITEFLASH_FEEDWTD					0X20000				//写过程时，写多少时喂狗







/* FLASH_Sector_0 - FLASH_Sector_11 在 stm32f4xx_flash.h 中有定义，扇区号，传递给函数 FlashEraseSector */
#if 0
TIZA_EXTERN_INTFLASH uint8 CpuFlashEraseSector(uint16 Sector);
#else
TIZA_EXTERN_INTFLASH uint8 CpuFlashEraseSector(uint32 Add);
#endif
TIZA_EXTERN_INTFLASH uint8 CpuFlashWrite(uint32 Addr, uint8 *Data, uint16 Len);
TIZA_EXTERN_INTFLASH uint8 CpuFlashRead(uint32 Addr, uint8 *Data, uint16 Len);
						   
#endif

















