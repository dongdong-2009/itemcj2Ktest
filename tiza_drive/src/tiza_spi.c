/**
PA4  	CS
PA5  	SCK
PA6  	MISO
PA7  	MOSI
**/

#define TIZA_SPI_GLOBAL
#define TIZA_SPI_DEBUG

#include "tiza_include.h"


//ָ���
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
Spi1��ʼ������
******************************************************/
#if SPI1_USE_DMA2 == 0		
static void Tiza_Spi1Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);		//ʹ��SPI1ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);			//DMA1ʱ��ʹ�� 

	//GPIOA4��ʼ������  							CS
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;					//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;				//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);									//��ʼ��
	//GPIOA5,6,7��ʼ������				SCK           MISO          MOSI
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;						//PA5~7���ù������	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;						//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);									//��ʼ��
		
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1); //PA5����Ϊ SPI1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1); //PA6����Ϊ SPI1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1); //PA7����Ϊ SPI1
	
	/* ���� Spi1 */
	SPI_DeInit(SPI1);		
	SPI_InitStructure.SPI_Direction 				= SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode 							= SPI_Mode_Master;									//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize 					= SPI_DataSize_8b;									//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL 							= SPI_CPOL_High;										//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA 							= SPI_CPHA_2Edge;										//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS 							= SPI_NSS_Soft;											//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;				//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit 					= SPI_FirstBit_MSB;									//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial 		= 7;																//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
 
 
	SPI_Cmd(SPI1, ENABLE); 														//ʹ��SPI����
	SPI_SSOutputCmd(SPI1, ENABLE);
	
//	SPI1_ReadWriteByte(0xff);													//��������		 
  
}

#else		//ʹ��DMA
static void Tiza_Spi1Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);		//ʹ��SPI1ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);			//DMA1ʱ��ʹ�� 

	//GPIOA4��ʼ������  							CS
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;					//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;				//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);									//��ʼ��
	//GPIOA5,6,7��ʼ������				SCK           MISO          MOSI
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;						//PA5~7���ù������	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;						//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);									//��ʼ��
		
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1); //PA5����Ϊ SPI1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1); //PA6����Ϊ SPI1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1); //PA7����Ϊ SPI1
		
	/* ���� DMA Stream */
	//SPI1_RX
	DMA_DeInit(DMA2_Stream2);																										//����ʼ��������2
	while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE){													//�ȴ�DMA������ 
		OSTimeDlyHMSM(0, 0, 0, 5);	
	}	
  DMA_InitStructure.DMA_Channel 						= DMA_Channel_3;  								//ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr 	= (u32)&SPI1->DR;									//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr 		= (u32)g_Spi1Dma2_Rx_buff;				//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR 								= DMA_DIR_PeripheralToMemory;			//���赽�洢��ģʽ
  DMA_InitStructure.DMA_BufferSize 					= TIZA_SPI1_RX_BUFF_LEN;					//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;			//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc					  = DMA_MemoryInc_Enable;						//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;		//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_Byte;				//�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode 								= DMA_Mode_Circular;							//ʹ��ѭ��ģʽ 
  DMA_InitStructure.DMA_Priority 						= DMA_Priority_High;							//�ߵ����ȼ�
  DMA_InitStructure.DMA_FIFOMode 						= DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold 			= DMA_FIFOThreshold_HalfFull;			// 1/2 FIFO	
  DMA_InitStructure.DMA_MemoryBurst 				= DMA_MemoryBurst_Single;					//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;			//����ͻ�����δ���
  DMA_Init(DMA2_Stream2, &DMA_InitStructure);				//��ʼ��DMA Stream
	
	//SPI1_TX
	DMA_DeInit(DMA2_Stream3);																										//����ʼ��������3
	while (DMA_GetCmdStatus(DMA2_Stream3) != DISABLE){													//�ȴ�DMA������ 
		OSTimeDlyHMSM(0, 0, 0, 5);	
	}	
  DMA_InitStructure.DMA_Channel 						= DMA_Channel_3;  								//ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr 	= (u32)&SPI1->DR;									//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr 		= (u32)g_Spi1Dma2_Tx_buff;				//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR 								= DMA_DIR_MemoryToPeripheral;			//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize 					= TIZA_SPI1_RX_BUFF_LEN;					//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;			//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc					  = DMA_MemoryInc_Enable;						//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;		//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_Byte;				//�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode 								= DMA_Mode_Circular;							//ʹ��ѭ��ģʽ 
  DMA_InitStructure.DMA_Priority 						= DMA_Priority_High;							//�ߵ����ȼ�
  DMA_InitStructure.DMA_FIFOMode 						= DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold 			= DMA_FIFOThreshold_HalfFull;			// 1/2 FIFO	
  DMA_InitStructure.DMA_MemoryBurst 				= DMA_MemoryBurst_Single;					//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;			//����ͻ�����δ���
  DMA_Init(DMA2_Stream3, &DMA_InitStructure);				//��ʼ��DMA Stream
	
	/* ���� Spi1 */
	SPI_DeInit(SPI1);		
	SPI_InitStructure.SPI_Direction 				= SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode 							= SPI_Mode_Master;									//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize 					= SPI_DataSize_8b;									//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL 							= SPI_CPOL_High;										//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA 							= SPI_CPHA_2Edge;										//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS 							= SPI_NSS_Soft;											//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;				//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit 					= SPI_FirstBit_MSB;									//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial 		= 7;																//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
 
 
	SPI_Cmd(SPI1, ENABLE); 														//ʹ��SPI����
	SPI_SSOutputCmd(SPI1, ENABLE);
	
  DMA_Cmd(DMA2_Stream2, ENABLE);    								//ʹ��DMAͨ��  
  DMA_Cmd(DMA2_Stream3, ENABLE);    								//ʹ��DMAͨ��  
  
	
}
#endif

