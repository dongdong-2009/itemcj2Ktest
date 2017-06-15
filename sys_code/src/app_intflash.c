

#define APP_INTFLASH_GLOBAL
#define APP_INTFLASH_DEBUG

#include "tiza_include.h"

#define BACKUP_WRITE_INT	256    //APP_SAVEINT_PARA_INDEXEND-APP_SAVEINT_INDEXSTART
uint8 Backup_Bef_Wr_Int[BACKUP_WRITE_INT];

//----------------------------------------------------------------------------//


/******************************************************
//дVIN��
//ͷ2A + VIN + У��
******************************************************/
void ProWrite_IntFlash_VIN(uint8 str[], uint16 len)
{
	uint8 data[25];
	uint8  i=0;
		
	if(len == 0){
		memcpy(&data[1], &g_Tmp_VIN_codes[0][1], 7);
		memcpy(&data[8], &g_Tmp_VIN_codes[1][1], 7);
		memcpy(&data[15], &g_Tmp_VIN_codes[2][1], 7);
		
		for(i=0; i<17; i++){
			if(data[1+i] != g_VIN[i])  break;  								//��Ҫ����VIN
		}
		if(i == 17)								{		goto 	RETURN_LAB;	}		//����Ҫ����

	}
	else{
		// ��ʼ��VIN
		memcpy(&data[1], str, 17);
	}
	
	data[0]  = 0x2A;
	data[18] = U8SumCheck(data, 18);
	
	CpuFlashRead(APP_SAVEINT_INDEXSTART, Backup_Bef_Wr_Int, BACKUP_WRITE_INT);
	memcpy(&Backup_Bef_Wr_Int[(APP_SAVEINT_VIN_INDEXSTART-APP_SAVEINT_INDEXSTART)], data, 19);
	
	CpuFlashEraseSector(APP_SAVEINT_INDEXSTART);
	CpuFlashWrite(APP_SAVEINT_INDEXSTART, Backup_Bef_Wr_Int, BACKUP_WRITE_INT);
	
	memcpy(g_VIN, &data[1], 17);
	
RETURN_LAB:
	return;
	
}

/******************************************************
//��VIN��
//ͷ2A + VIN + У��
******************************************************/
void ProRead_IntFlash_VIN(void)
{	
	uint8 data[20];
	
	//ͷ2A + VIN + У��
	CpuFlashRead(APP_SAVEINT_VIN_INDEXSTART, data, 19);
	
	if(data[0]==0x2A && data[18]==U8SumCheck(data, 18)){
		memcpy(g_VIN, &data[1], 17);
	}
	else {// ��ʼ��VIN		
		memcpy(&data[1], "01234567890123456", 17);
		ProWrite_IntFlash_VIN(&data[1], 17);	
	}
	
}

/******************************************************
//32960Э��ʹ�ò�����д����
******************************************************/
void ProWrite_SysPara(void)
{
	CpuFlashRead(APP_SAVEINT_INDEXSTART, Backup_Bef_Wr_Int, BACKUP_WRITE_INT);
	g_propara_union.Item.check = U8SumCheck(g_propara_union.arry,PRO_PARA_UNION_LEN-1);
	memcpy(&Backup_Bef_Wr_Int[(APP_SAVEINT_PARA_INDEXSTART-APP_SAVEINT_INDEXSTART)], g_propara_union.arry, PRO_PARA_UNION_LEN);
	
	CpuFlashEraseSector(APP_SAVEINT_INDEXSTART);
	CpuFlashWrite(APP_SAVEINT_INDEXSTART, Backup_Bef_Wr_Int, BACKUP_WRITE_INT);
	
}
void ProRead_SysPara(void)
{
	CpuFlashRead(APP_SAVEINT_PARA_INDEXSTART, g_propara_union.arry, PRO_PARA_UNION_LEN);
	
	if(g_propara_union.Item.check != U8SumCheck(g_propara_union.arry,PRO_PARA_UNION_LEN-1)){
		#ifdef APP_INTFLASH_DEBUG
		DPrint("ProRead_SysPara ERROR \n");
		#endif
		// �ָ�����������
		SetPara2FactoryReset();
		ProWrite_SysPara();
	}
	memcpy(g_propara_union.Item.g_para_HDVers,"Dh700",5);	
	memcpy(g_propara_union.Item.g_para_SFVers,"DsB23",5);	
}



/******************************************************
//��ȡ��д���ϴ���Ч��λ��Ϣ
//���� ��00��ȡ 01д��
//ͷ2A + ����γ��״̬ + У��  = 1+(4+4+1)+1 = 11
******************************************************/
void ReadWriteOldVaildPostion(uint8 rw)	
{
	uint8 data[15];
	union32 tmp;	
	
	if(rw == 0){		//��ȡ		
		CpuFlashRead(APP_SAVEINT_OLDGPS_INDEXSTART, data, 11);
		if(data[0]==0x2A && data[10]==U8SumCheck(data, 10)){
			tmp.byte.HH = data[1];
			tmp.byte.HL = data[2];
			tmp.byte.LH = data[3];
			tmp.byte.LL = data[4];
			g_gps_struct.gpsinform.subitem.longitude = tmp.dword;
			tmp.byte.HH = data[5];
			tmp.byte.HL = data[6];
			tmp.byte.LH = data[7];
			tmp.byte.LL = data[8];
			g_gps_struct.gpsinform.subitem.latitude = tmp.dword;
			g_gps_struct.gpsinform.subitem.statu.byte = data[9]|0x01;	//- - A(invalid)
		}
		else {			//�ڴ�û��λ������ʱ ʹ���Ͼ�����ľ�γ����ΪĬ�ϳ�ʼֵ
			g_gps_struct.gpsinform.subitem.longitude 	= 118733477;
			g_gps_struct.gpsinform.subitem.latitude  	= 31993519;
			g_gps_struct.gpsinform.subitem.statu.byte	= 0x01;		//E N A(invalid)				
			
			goto RET_WRITE;
		}
	}
	else{						//д��
		if(g_gps_struct.gpsinform.subitem.statu.bit.B0 == 0x00){//��Ч��λ
		RET_WRITE:
			data[0] = 0x2A;
			tmp.dword = g_gps_struct.gpsinform.subitem.longitude;
			data[1] = tmp.byte.HH;
			data[2] = tmp.byte.HL;
			data[3] = tmp.byte.LH;
			data[4] = tmp.byte.LL;
			tmp.dword = g_gps_struct.gpsinform.subitem.latitude;
			data[5] = tmp.byte.HH;
			data[6] = tmp.byte.HL;
			data[7] = tmp.byte.LH;
			data[8] = tmp.byte.LL;
			data[9] = g_gps_struct.gpsinform.subitem.statu.byte & 0x07;
			data[10] = U8SumCheck(data, 10);
	
			CpuFlashRead(APP_SAVEINT_INDEXSTART, Backup_Bef_Wr_Int, BACKUP_WRITE_INT);
			memcpy(&Backup_Bef_Wr_Int[(APP_SAVEINT_OLDGPS_INDEXSTART-APP_SAVEINT_INDEXSTART)], data, 11);
			
			CpuFlashEraseSector(APP_SAVEINT_INDEXSTART);
			CpuFlashWrite(APP_SAVEINT_INDEXSTART, Backup_Bef_Wr_Int, BACKUP_WRITE_INT);
				
		}
	}
	
}








