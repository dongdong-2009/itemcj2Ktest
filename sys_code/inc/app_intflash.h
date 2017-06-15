#ifndef __APP_INTFLASH_H 
#define __APP_INTFLASH_H

#include "stdint.h"

	#ifdef APP_INTFLASH_GLOBAL
		#define APP_INTFLSAH_EXTERN
	#else
		#define APP_INTFLSAH_EXTERN extern
	#endif
		
	
    /***************** �ڲ�FLASH���򻮷� ********************/
    /********************************************************/
    /*                                                      */
    /*           BOOT����      ����0 (16K)                  */
    /*                                                      */
    /********************************************************/
    /*                                                      */
    /*           �û��Զ���1   ����1 - ����4 (112K)         */
    //����1 (16K)                														*/
    //����2 (16K)                														*/
    //����3 (16K)     ���������� 														*/
    //����4 (64K)                														*/
    /*                                                      */
    /********************************************************/
    /*                                                      */
    /*           APP����       ����5 - ����7 (384K)         */
    /*                                                      */
    /********************************************************/
    /*                                                      */
    /*           ��������洢  ����8 - ����10 (384K)        */
    /*                                                      */
    /********************************************************/
    /*                                                      */
    /*           �û��Զ���2   ����11 - ����11 (128K)       */
    /*                                                      */
    /********************************************************/

		//����3 (16K)  
		//VIN					0 		----  31   	 (20)
		//ProPara			32 		----  127    (90)
		//oldgps			128 	----  143    (11)
		
		
	#define APP_SAVEINT_INDEXSTART  				ADDR_FLASH_SECTOR_11//ADDR_FLASH_SECTOR_3
	
	#define FTP_BLIND_FLASH_START 	 INTFlASH_STORE_START_ADDR
//	#define FTP_BLIND_FLASH_END 		(0x8040000 - 0x2000)

	#define APP_SAVEINT_VIN_INDEXSTART  		APP_SAVEINT_INDEXSTART
	#define APP_SAVEINT_VIN_INDEXEND    		(APP_SAVEINT_VIN_INDEXSTART + 20)		
	#define APP_SAVEINT_PARA_INDEXSTART  		(APP_SAVEINT_INDEXSTART + 32)
	#define APP_SAVEINT_PARA_INDEXEND    		(APP_SAVEINT_PARA_INDEXSTART + 90)
	#define APP_SAVEINT_OLDGPS_INDEXSTART  	(APP_SAVEINT_INDEXSTART + 128)
	#define APP_SAVEINT_OLDGPS_INDEXEND    	(APP_SAVEINT_OLDGPS_INDEXSTART + 11)


	APP_INTFLSAH_EXTERN void ProWrite_IntFlash_VIN(uint8 str[], uint16 len);
	APP_INTFLSAH_EXTERN void ProRead_IntFlash_VIN(void);
	APP_INTFLSAH_EXTERN void ProWrite_SysPara(void);
	APP_INTFLSAH_EXTERN void ProRead_SysPara(void);
	APP_INTFLSAH_EXTERN	void ReadWriteOldVaildPostion(uint8 rw);
		
#endif













