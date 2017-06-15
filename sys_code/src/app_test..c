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


#define TEST_LOCALCMD

//#define dEXTE_SPI_FLASH_test 
//#define dINTE_FLASH_test 
#define dACC_POWER_test
//#define dRTC_test




static void local_debug_cmd(void);
uint8 test_loginout_count =0;





//===========================================================================================//

/******************************************************
//EXTE_SPI_FLASH  测试函数
******************************************************/
#ifdef dEXTE_SPI_FLASH_test
uint8 spiflash_wbuff[4096];
uint8 spiflash_rbuff[4096];
void EXTE_SPI_FLASH_test(void)
{
	uint16 sec,i;
	
	for(i=0;i<4096;i++){
		spiflash_wbuff[i] = i%0xFF;
	}
		
		
	for(sec=0; sec < 2048; sec++){
		ExteFlashEraseSector(sec);
		
		ExteFlashWrite(sec*4096, spiflash_wbuff, 4096);
		DPrint("\n Sector %d spiflash_wbuff :\n", sec);
		for(i=0;i<4096;i++){
			spiflash_rbuff[i] = 0;
			
			DPrint("%o", spiflash_wbuff[i]);
			if((i%100) == 0)DealDebugSend(1);
		}
		ExteFlashRead(sec*4096, spiflash_rbuff, 4096);
		
		DPrint("\n Sector %d spiflash_rbuff :\n", sec);
		for(i=0;i<4096;i++){
			DPrint("%o", spiflash_rbuff[i]);
			if((i%100) == 0)DealDebugSend(1);
		}
		
	}
	
}
#endif
/******************************************************
//INT_FLASH  测试函数
******************************************************/
#ifdef dINTE_FLASH_test
uint8 intflash_wbuff[4096];
uint8 intflash_rbuff[4096];
void INTE_FLASH_test(void)
{
	uint16 i;
	
	for(i=0;i<4096;i++){
		intflash_wbuff[i] = i%0xFF;
	}
	
	CpuFlashEraseSector(INTFlASH_STORE_START_ADDR	);
	OSTimeDlyHMSM(0, 0, 1, 0);
	CpuFlashWrite(INTFlASH_STORE_START_ADDR, intflash_wbuff, 5);
	OSTimeDlyHMSM(0, 0, 1, 0);
	CpuFlashWrite(INTFlASH_STORE_START_ADDR+16, intflash_wbuff, 6);
	OSTimeDlyHMSM(0, 0, 1, 0);
	CpuFlashWrite(INTFlASH_STORE_START_ADDR+32, intflash_wbuff, 7);
	OSTimeDlyHMSM(0, 0, 1, 0);
	CpuFlashWrite(INTFlASH_STORE_START_ADDR+48, intflash_wbuff, 8);
	OSTimeDlyHMSM(0, 0, 1, 0);
	CpuFlashWrite(INTFlASH_STORE_START_ADDR+64, intflash_wbuff, 9);
	OSTimeDlyHMSM(0, 0, 1, 0);
	CpuFlashWrite(INTFlASH_STORE_START_ADDR+80, intflash_wbuff, 10);
	OSTimeDlyHMSM(0, 0, 1, 0);
	CpuFlashRead(INTFlASH_STORE_START_ADDR, intflash_rbuff, 4096);
	OSTimeDlyHMSM(0, 0, 1, 0);
	
	CpuFlashEraseSector(ADDR_FLASH_SECTOR_9	);
	OSTimeDlyHMSM(0, 0, 1, 0);
	CpuFlashWrite(ADDR_FLASH_SECTOR_9, intflash_wbuff, 4093);
	OSTimeDlyHMSM(0, 0, 1, 0);
	CpuFlashRead(ADDR_FLASH_SECTOR_9, intflash_rbuff, 4093);
	OSTimeDlyHMSM(0, 0, 1, 0);
	
	i = i;
}
#endif
/******************************************************
//ACC_POWER  测试函数
******************************************************/
#ifdef dACC_POWER_test
void ACC_POWER_test(void)
{
	static uint8 acc=0, pow=0, ring=0;
	if(CHECK_ACC()){
		if(acc==0)
			DPrint("\n ACC=OFF PC3=1 \n");
		acc=1;
	}
	else{
		if(acc==1)
			DPrint("\n ACC=ON PC3=0 \n");
		acc=0;
	}
	
	if(CHECK_POW()){
		if(pow==0)
			DPrint("\r\n POW=ON PD3=1 \r\n");
		pow=1;
	}
	else{
		if(pow==1)
			DPrint("\n POW=OFF PD3=0 \n");
		pow=0;
	}
	
	if(CHECK_RING()){
		if(ring==1)
			DPrint("\n CHECK_RING PF3=1 \n");
		ring=0;
	}
	else{
		if(ring==0)
			DPrint("\n CHECK_RING PF3=0 \n");
		ring=1;
	}
		
		
		
	DealDebugSend(1);	
}
#endif
	
