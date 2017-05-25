/*
			�ж����ȼ����ñ�
			
	00   	��ѹ�ж�
	02		TIME3				
	03		UART3				GPRS

	10    UART2				GPS
	11		UART5				485
	12		CAN
	13		RTC
	13		RTC ALARM
	
	20		DMA      		����adc
	
	33		UART4				�Ǳ�
	33		UART5				���ص��Դ���

*/





#ifndef __DRIVER_H
#define __DRIVER_H

	#ifdef DRIVER_GLOBAL
		#define DRIVER_EXTERN
	#else
		#define DRIVER_EXTERN extern
	#endif
	
	#include "stm32f4xx.h"
	
	///#define LOCAL_DEBUG
	
	#define NVIC_DISABLE()     (__disable_irq())
	#define NVIC_ENABLE()      (__enable_irq())
///����

	
///���	



	#define ON_GRE_LED()			(GPIO_ResetBits(GPIOB, GPIO_Pin_14))
	#define OFF_GRE_LED()			(GPIO_SetBits(GPIOB, GPIO_Pin_14))
	#define CPL_GRE_LED()			(GPIOB->ODR ^= GPIO_Pin_14)
		
	#define ON_RED_LED()			(GPIO_ResetBits(GPIOB, GPIO_Pin_15))
	#define OFF_RED_LED()			(GPIO_SetBits(GPIOB, GPIO_Pin_15))
	#define CPL_RED_LED()			(GPIOB->ODR ^= GPIO_Pin_15)








	
	DRIVER_EXTERN void GpioInit(void);
		
#endif
