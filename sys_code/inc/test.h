#ifndef __TEST_H
#define __TEST_H

#include "stdint.h"

	#ifdef TEST_STRUCT_GLOBAL
		#define EXTERN_TEST
	#else
		#define EXTERN_TEST extern
	#endif
	
	
	
	
EXTERN_TEST uint8 g_EnableCANAlarm;		//ʹ��CAN ���Բ�����ȡ������, 00��ʹ��   01ʹ��
EXTERN_TEST uint8 g_EnableHeart;			//ʹ���������Ͳ���ʵʱ���ݷ���Ӱ��  00��ʵʱӰ��  01����  
	
	

#endif
