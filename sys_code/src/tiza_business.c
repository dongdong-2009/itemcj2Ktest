#define BUSINESS_GLOABL
#define BUSINESS_DEBUG

#include "tiza_include.h"

//���Է�������
//uint8  GPRStestdata[13] = {0x26,0x50,0x05,0x18,0x60,0x49,0xaa,0x00,0x16,0x55,0xaa,0xbb,	0x62};
//uint8  GPRStestdata[600];
uint32 GPRStestcount = 1;
/***************************ģ�鸴λ���ƺ���***********
//ģ�鸴λ��������
******************************************************/
void BGprsResetPara(void)
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
	
//��ʼ��ģ��״̬
	g_gprs_data_struct.initStatus 		= GPRS_INIT_NO;
	g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_NO;
	g_gprs_data_struct.MserLinkStatus = GPRS_SERLINK_NO;
	g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_ZERO;
	g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_NO;
//����״̬��
	g_gprs_ctr_struct.business = GPRS_INIT;
	g_gprs_ctr_struct.ope = AT_INDEX;
	
	
//ҵ�����߼�
	g_business_struct.usemodule		= _YES;
	g_business_struct.needlinkser = _YES;
	g_business_struct.needopengps = _YES;//_NO;//
	
}


/******************************************************
//ģ�鷢��ǰ׼��
�����3��׼�����,������ʧ��
******************************************************/
static uint8 BusiSendReady(void)
{
	uint8 step = 0;
	
	if(g_business_struct.needlinkser == _NO){
		return step;
	}
	
	switch(step)
	{
		case 0:{
			if(g_gprs_data_struct.initStatus != GPRS_INIT_OK){
				g_gprs_ctr_struct.business = GPRS_INIT;
				g_gprs_ctr_struct.ope = AT_INDEX;
				break;
			}
			else
				step = 1;
		}
		case 1:{
			if(g_gprs_data_struct.netLinkStatus != GPRS_NETLINK_OK){
				g_gprs_ctr_struct.business = GPRS_CONNECT_NET;
				g_gprs_ctr_struct.ope = AT_NETCLOSE_INDEX;//AT_CIPMUX_INDEX;//
				break;
			}
			else
				step = 2;
		}
		case 2:{
			if(g_gprs_data_struct.MserLinkStatus != GPRS_SERLINK_OK){
				g_gprs_ctr_struct.business = GPRS_CONNECT_SERVER;
				break;
			}
			else
				step = 3;
		}
		default: break;
	}
	return step;
}

