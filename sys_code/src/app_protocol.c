#define APP_PROTOCOL_GLOBAL
#define APP_PROTOCOL_DEBUG


#define PRO0203_NEEEDACK   0

#include "tiza_include.h"

uint8 Packet_data[TMP_BUFF_MAXLEN];

/******************************************************
//Ӧ�ó��������ʼ��
******************************************************/
void ProPara_Init(void)
{
 	uint8 i;
	
	//======================================//
	//ʱ���ʼ��
	g_protime_union.Item.year 		= 17;
	g_protime_union.Item.month 		= 1;
	g_protime_union.Item.day 			= 1;
	g_protime_union.Item.hour 		= 1;
	g_protime_union.Item.minute 	= 1;
	g_protime_union.Item.second 	= 1;
	
	//======================================//
	g_provbattsys_union.Item.sigbatt_num   	 = PRO_SIGBATT_MAXNUMBER;// ���������ʼ��     ���CAN���
	g_protbattsys_union.Item.btprobe_num		 = PRO_BTPROBE_MAXNUMBER;// �¶�̽�������ʼ�� ���CAN���
	
	//======================================//
	g_proupgread_struct.flag 				= 0;							//����ӣ�1��Ҫ���� 2�������� 0��Ч
	
	g_pro_struct.link_center_flag 	= 0;							//����ƽ̨��־		1������		0δ����
	g_pro_struct.updata_sending 		= 1;							//�����ϴ���־		1�����ϴ� 0���� 
//	g_pro_struct.tx_lsnal_data_flag = FALSE;					//ä�������ݱ�־
	g_pro_struct.tx_one_lsnal_falg 	= 0;							//1:��ä���������ڷ���  0��Ч
	for(i=0; i<PRO_MAX_TX_BUF_ARRAY; i++){						//�����ڷ���buff
		g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;
	}
	
}

/******************************************************
//32960Э��ʹ�ò���Ĭ��ֵ
******************************************************/
void SetPara2FactoryReset(void)
{
	uint8 i ;
		
	g_propara_union.Item.SavePeri	 = 10000;	//10000ms
	g_propara_union.Item.NorUpPeri = 10;		//1~30s �����ϴ���Ϣ���
	g_propara_union.Item.AlrUpPeri = 1000;	//1000ms
	g_propara_union.Item.HeartPeri = 30;		//1~240s
	g_propara_union.Item.PAckTim	 = 30;		//ƽ̨��ʱ
	g_propara_union.Item.NLogTim	 = 10;		//����ʧ���ص�¼ʱ����min
	
	g_propara_union.Item.DomainLen = 4;										//ƽ̨��������									 										
	g_propara_union.Item.Domain[0] = 218;									//ƽ̨����	
	g_propara_union.Item.Domain[1] = 94;
	g_propara_union.Item.Domain[2] = 153;
	g_propara_union.Item.Domain[3] = 146;	
	g_propara_union.Item.Port			 = 9903;//27055;//			//ƽ̨�˿�	
	g_propara_union.Item.TAckTim	 = 0xFFFF;							//�ն�Ӧ��ʱʱ��
	g_propara_union.Item.PDomainLen = 4;									//ƽ̨��������									 										
	g_propara_union.Item.PDomain[0] = 218;								//����ƽ̨����	
	g_propara_union.Item.PDomain[1] = 94;
	g_propara_union.Item.PDomain[2] = 153;
	g_propara_union.Item.PDomain[3] = 146;	
	g_propara_union.Item.PPort			= 21000;//27055;//		//����ƽ̨�˿�	
	g_propara_union.Item.Monitor		= 0xFF;							//�Ƿ���������	
	
	for(i=4; i<PRO_DOMAINLEN_MAX;i++){
		g_propara_union.Item.Domain[i]  = 0x00;
		g_propara_union.Item.PDomain[i] = 0x00;
	}	
//	memcpy(g_propara_union.Item.g_para_HDVers,"Dh744",5);	
//	memcpy(g_propara_union.Item.g_para_SFVers,"Ds744",5);	
	memcpy(g_propara_union.Item.g_para_HDVers,"Dh700",5);	
	memcpy(g_propara_union.Item.g_para_SFVers,"Ds702",5);	
		
}

	

