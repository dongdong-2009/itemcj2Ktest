

#define TIZA_L218OPT_GLOBAL
#define TIZA_L218OPT_DEBUG

#include "tiza_include.h"


/****************查看TCP连接状态*************************
//
//返回值:0:成功; 1:失败;2:正在执行 
	//STATE: IP GPRSACT
	//STATE: CONNECT OK
	//STATE: TCP CLOSE
******************************************************/
static uint8 GprsCheckTCPStatusProcess(void)
{
	static uint8 flag = 0;
	if(flag == 0){
		L218SendAtCmd(AT_CIPSTATUS_INDEX, NULL, 0, (uint8 *)"STATE: ", 7, 1);
		flag = 1;
	}
	if(g_at_cmd_struct[AT_CIPSTATUS_INDEX].exe_flag == EXE_FAIL){
		flag = 0;
		return 1;
	}
	else if(g_at_cmd_struct[AT_CIPSTATUS_INDEX].exe_flag == EXE_NO){
		return 2;
	}
	flag = 0;
  return 0;	
}
/****************打开网络连接***************************
//
//返回值:0:成功; 1:失败;2:正在执行 
******************************************************/
static uint8 GprsConnectNetProcess(void)
{
	static uint8 flag = 0;
	uint8 res = 2;
	
	switch(g_gprs_ctr_struct.at)
	{
		//网络配置
		case AT_NETCLOSE_INDEX:{
			if(flag==0) {
				L218SendAtCmd(AT_NETCLOSE_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);	
				flag = 1;
			}
			if(g_at_cmd_struct[AT_NETCLOSE_INDEX].exe_flag == EXE_OK)
			{//成功
				g_gprs_ctr_struct.at = AT_CIPMUX_INDEX;
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_NETCLOSE_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				flag = 0;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_CIPMUX_INDEX:{
			if(flag==0) {
				L218SendAtCmd(AT_CIPMUX_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);		
				flag = 1;
			}
			if(g_at_cmd_struct[AT_CIPMUX_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_CSTT_INDEX;
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_CIPMUX_INDEX].exe_flag == EXE_FAIL){
				//g_gprs_ctr_struct.at = AT_NETCLOSE_INDEX;
				res = 1;
				flag = 0;
				break;
			}
			else{
			  break;  
			}
		}		
		case AT_CSTT_INDEX:{
			if(flag==0) {
				L218SendAtCmd(AT_CSTT_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);	
				flag = 1;
			}	
			if(g_at_cmd_struct[AT_CSTT_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_CIICR_INDEX;
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_CSTT_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				flag = 0;
				break;
			}
			else{
			  break;  
			}
		}
		//连接NET
		case AT_CIICR_INDEX:{
			if(flag==0) {
				L218SendAtCmd(AT_CIICR_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);	
				flag = 1;
			}
				
			if(g_at_cmd_struct[AT_CIICR_INDEX].exe_flag == EXE_OK){
				res = 0;
				flag = 0;
				break;
			}
			else if(g_at_cmd_struct[AT_CIICR_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				flag = 0;
				break;
			}
			else{
			  break;  
			}
		}

		default: 	res = 1;  flag = 0; break;
	}
		
  return res;	
}
/****************断开网络连接***************************
//
//返回值:0:成功; 1:失败;2:正在执行 
******************************************************/
static uint8 GprsDisonnectNetProcess(void)
{
  L218SendAtCmd(AT_NETCLOSE_INDEX, NULL, 0, (uint8 *)OK_ACK, 2, 1);
	if(g_at_cmd_struct[AT_NETCLOSE_INDEX].exe_flag == EXE_FAIL){
		return 1;
	}
	else if(g_at_cmd_struct[AT_NETCLOSE_INDEX].exe_flag == EXE_NO){
		return 2;
	}

  return 0;	
}

/****************打开服务器连接*************************
//
//返回值:0:成功; 1:失败;2:正在执行 
******************************************************/
static uint8 GprsConnectSerProcess(void)
{
	static uint8 flag = 0;
  char temp_cmd[45];

	if(flag == 0){
		//sprintf(temp_cmd, (const char*)AT_CIPOPEN_TCP, 218,94,153,146, 9903);
		sprintf(temp_cmd, (const char*)AT_CIPOPEN_TCP, g_gprs_data_struct.IP[0],g_gprs_data_struct.IP[1],g_gprs_data_struct.IP[2],g_gprs_data_struct.IP[3], g_gprs_data_struct.Port);
		g_at_cmd_struct[AT_CIPOPEN_TCP_INDEX].cmd_text = (uint8 *)temp_cmd;
		L218SendAtCmd(AT_CIPOPEN_TCP_INDEX, NULL, 0, (uint8 *)"CONNECT OK", 10, 1);
		flag = 1;
	}
	if(g_at_cmd_struct[AT_CIPOPEN_TCP_INDEX].exe_flag == EXE_FAIL){
		flag = 0;
		return 1;
	}
	else if(g_at_cmd_struct[AT_CIPOPEN_TCP_INDEX].exe_flag == EXE_NO){
		return 2;
	} 

	flag = 0;
  return 0;
}
/****************断开服务器连接*************************
//
//返回值:0:成功; 1:失败;2:正在执行 
******************************************************/
static uint8 GprsDisconnectSerProcess(void)
{
	static uint8 flag = 0;
	
	if(flag == 0){
		L218SendAtCmd(AT_CIPCLOSE_INDEX, NULL, 0, (uint8 *)OK_ACK, 2, 1);
		flag = 1;
	}
	if(g_at_cmd_struct[AT_CIPCLOSE_INDEX].exe_flag == EXE_FAIL){
		flag = 0;
		return 1;
	}
	else if(g_at_cmd_struct[AT_CIPCLOSE_INDEX].exe_flag == EXE_NO){
		return 2;
	}

	flag = 0;
  return 0;
}
/****************网络发送数据**************************
//
//返回值:0:成功; 1:失败; 2:超时 
******************************************************/
//                         发送数据    发送数据长度
static uint8 GprsSendDataProcess(uint8 data[], uint16 len)
{
	uint16 temp_len, i;
	uint8  temp_data1[40];
	uint8  temp_data[L218_MAX_BUF_LEN];//40];
	char   temp_ack[] = ">";
	#ifdef TIZA_L218OPT_DEBUG
		uint16 len_pt = 0;
	#endif	
	
	#if 0
		L218SendAtCmd(AT_CSQ_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);		
		if(g_at_cmd_struct[AT_CSQ_INDEX].exe_flag == EXE_OK){

		}
		else if(g_at_cmd_struct[AT_CSQ_INDEX].exe_flag == EXE_FAIL){
			//return 1;
		}
	#endif		
		
	g_gprs_rxstruct.tail = g_gprs_rxstruct.head;  //清GPRS接收缓存

	sprintf((char *)temp_data1, (const char*)AT_CIPSEND_TCP, len);
	g_at_cmd_struct[AT_CIPSEND_TCP_INDEX].cmd_text = (uint8 *)temp_data1;
	L218SendAtCmd(AT_CIPSEND_TCP_INDEX, NULL, 0, (uint8 *)temp_ack, StrLen((const uint8 *)temp_ack, 0), 0);
	if(g_at_cmd_struct[AT_CIPSEND_TCP_INDEX].exe_flag == EXE_FAIL){
		#ifdef TIZA_L218OPT_DEBUG
		DPrint("ack error \n");
		DealDebugSend(1);
		#endif	 	  
//		return 2;
	}
	
	
	#ifdef TIZA_L218OPT_DEBUG
		DPrint("\r\n GPRS send data is:\n");
		for(i=0;i<len;i++){			DPrint("%o", data[i]);		}	
		DPrint("\n");
		DealDebugSend(1);
	#endif
	
	
	
	OSTimeDlyHMSM(0, 0, 0, 100);   
	if(len > 0){
		L218_GPRSUART_Send(data, len);
//		L218_GPRSUART_Send((uint8 *)"\x0d\x0a", 2);
		L218_GPRSUARTSend_Ctr(1);
		
		for(i=0;i < 200;i++){  //200*50ms
			OSTimeDlyHMSM(0, 0, 0, 50);
			temp_len = L218UartIsRxDone(temp_data,L218_MAX_BUF_LEN);//40);
			if(temp_len > 0){
				#ifdef TIZA_L218OPT_DEBUG
					DebugSendDatas(temp_data+len_pt,temp_len-len_pt);	
					len_pt += temp_len;	
					DealDebugSend(1);					
				#endif
				if(SubMatch((uint8 *)CIPSEND_EXTRA_OK,7,temp_data,temp_len) != 0){
					ReadOverTailIndex(temp_len);
					return 0;
				}
				else{
					if(SubMatch((uint8 *)"ERROR",7,temp_data,temp_len) || SubMatch((uint8 *)"FAIL",4,temp_data,temp_len)){
						ReadOverTailIndex(temp_len);
						return 1;
					}
				}
			}
		}
		
		if(i)			{  	 return 2;		}				//超时		
	}
	
	return 0;
}


/****************模块初始化服务***********************
//初始化控制流程
//返回值:0：初始化成功;1:初始化失败;2:正在初始化
******************************************************/
static uint8 GprsInitProcess(void)
{
	static uint8 flag = 0;
	uint8 res = 2;
	
	switch(g_gprs_ctr_struct.at)
	{
		case AT_INDEX:{
			if(flag==0) {
				L218SendAtCmd(AT_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);	
				flag = 1;
			}
			if(g_at_cmd_struct[AT_INDEX].exe_flag == EXE_OK)
			{//成功
				flag = 0;
				g_gprs_ctr_struct.at = AT_E0_INDEX;
			}
			else if(g_at_cmd_struct[AT_INDEX].exe_flag == EXE_FAIL){
				flag = 0;
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_E0_INDEX:{
			if(flag==0) {
				L218SendAtCmd(AT_E0_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);			
				flag = 1;
			}
			if(g_at_cmd_struct[AT_E0_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_CPIN_INDEX;
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_E0_INDEX].exe_flag == EXE_FAIL){
				flag = 0;
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_CPIN_INDEX:{
			if(flag==0) {
				L218SendAtCmd(AT_CPIN_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);		
				flag = 1;
			}				
			if(g_at_cmd_struct[AT_CPIN_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_ICCID_INDEX;
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_CPIN_INDEX].exe_flag == EXE_FAIL){
				App_Module_Rest();			//读取SIM卡IMSI失败  重启模块
				OSTimeDlyHMSM(0, 0, 1, 0);
				res = 1;
				flag = 0;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_ICCID_INDEX:{
			if(flag==0) {
				L218SendAtCmd(AT_ICCID_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);		
				flag = 1;	
			}
			if(g_at_cmd_struct[AT_ICCID_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_CSQ_INDEX;
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_ICCID_INDEX].exe_flag == EXE_FAIL){
				OSTimeDlyHMSM(0, 0, 1, 0);
				res = 1;
				flag = 0;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_CSQ_INDEX:{
			if(flag==0) {
				L218SendAtCmd(AT_CSQ_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);	
				flag = 1;					
			}				
			if(g_at_cmd_struct[AT_CSQ_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_CREG_INDEX;
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_CSQ_INDEX].exe_flag == EXE_FAIL){
				OSTimeDlyHMSM(0, 0, 1, 0);
				res = 1;
				flag = 0;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_CREG_INDEX:{
			OSTimeDlyHMSM(0, 0, 2, 0);
			if(flag==0) {
				L218SendAtCmd(AT_CREG_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);		
				flag = 1;					
			}				
			if(g_at_cmd_struct[AT_CREG_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_CGATT_INDEX;
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_CREG_INDEX].exe_flag == EXE_FAIL){
				OSTimeDlyHMSM(0, 0, 1, 0);
				g_gprs_ctr_struct.at = AT_CSQ_INDEX;
				res = 1;
				flag = 0;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_CGATT_INDEX:{
			OSTimeDlyHMSM(0, 0, 1, 0);
			if(flag==0) {
				L218SendAtCmd(AT_CGATT_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);	
				flag = 1;					
			}						
			if(g_at_cmd_struct[AT_CGATT_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_GTPOS_INDEX;
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_CGATT_INDEX].exe_flag == EXE_FAIL){
				OSTimeDlyHMSM(0, 0, 1, 0);
				g_gprs_ctr_struct.at = AT_CSQ_INDEX;
				res = 1;
				flag = 0;
				break;
			}
			else{
			  break;  
			}
		}
		
		case AT_GTPOS_INDEX:{
			OSTimeDlyHMSM(0, 0, 4, 0);
			if(flag==0) {
				L218SendAtCmd(AT_GTPOS_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);	
				flag = 1;					
			}				
			if(g_at_cmd_struct[AT_GTPOS_INDEX].exe_flag == EXE_OK){
				flag = 0;
				res = 0;
				break;
			}
			else if(g_at_cmd_struct[AT_GTPOS_INDEX].exe_flag == EXE_FAIL){
				ReadWriteOldVaildPostion(0);		//获取基站位置失败 采用上次有效定位信息
				res = 0; // 1;  								//不能获取就算了   考虑使用旧的位置			
				flag = 0;		
				break;
			}
			else{
			  break;  
			}
		}

		default: 	res = 1;  	flag = 0; break;
	}
	
	return res; 
}

/****************打开GPS模块****************************
//初始化控制流程
//返回值:0：初始化成功;1:初始化失败;2:正在初始化
******************************************************/
static uint8 GpsOpenProcess(void)
{
	static uint8 flag = 0;
	uint8 res = 2;
	char temp_cmd[15];
	
	switch(g_gprs_ctr_struct.at)
	{
		case AT_GETGPS_INDEX:{
			if(flag==0) {
				L218SendAtCmd(AT_GETGPS_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);	
				flag = 1;
			} 					
			if(g_at_cmd_struct[AT_GETGPS_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_EGDCONT2_INDEX;
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_GETGPS_INDEX].exe_flag == EXE_FAIL){
				flag = 0;
				return 1;
			}
			else{
			  break;  
			} 
		}
		case AT_EGDCONT2_INDEX:{
			if(flag==0) {
				L218SendAtCmd(AT_EGDCONT2_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);
				flag = 1;
			}		
			if(g_at_cmd_struct[AT_EGDCONT2_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_EGDCONT1_INDEX;
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_EGDCONT2_INDEX].exe_flag == EXE_FAIL){
				flag = 0;
				return 1;
			}
			else{
			  break;  
			}
		}
		case AT_EGDCONT1_INDEX:{
			if(flag==0) {
				L218SendAtCmd(AT_EGDCONT1_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);	
				flag = 1;
			}
			if(g_at_cmd_struct[AT_EGDCONT1_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_MGPSTS_INDEX;
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_EGDCONT1_INDEX].exe_flag == EXE_FAIL){
				flag = 0;
				return 1;
			}
			else{
			  break;  
			}
		}
		case AT_MGPSTS_INDEX:{
			if(flag==0) {
				sprintf((char *)temp_cmd, (const char*)AT_MGPSTS, "?");
				g_at_cmd_struct[AT_MGPSTS_INDEX].cmd_text = (uint8 *)temp_cmd;
				L218SendAtCmd(AT_MGPSTS_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);	
				flag = 1;
			}
			OSTimeDlyHMSM(0, 0, 0, 20);			
			if(g_at_cmd_struct[AT_MGPSTS_INDEX].exe_flag == EXE_03_ON){
				g_gprs_ctr_struct.at = AT_MGPSEPO_INDEX;
				flag = 0;
				goto RETURN_MGPSEPO;
			}
			else if(g_at_cmd_struct[AT_MGPSTS_INDEX].exe_flag == EXE_04_OFF){
				 flag = 0;
			}
			else if(g_at_cmd_struct[AT_MGPSTS_INDEX].exe_flag == EXE_FAIL){
				flag = 0;
				return 1;
			}
			
			if(flag==0) {
				sprintf((char *)temp_cmd, (const char*)AT_MGPSTS, "=1");
				g_at_cmd_struct[AT_MGPSTS_INDEX].cmd_text = (uint8 *)temp_cmd;
				L218SendAtCmd(AT_MGPSTS_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);	
				flag = 1;				
			}				
			if(g_at_cmd_struct[AT_MGPSTS_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_MGPSEPO_INDEX;
				flag = 0;
				goto RETURN_MGPSEPO;
			}
			else if(g_at_cmd_struct[AT_MGPSTS_INDEX].exe_flag == EXE_FAIL){
				flag = 0;
				return 1;
			}
			
			break;
		}
		case AT_MGPSEPO_INDEX:{
		RETURN_MGPSEPO:
			if(flag==0) {
				sprintf((char *)temp_cmd, (const char*)AT_MGPSEPO, "?");
				g_at_cmd_struct[AT_MGPSEPO_INDEX].cmd_text = (uint8 *)temp_cmd;
				L218SendAtCmd(AT_MGPSEPO_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);		
				flag = 1;				
			}				
			if(g_at_cmd_struct[AT_MGPSEPO_INDEX].exe_flag == EXE_03_ON){
				g_gprs_ctr_struct.at = AT_EGPSC_INDEX;
				flag = 0;
				goto RETURN_EGPSC;
			}
			else if(g_at_cmd_struct[AT_MGPSEPO_INDEX].exe_flag == EXE_04_OFF){
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_MGPSEPO_INDEX].exe_flag == EXE_FAIL){
				flag = 0;
				return 1;
			}
			
			if(flag==0) {
				sprintf((char *)temp_cmd, (const char*)AT_MGPSEPO, "=1");
				g_at_cmd_struct[AT_MGPSEPO_INDEX].cmd_text = (uint8 *)temp_cmd;
				L218SendAtCmd(AT_MGPSEPO_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);			
				flag = 1;				
			}					
			if(g_at_cmd_struct[AT_MGPSEPO_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_EGPSC_INDEX;
				flag = 0;
				goto RETURN_EGPSC;
			}
			else if(g_at_cmd_struct[AT_MGPSEPO_INDEX].exe_flag == EXE_FAIL){
				flag = 0;
				return 1;
			}
			break;
		}
		case AT_EGPSC_INDEX:{
		RETURN_EGPSC:
			if(flag==0) {
				sprintf((char *)temp_cmd, (const char*)AT_EGPSC, "?");
				g_at_cmd_struct[AT_EGPSC_INDEX].cmd_text = (uint8 *)temp_cmd;
				L218SendAtCmd(AT_EGPSC_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);			
				flag = 1;				
			}				
			if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_03_ON){
				g_gprs_ctr_struct.at = AT_PMTK314_INDEX;
				flag = 0;
				goto RETURN_314;
			//	return 0;
			}
			else if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_04_OFF){
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_FAIL){
				flag = 0;
				return 1;
			}
			
			if(flag==0) {
				sprintf((char *)temp_cmd, (const char*)AT_EGPSC, "=1");	
				g_at_cmd_struct[AT_EGPSC_INDEX].cmd_text = (uint8 *)temp_cmd;
				L218SendAtCmd(AT_EGPSC_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);		
				flag = 1;				
			}							
			if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_PMTK314_INDEX;
				flag = 0;
				goto RETURN_314;
			//	res = 0;
			}
			else if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_FAIL){
				flag = 0;
				return 1;
			}
			break;  
		}
		
		case AT_PMTK314_INDEX:{
		RETURN_314:
			OSTimeDlyHMSM(0, 0, 4, 0); //>2S等待GPS信息同步完成
			if(flag==0) {
				L218SendAtCmd(AT_PMTK314_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);		
				flag = 1;				
			}							
			if(g_at_cmd_struct[AT_PMTK314_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.at = AT_PMTK386_INDEX;
				flag = 0;
			}
			else if(g_at_cmd_struct[AT_PMTK314_INDEX].exe_flag == EXE_FAIL){
				flag = 0;
				return 1;
			}
			break; 
		}
		case AT_PMTK386_INDEX:{
			if(flag==0) {
				L218SendAtCmd(AT_PMTK386_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);
				flag = 1;				
			}										
			if(g_at_cmd_struct[AT_PMTK386_INDEX].exe_flag == EXE_OK){
				flag = 0;
				res = 0;
			}
			else if(g_at_cmd_struct[AT_PMTK386_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				flag = 0;
			}
			break; 
		}
		
		default: 	res = 1;   flag = 0;break;
	}
	
	return res; 
}

/****************关闭GPS模块****************************
//
//返回值:0:成功; 1:失败;2:正在执行 
******************************************************/
static uint8 GpsCloseProcess(void)
{
	static uint8 flag = 0;
	char temp_cmd[15];
	
	if(flag==0) {
		sprintf((char *)temp_cmd, (const char*)AT_EGPSC, "?");
		g_at_cmd_struct[AT_EGPSC_INDEX].cmd_text = (uint8 *)temp_cmd;
		L218SendAtCmd(AT_EGPSC_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);	
		flag = 1;
	}	
	if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_04_OFF){
		flag = 0;
		return 0;
	}
	else if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_03_ON){
		flag = 0;
	}
	else if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_FAIL){
		flag = 0;
		return 1;
	}
	
	if(flag==0) {
		sprintf((char *)temp_cmd, (const char*)AT_EGPSC, "=0");
		g_at_cmd_struct[AT_EGPSC_INDEX].cmd_text = (uint8 *)temp_cmd;
		L218SendAtCmd(AT_EGPSC_INDEX, NULL, 0, (uint8 *)OK_ACK, 2, 1);
		flag = 1;
	}	
	if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_FAIL){
		flag = 0;
		return 1;
	}
	else if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_NO){
		return 2;
	}

	flag = 0;
  return 0;
}	

/****************获取GPS时间，日期**********************
//
//返回值:0:失败; 1:成功; 
******************************************************/
static uint8 GpsGetTime(uint8 utc_time[],uint8 hex_time[])
{
	uint8 i,res,u8_val;
	
	res = IsValidNum(utc_time,6);
	if(!res)
	{
		goto RETURN_LAB;
	}
	
	for(i=0;i<3;i++)
	{
		u8_val = (utc_time[i*2] - '0')*10;
		u8_val += utc_time[i*2+1] - '0';
		hex_time[i] = u8_val;
	}
	
	if((hex_time[0] >= 24)||(hex_time[1] >= 60)||(hex_time[2] >= 60))
	{
		res = FALSE;
	}
	else
	{
		res = TRUE;
	}
RETURN_LAB:
	return res;
}
static uint8 GpsGetDate(uint8 utc_date[],uint8 hex_date[])
{
	uint8 i,u8_val,res;
	
	res = IsValidNum(utc_date,6);
	if(!res)
	{
		goto RETURN_LAB;
	}
	
	for(i=0;i<3;i++)
	{
		u8_val = (utc_date[i*2] - '0')*10;
		u8_val += utc_date[i*2+1] - '0';
		hex_date[2-i] = u8_val;
	}
	
	if((hex_date[0] < 14 )||
	   (hex_date[0] >= 99)||
	   (hex_date[1] > 12 )||
	   (hex_date[1] < 1 )||
	   (hex_date[2] > 31)||
	   (hex_date[2] < 1))///年，月，日
	{
		res = FALSE;
	}
	else
	{
		res = TRUE;
	}
RETURN_LAB:
	return res;
}

/****************GPS UTC时间转到东8区******************
//
******************************************************/
static void GpsAdd8Hour(uint8 d_t[])
{
	if(d_t[3] < 16)
	{
		d_t[3] += 8;
	}
	else
	{
		d_t[3] += 8;
		d_t[3] = d_t[3] % 24;
		
		d_t[2] += 1;
		if(d_t[2] > 28)
		{
			switch(d_t[1])
			{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				{
					if(d_t[2] > 31)
					{
						d_t[2] = 1;
						d_t[1] += 1;
					}
					break;
				}
				case 2:
				{
					if((d_t[0] % 4 == 0)&&(d_t[0] % 100 != 0))
					{
						if(d_t[2] > 29)
						{
							d_t[2] = 1;
							d_t[1] += 1;
						}
					}
					else
					{
						if(d_t[2] > 28)
						{
							d_t[2] = 1;
							d_t[1] += 1;
						}
					}
					break;
				}
				case 4:
				case 6:
				case 9:
				case 11:
				{
					if(d_t[2] > 30)
					{
						d_t[2] = 1;
						d_t[1] += 1;
					}
					break;
				}
				case 12:
				{
					if(d_t[2] > 31)
					{
						d_t[2] = 1;
						d_t[1] = 1;
						d_t[0] += 1;
					}
					break;
				}
			}
		}
	}
}

/****************GPS字符浮点数转十六进制***************
//
//返回值:0:失败; 1:成功; 
******************************************************/
static uint8 GpsCharFloatDataToHex(uint8 gps_data[],uint8 len,uint8 type,uint32 *r_val)
{
	uint8 i,j,k,m,res;
	uint32 u32_val = 0,u32_val_div = 0;
	
	res = IsValidCharFloatNum(gps_data,len);
	if(!res)
	{
		goto RETURN_LAB;
	}
	
	for(i=0;i<len;i++)
	{
		if(gps_data[i] != '.')
		{
			u32_val *= 10;
			u32_val += gps_data[i] - 0x30;
			
		}
		else
		{
			j = len - i - 1;///小数点后的位数
			break;
		}
	}
	
	i++;
	if((type == LAT_TYPE)||(type == LONG_TYPE))///经纬度取小数点后5位
	{
		k = 0;
		if(j > 5)
		{
			j = 5;
		}
		else
		{
			if(j < 5)
			{
				k = 5 - j;///不足5位，补0个数
			}
		}
	}
	
	if((type == LAT_TYPE)||(type == LONG_TYPE))
	{
		u32_val_div = u32_val % 100;
		u32_val /= 100;
		u32_val *= 100;
		for(m=0;m<j;m++)///小数
		{
			u32_val *= 10;
			u32_val_div *= 10;
			u32_val_div += gps_data[i++] - 0x30;
		}
		
		for(m=0;m<k;m++)
		{
			u32_val *= 10;
			u32_val_div *= 10;
		}
		u32_val_div += 5;///l四舍五入
		
		u32_val_div = u32_val_div * 5;
		u32_val_div /= 3;
	}
	else
	{
		for(m=0;m<j;m++)///小数
		{
			u32_val *= 10;
			u32_val += gps_data[i++] - 0x30;
		}
	}

	if(type == LAT_TYPE)
	{
		u32_val += u32_val_div;
		u32_val /= 10;///精度，小数点后4位
		if(u32_val > 90000000)
		{
			res = FALSE;
			goto RETURN_LAB;
		}
	}
	else if(type == LONG_TYPE)
	{
		u32_val += u32_val_div;
		
		u32_val /= 10;
		if(u32_val > 180000000)
		{
			res = FALSE;
			goto RETURN_LAB;
		}
	}
	else if(type == SPEED_TYPE)
	{		
		for(i=0;i<j;i++)///取整
		{
			u32_val /= 10;
		}
		
		u32_val *= 1852;
		u32_val /= 1000;
		
		if(u32_val > 255)///节转KM/H
		{
			res = FALSE;
			goto RETURN_LAB;
		}
	}
	else if(type == DIR_TYPE)
	{
		for(i=0;i<j;i++)///取整
		{
			u32_val /= 10;
		}
		
		u32_val /= 2;///1代表2度
		
		if(u32_val > 180)
		{
			res = FALSE;
			goto RETURN_LAB;
		}
	}
	else if(type == AMP_TYPE)
	{
		for(i=0;i<j;i++)///取整
		{
			u32_val /= 10;
		}
		
		if(u32_val > 9999)
		{
			res = FALSE;
			goto RETURN_LAB;
		}
	}
	else
	{
		res = FALSE;
		goto RETURN_LAB;
	}
	
	*r_val = u32_val;
	res = TRUE;
RETURN_LAB:
	return res;
}

/****************获取GPRMC\GNGGA数据*******************
//
******************************************************/
static void GpsGetGprmcGpggaInfo(uint8 gps_data[],uint16 len,uint8 type)
{
	uint8 i,j,dot_index[14],t_d[6],res,res_1,res_2;
	uint8 lat_len,long_len,speed_len,dir_len,sat_len,amp_len;
	uint16 tmp_amp_val;
	uint32 lat_val,long_val,speed_val,dir_val,sat_val,amp_val;
	
	#ifdef TIZA_L218OPT_DEBUG
		char str_ch[256];
		uint8 str_len;
	#endif
	///寻找逗号
	if(len>100)len=100;
	j=0;
	for(i=0; i<len; i++)
	{
		if(gps_data[i] == ','){
			dot_index[j++] = i;
		}
		if(j>=13){
			break;
		}
		if(gps_data[i] == '*'){
			break;
		}
	}
	if(j==11 || j==13){}//GGA_j=13,RMC_j=11第二个逗号算起
	else								{			return;				}
	
	if(type == RMC_TYPE)
	{
		if((gps_data[0] != ',')&&(dot_index[0] >= 6)&&
		   (gps_data[dot_index[7]+1] != ',')&&((dot_index[8]-dot_index[7]) >= 6))
		{///提取日期时间
			res_1 = GpsGetTime(gps_data,t_d+3);
			res_2 = GpsGetDate(gps_data+dot_index[7]+1,t_d);
			
			if(res_1 && res_2){
				GpsAdd8Hour(t_d);				
				MemCpy(&g_gps_struct.gpsinform.subitem.year,t_d,6);				
				if(g_sysmiscrun_struct.need_check_time_flag==0x01 || g_sysmiscrun_struct.need_check_time_flag==0x03){	
					if(g_sysmiscrun_struct.need_check_time_flag==0x03){
						RtcInit();															//复位终端后 等GPS时间有效后重新初始化下RTC
					}
					RtcSetTime((RTC_ST *)&g_gps_struct.gpsinform.subitem.year);
					g_sysmiscrun_struct.need_check_time_flag = 0;
				}
				#ifdef TIZA_L218OPT_DEBUG
					str_len = sprintf(str_ch,"\r\n%02d年%02d月%02d日 %02d时%02d分%02d秒\t",t_d[0],t_d[1],t_d[2],t_d[3],t_d[4],t_d[5]);
					DebugSendDatas((uint8*)str_ch,str_len);
					DealDebugSend(1);
				#endif
			}
		}
		
		if(gps_data[dot_index[0]+1] != 'A'){//不定位
			g_gps_struct.gpsinform.subitem.statu.bit.B0 = 1;
//		OFF_GPS_LED();
		}
		else{
			g_gps_struct.gpsinform.subitem.statu.bit.B0 = 0;
//		ON_GPS_LED();
		}
		
		lat_len  = dot_index[2]-dot_index[1]-1;
		long_len = dot_index[4]-dot_index[3]-1;
		if((gps_data[dot_index[1]+1] != ',')&&(lat_len > 0) && (gps_data[dot_index[3]+1] != ',')&&(long_len > 0))
		{///提取纬度与经度
			if(((gps_data[dot_index[2]+1]=='N')||(gps_data[dot_index[1]+1]=='S')) && ((gps_data[dot_index[4]+1]=='E')||(gps_data[dot_index[3]+1]=='W')))	{
				res_1 = GpsCharFloatDataToHex(gps_data+dot_index[1]+1,lat_len,LAT_TYPE,&lat_val);
				res_2 = GpsCharFloatDataToHex(gps_data+dot_index[3]+1,long_len,LONG_TYPE,&long_val);
				
				if(res_1 && res_2){
					if(gps_data[dot_index[2]+1] == 'N'){		
						g_gps_struct.gpsinform.subitem.statu.bit.B1 = 0;					
					}
					else if(gps_data[dot_index[2]+1] == 'S'){						
						g_gps_struct.gpsinform.subitem.statu.bit.B1 = 1;	
					}
					if(gps_data[dot_index[4]+1] == 'E'){				
						g_gps_struct.gpsinform.subitem.statu.bit.B2 = 0;					
					}
					else if(gps_data[dot_index[4]+1] == 'W'){				
						g_gps_struct.gpsinform.subitem.statu.bit.B2 = 1;	
					}					
//						g_gps_struct.gpsinform.subitem.latitude = lat_val;
//						g_gps_struct.gpsinform.subitem.longitude = long_val;
					g_gps_struct.gpsinform.subitem.latitude  = g_gps_struct.gpsinform.subitem.latitude/2  + lat_val/2;
					g_gps_struct.gpsinform.subitem.longitude = g_gps_struct.gpsinform.subitem.longitude/2 + long_val/2;

					#ifdef TIZA_L218OPT_DEBUG
						str_len = sprintf(str_ch,"%ld%c %ld%c\t",lat_val,gps_data[dot_index[2]+1],long_val,gps_data[dot_index[4]+1]);
						DebugSendDatas((uint8*)str_ch,str_len);
						DealDebugSend(1);
					#endif					
				}
			}
		}
		
		speed_len = dot_index[6]-dot_index[5]-1;
		if((gps_data[dot_index[5]+1] != ',')&&(speed_len > 0))
		{///提取速度
			res = GpsCharFloatDataToHex(gps_data+dot_index[5]+1,speed_len,SPEED_TYPE,&speed_val);
			if(res)
			{
				g_gps_struct.gpsinform.subitem.speedk = speed_val;
				#ifdef TIZA_L218OPT_DEBUG
					str_len = sprintf(str_ch,"速度：%03d\t",speed_val);
					DebugSendDatas((uint8*)str_ch,str_len);
					DealDebugSend(1);
				#endif
			}
		}
		
		dir_len = dot_index[7]-dot_index[6]-1;
		if((gps_data[dot_index[6]+1] != ',')&&(dir_len > 0))
		{///提取航向
			res = GpsCharFloatDataToHex(gps_data+dot_index[6]+1,dir_len,DIR_TYPE,&dir_val);
			if(res)
			{
				g_gps_struct.gpsinform.subitem.direction = dir_val;
				#ifdef TIZA_L218OPT_DEBUG
					str_len = sprintf(str_ch,"航向：%03d 度\r\n",dir_val*2);
					DebugSendDatas((uint8*)str_ch,str_len);
					DealDebugSend(1);
				#endif
			}
		}
	}
	else if(type == GGA_TYPE)
	{
		sat_len = dot_index[6]-dot_index[5]-1;
		if((gps_data[dot_index[5]+1] != ',')&&(sat_len > 0)&&(sat_len <= 2))
		{///提取卫星个数
			res = IsValidNum(gps_data+dot_index[5]+1,sat_len);
			if(res){
				sat_val = 0;
				for(i=0;i<sat_len;i++){
					sat_val *= 10;
					sat_val += gps_data[dot_index[5]+1+i] - 0x30;
				}
				g_gps_struct.gpsinform.subitem.starnum = sat_val;
				#ifdef TIZA_L218OPT_DEBUG
					str_len = sprintf(str_ch,"可用卫星数: %02d\t",sat_val);
					DebugSendDatas((uint8*)str_ch,str_len);
					DealDebugSend(1);
				#endif
			}
		}
		else{
			g_gps_struct.gpsinform.subitem.starnum  = 0;
		}
		
		amp_len = dot_index[10]-dot_index[9]-1;
		if((gps_data[dot_index[9]+1] != ',')&&(amp_len > 0))
		{///提取海拔
			tmp_amp_val = 0x0000;
			i = 1;
			if(gps_data[dot_index[9]+1] == '-'){
				tmp_amp_val = 0x8000;
				i = 2;
				amp_len -= 1;
			}
			res = GpsCharFloatDataToHex(gps_data+dot_index[9]+i,amp_len,AMP_TYPE,&amp_val);
			if(res){
				amp_val = tmp_amp_val | amp_val;
				g_gps_struct.gpsinform.subitem.high = amp_val;
				#ifdef TIZA_L218OPT_DEBUG
					str_len = sprintf(str_ch,"海拔: %d\r\n",amp_val);
					DebugSendDatas((uint8*)str_ch,str_len);
					DealDebugSend(1);
				#endif
			}
		}
	}

	return;
}



/****************GPS模块数据捕捉***********************
//
******************************************************/
void SearchDataFromGPS(void)
{
	uint8 recdata[GPS_UART_RXBUF_LEN];
	uint16 reclen,index,mat_index;
	
	g_gps_rxstruct.head = (GPS_UART_RXBUF_LEN - DMA_GetCurrDataCounter(DMA1_Stream2)) % GPS_UART_RXBUF_LEN;
	
	if(g_gps_rxstruct.head != g_gps_rxstruct.tail)
	{//有数据
		reclen = (GPS_UART_RXBUF_LEN + g_gps_rxstruct.head - g_gps_rxstruct.tail) % GPS_UART_RXBUF_LEN;
		if(reclen < 400)		{			return;					}//至少400字节
		for(index=0;index < reclen;index++){
			g_gps_rxstruct.tail = ((++g_gps_rxstruct.tail) % GPS_UART_RXBUF_LEN);
			recdata[index] = g_gps_rxstruct.buf[((g_gps_rxstruct.tail) % GPS_UART_RXBUF_LEN)];
		}		
		#ifdef TIZA_L218OPT_DEBUG  //打印GPS所有信息
//			DebugSendDatas(recdata, reclen);
		#endif		
		
		mat_index = SubMatch((uint8*)"$GNRMC,", 7, recdata,reclen);
		if(mat_index > 0){
			//有GNRMC数据	
			GpsGetGprmcGpggaInfo(recdata+mat_index, reclen-mat_index, RMC_TYPE);
		}
		mat_index = SubMatch((uint8*)"$GNGGA,", 7, recdata,reclen);
		if(mat_index > 0){
			//有GNGGA数据
			GpsGetGprmcGpggaInfo(recdata+mat_index, reclen-mat_index, GGA_TYPE);
		} 
	
	}
}

/****************3G模块数据捕捉************************
//从接收缓存中查找来自于服务器的数据

******************************************************/
void SearchDataFromSer(void)
{
	uint8 recdata[L218_MAX_BUF_LEN];
	uint16 reclen,comma=0,colon,datanum=0;	
	#ifdef TIZA_L218OPT_DEBUG
		uint16 i;
	#endif		
	if(g_gprs_ctr_struct.business == GPRS_SEND_DATA){
		return;
	}
	
	reclen = L218UartIsRxDone(recdata,L218_MAX_BUF_LEN);
	if(reclen < 40){//"+RECEIVE,x,x:\r\n"至少15字节 +25协议最短
		return;
	}
	
	///插入tcp是否链路断开判断(链路断会主动回 \r\nCLOSED\r\n)
	if(SubMatch((uint8 *)"\r\nCLOSED\r\n", 10, recdata, reclen)){
		g_sysmiscrun_struct.ReceTCPClosed_flag = 10;
	}
	
	colon = SubMatch((uint8 *)GPRS_HAVE_RX_DATA,9,recdata,reclen);
	if(colon > 0){
		for(; colon < reclen; colon++){	//从9~20字段找逗号与冒号
			if(recdata[colon]==','){
				comma = colon;
			}
			else if(recdata[colon]==':'){
				break;
			}
		}
		for(++comma;comma < colon;comma++){	//有效数据长度
			datanum = datanum * 10 +(recdata[comma]-'0');
		}
		colon += 3;	//下标到有效数据开头
		//有接收数据
		#ifdef TIZA_L218OPT_DEBUG
//			DPrint("datanum=%d  colon=%d\n", datanum, colon);
			DebugSendDatas(recdata, colon); 
			for(i=colon;i<(reclen-2);i++){
				DPrint("%o", recdata[i]);
			}	
			DPrint("\n");
			DealDebugSend(1);
		#endif		
		ProProcess(recdata+colon,reclen-colon,colon);	
	}	
}

/******************FTP数据捕捉************************
//

******************************************************/
void SearchDataFromFtp(void)
{
/*	uint8 res=2,run=0;
	
	if(g_sysmiscrun_struct.ProgramUpgrade_flag == 2){
		run = 1;
		ftp_struct.ftp_txstep = e_ftpstart;
		while(run){
//			OSTimeDlyHMSM(0, 0, 0, 2);
			FtpMain();
			if(ftp_struct.ftp_upgrade_fail_flag == 1){	
				g_sysmiscrun_struct.ProgramUpgrade_flag = 0;
				run = 0;
				#ifdef L218_DEBUG
					LocalUartFixedLenSend((uint8*)"固件下载失败\r\n", 14);
				#endif
				SysReset();												// 下载固件失败,复位终端有参数需要纠正
			}
			else if(ftp_struct.ftp_upgrade_success_flag == 1){	
				g_sysmiscrun_struct.ProgramUpgrade_flag = 3;
				run = 0;
			}
		}
	}	

	if(ftp_struct.ftp_upgrade_success_flag == 1){//-成功获取更新代码,置标志位,让boot升级 
		res = FlashErase(FLASH_BOOT_PARA_START_ADDR);
		if(res){
			boot_struct.program_update_flag = VALID_VAL_DWORD_AA;
			boot_struct.sys_para_init_flag  = INVALID_VAL_DWORD_55;
			res = FlashWrite(FLASH_BOOT_PARA_START_ADDR,(uint8*)&boot_struct.program_update_flag,12);
			if(res){
				ftp_struct.ftp_upgrade_success_flag = 0;
				g_sysmiscrun_struct.ProgramUpgrade_flag = 0;
				#ifdef L218_DEBUG
				LocalUartFixedLenSend((uint8*)"Device upgrade success\r\n", 24);
				#endif
				SysReset();												// boot标志完成，复位终端到boot程序中升级固件
//			#ifdef L218_DEBUG
//				LocalUartFixedLenSend((uint8*)"Device upgrade success\r\n", 24);
//			}
//			else{
//				LocalUartFixedLenSend((uint8*)"Device upgrade fail\r\n", 21);
//			#endif
			}
		}
		#ifdef L218_DEBUG
		LocalUartFixedLenSend((uint8*)"flash erase or write error\r\n", 28);
		#endif
		g_sysmiscrun_struct.ProgramUpgrade_flag = 0;
		SysReset();	
	}
	*/
}
/******************************************************
//L218模块定时调度函数
//处理GPRS\GPS数据等进程
******************************************************/
void ModlueCalledProcess(void)
{
	uint8 res=2;

	switch(g_gprs_ctr_struct.business)
	{
		case GPRS_INIT:{
			res = GprsInitProcess();
			if(res == 0){
				g_gprs_ctr_struct.business = GPRS_NULL;
				g_gprs_ctr_struct.at = AT_NULL_INDEX;
				g_gprs_data_struct.initStatus = GPRS_INIT_OK;
			}
			else if(res == 2){
				g_gprs_data_struct.initStatus = GPRS_INIT_ING;
			}
			else{
				g_gprs_data_struct.initStatus = GPRS_INIT_NO;
			}
			break;
		}
	  case GPRS_CONNECT_NET:{
			res = GprsConnectNetProcess();
			if(res == 0){
				g_gprs_ctr_struct.business = GPRS_NULL;
				g_gprs_ctr_struct.at = AT_NULL_INDEX;
				g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_OK;
			}
			else if(res == 2){
				g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_ING;				
			}
			else{
				g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_NO;		
			}
			break;
		}
	  case GPRS_CONNECT_SERVER:{
			res = GprsConnectSerProcess();
			if(res == 0){
				g_gprs_ctr_struct.business = GPRS_NULL;
				g_gprs_ctr_struct.at = AT_NULL_INDEX;
				g_gprs_data_struct.MserLinkStatus 	= GPRS_SERLINK_OK;
			}
			else if(res == 2){
				g_gprs_data_struct.MserLinkStatus 	= GPRS_SERLINK_ING;				
			}
			else{
				g_gprs_data_struct.MserLinkStatus 	= GPRS_SERLINK_NO;		
			}
			break;
		}
		case GPRS_DISCONNECT_SERVER:{
			res = GprsDisconnectSerProcess();
			if(res == 0){
				g_gprs_ctr_struct.business = GPRS_NULL;
				g_gprs_ctr_struct.at = AT_NULL_INDEX;
				g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_NO;
			}
			else if(res == 2){
				g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_ING;				
			}
			else{
				g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_ERR;
			}
			break;
		}
    case GPRS_SEND_DATA:{
			if(g_gprs_data_struct.SendDataLen==0 || g_gprs_data_struct.SendData==NULL){
				#ifdef GPRS_DEBUG
				LocalUartFixedLenSend((uint8*)"g_gprs_data_struct.SendData==NULL\r\n", 35);
				#endif	
				g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_IDLE;				
				return;
			}
			res = GprsSendDataProcess(g_gprs_data_struct.SendData,g_gprs_data_struct.SendDataLen);//(NULL,0);//
			if(res == 0 || res == 1){				
				g_gprs_ctr_struct.business = GPRS_NULL;
				g_gprs_ctr_struct.at = AT_NULL_INDEX;
				g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_IDLE;
			}
			else{
				g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_BUSY;
			}			
			break;
		}
		case GPRS_DISCONNECT_NET:{
			res = GprsDisonnectNetProcess();
			if(res == 0){
				g_gprs_data_struct.MserLinkStatus 	= GPRS_SERLINK_NO;
			}
			else if(res == 2){
				g_gprs_data_struct.MserLinkStatus 	= GPRS_SERLINK_ING;				
			}
			else{
				g_gprs_data_struct.MserLinkStatus 	= GPRS_SERLINK_ERR;		
			}
			break;
		}
		case GPRS_RESET:
			L218Reset();
			break; 
		case GPRS_CHECK_TCPSTATUS:
			res = GprsCheckTCPStatusProcess();
			if(res == 0){				
			}
			else if(res == 2){				
			}
			else{				
			}
			g_gprs_ctr_struct.business = GPRS_NULL; 
			break; 
			
			
		case GPS_OPEN_NET:{
			res = GpsOpenProcess();
			if(res == 0){
				g_gprs_ctr_struct.business = GPRS_NULL;
				g_gprs_ctr_struct.at = AT_NULL_INDEX;
				g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_OK;
			}
			else if(res == 2){
				g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_ING;				
			}
			else{
				g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_NO;		
			}
			break;
		}
		case GPS_CLOSE_NET:{
			res = GpsCloseProcess();
			if(res == 0){
				g_gprs_ctr_struct.business = GPRS_NULL;
				g_gprs_ctr_struct.at = AT_NULL_INDEX;
				g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_NO;
			}
			else if(res == 2){
				g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_ING;				
			}
			else{
				g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_ERR;		
			}
			break;
		}
		default:	break;
	}
	if(g_gprs_ctr_struct.business!=GPRS_INIT && (res==0 || res==1)){
		g_gprs_ctr_struct.business = GPRS_NULL;
	}
	OSTimeDlyHMSM(0, 0, 0, 10);	//给时间喂狗，需测对1s发送有无影响
	SearchDataFromGPS();		//寻找GPS信息
	SearchDataFromSer();		//寻找服务器来的数据
	SearchDataFromFtp();		//检查有无FTP升级
	
} 






//--------------------------------------------------------------------------------------------------------------//



/***************************模块复位控制函数***********
//模块复位启动函数
******************************************************/
void BGprsResetPara(void)
{	
//初始化模块状态
	g_gprs_data_struct.initStatus 		= GPRS_INIT_NO;
	g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_NO;
	g_gprs_data_struct.MserLinkStatus = GPRS_SERLINK_NO;
	g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_ZERO;
	g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_NO;
//设置状态机
	g_gprs_ctr_struct.business = GPRS_INIT;
	g_gprs_ctr_struct.at = AT_INDEX;
	
	
//业务上逻辑
	g_business_struct.usemodule		= _YES;
	g_business_struct.needlinkser = _YES;
	g_business_struct.needopengps = _YES;//_NO;//
	
}


/******************************************************
//模块发送前准备
输出：3：准备完成,其他：失败
******************************************************/
uint8 BusiSendReady(void)
{
	static uint8 step = 0;
	
	switch(step)
	{
		case 0:{
			if(g_gprs_data_struct.initStatus != GPRS_INIT_OK){
				if(g_gprs_ctr_struct.business != GPRS_INIT){
					g_gprs_ctr_struct.business = GPRS_INIT;
					g_gprs_ctr_struct.at 			 = AT_INDEX;
				}
			}
			else  step = 1;			
			break;
		}
		case 1:{			
			if(g_business_struct.needopengps ==_YES){
				if(g_gprs_data_struct.GpsOpenStatus != GPS_OPEN_OK){
					BusiConGps();
				}
				else {
					step = 2;
				}
			}
			else{
				step = 2;
			}
			break;
		}
		case 2:{			
			if(g_gprs_data_struct.netLinkStatus != GPRS_NETLINK_OK){
				BusiConNet();
			}
			else  step = 3;
			break;
		}
		case 3:{
			if(g_business_struct.needlinkser == _NO)  {		return step;	}
			
			if(g_gprs_data_struct.MserLinkStatus != GPRS_SERLINK_OK){
				BusiConSer();
			}
			else  step = 4;
			break;
		}
		default: step = 0; break;
	}
	return step;
}

/******************************************************
//关闭GPS数据
输出：
******************************************************/
void BusiDisconGps(void)
{
	//关需要连接标志
	g_business_struct.needopengps = _NO;
	//清状态
	g_gprs_data_struct.GpsOpenStatus = GPS_OPEN_NO;
	//执行业务
	g_gprs_ctr_struct.business 			 = GPS_CLOSE_NET;
	g_gprs_ctr_struct.at 						 = AT_EGPSC_INDEX;
}
/******************************************************
//开启GPS数据
输出：
******************************************************/
void BusiConGps(void)
{
	if(g_gprs_ctr_struct.business == GPS_OPEN_NET)    return;
	
	//开需要连接标志
	g_business_struct.usemodule		= _YES;
	g_business_struct.needopengps = _YES;//_NO;//
	//清状态
	g_gprs_data_struct.GpsOpenStatus = GPS_OPEN_NO;
	//执行业务
	g_gprs_ctr_struct.business 			 = GPS_OPEN_NET;
	g_gprs_ctr_struct.at 						 = AT_GETGPS_INDEX;
}

/******************************************************
//启用模块与网络连接
输出：
******************************************************/
void BusiConNet(void)
{
	if(g_gprs_ctr_struct.business       == GPRS_CONNECT_NET) 		return;
	
	//开需要连接标志
	g_business_struct.usemodule		= _YES;
	//清状态
	g_gprs_data_struct.netLinkStatus = GPRS_NETLINK_NO;
	//执行业务
	g_gprs_ctr_struct.business = GPRS_CONNECT_NET;
	g_gprs_ctr_struct.at = AT_NETCLOSE_INDEX;
	
}
/******************************************************
//启用模块与网络断开
输出：
******************************************************/
void BusiDisconNet(void)
{
	#if 0
	//开需要连接标志
	//清状态
	//执行业务
	g_gprs_ctr_struct.business = GPRS_DISCONNECT_NET;
	g_gprs_ctr_struct.at = AT_NETCLOSE_INDEX;
	#endif
}

/******************************************************
//启用模块与服务器连接
输出：
******************************************************/
void BusiConSer(void)
{
	if(g_gprs_data_struct.MserLinkStatus == GPRS_SERLINK_OK) 		return;
	if(g_gprs_ctr_struct.business       == GPRS_CONNECT_SERVER) 		return;
	
	//开需要连接标志
	g_business_struct.usemodule		= _YES;
	g_business_struct.needlinkser = _YES;
	//清状态
	g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_ZERO;
	g_gprs_data_struct.MserLinkStatus = GPRS_SERLINK_NO;
	//执行业务
	g_gprs_ctr_struct.business = GPRS_CONNECT_SERVER;
	g_gprs_ctr_struct.at = AT_CIPOPEN_TCP_INDEX;
	
}
/******************************************************
//断开模块与服务器连接
输出：
******************************************************/
void BusiDisconSer(void)
{
	if(g_gprs_data_struct.MserLinkStatus == GPRS_SERLINK_NO) 
		return;
	
	//关需要连接标志
	g_business_struct.needlinkser = _NO;
	//清状态
	g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_ZERO;
	g_gprs_data_struct.MserLinkStatus = GPRS_SERLINK_NO;
	//执行业务
	g_gprs_ctr_struct.business 				= GPRS_DISCONNECT_SERVER;
	g_gprs_ctr_struct.at 							= AT_CIPCLOSE_INDEX;
}
/******************************************************
//检查TCP连接状态
输出：
******************************************************/
void BusiCheckTCPStatus(void)
{
	//开需要连接标志
	//清状态
	g_gprs_data_struct.CheckTCPStatus = 0xFF;
	//执行业务
	g_gprs_ctr_struct.business 				= GPRS_CHECK_TCPSTATUS;
	g_gprs_ctr_struct.at 							= AT_CIPSTATUS_INDEX;
	
}
/******************************************************
//复位模块
输出：
******************************************************/
void BusiResetModule(void)
{
	//关启用模块标志
	g_business_struct.usemodule = _NO;
	//重启模块电源	L218Reset();
	g_gprs_ctr_struct.business 	= GPRS_RESET;
	ModlueCalledProcess();
	//重置参数
	BGprsResetPara();
}



