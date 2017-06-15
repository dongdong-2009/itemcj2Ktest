
#ifndef __APP_L218_H
#define __APP_L218_H
	
#include <stdint.h>

		#ifdef APP_L218_GLOBAL
			#define APP_L218_EXTERN
		#else
			#define APP_L218_EXTERN extern
		#endif


	#include <stdint.h>

		
	APP_L218_EXTERN void App_Module_Rest(void);
	APP_L218_EXTERN void  App_DisconSer(void);
			
			
#endif

