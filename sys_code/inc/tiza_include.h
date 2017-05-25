#ifndef __INCLUDE_H
#define __INCLUDE_H   

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_conf.h"
	
	#define false FALSE	
	#define true TRUE
	#define NULL 0
//	
////	#define ENABLE_WATCHDOG
////	#define SYS_PARA_INIT
//	#define PPP_DEBUG
//	#define PRO_DEBUG
//	#define GPS_DEBUG
//	#define GPRS_DEBUG
//	#define CAN_ERROR_ALARM
	

	#define LEN_12 12
	#define LEN_16 16
	#define LEN_20 20
	#define LEN_32 32
	#define LEN_64 64

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
	#include "tiza_driver.h"
//	#include "tiza_l218.h"
//	#include "tiza_business.h"
//	#include "tiza_spi_flash.h"	
//	#include "tiza_protocol.h"
//	#include "tiza_can.h"
//	#include "tiza_syspara.h"
//	#include "tiza_systask.h"
//	#include "tiza_flash.h"
//	#include "tiza_ftp.h"
//	
//	
//	
//	#include "test.h"



	#define false FALSE	
	#define true TRUE
	#define NULL 0
	
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
	
	
	
	#define QLJ_DEBUG 1
	
	
#endif

