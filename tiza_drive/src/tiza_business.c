#define BUSINESS_GLOABL
#define BUSINESS_DEBUG
#define L218_GLOBAL
#define GPRS_GLOBAL

#include "tiza_include.h"



/***************************?��?��?��??????o����y***********
//?��?��?��?????��o����y
******************************************************/
void BGprsResetPara(void)
{

//����״̬��
	g_gprs_ctr_struct.business = GPRS_INIT;
	g_gprs_ctr_struct.ope = AT_INDEX;
	
	
}

/******************************************************
//ģ��ִ���������
�����
******************************************************/
void ExecuteModuleTask(void)
{
	if(g_gprs_ctr_struct.business == GPRS_INIT)
	{//-���ϵͳ��ʼ��
		
	}
	else if(g_gprs_ctr_struct.business)
	{//-�����ض�����ģ����
		
	}
	else if(g_gprs_ctr_struct.business)
	{
		
	}
	else
	{//-δ֪״̬����
		
	}
}

