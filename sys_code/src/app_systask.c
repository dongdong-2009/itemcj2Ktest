
#define APP_SYSTASK_GLOBAL
#define APP_SYSTASK_DEBUG

#include "tiza_include.h"
#include <os_cfg.h>
#include <os_cpu.h>

static void ProAlarmHeadTailSave(void);




void SysPara_Init(void)
{
	//======================================//
	g_sysm_timeout_struct.NorUpPeri_count = 0;							///���������ϴ���Ϣ��ʱ
	g_sysm_timeout_struct.upheart_count		= 0;							///ϵͳ�ϴ�������ʱ
//	sysm_timeout_struct.TAckTim_count		= 0;						///�ն�Ӧ��ʱʱ��		
	g_sysm_timeout_struct.PAckTim_count		= 0xFF;						///ƽ̨Ӧ��ʱʱ��		
	
//	//======================================//
//	g_sysm_on_off_struct.canrx_switch 				= SYSM_ON;
//	g_sysm_on_off_struct.GPRSPacketTx_switch	= SYSM_ON;	
//	g_sysm_on_off_struct.GPRSPeriodTx_switch 	= SYSM_ON;
//	g_sysm_on_off_struct.ADC_switch 					= SYSM_ON;
//	g_sysm_on_off_struct.extflash_w_switch		= SYSM_ON;
//	g_sysm_on_off_struct.flash_w_switch				= SYSM_ON;
//	
	//======================================//
	g_sysmiscrun_struct.have_sysAlarm_flag 	 = 0;						///ϵͳ������־ ͬʱsend_sysAlarm_count=0
	g_sysmiscrun_struct.stop_sysAlarm_flag 	 = 0;						//�ڷ��ͱ������ݹ����� ����ȡ����־  00��ʼ 01����ȡ��
	g_sysmiscrun_struct.have_sysAlarm_count  = 0;						///ϵͳ���ֱ�������ʱ
	g_sysmiscrun_struct.need_check_time_flag = 3;//1;						///��ҪУ��ϵͳ����ʱ��     01��GPS   02��ƽ̨  00��Ч
	g_sysmiscrun_struct.GPRSreconnect2plat   = 0;						///ģ��Ͽ�������������������		00��ʼ 01��Ҫ�Ͽ� 02��Ҫ���� 03�ȴ�����OK 04����OK 05����ģ��
	g_sysmiscrun_struct.TCP_Disconnect_count = 0xFFFF;			///�����������涨ʱ����û�������ϣ�����ģ��  FFFF��Ч

}

/******************************************************
//����GPRS�������ò���
******************************************************/
void SetGPRSNetPara(void){
//Port = 20000;//27055;//9903;//
	
	g_gprs_data_struct.SendDataLen=0;
  g_gprs_data_struct.SendData=(uint8 *)GPRStestdata;
	//��ҵƽ̨ 9903
	g_gprs_data_struct.IP[0] = g_propara_union.Item.Domain[0];
	g_gprs_data_struct.IP[1] = g_propara_union.Item.Domain[1];
	g_gprs_data_struct.IP[2] = g_propara_union.Item.Domain[2];
	g_gprs_data_struct.IP[3] = g_propara_union.Item.Domain[3];
	g_gprs_data_struct.Port = g_propara_union.Item.Port;
	//����ƽ̨ 20000
	g_gprs_data_struct.IP[0] = g_propara_union.Item.PDomain[0];
	g_gprs_data_struct.IP[1] = g_propara_union.Item.PDomain[1];
	g_gprs_data_struct.IP[2] = g_propara_union.Item.PDomain[2];
	g_gprs_data_struct.IP[3] = g_propara_union.Item.PDomain[3];
	g_gprs_data_struct.Port =  g_propara_union.Item.PPort;// 9903; //
	
	g_gprs_data_struct.setnetparaok_flag = 1;
}