/******************************************************
//RTC  测试函数
******************************************************/
#ifdef dRTC_test
void RTC_test(void)
{
	RTC_ST TestRtcTime;	

	DPrint("\n 测试RTC: ");
	if(RtcInit()==0)
	{
		DPrint("\n----RTC初始化成功");
		
	}
	else
	{
		DPrint("\n----RTC初始化失败");
	}
	
	
	DealDebugSend(1);
	TestRtcTime.second = 36;
	TestRtcTime.minute = 15;
	TestRtcTime.hour = 10;
	TestRtcTime.day = 29;
	TestRtcTime.month = 10;
	TestRtcTime.year = 16;
	if(RtcSetTime(&TestRtcTime) == 0)
	{
		DPrint("\n----时间设置成功,16年10月29号10:15:36");
	}
	else
	{
		DPrint("\n----时间设置失败,16年10月29号10:15:36");
	}
	DealDebugSend(1);
	if(RtcGetTime(&TestRtcTime) == 0)
	{
		DPrint("\n----时间读取成功,时间为:%d年%d月%d日%d:%d:%d",TestRtcTime.year,TestRtcTime.month,TestRtcTime.day,TestRtcTime.hour,TestRtcTime.minute,TestRtcTime.second);
	}
	else
	{
		DPrint("\n----时间读取失败");
	}
	DPrint("\nRTC测试完成!\n");
	DealDebugSend(1);
}
#endif	



//====================================================================================================//
void Test_task(void *pdata)
{	
	
	//g_EnableHeart = 1;
	
	
	while(1){
		OSTimeDlyHMSM(0, 0, 1, 0);
		CPL_RED_LED();
				
	
		
		
		
		
		
		
		
		
		//DPrint("\n g_sysmiscrun_struct.NLogTim_count = %d g_pro_struct.try_login_statu = %d\n",1, g_pro_struct.try_login_statu);
//		
//		DPrint(" g_sysalarm_struct.beforeheadindex = %d   g_sysalarm_struct.beforetailindex = %d\n",g_sysalarm_struct.beforeheadindex,g_sysalarm_struct.beforetailindex);
//		DPrint(" g_syslsnal_struct.headindex = %d   g_syslsnal_struct.tailindex = %d\n",g_syslsnal_struct.headindex,g_syslsnal_struct.tailindex);
//		DealDebugSend(1);
//		
		
		
		
		
		
		
		local_debug_cmd();
		
		
		#ifdef dEXTE_SPI_FLASH_test
		EXTE_SPI_FLASH_test();										//spiflash 测试
		#endif
		#ifdef dINTE_FLASH_test
		INTE_FLASH_test();												//intflash 测试
		#endif
		#ifdef dACC_POWER_test
		ACC_POWER_test();													//ACC POWER RING 引脚监测
		#endif
		#ifdef dRTC_test
		RTC_test();																//RTC 测试
		#endif

	}
}




static void local_debug_cmd(void)
{
	#ifdef TEST_LOCALCMD
	uint16 count, index, j;	
	uint8  data[50];
	uint8  dataip[4];
	uint16 dataport;
	
	
	//uart1 本地串口	
	DebugRecData(NULL, 0);
	count = ReadDebugData(data,50);
	if(count > 5)
	{
		if(SubMatch((uint8*)"CMD ALARM",9,data,count))						{ //报警
			g_sysmiscrun_struct.stop_sysAlarm_flag 	 = 0;
			if(g_sysmiscrun_struct.have_sysAlarm_flag==0){// || g_sysmiscrun_struct.have_sysAlarm_flag==3){
//				g_sysalarm_struct.aftertailindex = g_sysalarm_struct.afterheadindex;
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
			DPrint("\n g_EnableCANAlarm = %d", g_EnableCANAlarm);
			DealDebugSend(1);
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
			App_DisconSer();
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
			
			DPrint("n ipport= %d.%d.%d.%d:%d",dataip[0],dataip[1],dataip[2],dataip[3],dataport);
			DealDebugSend(1);
			
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
			
			g_sysmiscrun_struct.ProgramUpgrade_flag 						= 1;
			ReadyBeforeUpgrade();
			ftp_struct.ftp_upgrade_flag 						= 1;	
			g_sysmiscrun_struct.ProgramUpgrade_flag = 1;
		}
		else if(SubMatch((uint8*)"CMD CLEARLSNAL", 14,data,count)){	//清盲区
			g_syslsnal_struct.tailindex = g_syslsnal_struct.headindex;
			ExtFlashLsnalHeadTailSave();
		}
		else if(SubMatch((uint8*)"CMD REST",8,data,count))				{	//复位终端
			SysReset();
		}
		else if(SubMatch((uint8*)"CMD ICCID", 9, data, count))		{			//手动设置ICCID
			index = SubMatch((uint8*)"CMD ICCID", 9, data, count);
			if(data[index+20]=='*' && data[index+21]=='*'){
				MemCpy(g_dri_ICCID, data+index, 20);
			}
		}
		else if(SubMatch((uint8*)"CMD VERSION",11,data,count))		{	//版本号硬件加软件
			DPrint("\n hardware version is:  ");
			DebugSendDatas(g_propara_union.Item.g_para_HDVers,5);
			DPrint("\n software version is:  ");
			DebugSendDatas(g_propara_union.Item.g_para_SFVers,5);
			DPrint("\n");
			DealDebugSend(1);
		}
		
	}
	#endif

	
}
