#define APP_CAN_GLOBAL
//#define APP_CAN_DEBUG

#include "tiza_include.h"




APP_CAN1ID_STRUCT	g_app_can1id_struct = {
	0x00,
	//-------------------------------------//	
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 
	
	//-------------------------------------//	
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	
	//-------------------------------------//
	CAN1_R_0101, CAN1_R_0102, CAN1_R_0103, CAN1_R_0104, CAN1_R_0105, CAN1_R_0106, CAN1_R_0107, CAN1_R_0108, CAN1_R_0109, CAN1_R_0110,
	CAN1_R_0111, CAN1_R_0112, CAN1_R_0113, CAN1_R_0114, CAN1_R_0115, CAN1_R_0116, CAN1_R_0117, CAN1_R_0118, CAN1_R_0119, CAN1_R_0120,
	CAN1_R_0121, CAN1_R_0122, CAN1_R_0123, CAN1_R_0124, CAN1_R_0201, CAN1_R_0202, CAN1_R_0203, CAN1_R_10  , CAN1_R_11  , CAN1_R_12  , 
	CAN1_R_13  , CAN1_R_14  , CAN1_R_15  , CAN1_R_16  , CAN1_R_17  , CAN1_R_18  , CAN1_R_19  , CAN1_R_20  , CAN1_R_20  , CAN1_R_22  , 
	CAN1_R_23  , CAN1_R_24  , CAN1_R_25  , CAN1_R_27  ,																								
																											CAN1_R_26  , CAN1_R_03  , CAN1_R_04  , CAN1_R_05  , CAN1_R_06  , CAN1_R_07  ,
	CAN1_R_08  , CAN1_R_09  , 
	
};


uint8 g_Tmp_VIN_codes[3][8] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		
};

static void Can1Rx_Analyse(void);


/******************************************************
//CAN1 ��ʼ��һϵ�к���
******************************************************/
void App_Can1Init(void)
{
	
	#if CAN1_NEED_FILTER == 1
		CAN1_Switch_FilterSetInit();	
	#endif
	
	
	Can1OpenPort(250000);					//��CAN1
	
	#if CAN1_NEED_FILTER == 1	
	///���ö����˲��� CAN1_INDEP_INDEX_START~CAN1_FILTER_ALL_NUM 20~28    ��=10~13
	/// 							 APP_INDEP_INDEX_START~APP_CAN1ID_ALL_NUM  44~52
	Can1FilterSet(10, g_app_can1id_struct.Rec_canid[44], g_app_can1id_struct.Rec_canid[45], CAN_FilterMode_IdList);
	Can1FilterSet(11, g_app_can1id_struct.Rec_canid[46], g_app_can1id_struct.Rec_canid[47], CAN_FilterMode_IdList);
	Can1FilterSet(12, g_app_can1id_struct.Rec_canid[48], g_app_can1id_struct.Rec_canid[49], CAN_FilterMode_IdList);
	Can1FilterSet(13, g_app_can1id_struct.Rec_canid[50], g_app_can1id_struct.Rec_canid[51], CAN_FilterMode_IdList);
	
	#else
	///ʹ�������˲��������˲�	
//	Can1FilterSet(0, 0x1800D1F3, 0xFF80FFFF, CAN_FilterMode_IdMask);  //	
//	Can1FilterSet(1, CAN1_R_26,  CAN1_R_03 , CAN_FilterMode_IdList);
	
	Can1FilterSet(0, CAN1R_GROUP0_MATCH, CAN1R_GROUP0_SHIELD, CAN_FilterMode_IdMask);  //	
	Can1FilterSet(1, CAN1R_GROUP1_MATCH, CAN1R_GROUP1_SHIELD, CAN_FilterMode_IdMask);  //	
	Can1FilterSet(2, CAN1R_GROUP2_MATCH, CAN1R_GROUP2_SHIELD, CAN_FilterMode_IdMask);  //	
	Can1FilterSet(3, CAN1R_GROUP3_MATCH, CAN1R_GROUP3_SHIELD, CAN_FilterMode_IdMask);  //	
	Can1FilterSet(4, CAN1R_GROUP4_MATCH, CAN1R_GROUP4_SHIELD, CAN_FilterMode_IdMask);  //	

	Can1FilterSet(10, CAN1R_SIGLE0,  CAN1R_SIGLE1 , CAN_FilterMode_IdList);
	Can1FilterSet(11, CAN1R_SIGLE2,  CAN1R_SIGLE2 , CAN_FilterMode_IdList);


	#endif
	
	
	g_cans_bssic_struct.can1_rx_timeout = 0;
}

