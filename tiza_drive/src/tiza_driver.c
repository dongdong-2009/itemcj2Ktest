#define DRIVER_GLOBAL

#include "tiza_include.h"

//#define RTC_DEBUG

/******************************************************
GPIO��ʼ������
******************************************************/
void GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
/***	 
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIO_DeInit(GPIOD);
	GPIO_DeInit(GPIOE);
	GPIO_DeInit(GPIOF);
	GPIO_DeInit(GPIOG);
***/	
//	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE 
//											  | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG		, ENABLE); //ʹ��GPIO��ʱ��
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE 
											  | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG		, ENABLE); //ʹ��GPIO��ʱ��

///�������
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;									//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//50MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;									//����
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_Init(GPIOG, &GPIO_InitStructure);
		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;			//��ƺ��̵ƶ�ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;									//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;						//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;									//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);												//��ʼ��GPIO
	
}


