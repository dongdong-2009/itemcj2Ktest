
/*************************************************************
*					保存盲区8M-4096  （最后一段放下标）
*		盲区存储从地址0开始，否则要修改地址偏移与段偏移
*
*
*************************************************************/


#define APP_SPIFLASH_GLOBAL
#define APP_SPIFLASH_DEBUG

#include "tiza_include.h"



/******************************************************
//片外flash 按段保存盲区
有效总条数 APP_EF_LSNAL_NUM-APP_EF_EVERYSECTOR_NUM 

******************************************************/	
static void ExtFlashLsnalSectorSave(uint8 data[],uint16 len)
{
	uint8  tmp;
	uint16 sector;
	uint32 addr;
	
	if(len > APP_EF_EVERYLSNAL_SIZE1) 				{				goto RETURN_LAB;				}			//长度不对
	
	//满一段
	if((g_syslsnal_struct.headindex % APP_EF_EVERYSECTOR_NUM) == 0){							//FLASH擦除 4K字节/段 
		sector = (uint16)(g_syslsnal_struct.headindex / APP_EF_EVERYSECTOR_NUM);		//计算现在所在段号0~ 
		if(sector >= APP_EF_LSNALSECTOR_NUM)	{				goto RETURN_LAB;				}			//段号不对
		
		tmp = ExteFlashEraseSector(sector);
		if(tmp)																{				goto RETURN_LAB;				}			//段擦flash失败

		}
	
	//存一条
	addr = APP_EF_LSNAL_START_ADD + g_syslsnal_struct.headindex * APP_EF_EVERYLSNAL_SIZE1 ;
	if(addr > APP_EF_LSNAL_END_ADD)					{				goto RETURN_LAB;				}			//地址不对
	
	tmp = ExteFlashWrite(addr, data, len);
	if(tmp)																	{				goto RETURN_LAB;				}			//写flash失败
	
	
	g_syslsnal_struct.headindex = (g_syslsnal_struct.headindex+1) % APP_EF_LSNAL_NUM;
	
	if(g_syslsnal_struct.headindex == g_syslsnal_struct.tailindex){
	//if((g_syslsnal_struct.headindex/APP_EF_EVERYSECTOR_NUM) == (g_syslsnal_struct.tailindex/APP_EF_EVERYSECTOR_NUM)){
		//如果头和尾在同一段,说明头在该段第一条,该段其他盲区都在开始被擦除
		//尾跳到下一段的第一条,此时已舍弃本段的其他盲区数据
		tmp = APP_EF_EVERYSECTOR_NUM - (g_syslsnal_struct.tailindex % APP_EF_EVERYSECTOR_NUM);	//本次舍弃的盲区条数
		g_syslsnal_struct.tailindex = (g_syslsnal_struct.tailindex + tmp) % APP_EF_LSNAL_NUM;		//跳到下一段第一条
	}	
	
	ExtFlashLsnalHeadTailSave();
	
	g_syslsnal_struct.data[0] = INVALID_VAL_FF;
RETURN_LAB:
	return;
}


/******************************************************
//片外flash 单条盲区保存
//单条长度可同过宏设置，不得大于1024-5

******************************************************/	
static void ExtFlashLsnalDataSave(uint8 data[],uint16 len,uint8 cmd)
{	
	//有效标志单条长度*2 单条命令 ... 校验
	if(g_syslsnal_struct.data[0] != VALID_VAL_2A){																// 从无到有存入时需初始化
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
//盲区下标保存函数
// 后期考虑 减少flash擦写次数

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
	if(res){// 失败
		return;
	}
	ExteFlashWrite(APP_EF_LSNAL_INDEXADD, data, 9);
}

/******************************************************
//读盲区下标 片外flash
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
//压盲区函数

******************************************************/	
void PutIntoExtFlashLsnal(uint8 data[],uint16 len,uint8 cmd)
{
	switch(cmd){
		case PRO_UP_REAL_INFO_ID:
		case PRO_UP_LSNAL_INFO_ID:{
			if(cmd == PRO_UP_LSNAL_INFO_ID){	//清正在发送标志
				g_pro_struct.tx_one_lsnal_falg = 0; 
			}
			ExtFlashLsnalDataSave(data,len,cmd);///写盲区
			break;
		}
		case PRO_UP_LOGIN_ID:{
			break;
		}
		default:		break;
	}
}

/******************************************************
								0203不应答发送buff中无0203要压盲区
//系统退出前盲区补偿
//使用时需验证
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