/******************************************************
//Ӧ�ò���CAN���ͺ���
******************************************************/
void Can1Tx(void)
{
	uint8 heart_data[8] = {0xFD,0x20,0x00,0x00,0x00,0x00,0x00,0x00};
	
	Can1SendMessage(0X12345678, EXTENDED_CANID, heart_data, 8);
}

/******************************************************
//CAN1��⺯��
//5ms����
******************************************************/
void Can1Monitor(void)
{
  static uint16 count;
	
	if(++count >= 20000)  count = 0;		//100s 
	
	#if CAN1_NEED_FILTER == 1
	CAN1_Switch_FilterSet();						//��̬�����˲���
	#endif
	
	if((count%200) == 0)  {							//1s
		#if CAN1_NEED_FILTER == 1
		CAN1_Switch_Filter_Overtime(1);		//�˲�����ʱ���
		#endif
		
		if(g_cans_bssic_struct.can1_inited == FALSE){		
			App_Can1Init();
		}
		if(++g_cans_bssic_struct.can1_rx_timeout >= CAN1_RX_TIMEOUT){
			Can1ClosePort();
			OSTimeDlyHMSM(0, 0, 1, 0);
			App_Can1Init();
		}
		
		Can1Tx();													//��������  ����
	}
	if((count%100) == 1)  {							//0.5s
		Can1Rx_Analyse();									//CAN1���ս���
	}

}



/******************************************************
//CAN ��������
******************************************************/
void Cans_task(void *pdata)
{	 
	App_Can1Init();												//��ʼ��CAN1
	g_sysmiscrun_struct.tasks_initOK_flag.bit.B0 = 1;
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 0, 5);
		
		Can1Monitor();											//ע�����ʱ��
		
	}
}



