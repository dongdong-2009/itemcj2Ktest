/**
PA4  	CS
PA5  	SCK
PA6  	MISO
PA7  	MOSI
**/

#define TIZA_SPI_GLOBAL
#define TIZA_SPI_DEBUG

#include "tiza_include.h"


//指令表
#define W25X_WriteEnable			0x06 
#define W25X_WriteDisable			0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData					0x03 
#define W25X_FastReadData			0x0B 
#define W25X_FastReadDual			0x3B 
#define W25X_PageProgram			0x02 
#define W25X_BlockErase				0xD8 
#define W25X_SectorErase			0x20 
#define W25X_ChipErase				0xC7 
#define W25X_PowerDown				0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID					0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 



//================================================================================================================//
///SPI
/******************************************************
Spi1初始化函数
******************************************************/
#if SPI1_USE_DMA2 == 0		
static void Tiza_Spi1Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);		//使能SPI1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);			//DMA1时钟使能 

	//GPIOA4初始化设置  							CS
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;					//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;				//无上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);									//初始化
	//GPIOA5,6,7初始化设置				SCK           MISO          MOSI
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;						//PA5~7复用功能输出	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;						//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);									//初始化
		
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1); //PA5复用为 SPI1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1); //PA6复用为 SPI1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1); //PA7复用为 SPI1
	
	/* 配置 Spi1 */
	SPI_DeInit(SPI1);		
	SPI_InitStructure.SPI_Direction 				= SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode 							= SPI_Mode_Master;									//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize 					= SPI_DataSize_8b;									//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL 							= SPI_CPOL_High;										//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA 							= SPI_CPHA_2Edge;										//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS 							= SPI_NSS_Soft;											//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;				//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit 					= SPI_FirstBit_MSB;									//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial 		= 7;																//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
 
 
	SPI_Cmd(SPI1, ENABLE); 														//使能SPI外设
	SPI_SSOutputCmd(SPI1, ENABLE);
	
//	SPI1_ReadWriteByte(0xff);													//启动传输		 
  
}

#else		//使用DMA
static void Tiza_Spi1Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);		//使能SPI1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);			//DMA1时钟使能 

	//GPIOA4初始化设置  							CS
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;					//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;				//无上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);									//初始化
	//GPIOA5,6,7初始化设置				SCK           MISO          MOSI
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;						//PA5~7复用功能输出	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;						//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);									//初始化
		
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1); //PA5复用为 SPI1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1); //PA6复用为 SPI1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1); //PA7复用为 SPI1
		
	/* 配置 DMA Stream */
	//SPI1_RX
	DMA_DeInit(DMA2_Stream2);																										//反初始化数据流2
	while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE){													//等待DMA可配置 
		OSTimeDlyHMSM(0, 0, 0, 5);	
	}	
  DMA_InitStructure.DMA_Channel 						= DMA_Channel_3;  								//通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr 	= (u32)&SPI1->DR;									//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr 		= (u32)g_Spi1Dma2_Rx_buff;				//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR 								= DMA_DIR_PeripheralToMemory;			//外设到存储器模式
  DMA_InitStructure.DMA_BufferSize 					= TIZA_SPI1_RX_BUFF_LEN;					//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;			//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc					  = DMA_MemoryInc_Enable;						//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;		//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_Byte;				//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode 								= DMA_Mode_Circular;							//使用循环模式 
  DMA_InitStructure.DMA_Priority 						= DMA_Priority_High;							//高等优先级
  DMA_InitStructure.DMA_FIFOMode 						= DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold 			= DMA_FIFOThreshold_HalfFull;			// 1/2 FIFO	
  DMA_InitStructure.DMA_MemoryBurst 				= DMA_MemoryBurst_Single;					//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;			//外设突发单次传输
  DMA_Init(DMA2_Stream2, &DMA_InitStructure);				//初始化DMA Stream
	
	//SPI1_TX
	DMA_DeInit(DMA2_Stream3);																										//反初始化数据流3
	while (DMA_GetCmdStatus(DMA2_Stream3) != DISABLE){													//等待DMA可配置 
		OSTimeDlyHMSM(0, 0, 0, 5);	
	}	
  DMA_InitStructure.DMA_Channel 						= DMA_Channel_3;  								//通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr 	= (u32)&SPI1->DR;									//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr 		= (u32)g_Spi1Dma2_Tx_buff;				//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR 								= DMA_DIR_MemoryToPeripheral;			//存储器到外设模式
  DMA_InitStructure.DMA_BufferSize 					= TIZA_SPI1_RX_BUFF_LEN;					//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;			//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc					  = DMA_MemoryInc_Enable;						//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;		//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_Byte;				//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode 								= DMA_Mode_Circular;							//使用循环模式 
  DMA_InitStructure.DMA_Priority 						= DMA_Priority_High;							//高等优先级
  DMA_InitStructure.DMA_FIFOMode 						= DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold 			= DMA_FIFOThreshold_HalfFull;			// 1/2 FIFO	
  DMA_InitStructure.DMA_MemoryBurst 				= DMA_MemoryBurst_Single;					//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;			//外设突发单次传输
  DMA_Init(DMA2_Stream3, &DMA_InitStructure);				//初始化DMA Stream
	
	/* 配置 Spi1 */
	SPI_DeInit(SPI1);		
	SPI_InitStructure.SPI_Direction 				= SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode 							= SPI_Mode_Master;									//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize 					= SPI_DataSize_8b;									//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL 							= SPI_CPOL_High;										//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA 							= SPI_CPHA_2Edge;										//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS 							= SPI_NSS_Soft;											//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;				//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit 					= SPI_FirstBit_MSB;									//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial 		= 7;																//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
 
 
	SPI_Cmd(SPI1, ENABLE); 														//使能SPI外设
	SPI_SSOutputCmd(SPI1, ENABLE);
	
  DMA_Cmd(DMA2_Stream2, ENABLE);    								//使能DMA通道  
  DMA_Cmd(DMA2_Stream3, ENABLE);    								//使能DMA通道  
  
	
}
#endif

