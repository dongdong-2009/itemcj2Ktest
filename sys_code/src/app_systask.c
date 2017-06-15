
#define APP_SYSTASK_GLOBAL
#define APP_SYSTASK_DEBUG

#include "tiza_include.h"
#include <os_cfg.h>
#include <os_cpu.h>

static void ProAlarmHeadTailSave(void);




void SysPara_Init(void)
{
	//======================================//
	g_sysm_timeout_struct.NorUpPeri_count = 0;							///正常周期上传信息计时
	g_sysm_timeout_struct.upheart_count		= 0;							///系统上传心跳计时
//	sysm_timeout_struct.TAckTim_count		= 0;						///终端应答超时时间		
	g_sysm_timeout_struct.PAckTim_count		= 0xFF;						///平台应答超时时间		
	
//	//======================================//
//	g_sysm_on_off_struct.canrx_switch 				= SYSM_ON;
//	g_sysm_on_off_struct.GPRSPacketTx_switch	= SYSM_ON;	
//	g_sysm_on_off_struct.GPRSPeriodTx_switch 	= SYSM_ON;
//	g_sysm_on_off_struct.ADC_switch 					= SYSM_ON;
//	g_sysm_on_off_struct.extflash_w_switch		= SYSM_ON;
//	g_sysm_on_off_struct.flash_w_switch				= SYSM_ON;
//	
	//======================================//
	g_sysmiscrun_struct.have_sysAlarm_flag 	 = 0;						///系统报警标志 同时send_sysAlarm_count=0
	g_sysmiscrun_struct.stop_sysAlarm_flag 	 = 0;						//在发送报警数据过程中 报警取消标志  00初始 01报警取消
	g_sysmiscrun_struct.have_sysAlarm_count  = 0;						///系统出现报警倒计时
	g_sysmiscrun_struct.need_check_time_flag = 3;//1;						///需要校验系统日期时间     01从GPS   02从平台  00无效
	g_sysmiscrun_struct.GPRSreconnect2plat   = 0;						///模块断开服务器连接再重连接		00初始 01需要断开 02需要重连 03等待重连OK 04重连OK 05重启模块
	g_sysmiscrun_struct.TCP_Disconnect_count = 0xFFFF;			///断网计数，规定时间内没有连接上，重启模块  FFFF无效

}

/******************************************************
//设置GPRS发送所用参数
******************************************************/
void SetGPRSNetPara(void){
//Port = 20000;//27055;//9903;//
	
	g_gprs_data_struct.SendDataLen=0;
  g_gprs_data_struct.SendData=(uint8 *)GPRStestdata;
	//企业平台 9903
	g_gprs_data_struct.IP[0] = g_propara_union.Item.Domain[0];
	g_gprs_data_struct.IP[1] = g_propara_union.Item.Domain[1];
	g_gprs_data_struct.IP[2] = g_propara_union.Item.Domain[2];
	g_gprs_data_struct.IP[3] = g_propara_union.Item.Domain[3];
	g_gprs_data_struct.Port = g_propara_union.Item.Port;
	//公共平台 20000
	g_gprs_data_struct.IP[0] = g_propara_union.Item.PDomain[0];
	g_gprs_data_struct.IP[1] = g_propara_union.Item.PDomain[1];
	g_gprs_data_struct.IP[2] = g_propara_union.Item.PDomain[2];
	g_gprs_data_struct.IP[3] = g_propara_union.Item.PDomain[3];
	g_gprs_data_struct.Port =  g_propara_union.Item.PPort;// 9903; //
	
	g_gprs_data_struct.setnetparaok_flag = 1;
}




