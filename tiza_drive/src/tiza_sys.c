#define SYS_GLOBAL

#include "tiza_sys.h"

#define SYS_DEBUG

#ifdef CREATE_LIB
__asm void CPU_IntDis(void)
{
  CPSID   I
  BX      LR
}

__asm void CPU_IntEn(void)
{
  CPSIE   I
  BX      LR
}
#endif

//����CPU��ģ��ʱ��
void InitSystem(void)
{
	SystemInit();
}

//ticks:���Ķ�ʱ���жϼ��,��λ��ms
void SysTickConfig(uint32 ticks)
{
	uint32 reload;
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//SYSTICKʹ���ⲿʱ��Դ	 
	
	reload=168/8;							//ÿ���ӵļ������� ��λΪK		 
	reload*=(1000000/ticks);	//����delay_ostickspersec�趨���ʱ��
	//reloadΪ24λ�Ĵ���,���ֵ:16777215	
	if(reload > 0xFFFFFF)
		reload = 0xFFFFFF;
	
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//����SYSTICK�ж�
	SysTick->LOAD=reload; 					//ÿ1/OS_TICKS_PER_SEC���ж�һ��
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //����SYSTICK
}
