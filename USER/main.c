#include "tiza_include.h"

#define MAIN_TEST_TASK 
//#define MAIN_CANS_TASK 
#define MAIN_APPL218_TASK 
//#define MAIN_PERIOD_TASK


/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 			//��ʼ��������ȼ�����Ϊ���
#define LED0_TASK_PRIO       			12 
#define TEST_TASK_PRIO       			9 
#define CANS_TASK_PRIO       			8 
#define APPL218_TASK_PRIO       	6 
#define PERIOD_TASK_PRIO       		7 
//���������ջ��С
#define START_STK_SIZE  					64
#define LED0_STK_SIZE  		    		128
#define TEST_STK_SIZE							256
#define CANS_STK_SIZE							512
#define APPL218_STK_SIZE					1024
#define PERIOD_STK_SIZE						512
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
OS_STK TEST_TASK_STK[TEST_STK_SIZE];
OS_STK CANS_TASK_STK[CANS_STK_SIZE];
OS_STK APPL218_TASK_STK[APPL218_STK_SIZE];
OS_STK PERIOD_TASK_STK[PERIOD_STK_SIZE];
//������
void start_task(void *pdata);	
void led0_task(void *pdata);
void Test_task(void *pdata);
void Cans_task(void *pdata);
void APP_L218_task(void *pdata);
void Period_task(void *pdata);

//�ź���
OS_EVENT *Dprint_Semp;    //������Դ�ӡ�ź���

/******************************************************
ϵͳһЩģ��ĳ�ʼ��
******************************************************/
static void System_Mode_Init(void)
{	
	g_sysmiscrun_struct.tasks_initOK_flag.byte = 0;
	ProPara_Init();	
	SysPara_Init();
	
	Tiza_GpioInit();		        //��ʼ��LED�˿� 

	__NOP();__NOP();__NOP();__NOP();
	#if QLJ_DEBUG == 1
	#else
	Tiza_WdtInit(OPERATE_2WDGS, 2000);
	#endif
	
	Tiza_FeedWdg(OPERATE_2WDGS);
	Debug_init(115200);					//��ʼ�����ص��Դ���
	Tiza_AdcInit();
	ExteFlashInit();	
	Tiza_FeedWdg(OPERATE_2WDGS);
	
	DPrint("Device start...\n");
	DealDebugSend(1);
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
	
//	while(1)
//		Tiza_FeedWdg(OPERATE_2WDGS);
	
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	
}

 //��ʼ����
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
	
	 Dprint_Semp=OSSemCreate(1);  //�������Դ�ӡ�ź���
	
	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
	//-------------- ����ָ��	---------------------��ջ--------------------------------------------���ȼ�     
	OSTaskCreate(		led0_task,	(void *)0,	(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],	LED0_TASK_PRIO);						   
	
	#ifdef  MAIN_TEST_TASK
    (void)OSTaskCreateExt(Test_task,
                          (void *)0,                                			/* No arguments passed to OSTmrTask()      */
                          &TEST_TASK_STK[TEST_STK_SIZE - 1],        			/* Set Top-Of-Stack                        */
                          TEST_TASK_PRIO,
                          TEST_TASK_PRIO,
                          &TEST_TASK_STK[0],                        			/* Set Bottom-Of-Stack                     */
                          TEST_STK_SIZE,
                          (void *)0,                                      /* No TCB extension                        */
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);     /* Enable stack checking + clear stack     */
  #endif
													
	#ifdef  MAIN_CANS_TASK
	(void)OSTaskCreateExt(Cans_task,
												(void *)0,                                			/* No arguments passed to OSTmrTask()      */
												&CANS_TASK_STK[CANS_STK_SIZE - 1],        			/* Set Top-Of-Stack                        */
												CANS_TASK_PRIO,
												CANS_TASK_PRIO,
												&CANS_TASK_STK[0],                        			/* Set Bottom-Of-Stack                     */
												CANS_STK_SIZE,
												(void *)0,                                      /* No TCB extension                        */
												OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);     /* Enable stack checking + clear stack     */
	#endif
	#ifdef  MAIN_APPL218_TASK
	(void)OSTaskCreateExt(APP_L218_task,
												(void *)0,                                			/* No arguments passed to OSTmrTask()      */
												&APPL218_TASK_STK[APPL218_STK_SIZE - 1],     		/* Set Top-Of-Stack                        */
												APPL218_TASK_PRIO,
												APPL218_TASK_PRIO,
												&APPL218_TASK_STK[0],                      			/* Set Bottom-Of-Stack                     */
												APPL218_STK_SIZE,
												(void *)0,                                      /* No TCB extension                        */
												OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);     /* Enable stack checking + clear stack     */
	#endif
	#ifdef  MAIN_PERIOD_TASK
	(void)OSTaskCreateExt(Period_task,
												(void *)0,                                			/* No arguments passed to OSTmrTask()      */
												&PERIOD_TASK_STK[PERIOD_STK_SIZE - 1],     		/* Set Top-Of-Stack                        */
												PERIOD_TASK_PRIO,
												PERIOD_TASK_PRIO,
												&PERIOD_TASK_STK[0],                      			/* Set Bottom-Of-Stack                     */
												PERIOD_STK_SIZE,
												(void *)0,                                      /* No TCB extension                        */
												OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);     /* Enable stack checking + clear stack     */
	#endif
												
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	
	OS_EXIT_CRITICAL();							//�˳��ٽ���(���Ա��жϴ��)
} 

//LED0����
void led0_task(void *pdata)
{	 	
	g_sysmiscrun_struct.tasks_initOK_flag.bit.B3 = 1;
	while(1)
	{
	Tiza_FeedWdg(OPERATE_2WDGS);
//-		DealDebugSend(1);
//		ON_GRE_LED();
		OSTimeDlyHMSM(0, 0, 0, 100);		
//		Tiza_FeedWdg(OPERATE_2WDGS);
//		OSTimeDlyHMSM(0, 0, 0, 100);		
//		Tiza_FeedWdg(OPERATE_2WDGS);
//		OSTimeDlyHMSM(0, 0, 0, 100);		
//		Tiza_FeedWdg(OPERATE_2WDGS);
//		OFF_GRE_LED();
//		OSTimeDlyHMSM(0, 0, 0, 100);		
//		Tiza_FeedWdg(OPERATE_2WDGS);
//		OSTimeDlyHMSM(0, 0, 0, 100);		
//		Tiza_FeedWdg(OPERATE_2WDGS);
//		OSTimeDlyHMSM(0, 0, 0, 100);		
//		Tiza_FeedWdg(OPERATE_2WDGS);
		
	};
}