/******************************************************
//终端定时保存实时数据
//过检时使用
******************************************************/
static void ChargeSendTime(uint8 flag)
{
	//报警30+30+10使用，暂计时到天（一般不会在夜里0过检）
	if(flag==1){
		g_sysmiscrun_struct.send_sysAlarm_time[1].Item.second++;
		if(g_sysmiscrun_struct.send_sysAlarm_time[1].Item.second >= 60){
			g_sysmiscrun_struct.send_sysAlarm_time[1].Item.second = 0;
			g_sysmiscrun_struct.send_sysAlarm_time[1].Item.minute++;
		}
		if(g_sysmiscrun_struct.send_sysAlarm_time[1].Item.minute >= 60){
			g_sysmiscrun_struct.send_sysAlarm_time[1].Item.minute = 0;
			g_sysmiscrun_struct.send_sysAlarm_time[1].Item.hour++;
		}
		if(g_sysmiscrun_struct.send_sysAlarm_time[1].Item.hour >= 24){
			g_sysmiscrun_struct.send_sysAlarm_time[1].Item.hour = 0;
			g_sysmiscrun_struct.send_sysAlarm_time[1].Item.day++;
		}
	}
	else{
		g_sysmiscrun_struct.send_sysAlarm_time[0].Item.second++;
		if(g_sysmiscrun_struct.send_sysAlarm_time[0].Item.second >= 60){
			g_sysmiscrun_struct.send_sysAlarm_time[0].Item.second = 0;
			g_sysmiscrun_struct.send_sysAlarm_time[0].Item.minute++;
		}
		if(g_sysmiscrun_struct.send_sysAlarm_time[0].Item.minute >= 60){
			g_sysmiscrun_struct.send_sysAlarm_time[0].Item.minute = 0;
			g_sysmiscrun_struct.send_sysAlarm_time[0].Item.hour++;
		}
		if(g_sysmiscrun_struct.send_sysAlarm_time[0].Item.hour >= 24){
			g_sysmiscrun_struct.send_sysAlarm_time[0].Item.hour = 0;
			g_sysmiscrun_struct.send_sysAlarm_time[0].Item.day++;
		}	
			
	}
	
}
static void MarkSendTime(void)
{
	uint8 i;
	g_sysmiscrun_struct.send_sysAlarm_numb = 1;
	g_sysmiscrun_struct.send_sysAlarm_time[1] = g_protime_union; 
	g_sysmiscrun_struct.send_sysAlarm_time[0] = g_protime_union; 
	
	for(i=0; i < 31; i++){ // 后推31秒
			g_sysmiscrun_struct.send_sysAlarm_time[0].Item.second--;
		if(g_sysmiscrun_struct.send_sysAlarm_time[0].Item.second == 0xff){
			g_sysmiscrun_struct.send_sysAlarm_time[0].Item.second = 59;
			g_sysmiscrun_struct.send_sysAlarm_time[0].Item.minute--;
		}
		if(g_sysmiscrun_struct.send_sysAlarm_time[0].Item.minute == 0xff){
			g_sysmiscrun_struct.send_sysAlarm_time[0].Item.minute = 59;
			g_sysmiscrun_struct.send_sysAlarm_time[0].Item.hour--;
		}
		if(g_sysmiscrun_struct.send_sysAlarm_time[0].Item.hour == 0xff){
			g_sysmiscrun_struct.send_sysAlarm_time[0].Item.hour = 23;
			g_sysmiscrun_struct.send_sysAlarm_time[0].Item.day--;
		}
	}
//	DPrint("\nsend_sysAlarm_time[0]=%d send_sysAlarm_time[1]=%d \n",g_sysmiscrun_struct.send_sysAlarm_time[0].Item.second ,g_sysmiscrun_struct.send_sysAlarm_time[1].Item.second);
}


static void ProBeforeAlarmSave(uint8 data[],uint16 len)
{
	g_sysalarm_struct.beforeheadindex = (g_sysalarm_struct.beforeheadindex+1)%BEFORE_ALARM_MAXINDEX;
	memcpy(g_BeforeAlarmData[g_sysalarm_struct.beforeheadindex], data, len);
	if(g_sysalarm_struct.beforeheadindex == g_sysalarm_struct.beforetailindex){
		g_sysalarm_struct.beforetailindex = (g_sysalarm_struct.beforetailindex+1)%BEFORE_ALARM_MAXINDEX;
	}	
}