/******************************************************
	SPI速度设置函数
//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
******************************************************/
static void SPI1_SetSpeed(uint8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
} 

/******************************************************
	SPI读写函数
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
******************************************************/
static uint8 SPI1_ReadWriteByte(uint8 TxData)
{		 			 
 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){		//等待发送区空  
		OSTimeDlyHMSM(0, 0, 0, 1);	
	}

	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){	//等待接收完一个byte
		OSTimeDlyHMSM(0, 0, 0, 1);	
	}  

	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
}





//================================================================================================================//
///SPI_FLASH

///4096byte为一个Sector
///16个扇区为1个Block
///W25Q64
///容量为8M字节,共有64个Block,2048个Sector 


/******************************************************
//读取W25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
******************************************************/
static uint8 SpiFlashReadSR(void)   
{  
	uint8 byte=0;   
	SPI_CS_ENABLE;                            	//使能器件   
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    	//发送读取状态寄存器命令    
	byte=SPI1_ReadWriteByte(0Xff);            	//读取一个字节  
	SPI_CS_DISABLE;                            	//取消片选     
	return byte;   
} 

/******************************************************
//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
******************************************************/
static void SpiFlashWriteSR(uint8 sr)   
{   
	SPI_CS_ENABLE;                             //使能器件   
	SPI1_ReadWriteByte(W25X_WriteStatusReg);   //发送写取状态寄存器命令    
	SPI1_ReadWriteByte(sr);               		 //写入一个字节  
	SPI_CS_DISABLE;                            //取消片选     	      
}  

/******************************************************
//W25QXX写使能	
//将WEL置位   
******************************************************/
static void SpiFlashWriteEnable(void)   
{
	SPI_CS_ENABLE;                            //使能器件   
  SPI1_ReadWriteByte(W25X_WriteEnable);     //发送写使能  
	SPI_CS_DISABLE;                           //取消片选     	      
} 

/******************************************************
//W25QXX写禁止	
//将WEL清零  
******************************************************/
static void SpiFlashWriteDisable(void)   
{  
	SPI_CS_ENABLE;                            //使能器件   
  SPI1_ReadWriteByte(W25X_WriteDisable);  	//发送写禁止指令    
	SPI_CS_DISABLE;                           //取消片选     	      
} 	

/******************************************************
//等待空闲
******************************************************/
static void SpiFlashWaitBusy(void)   
{   
	while((SpiFlashReadSR()&0x01)==0x01){   // 等待BUSY位清空
		OSTimeDlyHMSM(0, 0, 0, 1);
	}
} 

/******************************************************
//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 	  
******************************************************/
#if 0
static uint16 SpiFlashReadID(void)
{
	uint16 Temp = 0;	  
	SPI_CS_ENABLE;				    
	SPI1_ReadWriteByte(W25X_ManufactDeviceID);//发送读取ID命令	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	SPI_CS_DISABLE;				    
	return Temp;
}   		    
#endif


/******************************************************
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:				数据存储区
//WriteAddr:			开始写入的地址(24bit)	0x00000000~0X007FFFFF
//NumByteToWrite:	要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	
******************************************************/ 
static void SpiFlashWritePage(uint8* pBuffer,uint32 WriteAddr,uint16 NumByteToWrite)
{
 	uint16 i;  
	
	SpiFlashWriteEnable();                  			//SET WEL 
	SPI_CS_ENABLE;                            		//使能器件   
	SPI1_ReadWriteByte(W25X_PageProgram);      		//发送写页命令   
	SPI1_ReadWriteByte((uint8)((WriteAddr)>>16)); //发送24bit地址    
	SPI1_ReadWriteByte((uint8)((WriteAddr)>>8));   
	SPI1_ReadWriteByte((uint8)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++){								//循环写数  
		SPI1_ReadWriteByte(pBuffer[i]);
	}
	SPI_CS_DISABLE;                            		//取消片选 
	SpiFlashWaitBusy();					   								//等待写入结束
}  


