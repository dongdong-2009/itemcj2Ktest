/*******************************
						DEBUG CMD
	报警						CMD ALARM
	恢复正常				CMD NORMAL
	使能CAN报警     CMD CANALARM1*
	禁止CAN报警			CMD CANALARM0*
	登入平台				CMD LOGIN
	登出平台				CMD LOGOUT
	断开网络				CMD DISCONNECT
	连接网络				CMD CONNECT
	独立心跳	      CMD ENHEART1*
	心跳受限实时		CMD ENHEART0*
	
	查看TCP连接状态 CMD TCPSTATUS
	更改IP端口			CMD IPPORT192.168.001.002:21**
	固件升级				CMD UPGRADE
	清盲区					CMD CLEARLSNAL	
	复位终端				CMD REST	
	输入ICCID				CMD ICCID01234567890123456789**
	查看版本				CMD VERSION
*******************************/













#define TEST_STRUCT_GLOBAL
#define TEST_DEBUG
#include "tiza_include.h"

#define FTP_LINK_EXAMP "ftp://Vehicle:Vehicle#*@202.102.090.179:21/TIZA_XGDL_SBJ_V803_170306.bin\x00"

#define TEST_LOCALCMD	
//#define TEST_READLSNAL	
//#define TEST_232_485	
#define TEST_FTP	

//#define TEST_LOGINOUT		1
//#define TEST_CHECKTIME		2
uint8 Test_Step = 0;

uint8 test_loginout_count =0;



static void test_Leds(void){

//		ON_WORK_LED();
//		OFF_GPS_LED();
//		CPL_ERR_LED();

	if(g_propostion_union.Item.status.bit.B0 ==1){
		OFF_GPS_LED();														//未定位
	}
	else{
		ON_GPS_LED();
	}
	if(g_pro_struct.login_center_flag == TRUE){	//已经连接
		ON_WORK_LED();
	}
	else{
		OFF_WORK_LED();														//网络连接标志
	}
	if(g_pro_struct.try_login_statu == 3){			//已经登录	
		ON_ERR_LED();
	}
	else{
		OFF_ERR_LED();														//网络连接标志
	}

}