/******************************************************
//����ͨѶ����
//���ģʽ����λ��ǰ
******************************************************/	
static uint8 ProVehicle(uint8 data[])
{
	uint8 index=0;	
	
	data[index++] = (uint8)PRO_VEHICLE_FLAG;
	//0~19
	data[index++] = g_provehice_union.Item.vehicle;
	data[index++] = g_provehice_union.Item.charge;
	data[index++] = g_provehice_union.Item.mode;
	data[index++] = g_provehice_union.arry[5];	//speed
	data[index++] = g_provehice_union.arry[4];
	data[index++] = g_provehice_union.arry[3];	//mileage
	data[index++] = g_provehice_union.arry[2];
	data[index++] = g_provehice_union.arry[1];
	data[index++] = g_provehice_union.arry[0];
	data[index++] = g_provehice_union.arry[7];	//voltage
	data[index++] = g_provehice_union.arry[6];
	data[index++] = g_provehice_union.arry[9];	//current
	data[index++] = g_provehice_union.arry[8];	
	data[index++] = g_provehice_union.Item.SOC;
	data[index++] = g_provehice_union.Item.DC_statu;	
	data[index++] = g_provehice_union.Item.stall.byte;
	data[index++] = g_provehice_union.arry[11];	//resistance
	data[index++] = g_provehice_union.arry[10];	
	data[index++] = g_provehice_union.Item.Accelerate;
	data[index++] = g_provehice_union.Item.braking;
	
	return index;
}
static uint8 ProMotor(uint8 data[])
{
	uint8 index=0,i;
	
	data[index++] = (uint8)PRO_MOTOR_FLAG;
	data[index++] = (uint8)PRO_MOTOR_NUMBER;
	//0~12 *i
	for(i=0;i < PRO_MOTOR_NUMBER;i++){
		data[index++] = g_promotor_union[i].Item.serial;
		data[index++] = g_promotor_union[i].Item.status;
		data[index++] = g_promotor_union[i].Item.ctr_tem;
		data[index++] = g_promotor_union[i].arry[5];	//rpm
		data[index++] = g_promotor_union[i].arry[4];
		data[index++] = g_promotor_union[i].arry[7];	//torque
		data[index++] = g_promotor_union[i].arry[6];
		data[index++] = g_promotor_union[i].Item.motor_tem;
		data[index++] = g_promotor_union[i].arry[9];	//ctr_in_vol
		data[index++] = g_promotor_union[i].arry[8];	
		data[index++] = g_promotor_union[i].arry[11];	//ctr_bus_cur
		data[index++] = g_promotor_union[i].arry[10];	
	}
	
	return index;
}
#if 0
// ����
static uint8 ProFuelCell(uint8 data[])
{
	uint8 index=0;
	
	data[index++] = (uint8)PRO_FUELCELL_FLAG;
	//0~18 + i
	data[index++] = g_profullcell_union.arry[1];	//voltage
	data[index++] = g_profullcell_union.arry[0];
	data[index++] = g_profullcell_union.arry[3];	//current
	data[index++] = g_profullcell_union.arry[2];
	data[index++] = g_profullcell_union.arry[5];	//consumpt
	data[index++] = g_profullcell_union.arry[4];
	g_profullcell_union.Item.tprobe_num = PRO_FTPROBE_NUMBER;
	data[index++] = g_profullcell_union.arry[7];	//tprobe_num
	data[index++] = g_profullcell_union.arry[6];	
	memcpy(&data[index],g_TProbe_val,g_profullcell_union.Item.tprobe_num);
	index += g_profullcell_union.Item.tprobe_num;	
	data[index++] = g_profullcell_union.arry[9];	//maxtem_val
	data[index++] = g_profullcell_union.arry[8];	
	data[index++] = g_profullcell_union.Item.maxtem_id;
	data[index++] = g_profullcell_union.arry[11];	//maxconcent
	data[index++] = g_profullcell_union.arry[10];	
	data[index++] = g_profullcell_union.Item.maxconcent_id;
	data[index++] = g_profullcell_union.arry[13];	//maxpre_val
	data[index++] = g_profullcell_union.arry[12];	
	data[index++] = g_profullcell_union.Item.maxpre_id;
	data[index++] = g_profullcell_union.Item.preDCstatu;
	
	return index;
}
static uint8 ProEngine(uint8 data[])
{
	uint8 index=0;
	
	data[index++] = (uint8)PRO_ENGINE_FLAG;
	//0~4
	data[index++] = g_proengine_union.Item.status;
	data[index++] = g_proengine_union.arry[1];	//rpm
	data[index++] = g_proengine_union.arry[0];
	data[index++] = g_proengine_union.arry[3];	//consumpt
	data[index++] = g_proengine_union.arry[2];
	
	return index;
}
#endif
static uint8 ProPostion(uint8 data[])
{
	uint8 index=0;
	
	data[index++] = (uint8)PRO_POSITION_FLAG;
	//0~8
	data[index++] = g_propostion_union.Item.status.byte;
	data[index++] = g_propostion_union.arry[3];	//longitude
	data[index++] = g_propostion_union.arry[2];
	data[index++] = g_propostion_union.arry[1];
	data[index++] = g_propostion_union.arry[0];
	data[index++] = g_propostion_union.arry[7];	//latitude
	data[index++] = g_propostion_union.arry[6];
	data[index++] = g_propostion_union.arry[5];
	data[index++] = g_propostion_union.arry[4];
	
	return index;
}
static uint8 ProExtreme(uint8 data[])
{
	uint8 index=0;
	
	data[index++] = (uint8)PRO_EXTREME_FLAG;
	//0~13
	data[index++] = g_proextreme_union.Item.maxvolsys_id;
	data[index++] = g_proextreme_union.Item.maxvolbat_id;
	data[index++] = g_proextreme_union.arry[3];	//maxvolbat_val
	data[index++] = g_proextreme_union.arry[2];
	data[index++] = g_proextreme_union.Item.minvolsys_id;
	data[index++] = g_proextreme_union.Item.minvolbat_id;
	data[index++] = g_proextreme_union.arry[7];	//minvolbat_val
	data[index++] = g_proextreme_union.arry[6];
	data[index++] = g_proextreme_union.Item.maxtemsys_id;
	data[index++] = g_proextreme_union.Item.maxtempro_id;
	data[index++] = g_proextreme_union.Item.maxtempro_val;
	data[index++] = g_proextreme_union.Item.mintemsys_id;
	data[index++] = g_proextreme_union.Item.mintempro_id;
	data[index++] = g_proextreme_union.Item.mintempro_val;
	
	return index;
}
static uint8 ProAlarm(uint8 data[])
{
	uint8 index=0,i;
	
	if(g_EnableCANAlarm == 0){						// qlj  ����ʱ����CAN����  ʹ���ֶ�����
		if(g_sysmiscrun_struct.have_sysAlarm_flag == 1){
			g_proalarm_union.Item.rank = 0x03;
			g_proalarm_union.Item.flag.dword = 0x00001000;		//DC-DC�¶�
		}
		else if(g_sysmiscrun_struct.have_sysAlarm_flag == 0){
			g_proalarm_union.Item.rank = 0x00;
			g_proalarm_union.Item.flag.dword = 0x00000000;		//DC-DC�¶�
		
		}
	}
	
	data[index++] = (uint8)PRO_ALARM_FLAG;
	//9+4(x+y+z+a)
	data[index++] = g_proalarm_union.Item.rank;
	data[index++] = g_proalarm_union.arry[3];	//flag
	data[index++] = g_proalarm_union.arry[2];
	data[index++] = g_proalarm_union.arry[1];
	data[index++] = g_proalarm_union.arry[0];
	data[index++] = g_proalarm_union.Item.storagenum;
	for(i=0;i < g_proalarm_union.Item.storagenum;i++){
		#if 0
		data[index++] = (uint8)((g_storage_alr[i]>>24) & 0xFF);	
		data[index++] = (uint8)((g_storage_alr[i]>>16) & 0xFF);
		data[index++] = (uint8)((g_storage_alr[i]>> 8) & 0xFF);
		data[index++] = (uint8)( g_storage_alr[i]      & 0xFF);
		#else
		data[index++] = (uint8)((g_storage_alr[0]>>24) & 0xFF);	
		data[index++] = (uint8)((g_storage_alr[0]>>16) & 0xFF);
		data[index++] = (uint8)((g_storage_alr[0]>> 8) & 0xFF);
		data[index++] = (uint8)( g_storage_alr[0]      & 0xFF);
		#endif
	}
	data[index++] = g_proalarm_union.Item.motornum;
	for(i=0;i < g_proalarm_union.Item.motornum;i++){
		#if 0
		data[index++] = (uint8)((g_motro_alr[i]>>24) & 0xFF);	
		data[index++] = (uint8)((g_motro_alr[i]>>16) & 0xFF);
		data[index++] = (uint8)((g_motro_alr[i]>> 8) & 0xFF);
		data[index++] = (uint8)( g_motro_alr[i]      & 0xFF);
		#else
		data[index++] = (uint8)((g_motro_alr[0]>>24) & 0xFF);	
		data[index++] = (uint8)((g_motro_alr[0]>>16) & 0xFF);
		data[index++] = (uint8)((g_motro_alr[0]>> 8) & 0xFF);
		data[index++] = (uint8)( g_motro_alr[0]      & 0xFF);
		#endif
	}
	data[index++] = g_proalarm_union.Item.enginenum;
	for(i=0;i < g_proalarm_union.Item.enginenum;i++){
		#if 0
		data[index++] = (uint8)((g_engine_alr[i]>>24) & 0xFF);	
		data[index++] = (uint8)((g_engine_alr[i]>>16) & 0xFF);
		data[index++] = (uint8)((g_engine_alr[i]>> 8) & 0xFF);
		data[index++] = (uint8)( g_engine_alr[i]      & 0xFF);
		#else
		data[index++] = 0;
		data[index++] = 0;
		data[index++] = 0;
		data[index++] = 0;
		#endif
	}
	data[index++] = g_proalarm_union.Item.othersnum;
	for(i=0;i < g_proalarm_union.Item.othersnum;i++){
		#if 0
		data[index++] = (uint8)((g_others_alr[i]>>24) & 0xFF);	
		data[index++] = (uint8)((g_others_alr[i]>>16) & 0xFF);
		data[index++] = (uint8)((g_others_alr[i]>> 8) & 0xFF);
		data[index++] = (uint8)( g_others_alr[i]      & 0xFF);
		#else
		data[index++] = (uint8)((g_others_alr[0]>>24) & 0xFF);	
		data[index++] = (uint8)((g_others_alr[0]>>16) & 0xFF);
		data[index++] = (uint8)((g_others_alr[0]>> 8) & 0xFF);
		data[index++] = (uint8)( g_others_alr[0]      & 0xFF);
		#endif
	}
	
	return index;
}
static uint8 ProBVoltage(uint8 data[])
{
	uint8 index=0,i,j;
	
	data[index++] = (uint8)PRO_BVOLTAGE_FLAG;
	data[index++] = (uint8)PRO_BATTSYS_NUMBER;
	
	g_provbattsys_union.Item.framebatt_id		 = 1;
	
	if(g_provbattsys_union.Item.sigbatt_num > PRO_SIGBATT_MAXNUMBER){
		g_provbattsys_union.Item.sigbatt_num = PRO_SIGBATT_MAXNUMBER;
	}
//	if(g_provbattsys_union.Item.framebatt_num <= PRO_SIGBATT_MAXNUMBER){  //��֡���200 ���ﲻ����96
		g_provbattsys_union.Item.framebatt_num = g_provbattsys_union.Item.sigbatt_num;
//	}
//	else{
//		g_provbattsys_union.Item.framebatt_num = PRO_SIGBATT_MAXNUMBER;
//	}
	//0~12 *i
	for(i=0;i < PRO_BATTSYS_NUMBER;i++){
		g_provbattsys_union.Item.serial = i + 1;
		data[index++] = g_provbattsys_union.Item.serial;
		data[index++] = g_provbattsys_union.arry[3];	//voltage
		data[index++] = g_provbattsys_union.arry[2];
		data[index++] = g_provbattsys_union.arry[5];	//current
		data[index++] = g_provbattsys_union.arry[4];
		data[index++] = g_provbattsys_union.arry[7];	//sigbatt_num
		data[index++] = g_provbattsys_union.arry[6];
		data[index++] = g_provbattsys_union.arry[9];	//framebatt_id
		data[index++] = g_provbattsys_union.arry[8];
		data[index++] = g_provbattsys_union.Item.framebatt_num;
		for(j=0; j < g_provbattsys_union.Item.framebatt_num; j++){
			data[index++] = (uint8)(g_sigbatt_val[i][j] >> 8);
			data[index++] = (uint8)(g_sigbatt_val[i][j] & 0xff);
		}
	}
	
	return index;
}
static uint8 ProBTemperature(uint8 data[])
{
	uint8 index=0,i;
	
	data[index++] = (uint8)PRO_BTEMPERATURE_FLAG;
	data[index++] = (uint8)PRO_BATTSYS_NUMBER;
	//0~12 *i  PRO_BTPROBE_MAXNUMBER
	for(i=0;i < PRO_BATTSYS_NUMBER;i++){
		g_protbattsys_union.Item.serial = i + 1;
		data[index++] = g_protbattsys_union.Item.serial;
		data[index++] = g_protbattsys_union.arry[1];	//btprobe_num
		data[index++] = g_protbattsys_union.arry[0];
		if(g_protbattsys_union.Item.btprobe_num > PRO_BTPROBE_MAXNUMBER){
			g_protbattsys_union.Item.btprobe_num = PRO_BTPROBE_MAXNUMBER;
		}			
		memcpy(&data[index],g_btprobe_val[i],g_protbattsys_union.Item.btprobe_num);
		index += g_protbattsys_union.Item.btprobe_num;//С��Խ��
	}
	
	return index;
}