/// 对外接口

/******************************************************
//初始化SPI FLASH
******************************************************/
uint8 ExteFlashInit(void)
{
	Tiza_Spi1Init();

	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);		//设置为42M时钟,高速模式 

	SPI_CS_DISABLE;

	return 0;
}
/******************************************************
//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//WriteAddr:			开始写入的地址(24bit)	0x00000000~0X007FFFFF
//pBuffer:				数据存储区
//NumByteToWrite:	要写入的字节数(最大65535)
//CHECK OK
******************************************************/
uint8 ExteFlashWrite(uint32 WriteAddr,uint8* pBuffer,uint16 NumByteToWrite)   
{ 			 		 
	uint16 pageremain;	   

	if(WriteAddr + NumByteToWrite >= EXTEFLASH_CAPCITY)
		return 1;
	
	pageremain = 256-WriteAddr%256; 														//单页剩余的字节数		 	    
	
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;		//不大于256个字节
	while(1)
	{	   
		SpiFlashWritePage(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;											//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  											//减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; 									//一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  										//不够256个字节了
		}
	};	    

	return 0;
}

/******************************************************
//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//ReadAddr:			开始读取的地址(24bit) 0x00000000~0X007FFFFF
//pBuffer:			数据存储区
//NumByteToRead:要读取的字节数(最大65535)
******************************************************/
uint8 ExteFlashRead(uint32 ReadAddr, uint8* pBuffer, uint16 NumByteToRead)   
{ 
 	uint16 i;   										    
	
	SPI_CS_ENABLE;                            		//使能器件   
	SPI1_ReadWriteByte(W25X_ReadData);         		//发送读取命令   
	SPI1_ReadWriteByte((uint8)((ReadAddr)>>16));  //发送24bit地址    
	SPI1_ReadWriteByte((uint8)((ReadAddr)>>8));   
	SPI1_ReadWriteByte((uint8)ReadAddr);   
	for(i=0;i<NumByteToRead;i++){ 
		pBuffer[i]=SPI1_ReadWriteByte(0XFF);   			//循环读数  
	}
	SPI_CS_DISABLE;  				    	      

	return 0;
}  

/******************************************************
//擦除整个芯片		  
//等待时间超长...
******************************************************/
uint8 ExteFlashEraseChip(void)   
{                                   
	SpiFlashWriteEnable();                  		//SET WEL 
	SpiFlashWaitBusy();   
	SPI_CS_ENABLE;                            	//使能器件   
	SPI1_ReadWriteByte(W25X_ChipErase);        	//发送片擦除命令  
	SPI_CS_DISABLE;                            	//取消片选     	      
	SpiFlashWaitBusy();   				   						//等待芯片擦除结束

	return 0;
}   

/******************************************************
//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置   0~2047
//擦除一个山区的最少时间:150ms
******************************************************/
uint8 ExteFlashEraseSector(uint16 SectorNum)   
{
	uint32 Dst_Addr;
	
//	//监视falsh擦除情况,测试用   
//	#ifdef TIZA_SPI_DEBUG
// 	printf("spi flash sector: 0x%X\r\n",SectorNum);	  
//	#endif

	if(SectorNum * EXTEFLASH_SECTOR_SIZE > EXTEFLASH_CAPCITY)
		return 1;
	
	Dst_Addr = SectorNum * EXTEFLASH_SECTOR_SIZE;	// 4096 bytes per page

	SpiFlashWriteEnable();                 	 			//SET WEL 	 
	SpiFlashWaitBusy();   
	SPI_CS_ENABLE;                            		//使能器件   
	SPI1_ReadWriteByte(W25X_SectorErase);      		//发送扇区擦除指令 
	SPI1_ReadWriteByte((uint8)((Dst_Addr)>>16));  //发送24bit地址    
	SPI1_ReadWriteByte((uint8)((Dst_Addr)>>8));   
	SPI1_ReadWriteByte((uint8)Dst_Addr);  
	SPI_CS_DISABLE;                            		//取消片选     	      
	SpiFlashWaitBusy();   				   							//等待擦除完成

	return 0;
} 

/******************************************************
//进入掉电模式
******************************************************/
uint8 ExteFlashPowerDown(void)   
{ 
	SPI_CS_ENABLE;                             //使能器件   
	SPI1_ReadWriteByte(W25X_PowerDown);        //发送掉电命令  
	SPI_CS_DISABLE;                            //取消片选     	      
	OSTimeDlyHMSM(0, 0, 0, 1);                 //等待TPD  

	return 0;
}  

/******************************************************
//唤醒
******************************************************/
uint8 ExteFlashWakeUp(void)   
{  
	SPI_CS_ENABLE;                            	//使能器件   
	SPI1_ReadWriteByte(W25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB    
	SPI_CS_DISABLE;                            	//取消片选     	      
	OSTimeDlyHMSM(0, 0, 0, 1);					        //等待TRES1

	return 0;
}  



















