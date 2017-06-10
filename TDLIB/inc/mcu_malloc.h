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
//定义三个内存池
#define SRAMIN	 0		//内部内存池
#define SRAMEX   1		//外部内存池
#define SRAMCCM  2		//CCM内存池(此部分SRAM仅仅CPU可以访问!!!)


#define SRAMBANK 	3	//定义支持的SRAM块数.	  

//mem1内存参数设定.mem1完全处于内部SRAM里面.
#define MEM1_BLOCK_SIZE			32  	  													//内存块大小为32字节
#define MEM1_MAX_SIZE           10*1024                                                     //最大管理内存 100K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE                               //内存表大小

//mem2内存参数设定.mem2的内存池处于外部SRAM里面
#define MEM2_BLOCK_SIZE			32  	  													//内存块大小为32字节
#define MEM2_MAX_SIZE           32                                                          //最大管理内存960K
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE                               //内存表大小
		 
//mem3内存参数设定.mem3处于CCM,用于管理CCM(特别注意,这部分SRAM,仅CPU可以访问!!)
#define MEM3_BLOCK_SIZE			32  	  													//内存块大小为32字节
#define MEM3_MAX_SIZE           60 *1   													//最大管理内存60K
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE                               //内存表大小


/* Exported types ------------------------------------------------------------*/
//内存管理控制器
struct _m_mallco_dev
{
	void (*init)(u8);                   //初始化
	u8 (*perused)(u8);                  //内存使用率
	u8 	*membase[SRAMBANK];				//内存池 管理SRAMBANK个区域的内存
	u16 *memmap[SRAMBANK]; 				//内存管理状态表
	u8  memrdy[SRAMBANK]; 				//内存管理是否就绪
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