void ProPutIntoAlarm(uint8 data[], uint16 len, uint8 cmd)
{
	uint8 tmpdata[512];
	uint16 tmplen;
	
	if(len == 0 ){
		tmplen = Pro_RealTime_Data(&tmpdata[4]);
	}
	else{
		memcpy(&tmpdata[4],data,len);
		tmplen = len;
	}	
	
	//可以在这里读取下FLASH  来区分要不要擦除
//	if(g_sysalarm_struct.data[0] != VALID_VAL_2A){		
		tmpdata[0] = VALID_VAL_2A;
		tmpdata[1] = tmplen >> 8;
		tmpdata[2] = tmplen & 0xFF;
		tmpdata[3] = cmd;//PRO_UP_LSNAL_INFO_ID;//PRO_UP_REAL_INFO_ID;//cmd;
		
		tmpdata[tmplen+4] = U8SumCheck(tmpdata,tmplen+4);
		if(cmd == PRO_UP_LSNAL_INFO_ID){
			ProBeforeAlarmSave(tmpdata,tmplen+5);				
		}
//	}
	
}

static void TerminalSavePeriod(uint16 past_ms)
{	
	if(g_sysmiscrun_struct.have_sysAlarm_flag == 1){//有报警
//		ProPutIntoAlarm(NULL, 0, PRO_UP_REAL_INFO_ID);		//1s执行一次保存
		
		//报警超时 停止1s报警上传转为正常上传  若仍需1s报警上传则把以下条件代码注释
		if(++g_sysmiscrun_struct.have_sysAlarm_count >= SYS_HAVEALARM_COUNT){
			g_sysmiscrun_struct.have_sysAlarm_count = SYS_HAVEALARM_COUNT;
			g_sysmiscrun_struct.have_sysAlarm_flag = 2;				//超时 置检测有无报警盲区
		}
	}
	else if(g_sysmiscrun_struct.have_sysAlarm_flag==0 || g_sysmiscrun_struct.have_sysAlarm_flag==3){	//无报警 或 无报警盲区
		if(g_sysmiscrun_struct.need_check_time_flag!=0x03){		//时间无效时不保存
			ProPutIntoAlarm(NULL, 0, PRO_UP_LSNAL_INFO_ID);
		}
	}
}

