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

                                 

//读取指定地址的字(32位数据) 
//faddr:读地址 
//返回值:对应数据.
static uint32 STMFLASH_ReadWord(uint32 faddr)
{
	return *(uint32*)faddr; 
}  

//读取指定地址的字节(8位数据) 
//faddr:读地址 
//返回值:对应数据.

static uint8 STMFLASH_ReadByte(uint32 faddr)
{
	return *(uint8*)faddr; 
}  

//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0~11,即addr所在的扇区
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
偏内flash写入函数     
//输入：
				Addr 片内flash地址   必须是4的整数倍，有32位对齐
				Data 写入数据buff
				Len  写入数据长度
//输出：
				0-success 1-failed 
//u8转u32写--注意字节与地址的对应关系  
//此处不校验0xFFFFFFFF,所以调用前要确保擦除过
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
		return 1;	//非法地址
		
	tmp_len = Len - Len%4;	
	addindex = Addr;										//写入的起始地址
	end_addr = Addr + tmp_len;					//写入的结束地址 前整4字节
	
	FLASH_Unlock();											//解锁 
	FLASH_DataCacheCmd(DISABLE);				//FLASH擦除期间,必须禁止数据缓存 		

	while(addindex < end_addr)					//写数据   4字节写
	{
//		FeedWtd();
		if(FLASH_ProgramWord(addindex,*(u32*)src) != FLASH_COMPLETE)//写入数据
		{ 
			res = FALSE;
			break;	
		}
		if(*(u32*)addindex != *(u32*)src)	//写入再度校验
		{
			res = FALSE;
			break;
		}
		addindex += 4;
		src 		 += 4;
	} 
	
	end_addr = Addr + Len;							//写入的结束地址 后不满4字节
	while(addindex < end_addr)					//写数据   1字节写
	{
		if(FLASH_ProgramByte(addindex, Data[tmp_len]) != FLASH_COMPLETE)//写入数据
		{ 
			res = FALSE;
			break;	
		}
		if(*(u8*)addindex != Data[tmp_len])	//写入再度校验
		{
			res = FALSE;
			break;
		}
		addindex++;
		tmp_len++;
		
	} 
	
	FLASH_DataCacheCmd(ENABLE);					//FLASH擦除结束,开启数据缓存
	FLASH_Lock();												//上锁

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
偏内flash读取函数
//输入：
				Addr 偏内flash地址
				Data 读取数据接收buff
				Len  读取数据长度
//输出：
				0-success 1-failed 
******************************************************/
uint8 CpuFlashRead(uint32 Addr, uint8 Data[], uint16 Len)
{
	uint32 i;

	if(STMFLASH_GetFlashSector(Addr)==FLASH_Sector_0 || STMFLASH_GetFlashSector(Addr+Len-1)==FLASH_Sector_0)
		return 1;

	if(Addr < STM32_FLASH_BASE || (Addr + Len) >= (STM32_FLASH_BASE + STM32_FLASH_SIZE))
		return 1;	//非法地址
	
	for(i=0; i < Len; i++)
	{
		Data[i] = STMFLASH_ReadByte(Addr);	//读取1个字节.
		Addr++;		//偏移1个字节.	
	}

	return 0;
}

/******************************************************
偏内flash段擦除
//输入：
				Sector FLASH_Sector_0 - FLASH_Sector_11  (stm32f4xx_flash.h 中定义)
				Add 实际偏内flash地址
//输出：
				0-success 1-failed 2-未完成
******************************************************/
#if 0
uint8 CpuFlashEraseSector(uint16 Sector)
{
	FLASH_Status status = FLASH_COMPLETE;

	if(!IS_F407_FLASH_SECTOR(Sector))
		return 1;

	if(IS_FORBIDDEN_FLASH_SECTOR(Sector))
		return 1;

	FLASH_Unlock();							//解锁 
	FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
	
	status = FLASH_EraseSector(Sector, VoltageRange_3);//VCC=2.7~3.6V之间!!
	//想直接使用flash地址  使用一下擦除指令
	//status=FLASH_EraseSector(STMFLASH_GetFlashSector(page_addr),VoltageRange_3);//VCC=2.7~3.6V之间!!
	FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	FLASH_Lock();								//上锁

	if(status != FLASH_COMPLETE)
		return 2;																			  //发生错误了

	return 0;
}

#else

uint8 CpuFlashEraseSector(uint32 Add)
{
	FLASH_Status status = FLASH_COMPLETE;


	FLASH_Unlock();							//解锁 
	FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
	
	//想直接使用flash地址  使用一下擦除指令
	status=FLASH_EraseSector(STMFLASH_GetFlashSector(Add),VoltageRange_3);//VCC=2.7~3.6V之间!!
	FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	FLASH_Lock();								//上锁

	if(status != FLASH_COMPLETE)
		return 2;																			  //发生错误了

	return 0;
}
#endif


/*
// 0 : success    1 : failed
uint8 FlashEraseAll(void)
{
	FLASH_Status status = FLASH_COMPLETE;

	FLASH_Unlock();									//解锁 
	FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
	
	status = FLASH_EraseAllSectors(VoltageRange_3);

	FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	FLASH_Lock();//上锁

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

