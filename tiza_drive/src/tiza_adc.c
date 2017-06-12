/**
PB0  	POW_C
PB1		BAT
**/


#define TIZA_ADC_GLOBAL

#include "tiza_include.h"



/******************************************************
Adc��ʼ������
******************************************************/
void Tiza_AdcInit(void)
{	
	GPIO_InitTypeDef  		GPIO_InitStructure;
	DMA_InitTypeDef  			DMA_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);			//ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 				//ʹ��ADC1ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);					//DMA1ʱ��ʹ�� 
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 | GPIO_Pin_1;		//PB0 PB1 ͨ��8 9
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;								//ģ������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//���� ��������Ч
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL ;					//����������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 					//����GPIO
	GPIO_Init(GPIOB, &GPIO_InitStructure);											//��ʼ�� IO  
	
	/* ���� DMA Stream */
	DMA_DeInit(DMA2_Stream4);																										//����ʼ��������4
	while (DMA_GetCmdStatus(DMA2_Stream4) != DISABLE){													//�ȴ�DMA������ 
		OSTimeDlyHMSM(0, 0, 0, 5);	
	}	
  DMA_InitStructure.DMA_Channel 						= DMA_Channel_0;  								//ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr 	= (u32)&ADC1->DR;									//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr 		= (u32)g_Adc1Dma2_buff;						//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR 								= DMA_DIR_PeripheralToMemory;			//���赽�洢��ģʽ
  DMA_InitStructure.DMA_BufferSize 					= 2;															//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;			//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc					  = DMA_MemoryInc_Enable;						//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_HalfWord;		//�������ݳ���:16λ
  DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_HalfWord;		//�洢�����ݳ���:16λ
  DMA_InitStructure.DMA_Mode 								= DMA_Mode_Circular;							//ʹ��ѭ��ģʽ 
  DMA_InitStructure.DMA_Priority 						= DMA_Priority_Low;								//�͵����ȼ�
  DMA_InitStructure.DMA_FIFOMode 						= DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold 			= DMA_FIFOThreshold_HalfFull;			// 1/2FIFO	
  DMA_InitStructure.DMA_MemoryBurst 				= DMA_MemoryBurst_Single;					//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;			//����ͻ�����δ���
  DMA_Init(DMA2_Stream4, &DMA_InitStructure);				//��ʼ��DMA Stream
	
	/* ���� ADC1 */
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);	  		//ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);				//��λ����	
	ADC_CommonInitStructure.ADC_Mode 							= ADC_Mode_Independent;							//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay 	= ADC_TwoSamplingDelay_20Cycles;		//���������׶�֮����ӳ�20��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode 		= ADC_DMAAccessMode_Disabled; 			//DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler 				= ADC_Prescaler_Div4;								//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);					//��ʼ��
	ADC_InitStructure.ADC_Resolution 							= ADC_Resolution_12b;								//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode 						= ENABLE;														//ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode 			= ENABLE;														//����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge 		= ADC_ExternalTrigConvEdge_None;		//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign 							= ADC_DataAlign_Right;							//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion 				= 2;																//2��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);								//ADC��ʼ��
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_144Cycles);				//����ģʽͨ������     
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_144Cycles);    
	  
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); //ʹ��ADC��DMA  	
	ADC_DMACmd(ADC1, ENABLE);   											//����ADC��DMA֧��
	ADC_Cmd(ADC1, ENABLE);														//����ADת����		
	ADC_SoftwareStartConv(ADC1);    									//����ADC1�����ת��  
  DMA_Cmd(DMA2_Stream4, ENABLE);    								//ʹ��DMAͨ��  
	
}


/******************************************************
��ʼ������
******************************************************/





