static void TerminalUpLsnalAlrm(void)
{
	uint8  rx_buf[TMP_BUFF_MAXLEN-25],data[5];	//buff考虑帧头尾-25
	uint8  sum=0;
	uint16 len;
	uint32 addr;	
	#ifdef SYS_TASK_DEBUG
		char str_ch[10];
		uint8 str_len;
	#endif
	
	if(g_sysmiscrun_struct.have_sysAlarm_flag != 1 && g_sysmiscrun_struct.have_sysAlarm_flag != 2){		
		MarkSendTime();
		return;												//不是报警  也  没有报警盲区
	}

	g_sysm_timeout_struct.upheart_count 		= 0;		//报警 时清心跳标志
	if(g_sysmiscrun_struct.send_sysAlarm_numb <= 30 ){	//使用发送时间确保报警报文时间连续 170504start
		len = Pro_RealTime_Data(&rx_buf[PRO_DATA_INDEX]);
		ProConstructFrameHead(rx_buf, len, PRO_UP_REAL_INFO_ID);	
		len += PRO_DATA_INDEX;	
		memcpy(&rx_buf[PRO_DATA_INDEX], g_sysmiscrun_struct.send_sysAlarm_time[1].arry, 6);		//发送时间		
		ProConstructFrameTail(rx_buf, len);
		ProPacket(rx_buf, len+1, PRO_UP_REAL_INFO_ID, FALSE, 0);
		
		ChargeSendTime(1);												//时间加1
		g_sysmiscrun_struct.send_sysAlarm_numb++;	//条数加1
	}																										//																	170504end
	else{		
//		g_sysmiscrun_struct.have_sysAlarm_flag = 2;
		if(g_sysalarm_struct.beforetailindex != g_sysalarm_struct.beforeheadindex){ //发送报警前的数据				
			if(g_BeforeAlarmData[g_sysalarm_struct.beforetailindex][0]==VALID_VAL_2A && g_BeforeAlarmData[g_sysalarm_struct.beforetailindex][3]==PRO_UP_LSNAL_INFO_ID){
				len = (g_BeforeAlarmData[g_sysalarm_struct.beforetailindex][1]<<8) + g_BeforeAlarmData[g_sysalarm_struct.beforetailindex][2];
				sum = U8SumCheck(g_BeforeAlarmData[g_sysalarm_struct.beforetailindex], len+4);			
				
				if(g_BeforeAlarmData[g_sysalarm_struct.beforetailindex][len+4] == sum){//
					if(g_sysmiscrun_struct.send_sysAlarm_numb%10 !=0){ //判断是否要插入发10s实时数据
						ProConstructFrameHead(rx_buf, len, PRO_UP_LSNAL_INFO_ID);
						memcpy(&rx_buf[PRO_DATA_INDEX], &g_BeforeAlarmData[g_sysalarm_struct.beforetailindex][4], len);
						len += PRO_DATA_INDEX;
						ChargeSendTime(0);																																		//时间减1  170504
						memcpy(&rx_buf[PRO_DATA_INDEX], g_sysmiscrun_struct.send_sysAlarm_time[0].arry, 6);		//发送时间 170504
						ProConstructFrameTail(rx_buf, len);
						
						g_sysalarm_struct.beforetailindex = (g_sysalarm_struct.beforetailindex+1)%BEFORE_ALARM_MAXINDEX;
						ProPacket(rx_buf, len+1, PRO_UP_LSNAL_INFO_ID, FALSE, 0);//TRUE);
					}
					else{//插入10秒一条实时数据   170504start
						len = Pro_RealTime_Data(&rx_buf[PRO_DATA_INDEX]);
						ProConstructFrameHead(rx_buf, len, PRO_UP_REAL_INFO_ID);	
						len += PRO_DATA_INDEX;	
						ProConstructFrameTail(rx_buf, len);
						ProPacket(rx_buf, len+1, PRO_UP_REAL_INFO_ID, FALSE, 0);
					}
					g_sysmiscrun_struct.send_sysAlarm_numb++;	//条数加1
					if(g_sysmiscrun_struct.send_sysAlarm_numb >= 64){
						g_sysalarm_struct.beforetailindex = g_sysalarm_struct.beforeheadindex;
					}											//			170504end
					
				}
				else{
					goto RETURN_LAB2;
				}
			}
			else{
				goto RETURN_LAB2;
			}				
		}
		else{//超时报警盲区数据发完
			if(g_sysmiscrun_struct.have_sysAlarm_flag == 2){
					g_sysmiscrun_struct.have_sysAlarm_flag = 3;	
					g_sysm_timeout_struct.upheart_count = g_propara_union.Item.HeartPeri;		//报警 结束给上传心跳标志
			}
		}

		
	}
	#ifdef SYS_TASK_DEBUG
		LocalUartFixedLenSend((uint8*)"\r\n g_sysmiscrun_struct.send_sysAlarm_numb = ", 44);
		str_len = sprintf(str_ch,"%d %.2x",g_sysmiscrun_struct.send_sysAlarm_numb, rx_buf[29]);
		LocalUartFixedLenSend((uint8*)str_ch,str_len);
//		LocalUartFixedLenSend((uint8*)"\r\n g_sysmiscrun_struct.send_sysAlarm_time[0].Item.second = ",59);
//		str_len = sprintf(str_ch,"%d ",g_sysmiscrun_struct.send_sysAlarm_time[0].Item.second);
//		LocalUartFixedLenSend((uint8*)str_ch,str_len);
//		LocalUartFixedLenSend((uint8*)"\r\n g_sysmiscrun_struct.send_sysAlarm_time[1].Item.second = ",59);
//		str_len = sprintf(str_ch,"%d ",g_sysmiscrun_struct.send_sysAlarm_time[1].Item.second);
//		LocalUartFixedLenSend((uint8*)str_ch,str_len);
	#endif
	
	return;
	
RETURN_LAB1:	
	g_sysalarm_struct.aftertailindex = (g_sysalarm_struct.aftertailindex+1)%SYS_ALARM_MAXINDEX;
	return;
RETURN_LAB2:	
	g_sysalarm_struct.beforetailindex = (g_sysalarm_struct.beforetailindex+1)%BEFORE_ALARM_MAXINDEX;
	return;
}


