#define TIZA_INTFLASH_GLOBAL
#define TIZA_INTFLASH_DENUG

#include "tiza_include.h"
 

#define IS_F407_FLASH_SECTOR(SECTOR) (((SECTOR) == FLASH_Sector_0)   || ((SECTOR) == FLASH_Sector_1)   ||\
																			((SECTOR) == FLASH_Sector_2)   || ((SECTOR) == FLASH_Sector_3)   ||\
																			((SECTOR) == FLASH_Sector_4)   || ((SECTOR) == FLASH_Sector_5)   ||\
																			((SECTOR) == FLASH_Sector_6)   || ((SECTOR) == FLASH_Sector_7)   ||\
																			((SECTOR) == FLASH_Sector_8)   || ((SECTOR) == FLASH_Sector_9)   ||\
																			((SECTOR) == FLASH_Sector_10)  || ((SECTOR) == FLASH_Sector_11))

#define IS_FORBIDDEN_FLASH_SECTOR(SECTOR) (((SECTOR) == FLASH_Sector_0)  || ((SECTOR) == FLASH_Sector_1)   ||\
																					((SECTOR) == FLASH_Sector_5)   || ((SECTOR) == FLASH_Sector_6)   ||\
																					((SECTOR) == FLASH_Sector_7))

                                 

//��ȡָ����ַ����(32λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
static uint32 STMFLASH_ReadWord(uint32 faddr)
{
	return *(uint32*)faddr; 
}  

//��ȡָ����ַ���ֽ�(8λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.

static uint8 STMFLASH_ReadByte(uint32 faddr)
{
	return *(uint8*)faddr; 
}  

//��ȡĳ����ַ���ڵ�flash����
//addr:flash��ַ
//����ֵ:0~11,��addr���ڵ�����
static uint16 STMFLASH_GetFlashSector(uint32 addr)
{
	if(addr < ADDR_FLASH_SECTOR_1)		return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)	return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)	return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)	return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)	return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)	return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)	return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)	return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)	return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
																		
																		return FLASH_Sector_11;	
}



/******************************************************
ƫ��flashд�뺯��     
//���룺
				Addr Ƭ��flash��ַ   ������4������������32λ����
				Data д������buff
				Len  д�����ݳ���
//�����
				0-success 1-failed 
//u8תu32д--ע���ֽ����ַ�Ķ�Ӧ��ϵ  
//�˴���У��0xFFFFFFFF,���Ե���ǰҪȷ��������
******************************************************/
uint8 CpuFlashWrite(uint32 Addr, uint8 Data[], uint16 Len)
{ 
  FLASH_Status status = FLASH_COMPLETE;
	uint8 res = TRUE;
	uint32 addindex = 0, end_addr = 0;	
	uint32 src = (uint32)Data;
	uint16 tmp_len;

	if(STMFLASH_GetFlashSector(Addr) == FLASH_Sector_0 || STMFLASH_GetFlashSector(Addr + Len -1) == FLASH_Sector_0)
		return 1;
	
	if(Addr < STM32_FLASH_BASE || (Addr + Len) >= (STM32_FLASH_BASE + STM32_FLASH_SIZE))
		return 1;	//�Ƿ���ַ
		
	tmp_len = Len - Len%4;	
	addindex = Addr;										//д�����ʼ��ַ
	end_addr = Addr + tmp_len;					//д��Ľ�����ַ ǰ��4�ֽ�
	
	FLASH_Unlock();											//���� 
	FLASH_DataCacheCmd(DISABLE);				//FLASH�����ڼ�,�����ֹ���ݻ��� 		

	while(addindex < end_addr)					//д����   4�ֽ�д
	{
//		FeedWtd();
		if(FLASH_ProgramWord(addindex,*(u32*)src) != FLASH_COMPLETE)//д������
		{ 
			res = FALSE;
			break;	
		}
		if(*(u32*)addindex != *(u32*)src)	//д���ٶ�У��
		{
			res = FALSE;
			break;
		}
		addindex += 4;
		src 		 += 4;
	} 
	
	end_addr = Addr + Len;							//д��Ľ�����ַ ����4�ֽ�
	while(addindex < end_addr)					//д����   1�ֽ�д
	{
		if(FLASH_ProgramByte(addindex, Data[tmp_len]) != FLASH_COMPLETE)//д������
		{ 
			res = FALSE;
			break;	
		}
		if(*(u8*)addindex != Data[tmp_len])	//д���ٶ�У��
		{
			res = FALSE;
			break;
		}
		addindex++;
		tmp_len++;
		
	} 
	
	FLASH_DataCacheCmd(ENABLE);					//FLASH��������,�������ݻ���
	FLASH_Lock();												//����

	if(res == TRUE)
	{
		return 0;
	}
	else
	{
		return 1;
	}
} 

