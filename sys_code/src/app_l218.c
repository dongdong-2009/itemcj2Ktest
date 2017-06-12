
#define APP_L218_GLOBAL
#define APP_L218_DEBUG

#include "tiza_include.h"


/******************************************************
//模块参数复位 从BGprsResetPara分出
输出：
******************************************************/
static uint8 Module_Para_Rest(void)
{
	g_gprs_data_struct.setnetparaok_flag 		= 0;
	SetGPRSNetPara();
//	g_propostion_union.Item.status.byte  		= 0x01;
	g_gps_struct.gpsinform.subitem.statu.byte = 0x01;									///初始化定位无效标志
	g_pro_struct.updata_sending 				= 0;
	g_pro_struct.updata_noacksend 			= 0;
	g_pro_struct.try_login_statu  			= 0;
	g_pro_struct.login_center_flag  		= FALSE;
	g_sysm_timeout_struct.NLogTim_count = 0xFFFF;		///三次登录失败后重新登录时间间隔	
}


/******************************************************
//应用层 复位模块
输出：
******************************************************/
uint8 App_Module_Rest(void)
{
	BusiResetModule();
	
	Module_Para_Rest();
}

/******************************************************
//应用层 断开模块与服务器连接
输出：
******************************************************/
void App_DisconSer(void)
{
	if(g_gprs_data_struct.MserLinkStatus == GPRS_SERLINK_NO) 
		return;
	g_pro_struct.updata_sending 				= 0;
	g_pro_struct.updata_noacksend 			= 0;
	g_pro_struct.try_login_statu  			= 0;
	g_pro_struct.login_center_flag  		= FALSE;
	g_sysm_timeout_struct.NLogTim_count = 0xFFFF;		///三次登录失败后重新登录时间间隔	
	
	BusiDisconSer();
	
}


/******************************************************
//模块重连平台程序
//如果重连接后3次登录失败就重启模块 在周期发送操作
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
		case 5:{ //重启模块
			g_sysmiscrun_struct.GPRSreconnect2plat = 0;
			App_Module_Rest();
		break;
		}
		default :{	break;		}
	}*/
}

/******************************************************
//模块空闲处理事项
输出：
******************************************************/
static void Module_Idel_Action(void)
{
	//置登录标志，只一次 登录平台
	if(g_pro_struct.try_login_statu == 0){		
		g_pro_struct.try_login_statu = 1;
	}

	//上传ACK（上发不需要应答的报文）
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
//模块阻塞时处理事项
输出：
******************************************************/
static void Module_Busy_Action(void)
{


}
/******************************************************
//模块执行任务调用
输出：
******************************************************/
void ExecuteModuleTask(void)
{	
	if(g_business_struct.usemodule == _NO)		{			return;		}		///模块调度函数
	
	ModlueCalledProcess();
	
	
	
	
	if(g_gprs_data_struct.setnetparaok_flag != 1){	return;		}			//参数未设置好，退出
		
	//准备使用GPRS发送数据		
	switch(g_gprs_data_struct.sendDataStatus)
	{
		case GPRS_SENDDATA_ZERO:{
			if(BusiSendReady() == 4){
				g_pro_struct.link_center_flag = 1;			//已连接到服务器
				g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_IDLE;
			}
			break;
		}
		
		case GPRS_SENDDATA_IDLE:{//空闲或发送成功
			Module_Idel_Action();						
			break;
		}
		case GPRS_SENDDATA_OUT:{//有数据要发送
			g_gprs_ctr_struct.business = GPRS_SEND_DATA;
			break;
		}
		case GPRS_SENDDATA_BUSY:{//阻塞或发送失败
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