//=======================================================================================================//
/******************************************************
//其他逻辑函数
******************************************************/
/******************************************************
RTC时间获取函数
年 月 日 时 分 秒
flag 01从RTC取时间  02 从GPS取时间
******************************************************/
void GetCalendarTime(uint8 date_time[],uint8 flag)
{
	RTC_ST rtc_time;
	
  if(flag == 0x01){
		RtcGetTime(&rtc_time);
		
		date_time[0] = rtc_time.year;				///年
		date_time[1] = rtc_time.month;			///得到月份
		date_time[2] = rtc_time.day;  			///得到日期 	   
		date_time[3] = rtc_time.hour;   		///小时
		date_time[4] = rtc_time.minute; 		///分钟	
		date_time[5] = rtc_time.second; 		///秒钟
	}
	else{
		date_time[0] = g_gps_struct.gpsinform.subitem.year;			///年
		date_time[1] = g_gps_struct.gpsinform.subitem.month;		///得到月份
		date_time[2] = g_gps_struct.gpsinform.subitem.day;  		///得到日期 	   
		date_time[3] = g_gps_struct.gpsinform.subitem.hour;   	///小时
		date_time[4] = g_gps_struct.gpsinform.subitem.minute; 	///分钟	
		date_time[5] = g_gps_struct.gpsinform.subitem.second; 	///秒钟
	}
}

/******************************************************
//ADC 外电与电池电压检测 函数
******************************************************/
static void Terminal_PowerBat_ADC(void)
{
	static uint32 adc_val[2] = {0x00,0x00};
	static uint8  s_conv_counter = 0;
	
/// ADC		
	adc_val[0] += g_Adc1Dma2_buff[0];///PWR_C
	adc_val[1] += g_Adc1Dma2_buff[1];///BAT_ADC
	s_conv_counter++;
	if(s_conv_counter >= 5)
	{
		s_conv_counter = 0x00;
		adc_val[0] = adc_val[0] / 5;
		adc_val[0] = adc_val[0]*303/4096;						///外电电压值，单位0.1V
		adc_result[0] = adc_val[0];

		adc_val[1] = adc_val[1] / 5;
		adc_val[1] = adc_val[1]*48/4096;						///电池电压值，单位0.1V    3.3/(220/320)
		adc_result[1] = adc_val[1];									///
		
		#ifdef APP_SYSTASK_DEBUG
			DPrint("\n PWR_C = %d, BAT_ADC = %d \n", adc_result[0], adc_result[1]);
			DealDebugSend(1);
		#endif
	}
}

/******************************************************
//终端断外电再上外电重启
//
******************************************************/
static void TerminalMainPowerStatu(void)
{
 static uint8 main_power_statu=0;
 
	switch(main_power_statu){
		case 0x01:{//首次上电
			if(CHECK_POW() == LOW_STATE){
				main_power_statu = 2;
			}
			break;
		}
		case 0x02:{//断电
			if(CHECK_POW() == HIGH_STATE){
				//再次上电，复位
				#ifdef APP_SYSTASK_DEBUG
				DPrint("\n System will Reset now !!!\n");
				main_power_statu = 0;
				#endif
				SysReset();
			}			
			break;
		}
		default:
			if(CHECK_POW() == HIGH_STATE){
				main_power_statu = 1;				
			}			
		break;
	}
}