/******************************************************
//�Ͽ�ģ�������������
�����
******************************************************/
void BusiDisconSer(void)
{
	if(g_gprs_data_struct.MserLinkStatus == GPRS_SERLINK_NO) 
		return;
	g_pro_struct.updata_sending 				= 0;
	g_pro_struct.updata_noacksend 			= 0;
	g_pro_struct.try_login_statu  			= 0;
	g_pro_struct.login_center_flag  		= FALSE;
	g_sysm_timeout_struct.NLogTim_count = 0xFFFF;		///���ε�¼ʧ�ܺ����µ�¼ʱ����	
	//����Ҫ���ӱ�־
	g_business_struct.needlinkser = _NO;
	//��״̬
	g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_ZERO;
	g_gprs_data_struct.MserLinkStatus = GPRS_SERLINK_NO;
	//ִ��ҵ��
	g_gprs_ctr_struct.business 				= GPRS_DISCONNECT_SERVER;
}
/******************************************************
//����ģ�������������
�����
******************************************************/
void BusiConSer(void)
{
	if(g_gprs_data_struct.MserLinkStatus == GPRS_SERLINK_OK) 
		return;
	//����Ҫ���ӱ�־
	g_business_struct.usemodule		= _YES;
	g_business_struct.needlinkser = _YES;
	//��״̬
	g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_ZERO;
	g_gprs_data_struct.MserLinkStatus = GPRS_SERLINK_NO;
	//ִ��ҵ��
	g_gprs_ctr_struct.business 				= GPRS_NULL;
}
/******************************************************
//���TCP����״̬
�����
******************************************************/
void BusiCheckTCPStatus(void)
{
	//����Ҫ���ӱ�־
	//��״̬
	g_gprs_data_struct.CheckTCPStatus = 0xFF;
	//ִ��ҵ��
	g_gprs_ctr_struct.business 				= GPRS_CHECK_TCPSTATUS;
}
/******************************************************
//�ر�GPS����
�����
******************************************************/
void BusiDisconGps(void)
{
	//����Ҫ���ӱ�־
	g_business_struct.needopengps = _NO;
	//��״̬
	g_gprs_data_struct.GpsOpenStatus = GPS_OPEN_NO;
	//ִ��ҵ��
	g_gprs_ctr_struct.business 			 = GPS_CLOSE_NET;
}
/******************************************************
//����GPS����
�����
******************************************************/
void BusiConGps(void)
{
	//����Ҫ���ӱ�־
	g_business_struct.usemodule		= _YES;
	g_business_struct.needopengps = _YES;//_NO;//
	//��״̬
	g_gprs_data_struct.GpsOpenStatus = GPS_OPEN_NO;
	//ִ��ҵ��
	g_gprs_ctr_struct.business 			 = GPS_OPEN_NET;
}
/******************************************************
//��λģ��
�����
******************************************************/
void BusiResetModule(void)
{
	//������ģ���־
	g_business_struct.usemodule = _NO;
	//����ģ���Դ	L218Reset();
	g_gprs_ctr_struct.business 	= GPRS_RESET;
	ModlueCalledProcess();
	//���ò���
	BGprsResetPara();
}