/******************************************************
	SPI�ٶ����ú���
//SPI1�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz��
******************************************************/
static void SPI1_SetSpeed(uint8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
} 

/******************************************************
	SPI��д����
//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
******************************************************/
static uint8 SPI1_ReadWriteByte(uint8 TxData)
{		 			 
 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){		//�ȴ���������  
		OSTimeDlyHMSM(0, 0, 0, 1);	
	}

	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ��byte  ����

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){	//�ȴ�������һ��byte
		OSTimeDlyHMSM(0, 0, 0, 1);	
	}  

	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	
}





//================================================================================================================//
///SPI_FLASH

///4096byteΪһ��Sector
///16������Ϊ1��Block
///W25Q64
///����Ϊ8M�ֽ�,����64��Block,2048��Sector 


/******************************************************
//��ȡW25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
******************************************************/
static uint8 SpiFlashReadSR(void)   
{  
	uint8 byte=0;   
	SPI_CS_ENABLE;                            	//ʹ������   
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    	//���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI1_ReadWriteByte(0Xff);            	//��ȡһ���ֽ�  
	SPI_CS_DISABLE;                            	//ȡ��Ƭѡ     
	return byte;   
} 

/******************************************************
//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
******************************************************/
static void SpiFlashWriteSR(uint8 sr)   
{   
	SPI_CS_ENABLE;                             //ʹ������   
	SPI1_ReadWriteByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	SPI1_ReadWriteByte(sr);               		 //д��һ���ֽ�  
	SPI_CS_DISABLE;                            //ȡ��Ƭѡ     	      
}  

/******************************************************
//W25QXXдʹ��	
//��WEL��λ   
******************************************************/
static void SpiFlashWriteEnable(void)   
{
	SPI_CS_ENABLE;                            //ʹ������   
  SPI1_ReadWriteByte(W25X_WriteEnable);     //����дʹ��  
	SPI_CS_DISABLE;                           //ȡ��Ƭѡ     	      
} 

/******************************************************
//W25QXXд��ֹ	
//��WEL����  
******************************************************/
static void SpiFlashWriteDisable(void)   
{  
	SPI_CS_ENABLE;                            //ʹ������   
  SPI1_ReadWriteByte(W25X_WriteDisable);  	//����д��ָֹ��    
	SPI_CS_DISABLE;                           //ȡ��Ƭѡ     	      
} 	

/******************************************************
//�ȴ�����
******************************************************/
static void SpiFlashWaitBusy(void)   
{   
	while((SpiFlashReadSR()&0x01)==0x01){   // �ȴ�BUSYλ���
		OSTimeDlyHMSM(0, 0, 0, 1);
	}
} 

/******************************************************
//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
******************************************************/
#if 0
static uint16 SpiFlashReadID(void)
{
	uint16 Temp = 0;	  
	SPI_CS_ENABLE;				    
	SPI1_ReadWriteByte(W25X_ManufactDeviceID);//���Ͷ�ȡID����	    
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
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:				���ݴ洢��
//WriteAddr:			��ʼд��ĵ�ַ(24bit)	0x00000000~0X007FFFFF
//NumByteToWrite:	Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	
******************************************************/ 
static void SpiFlashWritePage(uint8* pBuffer,uint32 WriteAddr,uint16 NumByteToWrite)
{
 	uint16 i;  
	
	SpiFlashWriteEnable();                  			//SET WEL 
	SPI_CS_ENABLE;                            		//ʹ������   
	SPI1_ReadWriteByte(W25X_PageProgram);      		//����дҳ����   
	SPI1_ReadWriteByte((uint8)((WriteAddr)>>16)); //����24bit��ַ    
	SPI1_ReadWriteByte((uint8)((WriteAddr)>>8));   
	SPI1_ReadWriteByte((uint8)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++){								//ѭ��д��  
		SPI1_ReadWriteByte(pBuffer[i]);
	}
	SPI_CS_DISABLE;                            		//ȡ��Ƭѡ 
	SpiFlashWaitBusy();					   								//�ȴ�д�����
}  


/// ����ӿ�

/******************************************************
//��ʼ��SPI FLASH
******************************************************/
uint8 ExteFlashInit(void)
{
	Tiza_Spi1Init();

	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);		//����Ϊ42Mʱ��,����ģʽ 

	SPI_CS_DISABLE;

	return 0;
}
/******************************************************
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//WriteAddr:			��ʼд��ĵ�ַ(24bit)	0x00000000~0X007FFFFF
//pBuffer:				���ݴ洢��
//NumByteToWrite:	Ҫд����ֽ���(���65535)
//CHECK OK
******************************************************/
uint8 ExteFlashWrite(uint32 WriteAddr,uint8* pBuffer,uint16 NumByteToWrite)   
{ 			 		 
	uint16 pageremain;	   

	if(WriteAddr + NumByteToWrite >= EXTEFLASH_CAPCITY)
		return 1;
	
	pageremain = 256-WriteAddr%256; 														//��ҳʣ����ֽ���		 	    
	
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;		//������256���ֽ�
	while(1)
	{	   
		SpiFlashWritePage(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;											//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  											//��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; 									//һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  										//����256���ֽ���
		}
	};	    

	return 0;
}