/******************************************************
//TCP链路状态检查
******************************************************/
static void Terminal_CheckTCPStatus(uint8 past_sec)
{
	switch(g_gprs_data_struct.CheckTCPStatus){
		case 0x00:
		case 0xFF:
			break;
		case 0x01:{
			#ifdef APP_SYSTASK_DEBUG
			DPrint("\n TCP CONNECTED \n");		
			DealDebugSend(1);
			#endif
			g_gprs_data_struct.CheckTCPStatus = 0xFF;
			g_sysmiscrun_struct.TCP_Disconnect_count = 0xFFFF;
			break;
		}
		case 0x02:{
			#ifdef APP_SYSTASK_DEBUG
			DPrint("\n TCP DISCONNECTED \n");		
			DealDebugSend(1);
			#endif
			if(	g_business_struct.usemodule==_YES && g_business_struct.needlinkser==_YES){
				g_sysmiscrun_struct.GPRSreconnect2plat   = 1; //置重连平台标志
				g_sysmiscrun_struct.TCP_Disconnect_count = 0;	//不能上线重启L218
			}
			g_gprs_data_struct.CheckTCPStatus = 0xFF;
			break;
		}
		default:
			break;
	}
	
	if(g_sysmiscrun_struct.TCP_Disconnect_count != 0xFFFF){
		g_sysmiscrun_struct.TCP_Disconnect_count += past_sec;
		if(g_sysmiscrun_struct.TCP_Disconnect_count  > 600){	//600s无连接，重启模块
			App_Module_Rest();			
			g_sysmiscrun_struct.TCP_Disconnect_count = 0xFFFF;
		}
	}
}

static void TerminalMiscPeriod(uint8 past_sec)
{		
	if(g_propara_union.Item.PAckTim!=0xFF && g_sysm_timeout_struct.PAckTim_count!=0xFF){
		if(++g_sysm_timeout_struct.PAckTim_count >= g_propara_union.Item.PAckTim){
			#ifdef APP_SYSTASK_DEBUG
			DPrint("平台超时 \n");		
			DealDebugSend(1);
			#endif
			g_sysm_timeout_struct.PAckTim_count = 0xFF;
		}
	}
}

static void TerminalNorUpPeriod(uint8 past_sec)
{			
	if(++g_sysm_timeout_struct.NorUpPeri_count >= g_propara_union.Item.NorUpPeri){	
		if(g_pro_struct.login_center_flag == TRUE){									//已经登录	
			if(g_sysmiscrun_struct.have_sysAlarm_flag==0 || g_sysmiscrun_struct.have_sysAlarm_flag==3){//没有报警	或超时 
				ProUpRealFormation(0x00);
			}
		}
		else{																												//未登录 存盲区
			if(g_gprs_data_struct.MserLinkStatus != GPRS_SERLINK_OK)	//未连接
				ProUpRealFormation(0x01);
		}
		g_sysm_timeout_struct.NorUpPeri_count = 0;
	}	
}

static void TerminalHeart(uint8 past_sec)
{
	if(++g_sysm_timeout_struct.upheart_count >= g_propara_union.Item.HeartPeri){	// 1s累加，发送心跳
		ProUpHeartBeat();
		g_sysm_timeout_struct.upheart_count = 0;
	}
}


static void TerminalWakeUp(void)
{
	
	
	if(0){//若唤醒出问题   复位
		SysReset();
	}
}
static void TerminalIntoSleep(void)
{
	uint8 rtc_counter;
	
	if(CHECK_POW() == HIGH_STATE){		return;		}			//有外电，不休眠

	//保存
	
	//关外电
	OFF_485232_PWR();
	OFF_CT5V_PWR();
//	OFF_GPRS_PWR();
		LOW_GPRS_DTR(); 	//休眠L218
	ADC_Cmd(ADC1, DISABLE);

	//进休眠
	
}


