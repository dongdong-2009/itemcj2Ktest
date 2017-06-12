#define TIZA_DRIVER_GLOBAL

#include "tiza_include.h"


/******************************************************
GPIO��ʼ������
******************************************************/
void Tiza_GpioInit(void)
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
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE 
											  | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG		, ENABLE); //ʹ��GPIO��ʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;									//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//50MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;									//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_All & 0x1FFF;					//PA13~15 JTAG
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_All & 0xFFE7;					//PB03~04 JTAG
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE , DISABLE); 			//�ر�GPIOE��ʱ��	(δʹ��)

///�������		
		
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14 | GPIO_Pin_15;		//��ƺ��̵� ��ӦIO��
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;								//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//�������
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;									//����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);												//��ʼ��GPIO
	
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;										//C_LED �Ƶƶ�ӦIO��
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;								//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//�������
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;									//����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//50MHz
  GPIO_Init(GPIOF, &GPIO_InitStructure);												//��ʼ��GPIO
	
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;										//CT_LOCK �̵��� ��ӦIO��
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;								//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//�������
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;									//����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//50MHz
  GPIO_Init(GPIOG, &GPIO_InitStructure);												//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_1;										//CT_SD��Դ ��������
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//�������
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//����GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_2;										//CT_485232��Դ ��������
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//�������
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//����GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_10;									//CT_5V 5V��Դ ��������
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//�������
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//����GPIO
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;										//POWERKEY_CONNECT��ģ��ػ���������
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//�������
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//����GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_8;										//CT_4V 4V��Դ ��������
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//�������
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//����GPIO
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_9;										//CTS_L218 ��������
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//�������
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//����GPIO
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_8;										//RSEET_L218 ��������
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//�������
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//����GPIO
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_12;										//DTR_L218 ��������
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//�������
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//����GPIO
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_14;									//WDI ι�⹷ ��������
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;//GPIO_Mode_OUT;								//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//�������
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//����GPIO
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
///��������

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_7;										// RING_L218 ���
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;									//����
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//���� ��������Ч
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//����GPIO
	GPIO_Init(GPIOF,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;										// ACC ���
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;									//����
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//���� ��������Ч
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//����GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;										// check power �����
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;									//����
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//���� ��������Ч
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//����GPIO
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	//=========================================================================//
	OFF_LOCK();						//�ؼ̵���
	ON_485232_PWR();			//��232 485 3.3V��Դ
	ON_CT5V_PWR();				//��5V�ܵ�Դ
	
}


/******************************************************
���Ź���ʼ������
���룺
			flag(1����): bit0�ڲ����Ź� bit1�ⲿ���Ź�
			ReloadVal	:	 �ڲ����Ź�����ʱ��(�����ڲ�ʱ��)
******************************************************/
void Tiza_WdtInit(uint8 flag, uint16 ReloadVal)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if((flag&OPERATE_IWDG) == OPERATE_IWDG){
	//�ڲ����Ź�	
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
		IWDG_SetPrescaler(IWDG_Prescaler_32);
		IWDG_SetReload(ReloadVal);
		IWDG_ReloadCounter();
		IWDG_Enable();
	}
		
	if((flag&OPERATE_EWDG) == OPERATE_EWDG){
	//�ⲿ���Ź�	
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_14;									//WDI ι�⹷ ��������
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//���
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//�������
		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//��������
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//����GPIO
		GPIO_Init(GPIOD,&GPIO_InitStructure);

		REST_WDT();
	}
}

/******************************************************
���Ź�ι������
���룺
			flag(1����): bit0�ڲ����Ź� bit1�ⲿ���Ź�
******************************************************/
void Tiza_FeedWdg(uint8 flag)
{
	
	if((flag&OPERATE_IWDG) == OPERATE_IWDG){
	//�ڲ����Ź�	
		IWDG_ReloadCounter();
	}
		
	if((flag&OPERATE_EWDG) == OPERATE_EWDG){
	//�ⲿ���Ź�	
		FEED_WDT();
	}
}





void SysReset(void)
{
//	while(1)
//	{
//		NVIC_DISABLE();
//	}
}




