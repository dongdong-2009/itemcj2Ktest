#define WATCH_DOG_GLOBAL

#include "tiza_wd.h"
#include "tiza_selfdef.h"

#define WATCHDOG_DEBUG

static uint8_t flag = FALSE;


//�ⲿӲ�����Ź�
void EWdtInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIOD��ʱ��

    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL; 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD, GPIO_Pin_14); //watch dog wdi
}

void ClearEWatchdog(void)
{
	if(flag)
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_14); //watch dog wdi
	}
	else
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_14); //watch dog wdi
	}
	
	flag = !flag;
}

//�ڲ��������Ź�
void WdtInit(uint16 ReloadValue)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	IWDG_SetPrescaler(IWDG_Prescaler_32);

	IWDG_SetReload(ReloadValue);

	IWDG_ReloadCounter();

	IWDG_Enable();
}

void ClearWatchdog(void)
{
	IWDG_ReloadCounter();
}

