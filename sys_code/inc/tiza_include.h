#ifndef __INCLUDE_H
#define __INCLUDE_H   

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_conf.h"
	
	enum{
		FALSE = 0,
		TRUE = !FALSE
	};
	
	#define false FALSE	
	#define true 	TRUE
	#define NULL 	0

	#define LEN_12 12
	#define LEN_16 16
	#define LEN_20 20
	#define LEN_32 32
	#define LEN_64 64
	
	#define VALID_VAL_2A					0x2A
	#define INVALID_VAL_FF				0xFF

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include "ucos_ii.h"
#include "os_cpu.h"
#include "os_cfg.h"

#include "sys.h"
#include "delay.h"
	
//	#include "tiza_selfdef.h"
//	#include "tiza_l218.h"
//	#include "tiza_business.h"
//	#include "tiza_spi_flash.h"	
//	
//	#include "tiza_syspara.h"
//	#include "tiza_systask.h"
//	#include "tiza_ftp.h"
//	
//	
//	

	///TIZA
	#include "tiza_driver.h"
	#include "tiza_adc.h"
	#include "tiza_spi.h"
	#include "tiza_can.h"
	#include "tiza_intflash.h"
	#include "tiza_RS232.h"
	#include "tiza_RS485.h"
	#include "tiza_sd.h"
	#include "tiza_rtc.h"
	#include "tiza_l218dri.h"
	#include "tiza_l218opt.h"
	
	#include "tiza_selfdef.h"
	#include "tiza_debug.h"
	#include "tiza_ftp.h"

	///SYS

	#include "app_spiflash.h"	
	#include "app_can.h"
	#include "app_intflash.h"	
	#include "app_l218.h"
	#include "app_protocol.h"
	#include "app_systask.h"
	#include "app_test.h"
	
/************************************************************************
*                  DEFINE DATA STRUCTURE
*************************************************************************/
	typedef union{
		INT16U	word;
		struct{
			INT8U  low;
			INT8U  high;
		} bytes;
	}WORD_UNION;
	
	#define RES_FALSE		0
	#define RES_TRUE		1
	#define RES_WAIT		2
	#define RES_BREAK		3
	#define RES_DELETE	4
	#define RES_NOFTPUP	5
	
	
//	#define INVALID_VAL_55  0x55
//	#define VALID_VAL_AA 		0xaa
	
	#define LEN_12 12
	#define LEN_16 16
	#define LEN_20 20
	#define LEN_32 32
	#define LEN_64 64
	
	
	
	#define QLJ_DEBUG        0//    1//     
	
	//对外申明信号量
	extern OS_EVENT *Dprint_Semp;    //定义调试打印信号量
#endif

