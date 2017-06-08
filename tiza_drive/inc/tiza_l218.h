#ifndef __TIZA_L218_H
#define __TIZA_L218_H
	
#include <stdint.h>

		#ifdef L218_GLOBAL
			#define L218_EXTERN
		#else
			#define L218_EXTERN extern
		#endif

///状态枚举
typedef enum
{
  GPRS_NULL = 0,
  GPRS_INIT,
  GPRS_CONNECT_NET,
  GPRS_CONNECT_SERVER,
  GPRS_DISCONNECT_SERVER,
	GPRS_DISCONNECT_NET,
	GPRS_SEND_DATA,
  GPRS_RESET,
	
	GPS_OPEN_NET,
	GPS_CLOSE_NET,
}GPRS_BUSINESS;

typedef struct
{///3G模块控制结构体
		uint8   business;  	    //业务
		uint8   ope;    		    //服务
}GPRS_CTR_STRUCT;

///变量
L218_EXTERN GPRS_CTR_STRUCT g_gprs_ctr_struct;
		
		
#endif













