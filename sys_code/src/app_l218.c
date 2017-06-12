
#define APP_L218_GLOBAL
#define APP_L218_DEBUG

#include "tiza_include.h"


/******************************************************
//ģ�������λ ��BGprsResetPara�ֳ�
�����
******************************************************/
static uint8 Module_Para_Rest(void)
{
	g_gprs_data_struct.setnetparaok_flag 		= 0;
	SetGPRSNetPara();
//	g_propostion_union.Item.status.byte  		= 0x01;
	g_gps_struct.gpsinform.subitem.statu.byte = 0x01;									///��ʼ����λ��Ч��־
	g_pro_struct.updata_sending 				= 0;
	g_pro_struct.updata_noacksend 			= 0;
	g_pro_struct.try_login_statu  			= 0;
	g_pro_struct.login_center_flag  		= FALSE;
	g_sysm_timeout_struct.NLogTim_count = 0xFFFF;		///���ε�¼ʧ�ܺ����µ�¼ʱ����	
}


/******************************************************
//Ӧ�ò� ��λģ��
�����
******************************************************/
uint8 App_Module_Rest(void)
{
	BusiResetModule();
	
	Module_Para_Rest();
}

/******************************************************
//Ӧ�ò� �Ͽ�ģ�������������
�����
******************************************************/
void App_DisconSer(void)
{
	if(g_gprs_data_struct.MserLinkStatus == GPRS_SERLINK_NO) 
		return;
	g_pro_struct.updata_sending 				= 0;
	g_pro_struct.updata_noacksend 			= 0;
	g_pro_struct.try_login_statu  			= 0;
	g_pro_struct.login_center_flag  		= FALSE;
	g_sysm_timeout_struct.NLogTim_count = 0xFFFF;		///���ε�¼ʧ�ܺ����µ�¼ʱ����	
	
	BusiDisconSer();
	
}


/******************************************************
//ģ������ƽ̨����
//��������Ӻ�3�ε�¼ʧ�ܾ�����ģ�� �����ڷ��Ͳ���
******************************************************/
static void GPRS_Reconnect2Platform(void)
{
/*	switch(g_sysmiscrun_struct.GPRSreconnect2plat){
		case 1:{
			App_DisconSer();
			g_sysmiscrun_struct.GPRSreconnect2plat = 2;
		break;
		}
		case 2:{
			if(g_gprs_data_struct.netLinkStatus == GPRS_NETLINK_NO){
				BusiConSer();
				g_sysmiscrun_struct.GPRSreconnect2plat = 3;
			}
			else if(g_gprs_data_struct.netLinkStatus == GPRS_NETLINK_ERR){
				#ifdef BUSINESS_DEBUG
				LocalUartFixedLenSend((uint8*)"\r\n redisconnect fail", 20);
				#endif
				g_sysmiscrun_struct.GPRSreconnect2plat = 1;
			}
		break;
		}
		case 3:{
			if(g_gprs_data_struct.netLinkStatus == GPRS_NETLINK_OK){
				g_sysmiscrun_struct.GPRSreconnect2plat = 4;
			}
			else if(g_gprs_data_struct.netLinkStatus == GPRS_NETLINK_ERR){
				#ifdef BUSINESS_DEBUG
				LocalUartFixedLenSend((uint8*)"\r\n reconnect fail", 17);
				#endif
				g_sysmiscrun_struct.GPRSreconnect2plat = 2;
			}
		break;
		}
		case 5:{ //����ģ��
			g_sysmiscrun_struct.GPRSreconnect2plat = 0;
			App_Module_Rest();
		break;
		}
		default :{	break;		}
	}*/
}

/******************************************************
//ģ����д�������
�����
******************************************************/
static void Module_Idel_Action(void)
{
	//�õ�¼��־��ֻһ�� ��¼ƽ̨
	if(g_pro_struct.try_login_statu == 0){		
		g_pro_struct.try_login_statu = 1;
	}

	//�ϴ�ACK���Ϸ�����ҪӦ��ı��ģ�
	if(g_pro_struct.updata_noacksend == 1){
		g_pro_struct.updata_noacksend = 2;
		g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_OUT;
		goto RETURN_LAB;
	}
	else if(g_pro_struct.updata_noacksend == 2){
		//g_gprs_ctr_struct.business = GPRS_NULL;
		g_pro_struct.updata_sending = 0;		
		g_pro_struct.updata_noacksend = 0;
		goto RETURN_LAB;
	}

RETURN_LAB:
	return;
}

/******************************************************
//ģ������ʱ��������
�����
******************************************************/
static void Module_Busy_Action(void)
{


}
/******************************************************
//ģ��ִ���������
�����
******************************************************/
void ExecuteModuleTask(void)
{	
	if(g_business_struct.usemodule == _NO)		{			return;		}		///ģ����Ⱥ���
	
	ModlueCalledProcess();
	
	
	
	
	if(g_gprs_data_struct.setnetparaok_flag != 1){	return;		}			//����δ���úã��˳�
		
	//׼��ʹ��GPRS��������		
	switch(g_gprs_data_struct.sendDataStatus)
	{
		case GPRS_SENDDATA_ZERO:{
			if(BusiSendReady() == 4){
				g_pro_struct.link_center_flag = 1;			//�����ӵ�������
				g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_IDLE;
			}
			break;
		}
		
		case GPRS_SENDDATA_IDLE:{//���л��ͳɹ�
			Module_Idel_Action();						
			break;
		}
		case GPRS_SENDDATA_OUT:{//������Ҫ����
			g_gprs_ctr_struct.business = GPRS_SEND_DATA;
			break;
		}
		case GPRS_SENDDATA_BUSY:{//��������ʧ��
			Module_Busy_Action();					
			break;
		}
		default:					break;	
	}
		
}

void APP_L218_task(void *pdata)
{
	static uint16 count;
	
	L218_Init();
	Module_Para_Rest();
	BGprsResetPara();
	g_sysmiscrun_struct.tasks_initOK_flag.bit.B1 = 1;
	while(1)
	{		
		OSTimeDlyHMSM(0, 0, 0, 20);
		if(++count > 500)  count=0;	//10s
		
		ExecuteModuleTask();
		
		
		
		if(count%5 == 0){	//2s
			GPRS_Reconnect2Platform();
		}

	}
	
}















