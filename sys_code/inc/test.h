#ifndef __TEST_H
#define __TEST_H

#include "stdint.h"

	#ifdef TEST_STRUCT_GLOBAL
		#define EXTERN_TEST
	#else
		#define EXTERN_TEST extern
	#endif
	
	
	
	
EXTERN_TEST uint8 g_EnableCANAlarm;		//使能CAN 可以产生和取消报警, 00不使能   01使能
EXTERN_TEST uint8 g_EnableHeart;			//使能心跳发送不受实时数据发送影响  00受实时影响  01独立  
	
	

#endif