/******************************************************
//�ն˶�ʱ����ʵʱ����
//����ʱʹ��
******************************************************/
static void ChargeSendTime(uint8 flag)
{
	//����30+30+10ʹ�ã��ݼ�ʱ���죨һ�㲻����ҹ��0���죩
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
	
	for(i=0; i < 31; i++){ // ����31��
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
	
	//�����������ȡ��FLASH  ������Ҫ��Ҫ����
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
	if(g_sysmiscrun_struct.have_sysAlarm_flag == 1){//�б���
//		ProPutIntoAlarm(NULL, 0, PRO_UP_REAL_INFO_ID);		//1sִ��һ�α���
		
		//������ʱ ֹͣ1s�����ϴ�תΪ�����ϴ�  ������1s�����ϴ����������������ע��
		if(++g_sysmiscrun_struct.have_sysAlarm_count >= SYS_HAVEALARM_COUNT){
			g_sysmiscrun_struct.have_sysAlarm_count = SYS_HAVEALARM_COUNT;
			g_sysmiscrun_struct.have_sysAlarm_flag = 2;				//��ʱ �ü�����ޱ���ä��
		}
	}
	else if(g_sysmiscrun_struct.have_sysAlarm_flag==0 || g_sysmiscrun_struct.have_sysAlarm_flag==3){	//�ޱ��� �� �ޱ���ä��
		if(g_sysmiscrun_struct.need_check_time_flag!=0x03){		//ʱ����Чʱ������
			ProPutIntoAlarm(NULL, 0, PRO_UP_LSNAL_INFO_ID);
		}
	}
}

static void TerminalUpLsnalAlrm(void)
{
	uint8  rx_buf[TMP_BUFF_MAXLEN-25],data[5];	//buff����֡ͷβ-25
	uint8  sum=0;
	uint16 len;
	uint32 addr;	
	#ifdef SYS_TASK_DEBUG
		char str_ch[10];
		uint8 str_len;
	#endif
	
	if(g_sysmiscrun_struct.have_sysAlarm_flag != 1 && g_sysmiscrun_struct.have_sysAlarm_flag != 2){		
		MarkSendTime();
		return;												//���Ǳ���  Ҳ  û�б���ä��
	}

	g_sysm_timeout_struct.upheart_count 		= 0;		//���� ʱ��������־
	if(g_sysmiscrun_struct.send_sysAlarm_numb <= 30 ){	//ʹ�÷���ʱ��ȷ����������ʱ������ 170504start
		len = Pro_RealTime_Data(&rx_buf[PRO_DATA_INDEX]);
		ProConstructFrameHead(rx_buf, len, PRO_UP_REAL_INFO_ID);	
		len += PRO_DATA_INDEX;	
		memcpy(&rx_buf[PRO_DATA_INDEX], g_sysmiscrun_struct.send_sysAlarm_time[1].arry, 6);		//����ʱ��		
		ProConstructFrameTail(rx_buf, len);
		ProPacket(rx_buf, len+1, PRO_UP_REAL_INFO_ID, FALSE, 0);
		
		ChargeSendTime(1);												//ʱ���1
		g_sysmiscrun_struct.send_sysAlarm_numb++;	//������1
	}																										//																	170504end
	else{		
//		g_sysmiscrun_struct.have_sysAlarm_flag = 2;
		if(g_sysalarm_struct.beforetailindex != g_sysalarm_struct.beforeheadindex){ //���ͱ���ǰ������				
			if(g_BeforeAlarmData[g_sysalarm_struct.beforetailindex][0]==VALID_VAL_2A && g_BeforeAlarmData[g_sysalarm_struct.beforetailindex][3]==PRO_UP_LSNAL_INFO_ID){
				len = (g_BeforeAlarmData[g_sysalarm_struct.beforetailindex][1]<<8) + g_BeforeAlarmData[g_sysalarm_struct.beforetailindex][2];
				sum = U8SumCheck(g_BeforeAlarmData[g_sysalarm_struct.beforetailindex], len+4);			
				
				if(g_BeforeAlarmData[g_sysalarm_struct.beforetailindex][len+4] == sum){//
					if(g_sysmiscrun_struct.send_sysAlarm_numb%10 !=0){ //�ж��Ƿ�Ҫ���뷢10sʵʱ����
						ProConstructFrameHead(rx_buf, len, PRO_UP_LSNAL_INFO_ID);
						memcpy(&rx_buf[PRO_DATA_INDEX], &g_BeforeAlarmData[g_sysalarm_struct.beforetailindex][4], len);
						len += PRO_DATA_INDEX;
						ChargeSendTime(0);																																		//ʱ���1  170504
						memcpy(&rx_buf[PRO_DATA_INDEX], g_sysmiscrun_struct.send_sysAlarm_time[0].arry, 6);		//����ʱ�� 170504
						ProConstructFrameTail(rx_buf, len);
						
						g_sysalarm_struct.beforetailindex = (g_sysalarm_struct.beforetailindex+1)%BEFORE_ALARM_MAXINDEX;
						ProPacket(rx_buf, len+1, PRO_UP_LSNAL_INFO_ID, FALSE, 0);//TRUE);
					}
					else{//����10��һ��ʵʱ����   170504start
						len = Pro_RealTime_Data(&rx_buf[PRO_DATA_INDEX]);
						ProConstructFrameHead(rx_buf, len, PRO_UP_REAL_INFO_ID);	
						len += PRO_DATA_INDEX;	
						ProConstructFrameTail(rx_buf, len);
						ProPacket(rx_buf, len+1, PRO_UP_REAL_INFO_ID, FALSE, 0);
					}
					g_sysmiscrun_struct.send_sysAlarm_numb++;	//������1
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
		else{//��ʱ����ä�����ݷ���
			if(g_sysmiscrun_struct.have_sysAlarm_flag == 2){
					g_sysmiscrun_struct.have_sysAlarm_flag = 3;	
					g_sysm_timeout_struct.upheart_count = g_propara_union.Item.HeartPeri;		//���� �������ϴ�������־
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
//�����߼�����
******************************************************/
/******************************************************
RTCʱ���ȡ����
�� �� �� ʱ �� ��
flag 01��RTCȡʱ��  02 ��GPSȡʱ��
******************************************************/
void GetCalendarTime(uint8 date_time[],uint8 flag)
{
	RTC_ST rtc_time;
	
  if(flag == 0x01){
		RtcGetTime(&rtc_time);
		
		date_time[0] = rtc_time.year;				///��
		date_time[1] = rtc_time.month;			///�õ��·�
		date_time[2] = rtc_time.day;  			///�õ����� 	   
		date_time[3] = rtc_time.hour;   		///Сʱ
		date_time[4] = rtc_time.minute; 		///����	
		date_time[5] = rtc_time.second; 		///����
	}
	else{
		date_time[0] = g_gps_struct.gpsinform.subitem.year;			///��
		date_time[1] = g_gps_struct.gpsinform.subitem.month;		///�õ��·�
		date_time[2] = g_gps_struct.gpsinform.subitem.day;  		///�õ����� 	   
		date_time[3] = g_gps_struct.gpsinform.subitem.hour;   	///Сʱ
		date_time[4] = g_gps_struct.gpsinform.subitem.minute; 	///����	
		date_time[5] = g_gps_struct.gpsinform.subitem.second; 	///����
	}
}

/******************************************************
//ADC ������ص�ѹ��� ����
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
		adc_val[0] = adc_val[0]*303/4096;						///����ѹֵ����λ0.1V
		adc_result[0] = adc_val[0];

		adc_val[1] = adc_val[1] / 5;
		adc_val[1] = adc_val[1]*48/4096;						///��ص�ѹֵ����λ0.1V    3.3/(220/320)
		adc_result[1] = adc_val[1];									///
		
		#ifdef APP_SYSTASK_DEBUG
			DPrint("\n PWR_C = %d, BAT_ADC = %d \n", adc_result[0], adc_result[1]);
			DealDebugSend(1);
		#endif
	}
}

/******************************************************
//�ն˶���������������
//
******************************************************/
static void TerminalMainPowerStatu(void)
{
 static uint8 main_power_statu=0;
 
	switch(main_power_statu){
		case 0x01:{//�״��ϵ�
			if(CHECK_POW() == LOW_STATE){
				main_power_statu = 2;
			}
			break;
		}
		case 0x02:{//�ϵ�
			if(CHECK_POW() == HIGH_STATE){
				//�ٴ��ϵ磬��λ
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
//TCP��·״̬���
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
				g_sysmiscrun_struct.GPRSreconnect2plat   = 1; //������ƽ̨��־
				g_sysmiscrun_struct.TCP_Disconnect_count = 0;	//������������L218
			}
			g_gprs_data_struct.CheckTCPStatus = 0xFF;
			break;
		}
		default:
			break;
	}
	
	if(g_sysmiscrun_struct.TCP_Disconnect_count != 0xFFFF){
		g_sysmiscrun_struct.TCP_Disconnect_count += past_sec;
		if(g_sysmiscrun_struct.TCP_Disconnect_count  > 600){	//600s�����ӣ�����ģ��
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
			DPrint("ƽ̨��ʱ \n");		
			DealDebugSend(1);
			#endif
			g_sysm_timeout_struct.PAckTim_count = 0xFF;
		}
	}
}

static void TerminalNorUpPeriod(uint8 past_sec)
{			
	if(++g_sysm_timeout_struct.NorUpPeri_count >= g_propara_union.Item.NorUpPeri){	
		if(g_pro_struct.login_center_flag == TRUE){									//�Ѿ���¼	
			if(g_sysmiscrun_struct.have_sysAlarm_flag==0 || g_sysmiscrun_struct.have_sysAlarm_flag==3){//û�б���	��ʱ 
				ProUpRealFormation(0x00);
			}
		}
		else{																												//δ��¼ ��ä��
			if(g_gprs_data_struct.MserLinkStatus != GPRS_SERLINK_OK)	//δ����
				ProUpRealFormation(0x01);
		}
		g_sysm_timeout_struct.NorUpPeri_count = 0;
	}	
}

static void TerminalHeart(uint8 past_sec)
{
	if(++g_sysm_timeout_struct.upheart_count >= g_propara_union.Item.HeartPeri){	// 1s�ۼӣ���������
		ProUpHeartBeat();
		g_sysm_timeout_struct.upheart_count = 0;
	}
}


static void TerminalWakeUp(void)
{
	
	
	if(0){//�����ѳ�����   ��λ
		SysReset();
	}
}
static void TerminalIntoSleep(void)
{
	uint8 rtc_counter;
	
	if(CHECK_POW() == HIGH_STATE){		return;		}			//����磬������

	//����
	
	//�����
	OFF_485232_PWR();
	OFF_CT5V_PWR();
//	OFF_GPRS_PWR();
		LOW_GPRS_DTR(); 	//����L218
	ADC_Cmd(ADC1, DISABLE);

	//������
	
}


/******************************************************
//�������ù���
//���͹�����ģ���ص�½ʱ�����ܹص����ڷ��ͺ���
******************************************************/
static void AlarmDisableFunction(void)
{/*
///����	
	if(g_sysmiscrun_struct.have_sysAlarm_flag==0 || g_sysmiscrun_struct.have_sysAlarm_flag==3){
		//�ޱ���  ������ä��
		g_sysm_on_off_struct.GPRSPeriodTx_switch 	= SYSM_ON;
		
	}
	else if(g_sysmiscrun_struct.have_sysAlarm_flag==1 || g_sysmiscrun_struct.have_sysAlarm_flag==2){
		//�б��� �� �б���ä��
		if(g_pro_struct.login_center_flag == TRUE){	//�Ѿ���¼	
			g_sysm_on_off_struct.GPRSPeriodTx_switch 	= SYSM_OFF;
		}
		else {
			g_sysm_on_off_struct.GPRSPeriodTx_switch 	= SYSM_ON;
		}
	}	*/
}
/******************************************************
//�����ж�
//���룺������־  00�� 01��
******************************************************/
void AlarmHaveJudge(uint8 flag)
{		
	if(g_EnableCANAlarm == 0)   return;
	
	if(flag==1 ){			//����
		g_sysmiscrun_struct.stop_sysAlarm_flag 	 = 0;	//�ָ� ����ȡ����־
		if(g_sysmiscrun_struct.have_sysAlarm_flag==0){// || g_sysmiscrun_struct.have_sysAlarm_flag==3){
//			g_sysalarm_struct.aftertailindex = g_sysalarm_struct.afterheadindex;
			ExtFlashLsnalHeadTailSave();
			g_sysmiscrun_struct.have_sysAlarm_flag = 1;
			g_sysmiscrun_struct.have_sysAlarm_count  = 0;	//�屨�����ͼ���
		}
	}
	else if(flag==0){	//����
		g_sysmiscrun_struct.stop_sysAlarm_flag 	 = 1;	//�ñ���ȡ����־
		if(g_sysmiscrun_struct.have_sysAlarm_flag!=0 && g_sysmiscrun_struct.have_sysAlarm_flag!=3){
			g_sysmiscrun_struct.have_sysAlarm_flag = 2;  //��־�������ä������
		}
		else{
			g_sysmiscrun_struct.have_sysAlarm_flag = 0;				
		}
	}
}


/******************************************************
//CAN���Ķ��ն˵Ķ���
//
******************************************************/
static void CAN2Terminal_Action(void)
{			
	//���͹����г��ֱ���ȡ�� �ҷ������  �ָ�������־
	if(g_sysmiscrun_struct.stop_sysAlarm_flag==1 && g_sysmiscrun_struct.have_sysAlarm_flag==3){
			g_sysmiscrun_struct.have_sysAlarm_flag = 0;	
	}
}

/******************************************************
//����ǰ��׼��
******************************************************/	
void ReadyBeforeUpgrade(void)
{
	uint32 ftpflshadd;
	
///����	
	if(g_sysmiscrun_struct.ProgramUpgrade_flag == 1){	
//		LocalUartFixedLenSend((uint8*)"��������\r\n", 10);
		//CAN
		OFF_CAN_PWR();			
//		CanDeInit();
		g_sysm_on_off_struct.canrx_switch 				= SYSM_OFF;
		//���ڷ���
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
//Ӧ�ó�������
******************************************************/
void OS_Timer0(OS_TMR *ptmr, void *p_arg)
{
	//1s��ʱ
	if((g_sysmiscrun_struct.tasks_initOK_flag.byte&0x0f) == 0x0F)
	{
		ProPeriodTx(1);															//���ڷ��ͺ���	
		TerminalSavePeriod(1000);										//ʵʱ���ݱ��� δ��¼ҲҪ��
		TerminalNorUpPeriod(1);											//���ڴ���ʵʱ����				
		if(g_pro_struct.login_center_flag == TRUE){	//�Ѿ���¼		
			TerminalHeart(1);
			TerminalUpLsnalAlrm();										//����ʱ��������
		}
	}
	
}
//���ڴ�������
void Period_task(void *pdata)
{
	OS_TMR *pTimer0 = 0;
	uint8   err = 0; 
	uint16 count_time;

#if QLJ_DEBUG == 1
//	memcpy(g_VIN,"A1234567890123456",17);		//�ݱ���
//	memcpy(g_VIN,"A0000015951621459",17);		//�ܽ��3
//	memcpy(g_VIN,"A0000013776516481",17);		//�ܽ��2 
//	memcpy(g_VIN,"A0000013515127369",17);		//�ܽ��1  
//	memcpy(g_VIN,"A0000013585107257",17);		//�Դ��  
//	memcpy(g_VIN,"A0000013601464223",17);		//Ǯ����
//	memcpy(g_VIN,"A0000015261409343",17);		//2017.04.21������ɳ
	ProRead_IntFlash_VIN();										//��ȡVIN��
	#else	
	ProRead_IntFlash_VIN();										//��ȡVIN��
	#endif
	ProRead_SysPara();
	OSTimeDlyHMSM(0, 0, 0, 500);	
	Read_Lsnal_Index();
	OSTimeDlyHMSM(0, 0, 0, 500);

	ReadWriteOldVaildPostion(0);	//��ȡ�����λ����Ϣ
	
	 pTimer0 = OSTmrCreate(0,
                      OS_TMR_CFG_TICKS_PER_SEC,
                      OS_TMR_OPT_PERIODIC,
                      //OS_TMR_OPT_ONE_SHOT, 
                      (OS_TMR_CALLBACK)OS_Timer0,
                      (uint8*)0,
                      (uint8*)"OSTIMER0",
                      &err);
	OSTmrStart(pTimer0, &err); //������ʱ��		
	
	
	g_sysmiscrun_struct.tasks_initOK_flag.bit.B2 = 1;
	while(1){
		OSTimeDlyHMSM(0, 0, 0, 200);	
		g_sysmiscrun_struct.task3_feeddog_count = 0;		///Period_task
		count_time = (count_time+1)%60000;
		
		
		
		
		
		GetCalendarTime(g_protime_union.arry, 0x01);							//��ȡʱ�� 01/02 RTC/GPS
		if(count_time%600 == 2){//2min	
			if(g_sysmiscrun_struct.have_sysAlarm_flag==0 || g_sysmiscrun_struct.have_sysAlarm_flag==3){
				g_sysmiscrun_struct.need_check_time_flag = 0x01;			//�ޱ����򱨾���ʱ  ��GPSУʱ		
			}
			ReadWriteOldVaildPostion(1);														//���涨λ��Чʱ��λ��	
		}

		
		
		
		if(count_time%5 == 3){//1	s						
			g_propostion_union.Item.status.byte = g_gps_struct.gpsinform.subitem.statu.byte & 0x07;	//��λ״̬
			g_propostion_union.Item.longitude   = g_gps_struct.gpsinform.subitem.longitude;
			g_propostion_union.Item.latitude    = g_gps_struct.gpsinform.subitem.latitude;
			
			CAN2Terminal_Action();											//CAN���Ķ��ն˵Ķ���
			AlarmDisableFunction();											//�������ù���	
			
		//	Terminal_PowerBat_ADC();										//��硢��ص�ѹ���
			TerminalMainPowerStatu();										//���Ͽ����ϵ縴λ
			Terminal_CheckTCPStatus(1); 								//TCP��·״̬���
			
		}
		
		
		//TerminalIntoSleep();				// qlj ���� ���޸�
			
	}
}