uint16 Pro_RealTime_Data(uint8 data[])
{
	uint16 len;
	uint8  *p;
	
	p = data;

	memcpy(p,g_protime_union.arry,6);		//ʱ��
	p += 6;
	
	p += ProVehicle(p);					//01		21
	p += ProMotor(p);						//02		14
	#if 0
	p += ProFuelCell(p);				//03		
	p += ProEngine(p);					//04
	#endif
	p += ProPostion(p);					//05		10
	p += ProExtreme(p);					//06		15
	p += ProAlarm(p);						//07		10
	p += ProBVoltage(p);				//08		204
	p += ProBTemperature(p);		//09		29
	
	len = p - data;
	return len;
}


	//	02 ʵʱ��Ϣ
/*********************************************
���� 00�������� 01��ä��
*********************************************/
void ProUpRealFormation(uint8 mode)	
{
	uint8 tx_buf[TMP_BUFF_MAXLEN-25],*p;	//buff����֡ͷβ-25
	uint16 tx_len;
	
	p = tx_buf;		
	
	#if PRO0203_NEEEDACK==1
	
	p += Pro_RealTime_Data(p);	

	tx_len = p - tx_buf;
	if(tx_len > (TMP_BUFF_MAXLEN-25)) tx_len = TMP_BUFF_MAXLEN-25;
	ProPacket(tx_buf, tx_len, PRO_UP_REAL_INFO_ID, TRUE, mode);
	
	#else
	tx_len = Pro_RealTime_Data(&tx_buf[PRO_DATA_INDEX]);
	if(tx_len > (TMP_BUFF_MAXLEN-25)) tx_len = TMP_BUFF_MAXLEN-25;
	
	if(mode == 0){	
		ProConstructFrameHead(tx_buf, tx_len, PRO_UP_REAL_INFO_ID);	
		tx_len += PRO_DATA_INDEX;	
		ProConstructFrameTail(tx_buf, tx_len);
		ProPacket(tx_buf, tx_len+1, PRO_UP_REAL_INFO_ID, FALSE, mode);
	
	}
	else if(mode == 1){	
		ProPacket(tx_buf, tx_len, PRO_UP_REAL_INFO_ID, FALSE, mode);
	}
	
	#endif
}
	// 03 ä����������
static void ProUpLsnalFormation(void)	
{
	uint8  tmpdata[APP_EF_EVERYLSNAL_SIZE1],index;
	uint16 tmplen;
	uint32 basicaddr;

	index = PRO_DATA_INDEX - 4;
	
	if(g_pro_struct.tx_one_lsnal_falg == 1)	{//��ä���������ڷ���
		goto RETURN_LAB;
	}
	
	if(g_syslsnal_struct.tailindex != g_syslsnal_struct.headindex){
		basicaddr = APP_EF_LSNAL_START_ADD + g_syslsnal_struct.tailindex * APP_EF_EVERYLSNAL_SIZE1;
		//if(ExteFlashRead(basicaddr, &tmpdata[index], APP_EF_EVERYLSNAL_SIZE1) == 0){   ʵ������С��512-25
		if(ExteFlashRead(basicaddr, &tmpdata[index], APP_EF_EVERYLSNAL_SIZE1-PRO_DATA_INDEX-1) == 0){
			goto RETURN_LAB;
		}
		
		tmplen = (tmpdata[index+1] << 8) + tmpdata[index+2] + 4;//����У��
		if(tmpdata[index+0]!=VALID_VAL_2A || tmplen>=512){
		// ���������־���� Խ�� �ص�=head
			g_syslsnal_struct.tailindex = g_syslsnal_struct.headindex;
			ExtFlashLsnalHeadTailSave();
			goto RETURN_LAB;
		}		
		if(tmpdata[index+tmplen] != U8SumCheck(&tmpdata[index], tmplen)){
			goto RETURN_LAB;
		}	
		
		#if PRO0203_NEEEDACK==1
		
		ProPacket(&tmpdata[index+4], tmplen-4, PRO_UP_LSNAL_INFO_ID,TRUE, 0); 
		
		g_pro_struct.tx_one_lsnal_falg = 1;
		
		#else
		tmplen -= 4;
		ProConstructFrameHead(tmpdata, tmplen, PRO_UP_LSNAL_INFO_ID);	
		tmplen += PRO_DATA_INDEX;	
		ProConstructFrameTail(tmpdata, tmplen);
		ProPacket(tmpdata, tmplen+1, PRO_UP_LSNAL_INFO_ID, FALSE, 0);

		g_syslsnal_struct.tailindex = (g_syslsnal_struct.tailindex+1)%SYS_LSNAL_SPIMAXINDEX;
		ExtFlashLsnalHeadTailSave();				
		g_pro_struct.tx_one_lsnal_falg = 0; 
		#endif
		
	}
RETURN_LAB:
	return;
}
	// 01 ����
