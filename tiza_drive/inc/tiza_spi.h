#ifndef __TIZA_SPI_H
#define __TIZA_SPI_H

	#ifdef TIZA_SPI_GLOBAL
		#define TIZA_SPI_EXTERN
	#else
		#define TIZA_SPI_EXTERN extern
	#endif
	
	#include "stm32f4xx.h"
	
	#define SPI1_USE_DMA2 	0										//�Ƿ�ʹ��DMA 0��ʹ�� 1ʹ��  δ���Թ�
		
	#define TIZA_SPI1_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
	#define TIZA_SPI1_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)	
		
	#if SPI1_USE_DMA2 == 0
		#define TIZA_SPI1_RX_BUFF_LEN			1024
		#define TIZA_SPI1_TX_BUFF_LEN			1024

		TIZA_SPI_EXTERN u8 g_Spi1Dma2_Rx_buff[TIZA_SPI1_RX_BUFF_LEN];
		TIZA_SPI_EXTERN u8 g_Spi1Dma2_Tx_buff[TIZA_SPI1_TX_BUFF_LEN];
	#endif
	

		
//========================================================================//
	#define SPI_CS_ENABLE 	TIZA_SPI1_CS_LOW() 
	#define SPI_CS_DISABLE 	TIZA_SPI1_CS_HIGH()
	
	#define EXTEFLASH_CAPCITY				8388608				// (8*1024*1024)
	#define EXTEFLASH_SECTOR_SIZE		4096					// һ��SECTOR��СΪ4096
	
	TIZA_SPI_EXTERN uint8 ExteFlashInit(void);
	TIZA_SPI_EXTERN uint8 ExteFlashWrite(uint32 WriteAddr,uint8* pBuffer,uint16 NumByteToWrite);	//��������д�����Ƿ����
	TIZA_SPI_EXTERN uint8 ExteFlashRead(uint32 ReadAddr, uint8* pBuffer, uint16 NumByteToRead);   //��ȡflash
	TIZA_SPI_EXTERN uint8 ExteFlashEraseChip(void);    	  				//��Ƭ�������ȴ�ʱ�䳤
	TIZA_SPI_EXTERN uint8 ExteFlashEraseSector(uint16 SectorNum);	//��������������һ������������ʱ��:150ms

	TIZA_SPI_EXTERN uint8 ExteFlashPowerDown(void);        				//�������ģʽ
	TIZA_SPI_EXTERN uint8 ExteFlashWakeUp(void);										//����
		

#endif