//=======================================================================================================================================================================//
///��ͨ�������޸Ĵ˲���
/******************************************************
//CAN1�������ݽ�������
//ע�⣺��Щ��������CAN������������ ȡ�ĸ� �������� qlj
******************************************************/
static void Can1Rx_Analyse(void)
{
	uint8  index;
	union32 tmpval;
	#ifdef APP_CAN_DEBUG
	uint8 i;
	#endif
	

	for(index=0; index <= CAN1_R_OTH_END_INDEX; index++){
		if( g_app_can1id_struct.Rec_flag[index] == 1){
			if(index>=CAN1_R_VOL_START_INDEX && index<=CAN1_R_VOL_END_INDEX){						//�����ѹ
				tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][0];
				tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][1];
				g_sigbatt_val[0][index*4] 	= tmpval.word.L;
				tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][2];
				tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][3];
				g_sigbatt_val[0][index*4+1] = tmpval.word.L;
				tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][4];
				tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][5];
				g_sigbatt_val[0][index*4+2] = tmpval.word.L;
				tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][6];
				tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][7];
				g_sigbatt_val[0][index*4+3] = tmpval.word.L;
				
			}
			else if(index>=CAN1_R_TEM_START_INDEX && index<=CAN1_R_TEM_END_INDEX){			//�����¶�		
				memcpy(&g_btprobe_val[0][(index-CAN1_R_TEM_START_INDEX)*8],g_app_can1id_struct.Rec_candata[index],8);
			}
			else{																																				//����
				switch(g_app_can1id_struct.Rec_canid[index]){
					case CAN1_R_03:{//BMS�����ѹ��ֵ����֡
						g_proextreme_union.Item.maxvolsys_id = g_app_can1id_struct.Rec_candata[index][0];
						g_proextreme_union.Item.maxvolbat_id = g_app_can1id_struct.Rec_candata[index][1];
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][2];
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][3];
						g_proextreme_union.Item.maxvolbat_val = tmpval.word.L;
						g_proextreme_union.Item.minvolsys_id 	= g_app_can1id_struct.Rec_candata[index][4];
						g_proextreme_union.Item.minvolbat_id 	= g_app_can1id_struct.Rec_candata[index][5];
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][6];
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][7];
						g_proextreme_union.Item.minvolbat_val = tmpval.word.L;
						break;
					}
					case CAN1_R_04:{//BMS�¶�ģ�鼫ֵ����֡				
						g_proextreme_union.Item.maxtemsys_id 	= g_app_can1id_struct.Rec_candata[index][0];
						g_proextreme_union.Item.maxtempro_id 	= g_app_can1id_struct.Rec_candata[index][1];
						g_proextreme_union.Item.maxtempro_val = g_app_can1id_struct.Rec_candata[index][2];
						g_proextreme_union.Item.mintemsys_id 	= g_app_can1id_struct.Rec_candata[index][3];
						g_proextreme_union.Item.mintempro_id 	= g_app_can1id_struct.Rec_candata[index][4];
						g_proextreme_union.Item.mintempro_val = g_app_can1id_struct.Rec_candata[index][5];
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][6];	//��Ե����
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][7];
						g_provehice_union.Item.resistance = tmpval.word.L;		
						break;
					}
					case CAN1_R_05:{//BMS����1
						g_proalarm_union.Item.rank 				= g_app_can1id_struct.Rec_candata[index][0];
						g_proalarm_union.Item.storagenum 	= g_app_can1id_struct.Rec_candata[index][1];
						if(g_proalarm_union.Item.rank == 0){
							g_proalarm_union.Item.flag.dword 	= 0;
							g_proalarm_union.Item.motornum 		= 0;
							g_proalarm_union.Item.enginenum 	= 0;
							g_proalarm_union.Item.othersnum 	= 0;
						}
						else if(g_proalarm_union.Item.rank == 1){
							g_storage_alr[0] = g_app_can1id_struct.Rec_candata[index][6];	//
							g_storage_alr[1] = g_app_can1id_struct.Rec_candata[index][7];
							g_storage_alr[2] = 0;
							g_storage_alr[3] = 0;
						}
						else if(g_proalarm_union.Item.rank == 2){
							g_storage_alr[0] = g_app_can1id_struct.Rec_candata[index][4];	//
							g_storage_alr[1] = g_app_can1id_struct.Rec_candata[index][5];
							g_storage_alr[2] = 0;
							g_storage_alr[3] = 0;
						}
						else if(g_proalarm_union.Item.rank == 3){
							g_storage_alr[0] = g_app_can1id_struct.Rec_candata[index][2];	//
							g_storage_alr[1] = g_app_can1id_struct.Rec_candata[index][3];
							g_storage_alr[2] = 0;
							g_storage_alr[3] = 0;		
						}	
						
						break;
					}
					case CAN1_R_06:{//BMS����2
//						 tmpval.dword = 0;
//						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][0];	//ͨ�ñ�����־
//						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][1];
//						g_proalarm_union.Item.flag.dword 	= tmpval.dword;			
						
						g_provehice_union.Item.charge = g_app_can1id_struct.Rec_candata[index][2];
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][3];	//���γ����
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][4];
						