void ProUpLogin(void)
{
	uint8 tx_buf[231],tx_len;	//buff����֡ͷβ-25
	uint16 login_number;
	
	memcpy(tx_buf,g_protime_union.arry,6);		//ʱ��
	//������ˮ�� 1~65531
	login_number = (g_prologin_union.Item.login_number+1)%MAX65531;
	if(login_number  == 0) g_prologin_union.Item.login_number = MAX65531;	
	else									 g_prologin_union.Item.login_number = login_number;		
	tx_buf[6] = g_prologin_union.arry[1]; 		//���ģʽ����λ��ǰ
	tx_buf[7] = g_prologin_union.arry[0]; 
	//������¼��Ϣ
	memcpy(g_prologin_union.Item.SIM_ICCID, g_dri_ICCID, 20);	
	memcpy(&tx_buf[8],&g_prologin_union.arry[2],22);		
	//�ɳ�索��ϵͳ���� ��ʱ������
	
	tx_len = 6 + 24;	
	ProPacket(tx_buf,tx_len,PRO_UP_LOGIN_ID,TRUE, 0);
}
	// 04 �ǳ�
void ProUpLogout(void)
{
	uint8 tx_buf[231],tx_len;	//buff����֡ͷβ-25
	
	memcpy(tx_buf,g_protime_union.arry,6);		//ʱ��
	//������ˮ�� 1~65531	 g_prologin_union.Item.login_number
	tx_buf[6] = g_prologin_union.arry[1]; 		//���ģʽ����λ��ǰ
	tx_buf[7] = g_prologin_union.arry[0]; 
	
	tx_len = 8;	
	ProPacket(tx_buf,tx_len,PRO_UP_LOGOUT_ID,TRUE, 0);	
}
	// 07 ����	
void ProUpHeartBeat(void)
{
	uint8 tx_buf[50],tx_len;
	
	ProPacket(tx_buf, 0, (uint8)PRO_UP_HEARTBEAT_ID,TRUE, 0);
}
	// 08 ��ƽ̨Уʱ	
void ProUpCheckTime(void)
{
	uint8 tx_buf[50],tx_len;
	
	ProPacket(tx_buf, 0, (uint8)PRO_UP_CHECKTIME_ID,TRUE, 0);
}

///Ӧ��
	//	������ѯ																						
static uint16 ProUpQueryParams(uint8 data[], uint8 len, uint8* tx_buf)	
{
	uint16 tx_len;
	uint8  i,*p;
	p = tx_buf;
	
	if(len == 0){//�޲�������
		goto RETURN_LAB;
	}
		
	*p++ = len; //����������
	for(i=1;i <= len;i++){
		switch(data[i]){
			case PRO_PARA_SAVEPERI_ID:{
				*p++ = (uint8)PRO_PARA_SAVEPERI_ID;
				*p++ = (uint8)(g_propara_union.Item.SavePeri >> 8);
				*p++ = (uint8)(g_propara_union.Item.SavePeri & 0xFF);				
				break;
			}
			case PRO_PARA_NORUPPERI_ID:{
				*p++ = (uint8)PRO_PARA_NORUPPERI_ID;
				*p++ = (uint8)(g_propara_union.Item.NorUpPeri >> 8);
				*p++ = (uint8)(g_propara_union.Item.NorUpPeri & 0xFF);
				break;
			}
			case PRO_PARA_ALRUPPERI_ID:{
				*p++ = (uint8)PRO_PARA_ALRUPPERI_ID;
				*p++ = (uint8)(g_propara_union.Item.AlrUpPeri >> 8);
				*p++ = (uint8)(g_propara_union.Item.AlrUpPeri & 0xFF);
				break;
			}
			case PRO_PARA_DOMAINLEN_ID:{
				*p++ = (uint8)PRO_PARA_DOMAINLEN_ID;
				*p++ = g_propara_union.Item.DomainLen;	
				break;
			}
			case PRO_PARA_DOMAIN_ID:{
				*p++ = (uint8)PRO_PARA_DOMAIN_ID;//ƽ̨����
				memcpy(p,g_propara_union.Item.Domain,g_propara_union.Item.DomainLen);		
				p +=  g_propara_union.Item.DomainLen;
				break;
			}
			case PRO_PARA_PORT_ID:{
				*p++ = (uint8)PRO_PARA_PORT_ID;
				*p++ = (uint8)(g_propara_union.Item.Port >> 8);
				*p++ = (uint8)(g_propara_union.Item.Port & 0xFF);
				break;
			}
			case PRO_PARA_HDVERSION_ID:{
				*p++ = (uint8)PRO_PARA_HDVERSION_ID;//Ӳ���汾
				memcpy(p,g_propara_union.Item.g_para_HDVers,5);		
				p += 5;
				break;
			}
			case PRO_PARA_SFVERSION_ID:{
				*p++ = (uint8)PRO_PARA_SFVERSION_ID;//����汾
				memcpy(p,g_propara_union.Item.g_para_SFVers,5);	
				p += 5;	
				break;
			}
			case PRO_PARA_HEARTPERI_ID:{
				*p++ = (uint8)PRO_PARA_HEARTPERI_ID;
				*p++ = g_propara_union.Item.HeartPeri;
				break;
			}
			case PRO_PARA_T_ACK_TIM_ID:{
				*p++ = (uint8)PRO_PARA_T_ACK_TIM_ID;
				*p++ = (uint8)(g_propara_union.Item.TAckTim >> 8);
				*p++ = (uint8)(g_propara_union.Item.TAckTim & 0xFF);
				break;
			}
			case PRO_PARA_P_ACK_TIM_ID:{
				*p++ = (uint8)PRO_PARA_P_ACK_TIM_ID;
				*p++ = (uint8)(g_propara_union.Item.PAckTim >> 8);
				*p++ = (uint8)(g_propara_union.Item.PAckTim & 0xFF);
				break;
			}
			case PRO_PARA_N_LOG_TIM_ID:{
				*p++ = (uint8)PRO_PARA_N_LOG_TIM_ID;
				*p++ = g_propara_union.Item.NLogTim;
				break;
			}
			case PRO_PARA_PDOMAINLEN_ID:{
				*p++ = (uint8)PRO_PARA_PDOMAINLEN_ID;
				*p++ = g_propara_union.Item.PDomainLen;
				break;
			}
			case PRO_PARA_PDOMAIN_ID:{
				*p++ = (uint8)PRO_PARA_PDOMAIN_ID;//����ƽ̨����
				memcpy(p,g_propara_union.Item.PDomain,g_propara_union.Item.PDomainLen);		
				p +=  g_propara_union.Item.PDomainLen;
				break;
			}
			case PRO_PARA_PPORT_ID:{
				*p++ = (uint8)PRO_PARA_PPORT_ID;
				*p++ = (uint8)(g_propara_union.Item.PPort >> 8);
				*p++ = (uint8)(g_propara_union.Item.PPort & 0xFF);
				break;
			}
			case PRO_PARA_MONITOR_ID:{
				*p++ = (uint8)PRO_PARA_MONITOR_ID;
				*p++ = g_propara_union.Item.Monitor;
				break;
			}
			
			default: break;
		}
	}
	
	tx_len = p - tx_buf;
	if(tx_len > 200) tx_len = 200;
	return tx_len;
	
RETURN_LAB:
	return 0;
}

