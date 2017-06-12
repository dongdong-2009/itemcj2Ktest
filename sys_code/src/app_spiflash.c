
/*************************************************************
*					����ä��8M-4096  �����һ�η��±꣩
*		ä���洢�ӵ�ַ0��ʼ������Ҫ�޸ĵ�ַƫ�����ƫ��
*
*
*************************************************************/


#define APP_SPIFLASH_GLOBAL
#define APP_SPIFLASH_DEBUG

#include "tiza_include.h"



/******************************************************
//Ƭ��flash ���α���ä��
��Ч������ APP_EF_LSNAL_NUM-APP_EF_EVERYSECTOR_NUM 

******************************************************/	
static void ExtFlashLsnalSectorSave(uint8 data[],uint16 len)
{
	uint8  tmp;
	uint16 sector;
	uint32 addr;
	
	if(len > APP_EF_EVERYLSNAL_SIZE1) 				{				goto RETURN_LAB;				}			//���Ȳ���
	
	//��һ��
	if((g_syslsnal_struct.headindex % APP_EF_EVERYSECTOR_NUM) == 0){							//FLASH���� 4K�ֽ�/�� 
		sector = (uint16)(g_syslsnal_struct.headindex / APP_EF_EVERYSECTOR_NUM);		//�����������ڶκ�0~ 
		if(sector >= APP_EF_LSNALSECTOR_NUM)	{				goto RETURN_LAB;				}			//�κŲ���
		
		tmp = ExteFlashEraseSector(sector);
		if(tmp)																{				goto RETURN_LAB;				}			//�β�flashʧ��

		}
	
	//��һ��
	addr = APP_EF_LSNAL_START_ADD + g_syslsnal_struct.headindex * APP_EF_EVERYLSNAL_SIZE1 ;
	if(addr > APP_EF_LSNAL_END_ADD)					{				goto RETURN_LAB;				}			//��ַ����
	
	tmp = ExteFlashWrite(addr, data, len);
	if(tmp)																	{				goto RETURN_LAB;				}			//дflashʧ��
	
	
	g_syslsnal_struct.headindex = (g_syslsnal_struct.headindex+1) % APP_EF_LSNAL_NUM;
	
	if(g_syslsnal_struct.headindex == g_syslsnal_struct.tailindex){
	//if((g_syslsnal_struct.headindex/APP_EF_EVERYSECTOR_NUM) == (g_syslsnal_struct.tailindex/APP_EF_EVERYSECTOR_NUM)){
		//���ͷ��β��ͬһ��,˵��ͷ�ڸöε�һ��,�ö�����ä�����ڿ�ʼ������
		//β������һ�εĵ�һ��,��ʱ���������ε�����ä������
		tmp = APP_EF_EVERYSECTOR_NUM - (g_syslsnal_struct.tailindex % APP_EF_EVERYSECTOR_NUM);	//����������ä������
		g_syslsnal_struct.tailindex = (g_syslsnal_struct.tailindex + tmp) % APP_EF_LSNAL_NUM;		//������һ�ε�һ��
	}	
	
	ExtFlashLsnalHeadTailSave();
	
	g_syslsnal_struct.data[0] = INVALID_VAL_FF;
RETURN_LAB:
	return;
}


/******************************************************
//Ƭ��flash ����ä������
//�������ȿ�ͬ�������ã����ô���1024-5

******************************************************/	
static void ExtFlashLsnalDataSave(uint8 data[],uint16 len,uint8 cmd)
{	
	//��Ч��־��������*2 �������� ... У��
	if(g_syslsnal_struct.data[0] != VALID_VAL_2A){																// ���޵��д���ʱ���ʼ��
		g_syslsnal_struct.data[0] = VALID_VAL_2A;
		g_syslsnal_struct.data[1] = len >> 8;
		g_syslsnal_struct.data[2] = len & 0xFF;
		g_syslsnal_struct.data[3] = cmd;
		
		MemCpy(g_syslsnal_struct.data+4, data, len);	
		g_syslsnal_struct.data[len+4] = U8SumCheck(g_syslsnal_struct.data,len+4);
		ExtFlashLsnalSectorSave(g_syslsnal_struct.data,len+5);	
	}
	
	return;
}