static void test_rs232_485(void)
{
	uint16 count,index,j;
	uint8  data[256];
	uint8  dataip[4];
	uint16 dataport;
	#ifdef TEST_DEBUG
		char str_ch[50];
		uint8 str_len;
	#endif
	
	#ifdef TEST_LOCALCMD
	//uart1 本地串口	
	g_local_uart_struct.rx_head = (LOCAL_UART_BUF_LEN - DMA_GetCurrDataCounter(DMA1_Channel5)) % LOCAL_UART_BUF_LEN;
	if(g_local_uart_struct.rx_head != g_local_uart_struct.rx_tail)
	{
		count = (LOCAL_UART_BUF_LEN + g_local_uart_struct.rx_head - g_local_uart_struct.rx_tail) % LOCAL_UART_BUF_LEN;
		for(index=0; index < count; index++){
			data[index] = g_local_uart_struct.rx_buf[((index+g_local_uart_struct.rx_tail) % LOCAL_UART_BUF_LEN)];
		}
		g_local_uart_struct.rx_tail = (g_local_uart_struct.rx_tail + count)%LOCAL_UART_BUF_LEN;
		
		if(SubMatch((uint8*)"CMD ALARM",9,data,count))						{ //报警
			g_sysmiscrun_struct.stop_sysAlarm_flag 	 = 0;
			if(g_sysmiscrun_struct.have_sysAlarm_flag==0){// || g_sysmiscrun_struct.have_sysAlarm_flag==3){
//				g_sysalarm_struct.aftertailindex = g_sysalarm_struct.afterheadindex;
				ProLsnalHeadTailSave();
				g_sysmiscrun_struct.have_sysAlarm_flag = 1;
			}
		}
		else if(SubMatch((uint8*)"CMD NORMAL",10,data,count))			{	//正常
			g_sysmiscrun_struct.stop_sysAlarm_flag 	 = 1;
			if(g_sysmiscrun_struct.have_sysAlarm_flag!=0 && g_sysmiscrun_struct.have_sysAlarm_flag!=3){
				//标志检查有无盲区数据
				g_sysmiscrun_struct.have_sysAlarm_flag = 2;  
			}
			else{
				g_sysmiscrun_struct.have_sysAlarm_flag = 0;				
			}
		}
		else if(SubMatch((uint8*)"CMD CANALARM",12,data,count))			{		//使能 或不使能 CAN控制报警
			if(SubMatch((uint8*)"CMD CANALARM1*",14,data,count)){
				g_EnableCANAlarm = 1;
			}
			else if(SubMatch((uint8*)"CMD CANALARM0*",14,data,count)){
				g_EnableCANAlarm = 0;
			}
			LocalUartFixedLenSend((uint8*)"\r\n g_EnableCANAlarm = ", 22);
			str_len = sprintf(str_ch,"%d \r\n", g_EnableCANAlarm);
			LocalUartFixedLenSend((uint8*)str_ch,str_len);
		}		
		
		else if(SubMatch((uint8*)"CMD LOGIN", 9,data,count))				{		//登入平台
			//ProUpLogin();
			g_pro_struct.try_login_statu  = 0;
			g_sysm_timeout_struct.NLogTim_count = 0xFFFF;
			g_pro_struct.login_center_flag  = FALSE;
		}
		else if(SubMatch((uint8*)"CMD LOGOUT", 10,data,count))			{		//登出平台
			ProUpLogout();
			g_pro_struct.try_login_statu  = 4;			
			test_loginout_count++;
		}
		else if(SubMatch((uint8*)"CMD DISCONNECT",14,data,count))		{		//断开服务器
			BusiDisconSer();
		}
		else if(SubMatch((uint8*)"CMD CONNECT", 11, data,count))		{		//连接服务器
			BusiConSer();
		}
		else if(SubMatch((uint8*)"CMD ENHEART", 11,data,count))			{		//使能心跳不受实时数据影响
			if(SubMatch((uint8*)"CMD ENHEART1*", 13,data,count)){
				g_EnableHeart = 1;
			}
			else if(SubMatch((uint8*)"CMD ENHEART0*", 13,data,count)){
				g_EnableHeart = 0;
			}
		}
		else if(SubMatch((uint8*)"CMD TCPSTATUS", 13, data,count))	{		//检查TCP连接状态
			if(g_gprs_data_struct.sendDataStatus==GPRS_SENDDATA_IDLE || g_gprs_data_struct.sendDataStatus==GPRS_SENDDATA_ZERO) {
				BusiCheckTCPStatus();																				//检查TCP连接状态
			}
		}
		else if(SubMatch((uint8*)"CMD IPPORT",10,data,count)){// && data[count]=='#' && count>27)		{	//更改IP PORT
			//CMD IPPORT192.168.001.002:21#
			index = SubMatch((uint8*)"CMD IPPORT",10,data,count);
			dataip[0]=(data[index]-0x30)*100 + (data[index+1]-0x30)*10 + (data[index+2]-0x30);
			dataip[1]=(data[index+4]-0x30)*100 + (data[index+5]-0x30)*10 + (data[index+6]-0x30);
			dataip[2]=(data[index+8]-0x30)*100 + (data[index+9]-0x30)*10 + (data[index+10]-0x30);
			dataip[3]=(data[index+12]-0x30)*100 + (data[index+13]-0x30)*10 + (data[index+14]-0x30);
			//i = SubMatch((uint8*)"**",1,data,count);
			
			dataport = 0;
			for(j=index+16;j < count;j++){
				if(data[j] == '*') break;
				dataport *= 10;
				dataport += data[j] - 0x30;
			}
			
		#ifdef TEST_DEBUG
			//str_len = sprintf(str_ch,"\r\n ipport= %d.%d.%d.%d:%d",dataip[0],dataip[1],dataip[2],dataip[3],dataport);

			LocalUartFixedLenSend((uint8*)"\r\n ipport= ", 11);
			str_len = sprintf(str_ch,"%d.%d.%d.%d:%d \r\n", dataip[0],dataip[1],dataip[2],dataip[3],dataport);
			LocalUartFixedLenSend((uint8*)str_ch,str_len);
		#endif
			
			g_propara_union.Item.PDomain[0] = dataip[0];								//公共平台域名	
			g_propara_union.Item.PDomain[1] = dataip[1];
			g_propara_union.Item.PDomain[2] = dataip[2];
			g_propara_union.Item.PDomain[3] = dataip[3];	
			g_propara_union.Item.PPort			= dataport;//27055;//		//公共平台端口	
			ProWrite_SysPara();
		}		
		else if(SubMatch((uint8*)"CMD UPGRADE",11,data,count))		{	//固件审计
			g_proupgread_struct.ip_domain[2] = 202;
			g_proupgread_struct.ip_domain[3] = 102;
			g_proupgread_struct.ip_domain[4] = 90;
			g_proupgread_struct.ip_domain[5] = 179;
			g_proupgread_struct.port         = 21;
			memcpy(g_proupgread_struct.user_name,"Vehicle",7);
			g_proupgread_struct.user_name[7] = 0;
			memcpy(g_proupgread_struct.pass_word,"Vehicle#*",9);
			g_proupgread_struct.pass_word[9] = 0;
			memcpy(g_proupgread_struct.file_path,"/TIZA_FTP.bin",13);
			g_proupgread_struct.file_path[13] = 0;
			
//			ftp_struct.ftp_upgrade_flag 						= 1;	
			g_sysmiscrun_struct.ProgramUpgrade_flag = 1;
		}
		else if(SubMatch((uint8*)"CMD CLEARLSNAL", 14,data,count)){	//清盲区
			g_syslsnal_struct.tailindex = g_syslsnal_struct.headindex;
			ProLsnalHeadTailSave();
		}
		else if(SubMatch((uint8*)"CMD REST",8,data,count))				{	//复位终端
			SysReset();
		}
		else if(SubMatch((uint8*)"CMD ICCID", 9, data, count))		{			//手动设置ICCID
			index = SubMatch((uint8*)"CMD ICCID", 9, data, count);
			if(data[index+20]=='*' && data[index+21]=='*'){
				MemCpy(g_ICCID, data+index, 20);
			}
		}
		else if(SubMatch((uint8*)"CMD VERSION",11,data,count))		{	//版本号硬件加软件
			LocalUartFixedLenSend((uint8*)"\r\n hardware version is:  ", 25);
			LocalUartFixedLenSend(g_propara_union.Item.g_para_HDVers,5);
			LocalUartFixedLenSend((uint8*)"\r\n software version is:  ", 25);
			LocalUartFixedLenSend(g_propara_union.Item.g_para_SFVers,5);
			LocalUartFixedLenSend((uint8*)"\r\n", 2);
		}
		
	}
	#endif
	#ifdef TEST_232_485
	//uart4
	
	if(g_meter_uart_struct.rx_head != g_meter_uart_struct.rx_tail)
	{
		count = (METER_UART_BUF_LEN + g_meter_uart_struct.rx_head - g_meter_uart_struct.rx_tail) % METER_UART_BUF_LEN;
		for(index=0; index < count; index++){
			data[index] = g_meter_uart_struct.rx_buf[((index+g_meter_uart_struct.rx_tail) % METER_UART_BUF_LEN)];
		}
		g_meter_uart_struct.rx_tail = (g_meter_uart_struct.rx_tail + count)%METER_UART_BUF_LEN;
		MeterUartFixedLenSend(data,count);
	}
	//uart5
	if(g_rs485_uart_struct.rx_head != g_rs485_uart_struct.rx_tail)
	{
		count = (RS485_UART_BUF_LEN + g_rs485_uart_struct.rx_head - g_rs485_uart_struct.rx_tail) % RS485_UART_BUF_LEN;
		for(index=0; index < count; index++){
			data[index] = g_rs485_uart_struct.rx_buf[((index+g_rs485_uart_struct.rx_tail) % RS485_UART_BUF_LEN)];
		}
		g_rs485_uart_struct.rx_tail = (g_rs485_uart_struct.rx_tail + count)%RS485_UART_BUF_LEN;
		RS485UartFixedLenSend(data,count);
	}
		
	#endif	

}