static void ProUpAck(uint8 data[], uint16 len, uint8 flag)
{
	uint8  tx_buf[225];	
	
	memcpy(tx_buf, data, len);
	tx_buf[3]   = flag;
	ProConstructFrameTail(tx_buf, len);
	ProPacket(tx_buf, len+1, PRO_UP_REAL_INFO_ID, FALSE, 0);
}


/******����ͨѶ����******/

static void ProCtrUpgrade(uint8 data[],uint8 len)
{
	 //��;���ָ���ĳ������û����Ϊ��
	//URL��ַ;���ŵ�����;�����û���;��������;��ַ;�˿�;�����̴���;Ӳ���汾;�̼��汾;���ӵ�����������ʱ��
	uint8 res=0,i,j=0,index[PRO_UPGRADE_PARA_NUM];
	uint8  tmpdata[40];
	
	for(i=0; i < len; i++){
		if(data[i] == ';'){
			index[j++] = i;
		}
	}
	index[j++] = len;
	if(j!=PRO_UPGRADE_PARA_NUM){
		goto RETURN_LAB;
	}
	
	if(index[0]){
		memcpy(g_proupgread_struct.file_path, data, index[0]);
		g_proupgread_struct.file_path[index[0]] = 0; //���ռӽ�����
		res++;
	}	
	i = index[0] + 1;//��ʼ�±�
	j = index[1] - i;//����
	
	if(j){
		memcpy(g_proupgread_struct.dial_APN, &data[i], j);
		g_proupgread_struct.dial_APN[j] = 0; //���ռӽ�����
		res++;
	}
	i = index[1] + 1;//��ʼ�±�
	j = index[2] - i;//����
	if(j){
		memcpy(g_proupgread_struct.user_name, &data[i], j);
		g_proupgread_struct.user_name[j] = 0; //���ռӽ�����
		res++;
	}				
	i = index[2] + 1;//��ʼ�±�
	j = index[3] - i;//����
	if(j){
		memcpy(g_proupgread_struct.pass_word, &data[i], j);
		g_proupgread_struct.pass_word[j] = 0; //���ռӽ�����
		res++;
	}				
	i = index[3] + 1;//��ʼ�±�
	j = index[4] - i;//����
	if(j>=4){
		if(j==4){
			g_proupgread_struct.ip_domain[0] = 0;
			g_proupgread_struct.ip_domain[1] = 0;
			memcpy(&g_proupgread_struct.ip_domain[2], &data[i], j);
		}
		else{
			memcpy(g_proupgread_struct.ip_domain, &data[i], j);
		}
		res++;
	}				
	i = index[4] + 1;//��ʼ�±�
	j = index[5] - i;//����
	if(j==2){
		g_proupgread_struct.port = (data[i]<<8) + data[i+1]; 
		res++;
	}				
	i = index[5] + 1;//��ʼ�±�
	j = index[6] - i;//����
	if(j==4){//�ַ���
		g_proupgread_struct.terminal_id = (data[i]<<24) +(data[i+1]<<16) +(data[i+2]<<8) + data[i+3]; 
		res++;
	}				
	i = index[6] + 1;//��ʼ�±�
	j = index[7] - i;//����
	if(j>=4){
		memcpy(g_proupgread_struct.HDVers, &data[i], 5);
		res++;
	}
	i = index[7] + 1;//��ʼ�±�
	j = index[8] - i;//����
	if(j>=5){
		memcpy(g_proupgread_struct.SFVers, &data[i], 5);
		res++;
	}	
	i = index[8] + 1;//��ʼ�±�
	j = index[9] - i;//����
	if(j==2){
		g_proupgread_struct.overtime = (data[i]<<8) + data[i+1]; 
		res++;
	}
	
	memcpy(tmpdata, data - 31, 30);
	tmpdata[22] = 0;
	tmpdata[23] = 6;
	if(res == 10){
		ProUpAck(tmpdata, 30, PRO_ACK_SUCCEED_FLAG);//ACK
		g_sysmiscrun_struct.ProgramUpgrade_flag = 1;
	}
	else{
		ProUpAck(tmpdata, 30, PRO_ACK_FAILED_FLAG);// 
	}
	
	
RETURN_LAB:
	return;
}
static void ProDownAck(uint8 data[],uint16 len)
{
	uint8 i,j,num,tx_cmd,rx_cmd,rx_ack;
	#ifdef PROTOCOL_DEBUG
		char str_ch[10];
		uint8 str_len;
	#endif

	rx_cmd = data[PRO_CMD_INDEX];
	rx_ack = data[PRO_ACK_INDEX];
	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){//��ѯ�ȽϷ���ָ����Ӧ��ָ��ж�Ӧ���־
		tx_cmd = g_pro_struct.tx_struct.re_tx_buf[i][PRO_CMD_INDEX];
		if((tx_cmd==rx_cmd)&&(g_pro_struct.tx_struct.re_tx_full_flag[i]==TRUE)){
			
			if(rx_ack!=PRO_ACK_SUCCEED_FLAG){
				#ifdef PROTOCOL_DEBUG
					LocalUartFixedLenSend((uint8*)"\r\n ACK ERROR ID = ", 18);
					str_len = sprintf(str_ch,"%d ",rx_ack);
					LocalUartFixedLenSend((uint8*)str_ch,str_len);
				#endif
				//goto RETURN_LAB;
			}

			switch(rx_cmd){
				#if PRO0203_NEEEDACK==1
				case PRO_UP_REAL_INFO_ID:
				case PRO_UP_LSNAL_INFO_ID:
				#endif
				case PRO_UP_LOGIN_ID:
				case PRO_UP_LOGOUT_ID:{																	//Ӧ���ʱ���
//					num = 0;
//					for(j=0;j<6;j++){
//						if(data[PRO_DATA_INDEX+j] == g_pro_struct.tx_struct.re_tx_buf[i][PRO_DATA_INDEX+j]){
//							num++;
//						}
//					}
//					if(num == 6){																					//ʱ��������У����ȷ
						g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;	//ȡ�����ͻ����־
						goto RETURN_LAB1;
//					}
//					
//				break;
				}
				case PRO_UP_HEARTBEAT_ID:{														//����ʱ��ľͲ�У����			
					
					g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;	//ȡ�����ͻ����־
					goto RETURN_LAB1;
//				break;
				}
				case PRO_UP_CHECKTIME_ID:{
					g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;	//ȡ�����ͻ����־
					goto RETURN_LAB1;
//				break;
				}
				
				default : break;
			} 
			
		}
	}
	if(i == PRO_MAX_TX_BUF_ARRAY){
		goto RETURN_LAB;
	}
	