/******************************************************
//ä���±걣�溯��
// ���ڿ��� ����flash��д����

******************************************************/	
void ExtFlashLsnalHeadTailSave(void)			
{
	uint8  data[9],res;

	data[0] = (uint8)(g_syslsnal_struct.headindex >> 24) & 0xFF;
	data[1] = (uint8)(g_syslsnal_struct.headindex >> 16) & 0xFF;
	data[2] = (uint8)(g_syslsnal_struct.headindex >>  8) & 0xFF;
	data[3] = (uint8) g_syslsnal_struct.headindex        & 0xFF;
	data[4] = (uint8)(g_syslsnal_struct.tailindex >> 24) & 0xFF;
	data[5] = (uint8)(g_syslsnal_struct.tailindex >> 16) & 0xFF;
	data[6] = (uint8)(g_syslsnal_struct.tailindex >>  8) & 0xFF;
	data[7] = (uint8) g_syslsnal_struct.tailindex        & 0xFF;
	
	data[8] = U8SumCheck(data,8);
	res = ExteFlashEraseSector(APP_EF_LSNAL_INDEXSECTOR);	
	if(res){// ʧ��
		return;
	}
	ExteFlashWrite(APP_EF_LSNAL_INDEXADD, data, 9);
}

/******************************************************
//��ä���±� Ƭ��flash
******************************************************/
void Read_Lsnal_Index(void)
{
	uint8  data[9];
	union32 tmp;
	
	ExteFlashRead(APP_EF_LSNAL_INDEXADD, data, 9);
	
	if(data[8] == U8SumCheck(data,8)){		
		tmp.byte.HH = data[0];
		tmp.byte.HL = data[1];
		tmp.byte.LH = data[2];
		tmp.byte.LL = data[3];
		g_syslsnal_struct.headindex = tmp.dword;
		tmp.byte.HH = data[4];
		tmp.byte.HL = data[5];
		tmp.byte.LH = data[6];
		tmp.byte.LL = data[7];
		g_syslsnal_struct.tailindex = tmp.dword;
	}
	else {
		#ifdef APP_SPIFLASH_DEBUG
		DPrint("ExteFlashRead index ERROR\n");
		DebugSendDatas(data, 9);
		DealDebugSend(1);
		#endif
		g_syslsnal_struct.headindex = 0;
		g_syslsnal_struct.tailindex = 0;
		ExtFlashLsnalHeadTailSave();
	}
}
/******************************************************
//ѹä������

******************************************************/	
void PutIntoExtFlashLsnal(uint8 data[],uint16 len,uint8 cmd)
{
	switch(cmd){
		case PRO_UP_REAL_INFO_ID:
		case PRO_UP_LSNAL_INFO_ID:{
			if(cmd == PRO_UP_LSNAL_INFO_ID){	//�����ڷ��ͱ�־
				g_pro_struct.tx_one_lsnal_falg = 0; 
			}
			ExtFlashLsnalDataSave(data,len,cmd);///дä��
			break;
		}
		case PRO_UP_LOGIN_ID:{
			break;
		}
		default:		break;
	}
}

/******************************************************
								0203��Ӧ����buff����0203Ҫѹä��
//ϵͳ�˳�ǰä������
//ʹ��ʱ����֤
******************************************************/	
#if 0
void ProLsnalSysExit(void)
{
	uint8 i;
	
	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){
		if(g_pro_struct.tx_struct.re_tx_full_flag[i] == TRUE){
			g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;
			PutIntoExtFlashLsnal(g_pro_struct.tx_struct.re_tx_buf[i]+PRO_DATA_INDEX,
							g_pro_struct.tx_struct.re_tx_len[i]-PRO_DATA_INDEX-1,
							g_pro_struct.tx_struct.re_tx_buf[i][PRO_CMD_INDEX]);
		}
	}	
}
#endif