void Test_task(void *pdata)
{	
	#ifdef TEST_232_485
		UsartInit(METER_USART  ,METER_USART_BPR  ,USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);	
		UsartInit(RS485_USART  ,RS485_USART_BPR  ,USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);
	#endif
	
	
	g_EnableCANAlarm = 0;
	g_EnableHeart    = 1;
	
	g_sysmiscrun_struct.tasks_initOK_flag.bit.B3 = 1;
	
	while(1){
		OSTimeDlyHMSM(0, 0, 1, 0);
		g_sysmiscrun_struct.task4_feeddog_count = 0;		///Test_task
		
		test_Leds();
		test_rs232_485();		
//	  printf("\r\n g_sysmiscrun_struct.NLogTim_count = %d g_pro_struct.try_login_statu = %d\r\n",g_sysmiscrun_struct.NLogTim_count,g_pro_struct.try_login_statu);
//		printf(" g_sysalarm_struct.afterheadindex = %d   g_sysalarm_struct.aftertailindex = %d\r\n",g_sysalarm_struct.afterheadindex,g_sysalarm_struct.aftertailindex);
//		printf(" g_sysalarm_struct.beforeheadindex = %d   g_sysalarm_struct.beforetailindex = %d\r\n",g_sysalarm_struct.beforeheadindex,g_sysalarm_struct.beforetailindex);
		printf(" g_syslsnal_struct.headindex = %d   g_syslsnal_struct.tailindex = %d\r\n",g_syslsnal_struct.headindex,g_syslsnal_struct.tailindex);
//		printf(" g_sysmiscrun_struct.have_sysAlarm_flag = %d \r\n",g_sysmiscrun_struct.have_sysAlarm_flag);
//		printf("\r\n PWR_C = %d BAT_ADC = %d\r\n",adc_result[0],adc_result[1]);
//		printf("\r\n g_gprs_data_struct.CheckTCPStatus = %d\r\n",g_gprs_data_struct.CheckTCPStatus);
		printf("\r\n g_sysm_timeout_struct.upheart_count = %d\r\n", g_sysm_timeout_struct.upheart_count);
	

		
//		if(CHECK_ACC()){
//			LocalUartFixedLenSend((uint8*)"\r\n ACC=OFF PD9=1 \r\n",19);
//		}
//		else{
//			LocalUartFixedLenSend((uint8*)"\r\n ACC=ON  PD9=0 \r\n",19);
//		}
		
		
	}
}