/******************************************************
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//ReadAddr:			��ʼ��ȡ�ĵ�ַ(24bit) 0x00000000~0X007FFFFF
//pBuffer:			���ݴ洢��
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
******************************************************/
uint8 ExteFlashRead(uint32 ReadAddr, uint8* pBuffer, uint16 NumByteToRead)   
{ 
 	uint16 i;   										    
	
	SPI_CS_ENABLE;                            		//ʹ������   
	SPI1_ReadWriteByte(W25X_ReadData);         		//���Ͷ�ȡ����   
	SPI1_ReadWriteByte((uint8)((ReadAddr)>>16));  //����24bit��ַ    
	SPI1_ReadWriteByte((uint8)((ReadAddr)>>8));   
	SPI1_ReadWriteByte((uint8)ReadAddr);   
	for(i=0;i<NumByteToRead;i++){ 
		pBuffer[i]=SPI1_ReadWriteByte(0XFF);   			//ѭ������  
	}
	SPI_CS_DISABLE;  				    	      

	return 0;
}  

/******************************************************
//��������оƬ		  
//�ȴ�ʱ�䳬��...
******************************************************/
uint8 ExteFlashEraseChip(void)   
{                                   
	SpiFlashWriteEnable();                  		//SET WEL 
	SpiFlashWaitBusy();   
	SPI_CS_ENABLE;                            	//ʹ������   
	SPI1_ReadWriteByte(W25X_ChipErase);        	//����Ƭ��������  
	SPI_CS_DISABLE;                            	//ȡ��Ƭѡ     	      
	SpiFlashWaitBusy();   				   						//�ȴ�оƬ��������

	return 0;
}   

/******************************************************
//����һ������
//Dst_Addr:������ַ ����ʵ����������   0~2047
//����һ��ɽ��������ʱ��:150ms
******************************************************/
uint8 ExteFlashEraseSector(uint16 SectorNum)   
{
	uint32 Dst_Addr;
	
//	//����falsh�������,������   
//	#ifdef TIZA_SPI_DEBUG
// 	printf("spi flash sector: 0x%X\r\n",SectorNum);	  
//	#endif

	if(SectorNum * EXTEFLASH_SECTOR_SIZE > EXTEFLASH_CAPCITY)
		return 1;
	
	Dst_Addr = SectorNum * EXTEFLASH_SECTOR_SIZE;	// 4096 bytes per page

	SpiFlashWriteEnable();                 	 			//SET WEL 	 
	SpiFlashWaitBusy();   
	SPI_CS_ENABLE;                            		//ʹ������   
	SPI1_ReadWriteByte(W25X_SectorErase);      		//������������ָ�� 
	SPI1_ReadWriteByte((uint8)((Dst_Addr)>>16));  //����24bit��ַ    
	SPI1_ReadWriteByte((uint8)((Dst_Addr)>>8));   
	SPI1_ReadWriteByte((uint8)Dst_Addr);  
	SPI_CS_DISABLE;                            		//ȡ��Ƭѡ     	      
	SpiFlashWaitBusy();   				   							//�ȴ��������

	return 0;
} 

/******************************************************
//�������ģʽ
******************************************************/
uint8 ExteFlashPowerDown(void)   
{ 
	SPI_CS_ENABLE;                             //ʹ������   
	SPI1_ReadWriteByte(W25X_PowerDown);        //���͵�������  
	SPI_CS_DISABLE;                            //ȡ��Ƭѡ     	      
	OSTimeDlyHMSM(0, 0, 0, 1);                 //�ȴ�TPD  

	return 0;
}  

/******************************************************
//����
******************************************************/
uint8 ExteFlashWakeUp(void)   
{  
	SPI_CS_ENABLE;                            	//ʹ������   
	SPI1_ReadWriteByte(W25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB    
	SPI_CS_DISABLE;                            	//ȡ��Ƭѡ     	      
	OSTimeDlyHMSM(0, 0, 0, 1);					        //�ȴ�TRES1

	return 0;
}  



