RETURN_LAB1:
	g_sysm_timeout_struct.PAckTim_count = 0xFF;			//��ƽ̨��ʱ��ʱ
	
	switch(rx_cmd){	
		case PRO_UP_LOGIN_ID:{
			g_pro_struct.try_login_statu = 3;
			g_sysm_timeout_struct.NLogTim_count = 0xFFFF;		//	��Ч
			g_pro_struct.login_center_flag = TRUE;
			break;
		}
		case PRO_UP_LOGOUT_ID:{
			g_pro_struct.try_login_statu = 5;
			g_pro_struct.login_center_flag  = FALSE;
			break;
		}
		case PRO_UP_CHECKTIME_ID:{
//			g_sysmiscrun_struct.need_check_time_flag = 0x02;			//��ƽ̨Уʱ
			break;
		}		
		#if PRO0203_NEEEDACK==1
		case PRO_UP_LSNAL_INFO_ID:{				
			if(g_sysm_on_off_struct.GPRSPeriodTx_switch == SYSM_ON){
//				if(g_pro_struct.tx_lsnal_data_flag == FALSE){
					//��ͨä������
					g_syslsnal_struct.tailindex = (g_syslsnal_struct.tailindex+1)%SYS_LSNAL_SPIMAXINDEX;
					ExtFlashLsnalHeadTailSave();				
					g_pro_struct.tx_one_lsnal_falg = 0; 
//				}
			}
			break;
		}
		#endif
		default:{
			break;
		}
	}
	
//	g_pro_struct.tx_lsnal_data_flag = TRUE;
	
RETURN_LAB:
	return;
}
static void ProDownParaQuiry(uint8 data[],uint8 len)
{
	uint8  tx_buf[225];	//buff����֡ͷβ-25
	uint16 tx_len;
	uint8  tmp_num;
	
	tmp_num = data[PRO_DATA_INDEX+6];
	memcpy(tx_buf, data, PRO_DATA_INDEX+6);		//�������� ��ʱ��
	tx_len = ProUpQueryParams(data+PRO_DATA_INDEX+6, tmp_num, &tx_buf[PRO_DATA_INDEX+6]);
	if(tx_len){
		tx_len += 6;
		tx_buf[22] = (uint8)(tx_len >> 8);
		tx_buf[23] = (uint8)(tx_len & 0xFF);
		tx_len += PRO_DATA_INDEX;
		ProUpAck(tx_buf, tx_len, PRO_ACK_SUCCEED_FLAG);		
	}
	else{
		tx_buf[22] = 0;
		tx_buf[23] = 6;
		ProUpAck(tx_buf, tx_len, PRO_ACK_FAILED_FLAG);		
	}
}
static void ProDownSetPara(uint8 data[],uint8 len)
{
	uint8  *p,tx_buf[40];
	uint8  tmp_num,i,tmp_id;//,tx_len;
	uint16 tmp;
	
	memcpy(tx_buf, data, PRO_DATA_INDEX+6);
	tmp_num = data[PRO_DATA_INDEX+6];
	p = data+PRO_DATA_INDEX+7;
	for(i=0;i < tmp_num;i++){
		tmp_id = *p++;
		switch(tmp_id){
			case PRO_PARA_SAVEPERI_ID:{
				tmp = *p++;
				tmp = (tmp<<8) + *p++;
				if(tmp<=60000 || tmp==0xFFFF)
					g_propara_union.Item.SavePeri = tmp;		
				else 
					goto RETURN_LAB;
				break;
			}
			case PRO_PARA_NORUPPERI_ID:{
				tmp = *p++;
				tmp = (tmp<<8) + *p++;
				if((tmp>=1 && tmp<=600) || tmp==0xFFFF)
					g_propara_union.Item.NorUpPeri = tmp;		
				else 
					goto RETURN_LAB;		
				break;
			}
			case PRO_PARA_ALRUPPERI_ID:{
				tmp = *p++;
				tmp = (tmp<<8) + *p++;
				if(tmp<=60000 || tmp==0xFFFF)
					g_propara_union.Item.AlrUpPeri = tmp;		
				else 
					goto RETURN_LAB;		
				break;
			}
			case PRO_PARA_DOMAINLEN_ID:{
				tmp = *p++;
				if(tmp <= PRO_DOMAINLEN_MAX)
					g_propara_union.Item.DomainLen = tmp;		
				else 
					goto RETURN_LAB;		
				break;
			}
			case PRO_PARA_DOMAIN_ID:{
				#if 0 
 				for(tmp=0;tmp < PRO_DOMAINLEN_MAX;tmp++){
					if(*(p+tmp) == 0){// qlj �������������������ַ�����0��β?
						p += (tmp+1);
						break;
					}
					tmp_data[tmp] = *(p+tmp);
				}
				if(tmp != PRO_DOMAINLEN_MAX){
					g_propara_union.Item.DomainLen = tmp;
					memcpy(g_propara_union.Item.Domain, tmp_data, g_propara_union.Item.DomainLen);	
				} 
				#else
				//����������ǰ�����������ȿ���ֱ����
				if(g_propara_union.Item.DomainLen <= PRO_DOMAINLEN_MAX){	//��ֹԽ��
					memcpy(g_propara_union.Item.Domain, p, g_propara_union.Item.DomainLen);
					p += g_propara_union.Item.DomainLen;
				}
				#endif
				else 
					goto RETURN_LAB;		
				break;
			}
			case PRO_PARA_PORT_ID:{
				tmp = *p++;
				tmp = (tmp<<8) + *p++;
				if(tmp<=65531 || tmp==0xFFFF)
					g_propara_union.Item.Port = tmp;		
				else 
					goto RETURN_LAB;		
				break;
			}
			case PRO_PARA_HEARTPERI_ID:{
				tmp = *p++;
				if((tmp>=1 && tmp<=240) || tmp==0xFF)
					g_propara_union.Item.HeartPeri = tmp;		
				else 
					goto RETURN_LAB;		
				break;
			}
			case PRO_PARA_T_ACK_TIM_ID:{
				tmp = *p++;
				tmp = (tmp<<8) + *p++;
				if((tmp>=1 && tmp<=600) || tmp==0xFFFF)
					g_propara_union.Item.TAckTim = tmp;		
				else 
					goto RETURN_LAB;			
				break;
			}
			case PRO_PARA_P_ACK_TIM_ID:{
				tmp = *p++;
				tmp = (tmp<<8) + *p++;
				if((tmp>=1 && tmp<=600) || tmp==0xFFFF)
					g_propara_union.Item.PAckTim = tmp;		
				else 
					goto RETURN_LAB;	
				break;
			}
			case PRO_PARA_N_LOG_TIM_ID:{
				tmp = *p++;
				if((tmp>=1 && tmp<=240) || tmp==0xFF)
					g_propara_union.Item.NLogTim = tmp;		
				else 
					goto RETURN_LAB;	
				break;
			}
			case PRO_PARA_PDOMAINLEN_ID:{
				tmp = *p++;
				if(tmp <= PRO_DOMAINLEN_MAX) 
					g_propara_union.Item.PDomainLen = tmp;		
				else 
					goto RETURN_LAB;		
				break;
			}
			case PRO_PARA_PDOMAIN_ID:{
				#if 0
 				for(tmp=0;tmp < PRO_DOMAINLEN_MAX;tmp++){
					if(*(p+tmp) == 0){// qlj �������������������ַ�����0��β?
						p += (tmp+1);
						break;
					}
					tmp_data[tmp] = *(p+tmp);
				}
				if(tmp != PRO_DOMAINLEN_MAX){
					g_propara_union.Item.PDomainLen = tmp;
					memcpy(g_propara_union.Item.PDomain, tmp_data, g_propara_union.Item.PDomainLen);	
				} 
				#else
				//����������ǰ�����������ȿ���ֱ����
				if(g_propara_union.Item.PDomainLen <= PRO_DOMAINLEN_MAX){	//��ֹԽ��
					memcpy(g_propara_union.Item.PDomain, p, g_propara_union.Item.PDomainLen);
					p += g_propara_union.Item.PDomainLen;
				}
				#endif
				else 
					goto RETURN_LAB;	
				break;
			}
			case PRO_PARA_PPORT_ID:{
				tmp = *p++;
				tmp = (tmp<<8) + *p++;
				if(tmp<=65531|| tmp==0xFFFF)
					g_propara_union.Item.PPort = tmp;		
				else 
					goto RETURN_LAB;	
				break;
			}
			case PRO_PARA_MONITOR_ID:{
				tmp = *p++;
				if((tmp>=1 && tmp<=2) || tmp==0xFF)
					g_propara_union.Item.Monitor = tmp;		
				else 
					goto RETURN_LAB;	
				break;
			}
			
			
			case PRO_PARA_HDVERSION_ID:{				
				memcpy(g_propara_union.Item.g_para_HDVers, p, 5);
				p += 5;				
				break;
			}
			case PRO_PARA_SFVERSION_ID:{				
				memcpy(g_propara_union.Item.g_para_SFVers, p, 5);
				p += 5;				
				break;
			}
			
			default:	
				goto RETURN_LAB;	
			//break;
		}
	}
	tx_buf[22] = 0;
	tx_buf[23] = 6;
 	ProUpAck(tx_buf, 30, PRO_ACK_SUCCEED_FLAG);// ACK
	ProWrite_SysPara();		//����
	SysReset();						//��λ
	return;	
	
RETURN_LAB:
	tx_buf[22] = 0;
	tx_buf[23] = 6;
 	ProUpAck(tx_buf, 30, PRO_ACK_FAILED_FLAG);// ACK
	return;	
}
static void ProDownControl(uint8 data[],uint8 len)	
{
	uint8  tmp_id,tx_buf[40];
	uint16 tmp;
	
	tx_buf[22] = 0;
	tx_buf[23] = 6;
	memcpy(tx_buf, data, PRO_DATA_INDEX+6);
	
	tmp = PRO_DATA_INDEX+1 +7;	//25+6+1 ���ٶ����ֽ�
	tmp_id = data[PRO_DATA_INDEX+6];	//ʱ��6 ����1
	
	switch(tmp_id){
		case PRO_CTR_UPGRADE_ID:{	
			if(len > (tmp+24)){
				ProCtrUpgrade(&data[PRO_DATA_INDEX+7], len-tmp);
			}			
			return;
		}
		case PRO_CTR_OFFTERM_ID:{		
			ProUpAck(tx_buf, 30, PRO_ACK_SUCCEED_FLAG);// ACK
			//�ر��ն�
			return;
		}
		case PRO_CTR_RSTTERM_ID:{	
			ProUpAck(tx_buf, 30, PRO_ACK_SUCCEED_FLAG);// ACK
			SysReset();						//����ä��&��λ
			return;
		}
		case PRO_CTR_FACTRST_ID:{		
			ProUpAck(tx_buf, 30, PRO_ACK_SUCCEED_FLAG);// ACK
			//��û��Ҫ�����
			//�ָ���������
			SetPara2FactoryReset();
			ProWrite_SysPara();
			SysReset();						//����ä��&��λ
			return;
		}
		case PRO_CTR_DISLINKSER_ID:{	
			ProUpAck(tx_buf, 30, PRO_ACK_SUCCEED_FLAG);// ACK
			//��û��Ҫ�����
			//�Ͽ�����	
			return;
		}
		case PRO_CTR_ALRTERM_ID:{		
			ProUpAck(tx_buf, 30, PRO_ACK_SUCCEED_FLAG);// ACK
			// qlj ��ô��������
			return;
		}
		case PRO_CTR_MONITOR_ID:{	
			ProUpAck(tx_buf, 30, PRO_ACK_SUCCEED_FLAG);// ACK
			g_propara_union.Item.Monitor = 0x01;	//�ó�����־
			// qlj ��ôȡ����־������ʱҪ����Щ������
			return;
		}		
		default:	 break;
	}
	
 	ProUpAck(tx_buf, 30, PRO_ACK_FAILED_FLAG);// ҪACK
}


