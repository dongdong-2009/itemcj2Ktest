/*
			中断优先级配置表


*/

/*
			DMA配置表
	ADC1		DMA1 STREAM4

*/

#ifndef __TIZA_DRIVER_H
#define __TIZA_DRIVER_H

	#ifdef TIZA_DRIVER_GLOBAL
		#define TIZA_DRIVER_EXTERN
	#else
		#define TIZA_DRIVER_EXTERN extern
	#endif
	
	#include "stm32f4xx.h"
	
	///#define LOCAL_DEBUG
	
	#define NVIC_DISABLE()     (__disable_irq())
	#define NVIC_ENABLE()      (__enable_irq())
	
///GPIO START	
	///输入
	#define LOW_STATE			0
	#define HIGH_STATE		1
	
	#define CHECK_RING()		(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_3))///L218振铃监测  0 
	
	#define CHECK_ACC()			(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3))///ACC电源监测  0 ACC有电
	
	#define CHECK_POW()			(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3))///总电源监测  	1 有电

	
	///输出	
	#define ON_GRE_LED()				(GPIO_ResetBits(GPIOB, GPIO_Pin_14))
	#define OFF_GRE_LED()				(GPIO_SetBits(GPIOB, GPIO_Pin_14))
	#define CPL_GRE_LED()				(GPIOB->ODR ^= GPIO_Pin_14)
		
	#define ON_RED_LED()				(GPIO_ResetBits(GPIOB, GPIO_Pin_15))
	#define OFF_RED_LED()				(GPIO_SetBits(GPIOB, GPIO_Pin_15))
	#define CPL_RED_LED()				(GPIOB->ODR ^= GPIO_Pin_15)
	
	#define ON_YELL_LED()				(GPIO_ResetBits(GPIOF, GPIO_Pin_8))
	#define OFF_YELL_LED()			(GPIO_SetBits(GPIOF, GPIO_Pin_8))
	#define CPL_YELL_LED()			(GPIOF->ODR ^= GPIO_Pin_8)

	#define ON_LOCK()						(GPIO_ResetBits(GPIOG, GPIO_Pin_8))
	#define OFF_LOCK()					(GPIO_SetBits(GPIOG, GPIO_Pin_8))
	#define CPL_LOCK()					(GPIOG->ODR ^= GPIO_Pin_8)

	#define ON_SD_PWR()					(GPIO_SetBits(GPIOC, GPIO_Pin_1))
	#define OFF_SD_PWR()				(GPIO_ResetBits(GPIOC, GPIO_Pin_1))
	#define CPL_SD_PWR()				(GPIOC->ODR ^= GPIO_Pin_1)

	#define ON_485232_PWR()			(GPIO_SetBits(GPIOC, GPIO_Pin_2))
	#define OFF_485232_PWR()		(GPIO_ResetBits(GPIOC, GPIO_Pin_2))
	#define CPL_485232_PWR()		(GPIOC->ODR ^= GPIO_Pin_2)

	#define ON_CT5V_PWR()				(GPIO_SetBits(GPIOD, GPIO_Pin_10))
	#define OFF_CT5V_PWR()			(GPIO_ResetBits(GPIOD, GPIO_Pin_10))
	#define CPL_CT5V_PWR()			(GPIOD->ODR ^= GPIO_Pin_10)

	#define HIGH_POWERKEY()			(GPIO_ResetBits(GPIOC, GPIO_Pin_0))
	#define LOW_POWERKEY()			(GPIO_SetBits(GPIOC, GPIO_Pin_0))
	#define CPL_POWERKEY()			(GPIOC->ODR ^= GPIO_Pin_0)

	#define ON_CT4V_PWR()				(GPIO_SetBits(GPIOD, GPIO_Pin_8))
	#define OFF_CT4V_PWR()			(GPIO_ResetBits(GPIOD, GPIO_Pin_8))
	#define CPL_CT4V_PWR()			(GPIOD->ODR ^= GPIO_Pin_8)

	#define ON_CTS_L218()				(GPIO_SetBits(GPIOD, GPIO_Pin_9))
	#define OFF_CTS_L218()			(GPIO_ResetBits(GPIOD, GPIO_Pin_9))
	#define CPL_CTS_L218()			(GPIOD->ODR ^= GPIO_Pin_9)

	#define ON_REST_L218()			(GPIO_SetBits(GPIOF, GPIO_Pin_8))
	#define OFF_REST_L218()			(GPIO_ResetBits(GPIOF, GPIO_Pin_8))
	#define CPL_REST_L218()			(GPIOF->ODR ^= GPIO_Pin_8)

	#define HIGH_DTR_L218()			(GPIO_ResetBits(GPIOF, GPIO_Pin_12))
	#define LOW_DTR_L218()			(GPIO_SetBits(GPIOF, GPIO_Pin_12))
	#define CPL_DTR_L218()			(GPIOF->ODR ^= GPIO_Pin_12)

	#define SET_WDT()						(GPIO_SetBits(GPIOD, GPIO_Pin_14))
	#define REST_WDT()					(GPIO_ResetBits(GPIOD, GPIO_Pin_14))
	#define CPL_WDT()						(GPIOD->ODR ^= GPIO_Pin_14)

///GPIO END
					///L218
	#define ON_GPRS_PWR()			ON_CT4V_PWR()
	#define OFF_GPRS_PWR()		OFF_CT4V_PWR()
	#define LOW_GPRS_IGT()		LOW_POWERKEY()
	#define HIGH_GPRS_IGT()		HIGH_POWERKEY()
	#define LOW_GPRS_DTR()		LOW_DTR_L218()
	#define HIGH_GPRS_DTR()		HIGH_DTR_L218()
					///CAN
	#define ON_CAN_PWR()			ON_CT5V_PWR()
	#define OFF_CAN_PWR()			OFF_CT5V_PWR()
					///EXT DOG
	#define FEED_WDT()       	CPL_WDT()	




//---------------------------------------------------------------------------------//
///
#define OPERATE_IWDG		0X01 
#define OPERATE_EWDG		0X02

#define OPERATE_2WDGS		0X03






#define GPRS_UART_BUF_LEN  		1500







	
	TIZA_DRIVER_EXTERN void Tiza_GpioInit(void);
	TIZA_DRIVER_EXTERN void Tiza_WdtInit(uint8 flag, uint16 ReloadVal);
	TIZA_DRIVER_EXTERN void Tiza_FeedWdg(uint8 flag);
		
	TIZA_DRIVER_EXTERN void SysReset(void);
		
#endif




