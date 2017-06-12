
#ifndef __TIZA_ADC_H
#define __TIZA_ADC_H

	#ifdef TIZA_ADC_GLOBAL
		#define TIZA_ADC_EXTERN
	#else
		#define TIZA_ADC_EXTERN extern
	#endif
	
	#include "stm32f4xx.h"
	
	

	TIZA_ADC_EXTERN u16 g_Adc1Dma2_buff[16];
	
	TIZA_ADC_EXTERN void Tiza_AdcInit(void);
		
#endif
