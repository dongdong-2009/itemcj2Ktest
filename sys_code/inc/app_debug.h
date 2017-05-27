#ifndef __APP_DEBUG_H
#define __APP_DEBUG_H

#include "tiza_include.h"

	#ifdef APP_DEBUG_GLOBAL
		#define EXTERN_APP_DEBUG
	#else
		#define EXTERN_APP_DEBUG extern
	#endif

	EXTERN_APP_DEBUG void DebugRecData(INT8U* Dat,INT16U Len);

	

	
#endif

