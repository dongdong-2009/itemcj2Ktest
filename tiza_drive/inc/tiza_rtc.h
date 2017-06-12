#ifndef __TIZA_RTC_H
#define __TIZA_RTC_H	 

	#ifdef TIZA_RTC_GLOBAL
		#define TIZA_EXTERN_RTC
	#else
		#define TIZA_EXTERN_RTC extern
	#endif
		
	#include "stm32f4xx.h"

	typedef struct
	{
		uint8 year;
		uint8 month;
		uint8 day;
		uint8 hour;
		uint8 minute;
		uint8 second;
	}RTC_ST;

TIZA_EXTERN_RTC uint8 RtcInit(void);						//RTC初始化
TIZA_EXTERN_RTC uint8 RtcGetTime(RTC_ST *time);
TIZA_EXTERN_RTC uint8 RtcSetTime(RTC_ST *time);
TIZA_EXTERN_RTC uint8 RtcSetAlarm(uint16 Sec);		// 需要先调用RtcInit()初始化RTC

#endif