/******************************************************
//报警禁用功能
//发送过程中模块重登陆时，不能关掉周期发送函数
******************************************************/
static void AlarmDisableFunction(void)
{/*
///报警	
	if(g_sysmiscrun_struct.have_sysAlarm_flag==0 || g_sysmiscrun_struct.have_sysAlarm_flag==3){
		//无报警  报警无盲区
		g_sysm_on_off_struct.GPRSPeriodTx_switch 	= SYSM_ON;
		
	}
	else if(g_sysmiscrun_struct.have_sysAlarm_flag==1 || g_sysmiscrun_struct.have_sysAlarm_flag==2){
		//有报警 或 有报警盲区
		if(g_pro_struct.login_center_flag == TRUE){	//已经登录	
			g_sysm_on_off_struct.GPRSPeriodTx_switch 	= SYSM_OFF;
		}
		else {
			g_sysm_on_off_struct.GPRSPeriodTx_switch 	= SYSM_ON;
		}
	}	*/
}
/******************************************************
//报警判断
//输入：报警标志  00无 01有
******************************************************/
void AlarmHaveJudge(uint8 flag)
{		
	if(g_EnableCANAlarm == 0)   return;
	
	if(flag==1 ){			//报警
		g_sysmiscrun_struct.stop_sysAlarm_flag 	 = 0;	//恢复 报警取消标志
		if(g_sysmiscrun_struct.have_sysAlarm_flag==0){// || g_sysmiscrun_struct.have_sysAlarm_flag==3){
//			g_sysalarm_struct.aftertailindex = g_sysalarm_struct.afterheadindex;
			ExtFlashLsnalHeadTailSave();
			g_sysmiscrun_struct.have_sysAlarm_flag = 1;
			g_sysmiscrun_struct.have_sysAlarm_count  = 0;	//清报警发送计数
		}
	}
	else if(flag==0){	//正常
		g_sysmiscrun_struct.stop_sysAlarm_flag 	 = 1;	//置报警取消标志
		if(g_sysmiscrun_struct.have_sysAlarm_flag!=0 && g_sysmiscrun_struct.have_sysAlarm_flag!=3){
			g_sysmiscrun_struct.have_sysAlarm_flag = 2;  //标志检查有无盲区数据
		}
		else{
			g_sysmiscrun_struct.have_sysAlarm_flag = 0;				
		}
	}
}


/******************************************************
//CAN报文对终端的动作
//
******************************************************/
static void CAN2Terminal_Action(void)
{			
	//发送过程中出现报警取消 且发送完成  恢复报警标志
	if(g_sysmiscrun_struct.stop_sysAlarm_flag==1 && g_sysmiscrun_struct.have_sysAlarm_flag==3){
			g_sysmiscrun_struct.have_sysAlarm_flag = 0;	
	}
}