/******************************************************
ƫ��flash��ȡ����
//���룺
				Addr ƫ��flash��ַ
				Data ��ȡ���ݽ���buff
				Len  ��ȡ���ݳ���
//�����
				0-success 1-failed 
******************************************************/
uint8 CpuFlashRead(uint32 Addr, uint8 Data[], uint16 Len)
{
	uint32 i;

	if(STMFLASH_GetFlashSector(Addr)==FLASH_Sector_0 || STMFLASH_GetFlashSector(Addr+Len-1)==FLASH_Sector_0)
		return 1;

	if(Addr < STM32_FLASH_BASE || (Addr + Len) >= (STM32_FLASH_BASE + STM32_FLASH_SIZE))
		return 1;	//�Ƿ���ַ
	
	for(i=0; i < Len; i++)
	{
		Data[i] = STMFLASH_ReadByte(Addr);	//��ȡ1���ֽ�.
		Addr++;		//ƫ��1���ֽ�.	
	}

	return 0;
}

/******************************************************
ƫ��flash�β���
//���룺
				Sector FLASH_Sector_0 - FLASH_Sector_11  (stm32f4xx_flash.h �ж���)
				Add ʵ��ƫ��flash��ַ
//�����
				0-success 1-failed 2-δ���
******************************************************/
#if 0
uint8 CpuFlashEraseSector(uint16 Sector)
{
	FLASH_Status status = FLASH_COMPLETE;

	if(!IS_F407_FLASH_SECTOR(Sector))
		return 1;

	if(IS_FORBIDDEN_FLASH_SECTOR(Sector))
		return 1;

	FLASH_Unlock();							//���� 
	FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
	
	status = FLASH_EraseSector(Sector, VoltageRange_3);//VCC=2.7~3.6V֮��!!
	//��ֱ��ʹ��flash��ַ  ʹ��һ�²���ָ��
	//status=FLASH_EraseSector(STMFLASH_GetFlashSector(page_addr),VoltageRange_3);//VCC=2.7~3.6V֮��!!
	FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();								//����

	if(status != FLASH_COMPLETE)
		return 2;																			  //����������

	return 0;
}

#else

uint8 CpuFlashEraseSector(uint32 Add)
{
	FLASH_Status status = FLASH_COMPLETE;


	FLASH_Unlock();							//���� 
	FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
	
	//��ֱ��ʹ��flash��ַ  ʹ��һ�²���ָ��
	status=FLASH_EraseSector(STMFLASH_GetFlashSector(Add),VoltageRange_3);//VCC=2.7~3.6V֮��!!
	FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();								//����

	if(status != FLASH_COMPLETE)
		return 2;																			  //����������

	return 0;
}
#endif


/*
// 0 : success    1 : failed
uint8 FlashEraseAll(void)
{
	FLASH_Status status = FLASH_COMPLETE;

	FLASH_Unlock();									//���� 
	FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
	
	status = FLASH_EraseAllSectors(VoltageRange_3);

	FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����

	if(status == FLASH_COMPLETE)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
*/

