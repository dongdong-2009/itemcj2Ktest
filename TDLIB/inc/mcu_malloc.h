/**
  ******************************************************************************
  * @file    mcu_malloc.h
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    09-June-2017
  * @brief   This file contains all the functions prototypes for the CRC firmware 
  *          library.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCU_MALLOC_H
#define __MCU_MALLOC_H

	#ifdef MCU_MALLOC_GLOBAL
		#define MCU_MALLOC_EXTERN
	#else
		#define MCU_MALLOC_EXTERN extern
	#endif

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mcu.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */
//���������ڴ��
#define SRAMIN	 0		//�ڲ��ڴ��
#define SRAMEX   1		//�ⲿ�ڴ��
#define SRAMCCM  2		//CCM�ڴ��(�˲���SRAM����CPU���Է���!!!)


#define SRAMBANK 	3	//����֧�ֵ�SRAM����.	  

//mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM����.
#define MEM1_BLOCK_SIZE			32  	  													//�ڴ���СΪ32�ֽ�
#define MEM1_MAX_SIZE           10*1024                                                     //�������ڴ� 100K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE                               //�ڴ���С

//mem2�ڴ�����趨.mem2���ڴ�ش����ⲿSRAM����
#define MEM2_BLOCK_SIZE			32  	  													//�ڴ���СΪ32�ֽ�
#define MEM2_MAX_SIZE           32                                                          //�������ڴ�960K
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE                               //�ڴ���С
		 
//mem3�ڴ�����趨.mem3����CCM,���ڹ���CCM(�ر�ע��,�ⲿ��SRAM,��CPU���Է���!!)
#define MEM3_BLOCK_SIZE			32  	  													//�ڴ���СΪ32�ֽ�
#define MEM3_MAX_SIZE           60 *1   													//�������ڴ�60K
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE                               //�ڴ���С


/* Exported types ------------------------------------------------------------*/
//�ڴ���������
struct _m_mallco_dev
{
	void (*init)(u8);                   //��ʼ��
	u8 (*perused)(u8);                  //�ڴ�ʹ����
	u8 	*membase[SRAMBANK];				//�ڴ�� ����SRAMBANK��������ڴ�
	u16 *memmap[SRAMBANK]; 				//�ڴ����״̬��
	u8  memrdy[SRAMBANK]; 				//�ڴ�����Ƿ����
};
/* Exported constants --------------------------------------------------------*/

/** @defgroup CRC_Exported_Constants
  * @{
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/  
MCU_MALLOC_EXTERN void my_mem_init(u8 memx);
MCU_MALLOC_EXTERN u8 my_mem_perused(u8 memx);

MCU_MALLOC_EXTERN void myfree(u8 memx,void *ptr);
MCU_MALLOC_EXTERN void *mymalloc(u8 memx,u32 size);
MCU_MALLOC_EXTERN void *myrealloc(u8 memx,void *ptr,u32 size);

#ifdef __cplusplus
}
#endif

#endif /* __MCU_MALLOC_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
