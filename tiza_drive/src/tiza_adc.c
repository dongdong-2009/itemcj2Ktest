/**
PB0  	POW_C
PB1		BAT
**/


#define TIZA_ADC_GLOBAL

#include "tiza_include.h"



/******************************************************
Adc初始化函数
******************************************************/
void Tiza_AdcInit(void)
{	
	GPIO_InitTypeDef  		GPIO_InitStructure;
	DMA_InitTypeDef  			DMA_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);			//使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 				//使能ADC1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);					//DMA1时钟使能 
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 | GPIO_Pin_1;		//PB0 PB1 通道8 9
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;								//模拟输入
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//推挽 对输入无效
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL ;					//不带上下拉
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 					//高速GPIO
	GPIO_Init(GPIOB, &GPIO_InitStructure);											//初始化 IO  
	
	/* 配置 DMA Stream */
	DMA_DeInit(DMA2_Stream4);																										//反初始化数据流4
	while (DMA_GetCmdStatus(DMA2_Stream4) != DISABLE){													//等待DMA可配置 
		OSTimeDlyHMSM(0, 0, 0, 5);	
	}	
  DMA_InitStructure.DMA_Channel 						= DMA_Channel_0;  								//通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr 	= (u32)&ADC1->DR;									//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr 		= (u32)g_Adc1Dma2_buff;						//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR 								= DMA_DIR_PeripheralToMemory;			//外设到存储器模式
  DMA_InitStructure.DMA_BufferSize 					= 2;															//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;			//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc					  = DMA_MemoryInc_Enable;						//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_HalfWord;		//外设数据长度:16位
  DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_HalfWord;		//存储器数据长度:16位
  DMA_InitStructure.DMA_Mode 								= DMA_Mode_Circular;							//使用循环模式 
  DMA_InitStructure.DMA_Priority 						= DMA_Priority_Low;								//低等优先级
  DMA_InitStructure.DMA_FIFOMode 						= DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold 			= DMA_FIFOThreshold_HalfFull;			// 1/2FIFO	
  DMA_InitStructure.DMA_MemoryBurst 				= DMA_MemoryBurst_Single;					//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;			//外设突发单次传输
  DMA_Init(DMA2_Stream4, &DMA_InitStructure);				//初始化DMA Stream
	
	/* 配置 ADC1 */
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);	  		//ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);				//复位结束	
	ADC_CommonInitStructure.ADC_Mode 							= ADC_Mode_Independent;							//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay 	= ADC_TwoSamplingDelay_20Cycles;		//两个采样阶段之间的延迟20个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode 		= ADC_DMAAccessMode_Disabled; 			//DMA失能
	ADC_CommonInitStructure.ADC_Prescaler 				= ADC_Prescaler_Div4;								//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);					//初始化
	ADC_InitStructure.ADC_Resolution 							= ADC_Resolution_12b;								//12位模式
	ADC_InitStructure.ADC_ScanConvMode 						= ENABLE;														//扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode 			= ENABLE;														//连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge 		= ADC_ExternalTrigConvEdge_None;		//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign 							= ADC_DataAlign_Right;							//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion 				= 2;																//2个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);								//ADC初始化
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_144Cycles);				//规则模式通道配置     
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_144Cycles);    
	  
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); //使能ADC的DMA  	
	ADC_DMACmd(ADC1, ENABLE);   											//开启ADC的DMA支持
	ADC_Cmd(ADC1, ENABLE);														//开启AD转换器		
	ADC_SoftwareStartConv(ADC1);    									//开启ADC1的软件转换  
  DMA_Cmd(DMA2_Stream4, ENABLE);    								//使能DMA通道  
	
}


/******************************************************
初始化函数
******************************************************/





















