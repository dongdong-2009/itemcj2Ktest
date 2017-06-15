#include "tiza_include.h"

#define MAIN_TEST_TASK 
//#define MAIN_CANS_TASK 
#define MAIN_APPL218_TASK 
//#define MAIN_PERIOD_TASK


/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			10 			//开始任务的优先级设置为最低
#define LED0_TASK_PRIO       			12 
#define TEST_TASK_PRIO       			9 
#define CANS_TASK_PRIO       			8 
#define APPL218_TASK_PRIO       	6 
#define PERIOD_TASK_PRIO       		7 
//设置任务堆栈大小
#define START_STK_SIZE  					64
#define LED0_STK_SIZE  		    		128
#define TEST_STK_SIZE							256
#define CANS_STK_SIZE							512
#define APPL218_STK_SIZE					1024
#define PERIOD_STK_SIZE						512
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
OS_STK TEST_TASK_STK[TEST_STK_SIZE];
OS_STK CANS_TASK_STK[CANS_STK_SIZE];
OS_STK APPL218_TASK_STK[APPL218_STK_SIZE];
OS_STK PERIOD_TASK_STK[PERIOD_STK_SIZE];
//任务函数
void start_task(void *pdata);	
void led0_task(void *pdata);
void Test_task(void *pdata);
void Cans_task(void *pdata);
void APP_L218_task(void *pdata);
void Period_task(void *pdata);

//信号量
OS_EVENT *Dprint_Semp;    //定义调试打印信号量

/******************************************************
系统一些模块的初始化
******************************************************/
static void System_Mode_Init(void)
{	
	g_sysmiscrun_struct.tasks_initOK_flag.byte = 0;
	ProPara_Init();	
	SysPara_Init();
	
	Tiza_GpioInit();		        //初始化LED端口 

	__NOP();__NOP();__NOP();__NOP();
	#if QLJ_DEBUG == 1
	#else
	Tiza_WdtInit(OPERATE_2WDGS, 2000);
	#endif
	
	Tiza_FeedWdg(OPERATE_2WDGS);
	Debug_init(115200);					//初始化本地调试串口
	Tiza_AdcInit();
	ExteFlashInit();	
	Tiza_FeedWdg(OPERATE_2WDGS);
	
	DPrint("Device start...\n");
	DealDebugSend(1);
	__NOP();__NOP();__NOP();__NOP();
}

/******************************************************
Main函数
******************************************************/
int main(void)
{ 
	delay_init(168);		  															//初始化延时函数
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//中断分组配置
	System_Mode_Init();   															//初始化模块
	
//	while(1)
//		Tiza_FeedWdg(OPERATE_2WDGS);
	
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	
}

 //开始任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
	
	 Dprint_Semp=OSSemCreate(1);  //创建调试打印信号量
	
	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
	//-------------- 任务指针	---------------------堆栈--------------------------------------------优先级     
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
												
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	
	OS_EXIT_CRITICAL();							//退出临界区(可以被中断打断)
} 

//LED0任务
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