//						tmpval.dword = 0;
//						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][5];	//�ۼƳ����
//						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][6];					
//						tmpval.byte.HL = g_app_can1id_struct.Rec_candata[index][7];	
				 
						break;
					}
					case CAN1_R_07:{//BMSϵͳ��Ϣ1										
						g_prologin_union.Item.storage_num = g_app_can1id_struct.Rec_candata[index][0];//�ɳ�索����ϵͳ����
						g_provbattsys_union.Item.serial = g_app_can1id_struct.Rec_candata[index][1];//�ɳ�索����ϵͳ�� 
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][2];	//
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][3];
						g_provbattsys_union.Item.voltage  = tmpval.word.L;		
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][4];	//
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][5];
						g_provbattsys_union.Item.current  = tmpval.word.L;		
						
						g_prologin_union.Item.storage_len = g_app_can1id_struct.Rec_candata[index][6];
						break;
					}
					case CAN1_R_08:{//BMSϵͳ��Ϣ2
						g_protbattsys_union.Item.serial = g_app_can1id_struct.Rec_candata[index][0];//�ɳ�索����ϵͳ�� 
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][1];	//
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][2];
						g_protbattsys_union.Item.btprobe_num = tmpval.word.L;		 
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][3];	//
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][4];
						g_provbattsys_union.Item.sigbatt_num = tmpval.word.L;		
						break;
					}
					case CAN1_R_09:{//BMSϵͳ��Ϣ3
						g_provehice_union.Item.SOC = g_app_can1id_struct.Rec_candata[index][2];//
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][3];	//
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][4];
						g_provehice_union.Item.current = tmpval.word.L;		 
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][5];	//
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][6];
						g_provehice_union.Item.voltage = tmpval.word.L;		
					
						break;
					}
					case CAN1_R_10:{//VCU����1
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][0] & 0x0F;//  ����״̬
						if(tmpval.byte.LL>=0x01 && tmpval.byte.LL<=0x03){
							g_provehice_union.Item.vehicle = tmpval.byte.LL;
						}
						else if(tmpval.byte.LL==0x0E || tmpval.byte.LL==0x0F){
							g_provehice_union.Item.vehicle = tmpval.byte.LL | 0XF0;
						}
						tmpval.byte.LL = (g_app_can1id_struct.Rec_candata[index][0]>>4) & 0x0F;//  ����״̬
						if(tmpval.byte.LL>=0x01 && tmpval.byte.LL<=0x03){
							g_provehice_union.Item.mode = tmpval.byte.LL;
						}
						else if(tmpval.byte.LL==0x0E || tmpval.byte.LL==0x0F){
							g_provehice_union.Item.mode = tmpval.byte.LL | 0XF0;
						}
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][1];	//
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][2];
						g_provehice_union.Item.speed = tmpval.word.L;		 
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][3];	//
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][4];
						tmpval.byte.HL = g_app_can1id_struct.Rec_candata[index][5];	//
						tmpval.byte.HH = g_app_can1id_struct.Rec_candata[index][6];
						g_provehice_union.Item.mileage = tmpval.dword;		
						
						break;
					}
					case CAN1_R_11:{//VCU����2				
						g_provehice_union.Item.DC_statu 	= g_app_can1id_struct.Rec_candata[index][4];//
						g_provehice_union.Item.stall.byte = g_app_can1id_struct.Rec_candata[index][5];//
						break;
					}
					case CAN1_R_12:{//VCU����3
						g_promotor_union[0].Item.serial 	= g_app_can1id_struct.Rec_candata[index][0];//
						g_promotor_union[0].Item.status 	= g_app_can1id_struct.Rec_candata[index][1];//
						g_promotor_union[0].Item.ctr_tem 	= g_app_can1id_struct.Rec_candata[index][2];//
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][3];	//
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][4];
						g_promotor_union[0].Item.rpm = tmpval.word.L;		 
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][5];	//
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][6];
						g_promotor_union[0].Item.torque = tmpval.word.L;		 
						g_promotor_union[0].Item.motor_tem 	= g_app_can1id_struct.Rec_candata[index][7];//
					
						break;
					}
					case CAN1_R_13:{//VCU����4
						g_promotor_union[0].Item.serial 	= g_app_can1id_struct.Rec_candata[index][0];//
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][1];	//
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][2];
						g_promotor_union[0].Item.ctr_in_vol = tmpval.word.L;		 
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][3];	//
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][4];
						g_promotor_union[0].Item.ctr_bus_cur = tmpval.word.L;		 
						g_provehice_union.Item.Accelerate 	= g_app_can1id_struct.Rec_candata[index][5];//
						g_provehice_union.Item.braking 	= g_app_can1id_struct.Rec_candata[index][6];//
						break;
					}
					case CAN1_R_14:{//VCU����5
						g_proalarm_union.Item.rank 	= g_app_can1id_struct.Rec_candata[index][0];//
						tmpval.byte.LL = g_app_can1id_struct.Rec_candata[index][1];	//ͨ�ñ�����־
						tmpval.byte.LH = g_app_can1id_struct.Rec_candata[index][2];
						tmpval.byte.HL = g_app_can1id_struct.Rec_candata[index][3];	
						tmpval.byte.HH = g_app_can1id_struct.Rec_candata[index][4];
						g_proalarm_union.Item.flag.dword 	= tmpval.dword;		
						g_proalarm_union.Item.storagenum 	= g_app_can1id_struct.Rec_candata[index][5];
						g_proalarm_union.Item.enginenum 	= g_app_can1id_struct.Rec_candata[index][6];
						g_proalarm_union.Item.othersnum 	= g_app_can1id_struct.Rec_candata[index][7];					
						
						if(g_proalarm_union.Item.rank == 0x03){
							AlarmHaveJudge(1);
						}
						else{
							AlarmHaveJudge(0);
						}
						break;
					}
					case CAN1_R_15:{//VCU����6
						g_motro_alr[0] = g_app_can1id_struct.Rec_candata[index][0];//
						g_motro_alr[1] = g_app_can1id_struct.Rec_candata[index][1];//
						g_motro_alr[2] = g_app_can1id_struct.Rec_candata[index][2];//
						g_motro_alr[3] = g_app_can1id_struct.Rec_candata[index][3];//
						
						g_others_alr[0] = g_app_can1id_struct.Rec_candata[index][4];//
						g_others_alr[1] = g_app_can1id_struct.Rec_candata[index][5];//
						g_others_alr[2] = g_app_can1id_struct.Rec_candata[index][6];//
						g_others_alr[3] = g_app_can1id_struct.Rec_candata[index][7];//
						break;
					}
					//case CAN1_16~26   //����
					
					case CAN1_R_26:{//VIN	
						#if 0
						// ���� �ж��д��������п��ܵ�1��2���ղ��� һֱ����3������
						if(g_app_can1id_struct.Rec_candata[index][0] == 1)
							memcpy(g_Tmp_VIN_codes[0], g_app_can1id_struct.Rec_candata[index], 8);
						else if(g_app_can1id_struct.Rec_candata[index][0] == 2)
							memcpy(g_Tmp_VIN_codes[1], g_app_can1id_struct.Rec_candata[index], 8);
						else if(g_app_can1id_struct.Rec_candata[index][0] == 3)
							memcpy(g_Tmp_VIN_codes[2], g_app_can1id_struct.Rec_candata[index], 8);
						#endif
					
						if(g_Tmp_VIN_codes[0][0]!=0 && g_Tmp_VIN_codes[1][0]!=0 && g_Tmp_VIN_codes[2][0]!=0 ){
							
							ProWrite_IntFlash_VIN(NULL, 0);					//дVIN
						}
						break;
					}
					
					default:				break;
				}
			}
		
			
			g_app_can1id_struct.Rec_flag[index] = 0XFF;					//����±�־	
	
			#ifdef APP_CAN_DEBUG			
			DPrint("\n CANID=%x 	DATA= ", g_app_can1id_struct.Rec_canid[index]);
			for(i=0;i < 8;i++){
				DPrint("%o", g_app_can1id_struct.Rec_candata[index][i]);
			}
			DPrint("\n");			
			#endif
			
		}
		
	}
	
}







