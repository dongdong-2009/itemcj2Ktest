#define BUSINESS_GLOABL
#define BUSINESS_DEBUG
#define L218_GLOBAL
#define GPRS_GLOBAL

#include "tiza_include.h"



/***************************?￡?é?′??????oˉêy***********
//?￡?é?′?????ˉoˉêy
******************************************************/
void BGprsResetPara(void)
{

//设置状态机
	g_gprs_ctr_struct.business = GPRS_INIT;
	g_gprs_ctr_struct.ope = AT_INDEX;
	
	
}

/******************************************************
//模块执行任务调用
输出：
******************************************************/
void ExecuteModuleTask(void)
{
	if(g_gprs_ctr_struct.business == GPRS_INIT)
	{//-完成系统初始化
		
	}
	else if(g_gprs_ctr_struct.business)
	{//-处于特定功能模块中
		
	}
	else if(g_gprs_ctr_struct.business)
	{
		
	}
	else
	{//-未知状态处理
		
	}
}