/*******�������********/
void ProConstructFrameHead(uint8 data[],uint16 tx_len,uint8 cmd)
{
	//���ģʽ����λ��ǰ
	uint8 *p = data;

	*p++ = 0x23;									//'#'
	*p++ = 0x23;									//'#'
	*p++ = cmd;										//�����ʶ
	*p++ = PRO_ACK_CMD_FLAG;			//������֡
	memcpy(p,g_VIN,17);
	p += 17;
	*p++ = PRO_DISENCRYT_MODE;
	*p++ = (uint8)(tx_len >> 8);		//���ݵ�Ԫ����
	*p++ = (uint8)(tx_len & 0xFF);	
	//���ݵ�Ԫ
	//У����
	
	//g_pro_struct.tx_seq++;
}
void ProConstructFrameTail(uint8 data[],uint16 tx_len)
{
	//���֡β
	data[tx_len] = XorCheck(&data[2],tx_len-2);	//BCC���У��
}

/******************************************************
���д������
���룺tx_data ������
			tx_len  �����ݳ���
			tx_cmd	����ָ��
			ack_flag		Ӧ���־ 00 ����ҪӦ��  01 ��ҪӦ�� 
			mode		00�������� 01��ä��
******************************************************/

void ProPacket(uint8 tx_data[],uint16 tx_len,uint8 tx_cmd,uint8 ack_flag, uint8 mode)
{
	uint8 i,*p;//, tmp_data[TMP_BUFF_MAXLEN];
	uint16 tmp_len;

	p = Packet_data;

	
 	if(mode == 0x01){																					//��ä��
		goto LSNAL_LAB;
	} 
	
	if(ack_flag == 0)
	{
		if(g_gprs_data_struct.sendDataStatus == GPRS_SENDDATA_IDLE){
			g_gprs_data_struct.SendDataLen = tx_len;
			memcpy(GPRStestdata, tx_data, g_gprs_data_struct.SendDataLen);
			g_pro_struct.updata_noacksend = 1;			
			
			if(g_EnableHeart == 0){	
				g_sysm_timeout_struct.upheart_count 		= 0;		///����������ʱ��0
			}
			g_sysm_timeout_struct.PAckTim_count				= 0;		///ƽ̨��ʱ��ʱ
		}
		goto RETURN_LAB;
	}
	
	ProConstructFrameHead(p,tx_len,tx_cmd);
	p += PRO_DATA_INDEX;
	MemCpy(p,tx_data,tx_len);
	tmp_len = PRO_DATA_INDEX + tx_len;
	ProConstructFrameTail(Packet_data,tmp_len);
	tmp_len += 1;		//��һ��У����
	
	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){
		if(g_pro_struct.tx_struct.re_tx_full_flag[i] == FALSE){
			break;
		}
	}
	if(i == PRO_MAX_TX_BUF_ARRAY){
		if(ack_flag){
			goto LSNAL_LAB;
		}
	}
	
	MemCpy(g_pro_struct.tx_struct.re_tx_buf[i],Packet_data,tmp_len);
	switch(g_pro_struct.tx_struct.re_tx_buf[i][2]){
		case PRO_UP_LOGIN_ID:{
			g_pro_struct.tx_struct.re_tx_time[i] = g_propara_union.Item.NLogTim;//
			break;
		}	
		case PRO_UP_REAL_INFO_ID:
		case PRO_UP_LSNAL_INFO_ID:
		case PRO_UP_LOGOUT_ID:
		case PRO_UP_HEARTBEAT_ID:
		case PRO_UP_CHECKTIME_ID:{
			g_pro_struct.tx_struct.re_tx_time[i] = PRO_NORMAL_RETX_TIME;//
			break;
		}
		default : break;
	}
	g_pro_struct.tx_struct.re_tx_len[i] = tmp_len;
	g_pro_struct.tx_struct.re_tx_counter[i] = 0;
	g_pro_struct.tx_struct.re_tx_sec_counter[i] = g_pro_struct.tx_struct.re_tx_time[i];
	g_pro_struct.tx_struct.re_tx_full_flag[i] = TRUE;
	return;
	
