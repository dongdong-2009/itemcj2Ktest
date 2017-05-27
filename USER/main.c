#include "tiza_include.h"




/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			10 			//开始任务的优先级设置为最低
#define LED0_TASK_PRIO       			7 
#define LED1_TASK_PRIO       			6 
//设置任务堆栈大小
#define START_STK_SIZE  					64
#define LED0_STK_SIZE  		    		64
#define LED1_STK_SIZE  						64
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//任务函数
void start_task(void *pdata);	
void led0_task(void *pdata);
void led1_task(void *pdata);
 			   
/******************************************************
系统一些模块的初始化
******************************************************/
static void System_Mode_Init(void)
{
	GpioInit();		        //初始化LED端口 
	
	
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
	
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	
}

 //开始任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
	
	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
	
	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
	OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	 				   
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
} 

//LED0任务
void led0_task(void *pdata)
{	 	
	uint8 temp_data[20],len,i,j=0;
	uint16 mat_index;
	
	Debug_init(115200);
	//-DPrint("\n硬件定时器测试:完成时间:%dms",5000);	//-向FIFO中写入数据.
	//-DealDebugSend(1);
	RS232_init(115200);
	
	while(1)
	{
		ON_GRE_LED();
		delay_ms(300);
		OFF_GRE_LED();
		delay_ms(300);
		
		len = ReadDebugData(temp_data,20);
		if(len)
		{
			DPrint("\nDebug接收测试:打印接收指令:%s",temp_data);
			DealDebugSend(1);
			mat_index = SubMatch("ok",2,temp_data,len);
			if(mat_index)
			{
				DPrint("\nDebug接收测试:打印次数:%d次",j);
				j++;
				DealDebugSend(1);
			}
		}
		//-for(i=0;i<len;i++)
			//-DebugSendData(temp_data[i]);
//		DPrint("\nDebug测试:打印次数:%d次",j);
//		j++;
//		DealDebugSend(1);
		
		len = ReadRS232Data(temp_data,20);
		if(len)
		{
			RS232Print("\nRS232接收测试:打印接收指令:%s",temp_data);
			DealRS232Send(1);
			mat_index = SubMatch("ok",2,temp_data,len);
			if(mat_index)
			{
				RS232Print("\nRS232接收测试:打印次数:%d次",j);
				j++;
				DealRS232Send(1);
			}
		}
	}
}

//LED1任务
void led1_task(void *pdata)
{	  
	
	
	
	while(1)
	{
		ON_RED_LED();
		delay_ms(300);
		OFF_RED_LED();
		delay_ms(300);
		//-RS232SendData(0xAA);
//		RS232Print("\n硬件定时器测试:完成时间:%dms",5000);	//-向FIFO中写入数据.
//		DealRS232Send(1);
	}
}