/******************************************************
//ģ������ƽ̨����
//��������Ӻ�3�ε�¼ʧ�ܾ�����ģ�� �����ڷ��Ͳ���
******************************************************/
static void GPRS_Reconnect2Platform(void)
{
	switch(g_sysmiscrun_struct.GPRSreconnect2plat){
		case 1:{
			BusiDisconSer();
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
			BusiResetModule();
		break;
		}
		default :{	break;		}
	}
}
/******************************************************
//ģ��ִ���������
�����
******************************************************/
void ExecuteModuleTask(void)
{	
		if(g_business_struct.usemodule == _NO){
			return;
		}
		///ģ����Ⱥ���
		ModlueCalledProcess();
			
		if(g_sysmiscrun_struct.ProgramUpgrade_flag > 1){ // 0 1
			return ;
		}
		if(g_gprs_data_struct.initStatus != GPRS_INIT_OK){	
			g_pro_struct.link_center_flag = 0;			
			return;
		}
		g_pro_struct.link_center_flag = 1;		

		///           ������GPS               ��               GPS��ȷ��(�жϿ�����Ч��λ��־) 
		if(g_business_struct.needopengps==_NO || g_gprs_data_struct.GpsOpenStatus==GPS_OPEN_OK)
		{	
			if(g_gprs_data_struct.setnetparaok_flag != 1){
				return;		//����δ���úã��˳�
			}
			if(GpsAssistProcess() == 2){
				return;
			}

			//׼��ʹ��GPRS��������		
			switch(g_gprs_data_struct.sendDataStatus)
			{
				case GPRS_SENDDATA_ZERO:{
					if(BusiSendReady() == 3){
						g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_IDLE;
					}
					break;
				}
				case GPRS_SENDDATA_IDLE:{//���л��ͳɹ�
					if(g_pro_struct.try_login_statu == 0){		//�õ�¼��־��ֻһ��
						g_pro_struct.try_login_statu = 1;
					}
					//�ϴ�ACK���Ϸ�����ҪӦ��ı��ģ�
					if(g_pro_struct.updata_noacksend == 1){
						g_pro_struct.updata_noacksend = 2;
						g_gprs_ctr_struct.business = GPRS_NULL;
						g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_OUT;
						break;
					}
					else if(g_pro_struct.updata_noacksend == 2){
						g_gprs_ctr_struct.business = GPRS_NULL;
						g_pro_struct.updata_sending = 0;		
						g_pro_struct.updata_noacksend = 0;
						break;
					}
					//��������
					if(g_sysmiscrun_struct.ProgramUpgrade_flag == 1){
						ReadyBeforeUpgrade();
						break;
					}
					//�յ�CLOSED�󵹼���
					if(g_sysmiscrun_struct.ReceTCPClosed_flag == 0){
						g_sysmiscrun_struct.ReceTCPClosed_flag = 0xFF;
					}
					else if(g_sysmiscrun_struct.ReceTCPClosed_flag < 0xFF){
						g_sysmiscrun_struct.ReceTCPClosed_flag--;
					}
					
					g_gprs_ctr_struct.business = GPRS_NULL;
					g_pro_struct.updata_sending = 0;				
					break;
				}
				case GPRS_SENDDATA_OUT:{//������Ҫ����
					g_gprs_ctr_struct.business = GPRS_SEND_DATA;
					break;
				}
				case GPRS_SENDDATA_BUSY:{//��������ʧ��
////					#ifdef BUSINESS_DEBUG
////						printf("\r\n L218 send data fail,we will reset l218 module\r\n");	
////					#endif
////					BusiResetModule();

//					#ifdef BUSINESS_DEBUG
//					LocalUartFixedLenSend((uint8*)"\r\n L218 send data fail,we will reconnect to platform now;\r\n", 59);
//					#endif
//					g_sysmiscrun_struct.GPRSreconnect2plat   = 1; //������ƽ̨��־
////					g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_IDLE;
					
					if( g_sysmiscrun_struct.ReceTCPClosed_flag != 0xFF){
						//TCP�Ͽ���·��
						g_sysmiscrun_struct.GPRSreconnect2plat   = 1; //������ƽ̨��־
					}
					else{
						g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_IDLE;
						BusiCheckTCPStatus();													//���TCP����״̬
					}
					
					break;
				}
				default:					break;
			}
 		}
		///                   ����GPS               ��               GPS��δ�ɹ��� 
		else if(g_business_struct.needopengps==_YES && g_gprs_data_struct.GpsOpenStatus!=GPS_OPEN_OK)
		{
			if(g_gprs_data_struct.GpsOpenStatus != GPS_OPEN_ING){
				g_gprs_ctr_struct.business = GPS_OPEN_NET;
				g_gprs_ctr_struct.ope = AT_GETGPS_INDEX;
			}
		}
}


/******************************************************
//L218����
******************************************************/
void L218_task(void *pdata)
{	 	
	static uint16 count;
	
	L218PowerInit();	
	BGprsResetPara();
	#ifdef L218_DEBUG
	LocalUartFixedLenSend((uint8*)"\r\n##### L218 POWER OK #####\r\n", 29);
	#endif	
	

/////IP��220.169.30.122 �˿ڣ�9876
//	g_gprs_data_struct.IP[0] = 220;
//	g_gprs_data_struct.IP[1] = 169;
//	g_gprs_data_struct.IP[2] = 30;
//	g_gprs_data_struct.IP[3] = 122;
//	g_gprs_data_struct.Port = 9876;

	SetGPRSNetPara();
//	ProParaInit();
	
	g_sysmiscrun_struct.tasks_initOK_flag.bit.B1 = 1;
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 0, 20);	
		if(++count > 500)  count=0;	//10s
		
		g_sysmiscrun_struct.task2_feeddog_count = 0;		///L218_task
		
		ExecuteModuleTask();
		
		if(count%5 == 0){	//2s
			GPRS_Reconnect2Platform();
		}
	}
}