LSNAL_LAB:
		PutIntoExtFlashLsnal(tx_data,tx_len,tx_cmd);//������

RETURN_LAB:
	return;
}
///���ڷ���
void ProPeriodTx(uint16 past_sec)
{
	uint16 i;
	static uint16 count;
	#ifdef PRO_DEBUG
		char str_ch[10];
		uint8 str_len;
		uint16 j;
	#endif
	
	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){										//��ѯ�ط�buff
		if(g_pro_struct.tx_struct.re_tx_sec_counter[i]>=g_pro_struct.tx_struct.re_tx_time[i] && g_pro_struct.tx_struct.re_tx_time[i]!=0){ //�ط�����		
			g_pro_struct.tx_struct.re_tx_sec_counter[i] = 0;
			if(g_pro_struct.tx_struct.re_tx_full_flag[i] == TRUE){			//������
				if(g_pro_struct.tx_struct.re_tx_counter[i] < 3){  //�ط�С��3��		
					if(g_pro_struct.updata_sending == 0){
						g_pro_struct.tx_struct.re_tx_counter[i]++;			

						if(g_gprs_data_struct.sendDataStatus == GPRS_SENDDATA_IDLE){
							g_gprs_data_struct.SendDataLen = g_pro_struct.tx_struct.re_tx_len[i];
							memcpy(GPRStestdata, g_pro_struct.tx_struct.re_tx_buf[i], g_gprs_data_struct.SendDataLen);
							g_pro_struct.updata_sending = 1;				
							g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_OUT;
						}
						if(g_EnableHeart == 0){	
							g_sysm_timeout_struct.upheart_count 		= 0;		///����������ʱ��0
						}
						g_sysm_timeout_struct.PAckTim_count				= 0;		///ƽ̨��ʱ��ʱ
					}
					break;
				}
				else{
					if(g_pro_struct.tx_struct.re_tx_buf[i][2] == PRO_UP_LOGIN_ID){
						g_pro_struct.tx_struct.re_tx_buf[i][2] = 0;	//�巢��buf
						g_sysm_timeout_struct.NLogTim_count = 0;			//��Ǳ���3�ε�¼ʧ��
					}
					else{	
						g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;
						PutIntoExtFlashLsnal(g_pro_struct.tx_struct.re_tx_buf[i]+PRO_DATA_INDEX,
										g_pro_struct.tx_struct.re_tx_len[i]-PRO_DATA_INDEX-1,
										g_pro_struct.tx_struct.re_tx_buf[i][PRO_CMD_INDEX]);
					}
				}
			}
		}
	}
	
	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){//��ѯ�ط�buff���������ݵ�buff�����
		if(g_pro_struct.tx_struct.re_tx_full_flag[i] == TRUE){
			g_pro_struct.tx_struct.re_tx_sec_counter[i] += past_sec;
		}
	}
	
	
///�������ä�����ݷ���
	if(g_pro_struct.login_center_flag == TRUE){
		#if PRO0203_NEEEDACK==1
		ProUpLsnalFormation();
		#else
		count += past_sec;
		if(count >= 5){	// qlj ���ڿɸĳɿɵ���
			count = 0;
			ProUpLsnalFormation();
		}
		#endif
	}
	
///�Ѿ����ӵ�����ƽ̨
///��ʼ��¼ƽ̨
	if(g_pro_struct.link_center_flag == 1){
		if(g_pro_struct.try_login_statu == 1){			//ʹ�ܵ�½
			g_pro_struct.try_login_statu = 2;
			ProUpLogin();
		}
		else if(g_pro_struct.try_login_statu == 2){	//���ڵ�½
			if(g_sysm_timeout_struct.NLogTim_count < g_propara_union.Item.NLogTim*60){				
				//ǰ3�ε�¼ʧ�ܣ���ʼ�ص�¼30min��ʱ
				g_sysm_timeout_struct.NLogTim_count += past_sec;
				if(g_sysmiscrun_struct.GPRSreconnect2plat == 0x04) {
					g_sysmiscrun_struct.GPRSreconnect2plat = 0x05;
				}
			}
			else if(g_sysm_timeout_struct.NLogTim_count == 0xFFFF){	//������һ�Σ���Чʱ������Ҫ��ʱ
			}
			else {																								// 30min�����ص�¼
				g_pro_struct.try_login_statu = 1;										
				g_sysm_timeout_struct.NLogTim_count = 0xFFFF;					//��Ϊ��Ч
			}
		}
	}
	
}



/******************************************************
Э�鴦������
����	data	������
			len		�����ݳ���
			colon	��ser���յı�־���ȣ�0x23ǰ����������
******************************************************/
void ProProcess(uint8 data[], uint16 len, uint16 colon)
{
	uint8* p;
	uint16 tmp_len;
	
	p = data;
RETURN_LAB:
	if(p[0]!='#' || p[1]!='#' ){							//��ʼ�����ԣ���������
		goto RETURN_LAB2;
	}
	tmp_len  = p[PRO_DATALEN_INDEX];
	tmp_len += p[PRO_DATALEN_INDEX+1];
	tmp_len += 25;
	if(tmp_len > len){												//���Ȳ��� tmp_lenԤ�ⳤ�ȣ�lenʵ�ʽ��ճ���
		goto RETURN_LAB1;
	}
	if(p[tmp_len - 1] != XorCheck(&p[2], tmp_len-3)){		//У�鲻�ԣ���������
		goto RETURN_LAB2;
	}
	ReadOverTailIndex(colon+tmp_len);					//�±��ƶ�����������
	
	if(p[PRO_ACK_INDEX] != 0xFE){
		ProDownAck(p,tmp_len);
	}
	else{
		#ifdef TIZA_L218OPT_DEBUG
			DPrint("\n ProProcess ProDown ID = ",p[PRO_CMD_INDEX]);
			DealDebugSend(1);
		#endif
	
		switch(p[PRO_CMD_INDEX]){
			case PRO_DOWN_INQUIRY_ID:{
				ProDownParaQuiry(p,tmp_len);
				break;
			}
			case PRO_DOWN_SET_ID:{
				ProDownSetPara(p,tmp_len);
				break;
			}
			case PRO_DOWN_CONTROL_ID:{
				ProDownControl(p,tmp_len);
				break;
			}		
			default:			break;
		}
	}
	
	
	len  -= tmp_len;													//��һ���к�����Ӧ��
	if(len > 25){		
		p = &data[colon+tmp_len];
		colon = 0;
		goto RETURN_LAB;
	}
	
RETURN_LAB1:
	return;
	
RETURN_LAB2:	
	ReadOverTailIndex(colon);								//�±��ƶ�����Ч���ݿ�ͷ0x23 ɾ����ͷ��+RECEIVE...��
	return;
}