/******************************************************
//升级前的准备
******************************************************/	
void ReadyBeforeUpgrade(void)
{
	uint32 ftpflshadd;
	
///升级	
	if(g_sysmiscrun_struct.ProgramUpgrade_flag == 1){	
//		LocalUartFixedLenSend((uint8*)"正在升级\r\n", 10);
		//CAN
		OFF_CAN_PWR();			
//		CanDeInit();
		g_sysm_on_off_struct.canrx_switch 				= SYSM_OFF;
		//周期发送
		g_sysm_on_off_struct.GPRSPeriodTx_switch 	= SYSM_OFF;
		g_sysm_on_off_struct.flash_w_switch				= SYSM_OFF;
		
		for(ftpflshadd=FlASH_STORE_START_ADD; ftpflshadd < FLASH_STORE_END_ADD;){
			OSTimeDlyHMSM(0, 0, 0, 20);
			Tiza_FeedWdg(OPERATE_2WDGS);
			CpuFlashEraseSector(ftpflshadd);
			ftpflshadd += 0x20000;
		}	
		Tiza_FeedWdg(OPERATE_2WDGS);
		ftp_struct.ftp_upgrade_flag 						= 1;	
		g_sysmiscrun_struct.ProgramUpgrade_flag = 2;
	}
}
/******************************************************
//应用程序任务
******************************************************/
void OS_Timer0(OS_TMR *ptmr, void *p_arg)
{
	//1s软定时
	if((g_sysmiscrun_struct.tasks_initOK_flag.byte&0x0f) == 0x0F)
	{
		ProPeriodTx(1);															//周期发送函数	
		TerminalSavePeriod(1000);										//实时数据保存 未登录也要存
		TerminalNorUpPeriod(1);											//周期处理实时数据				
		if(g_pro_struct.login_center_flag == TRUE){	//已经登录		
			TerminalHeart(1);
			TerminalUpLsnalAlrm();										//报警时发送数据
		}
	}
	
}
//周期处理任务
void Period_task(void *pdata)
{
	OS_TMR *pTimer0 = 0;
	uint8   err = 0; 
	uint16 count_time;

#if QLJ_DEBUG == 1
//	memcpy(g_VIN,"A1234567890123456",17);		//暂保留
//	memcpy(g_VIN,"A0000015951621459",17);		//周姐测3
//	memcpy(g_VIN,"A0000013776516481",17);		//周姐测2 
//	memcpy(g_VIN,"A0000013515127369",17);		//周姐测1  
//	memcpy(g_VIN,"A0000013585107257",17);		//赵达军  
//	memcpy(g_VIN,"A0000013601464223",17);		//钱利军
//	memcpy(g_VIN,"A0000015261409343",17);		//2017.04.21发给长沙
	ProRead_IntFlash_VIN();										//读取VIN码
	#else	
	ProRead_IntFlash_VIN();										//读取VIN码
	#endif
	ProRead_SysPara();
	OSTimeDlyHMSM(0, 0, 0, 500);	
	Read_Lsnal_Index();
	OSTimeDlyHMSM(0, 0, 0, 500);

	ReadWriteOldVaildPostion(0);	//读取保存的位置信息
	
	 pTimer0 = OSTmrCreate(0,
                      OS_TMR_CFG_TICKS_PER_SEC,
                      OS_TMR_OPT_PERIODIC,
                      //OS_TMR_OPT_ONE_SHOT, 
                      (OS_TMR_CALLBACK)OS_Timer0,
                      (uint8*)0,
                      (uint8*)"OSTIMER0",
                      &err);
	OSTmrStart(pTimer0, &err); //启动定时器		
	
	
	g_sysmiscrun_struct.tasks_initOK_flag.bit.B2 = 1;
	while(1){
		OSTimeDlyHMSM(0, 0, 0, 200);	
		g_sysmiscrun_struct.task3_feeddog_count = 0;		///Period_task
		count_time = (count_time+1)%60000;
		
		
		
		
		
		GetCalendarTime(g_protime_union.arry, 0x01);							//读取时间 01/02 RTC/GPS
		if(count_time%600 == 2){//2min	
			if(g_sysmiscrun_struct.have_sysAlarm_flag==0 || g_sysmiscrun_struct.have_sysAlarm_flag==3){
				g_sysmiscrun_struct.need_check_time_flag = 0x01;			//无报警或报警超时  从GPS校时		
			}
			ReadWriteOldVaildPostion(1);														//保存定位有效时的位置	
		}

		
		
		
		if(count_time%5 == 3){//1	s						
			g_propostion_union.Item.status.byte = g_gps_struct.gpsinform.subitem.statu.byte & 0x07;	//定位状态
			g_propostion_union.Item.longitude   = g_gps_struct.gpsinform.subitem.longitude;
			g_propostion_union.Item.latitude    = g_gps_struct.gpsinform.subitem.latitude;
			
			CAN2Terminal_Action();											//CAN报文对终端的动作
			AlarmDisableFunction();											//报警禁用功能	
			
		//	Terminal_PowerBat_ADC();										//外电、电池电压检查
			TerminalMainPowerStatu();										//外电断开在上电复位
			Terminal_CheckTCPStatus(1); 								//TCP链路状态检查
			
		}
		
		
		//TerminalIntoSleep();				// qlj 休眠 待修改
			
	}
}

