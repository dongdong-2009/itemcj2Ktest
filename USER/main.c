#include "tiza_include.h"




/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 			//��ʼ��������ȼ�����Ϊ���
#define LED0_TASK_PRIO       			7 
#define LED1_TASK_PRIO       			6 
//���������ջ��С
#define START_STK_SIZE  					64
#define LED0_STK_SIZE  		    		64
#define LED1_STK_SIZE  						64
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//������
void start_task(void *pdata);	
void led0_task(void *pdata);
void led1_task(void *pdata);
 			   
/******************************************************
ϵͳһЩģ��ĳ�ʼ��
******************************************************/
static void System_Mode_Init(void)
{
	GpioInit();		        //��ʼ��LED�˿� 
	
	
	__NOP();__NOP();__NOP();__NOP();
}

/******************************************************
Main����
******************************************************/
int main(void)
{ 
	delay_init(168);		  															//��ʼ����ʱ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//�жϷ�������
	System_Mode_Init();   															//��ʼ��ģ��
	
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	
}

 //��ʼ����
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
	
	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
	
	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
	OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	 				   
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
} 

//LED0����
void led0_task(void *pdata)
{	 	
	while(1)
	{
		ON_GRE_LED();
		delay_ms(300);
		OFF_GRE_LED();
		delay_ms(300);
	};
}

//LED1����
void led1_task(void *pdata)
{	  
	while(1)
	{
		ON_RED_LED();
		delay_ms(300);
		OFF_RED_LED();
		delay_ms(300);
	};
}



