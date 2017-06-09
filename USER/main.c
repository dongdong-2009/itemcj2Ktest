#include "tiza_include.h"

#define TEST_SD 0
#define TEST_RTC 1
#define TEST_GREEN 0

#define S_DEBUGF DPrint

INT16U TimeDelay;
QUEUE L218_AT_data;
uint8 L218_data[256];

typedef struct
{
  INT8U   	StoreName[13];				//正在存储数据的文件名称；
  INT32U 		StoreFileIndexOffset;	//正在存储的文件在索引文件中的偏移地址
  INT8U   	ReadName[13];			//正在读取数据的文件名称；
  INT32U		ReadFileSize;			//正在读取的文件的大小
  INT32U		ReadFileOffset;		//正在读取的文件的偏移地址;
	INT32U		ReadFileIndexOffset;	//正在读取的文件在INDEX中的偏移地址;
	INT16U		LastSendLen;			//上一次发送的数据长度
}XCPCCP_FILE;
XCPCCP_FILE  XcpCcpFile;

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
	
//	while(1)
//		ClearWatchdog();
	
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
//-	OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	 				   
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
} 

//LED0任务
void led0_task(void *pdata)
{	 	
	uint8 temp_data[20],i,j=0;
	uint16 mat_index;
	INT8U res,res1;
	INT32U len;
	
	INT8U TestData[200];
	INT8U Dat[1053];

	INT8U Dat1[1053];
	RTC_ST rtc;
	INT32U ReadOffset,ReadLen,TotalLen;
	SD_STATUS SdStatus;
	RTC_ST TestRtcTime;	
	
	Debug_init(115200);
	//-DPrint("\n硬件定时器测试:完成时间:%dms",5000);	//-向FIFO中写入数据.
	//-DealDebugSend(1);
//	RS232_init(115200);
//	RS485_init(115200);
	
#if TEST_SD > 0
	DPrint("\nSD卡模块测试:");
	DealDebugSend(1);
	ClearWatchdog();
	TickDelay = 5000;
	res = SdInit();
	switch(res)
	{
		case 0:
			DPrint("\n----SD卡初始化成功.");
			break;
		case 1:
			DPrint("\n----SD卡初始化失败.");
			break;
		case 2:
			DPrint("\n----无SD卡.");
			break;
		case 3:
			DPrint("\n----SD卡损坏.");
			break;
	}
	DPrint("\n----SD卡初始化时间:%dms",(5000 - TickDelay));
	DealDebugSend(1);
	
	TickDelay = 5000;
	DPrint("\n----SD卡格式化");
	DealDebugSend(1);
	res = SDFormat();
	switch(res)
	{
		case 0:
			DPrint("\n----SD卡格式化成功");
			break;
		case 1:
			DPrint("\n----SD卡格式化失败.");
			break;
		case 2:
			DPrint("\n----SD卡不存在.");
			break;
		case 3:
			DPrint("\n----SD卡损坏.");
			break;
	}
	DPrint("\n----SD卡格式化时间:%dms",(5000 - TickDelay));
	DealDebugSend(1);
	
	ClearWatchdog();
	DPrint("\n----SD卡新建文件");
	DealDebugSend(1);
	TickDelay = 5000;
	res = SDNewFile("SD card","txt",rtc);
	switch(res)
	{
		case 0:
			DPrint("\n----新建文件[SD card.txt]成功");
			break;
		case 1:
			DPrint("\n----[SD card.txt]文件已存在.");
			break;
		case 2:
			DPrint("\n----SD卡已满.");
			break;
		case 3:
			DPrint("\n----SD卡不存在.");
			break;
		case 4:
			DPrint("\n----SD卡损坏.");
			break;
	}
	DPrint("\n----SD卡新建文件时间:%dms",(5000 - TickDelay));
	DealDebugSend(1);
	ClearWatchdog();
	
//	DPrint("\n新建300个文件");
//	for(i = 0;i< 300;i++)
//	{
//		TestData[0] = i/100 + 0x30;
//		len = i%100;
//		TestData[1] = len/10 + 0x30;
//		len = len%10;
//		TestData[2] = len + 0x30;
//		TestData[3] = 0;
////-		RtcGetTime(&rtc);
//		res = SDNewFile(TestData,"txt",rtc);
//		if(res == 0)
//		{
//			DPrint("\n----新建文件%m成功",TestData,3);
//		}
//		else
//		{
//			DPrint("\n----新建文件%m失败，返回值 = %d",TestData,3,res);
//		}
//		DealDebugSend(1);
//		ClearWatchdog();
//	}
//	DPrint("\n----测试不打开文件直接操作函数的时间");
//	DealDebugSend(1);
	
//	TickDelay = 5000;
//	memset(TestData,0x3a,200);
//	res = SDAddData("SD card","txt",TestData,200,rtc);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----[SD card.txt]追加数据成功");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]文件不存在.");
//			break;
//		case 2:
//			DPrint("\n----SD卡已满.");
//			break;
//		case 3:
//			DPrint("\n----SD卡不存在.");
//			break;
//		case 4:
//			DPrint("\n----SD卡损坏.");
//			break;
//	}
//	DPrint("\n----SD卡追加数据时间:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	ClearWatchdog();
//	TickDelay = 5000;
//	res = SDFileCheck("SD card","txt",&len);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----[SD card.txt]文件大小读取成功,%x",len);
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]文件不存在.");
//			break;
//		case 2:
//			DPrint("\n----SD卡不存在.");
//			break;
//		case 3:
//			DPrint("\n----SD卡损坏.");
//			break;
//	}
//	DPrint("\n----SD卡检查文件时间:%dms",(5000 - TickDelay));
//	DealDebugSend(1);

//	ClearWatchdog();
//	TickDelay = 5000;
//	memset(TestData,0x33,20);
//	res = SDModifyData("SD card","txt",5,TestData,20,rtc);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----将[SD card.txt]中第5个字节开始的数据修改为0x33修改数据成功");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]文件不存在.");
//			break;
//		case 2:
//			DPrint("\n----SD卡不存在.");
//			break;
//		case 3:
//			DPrint("\n----SD卡损坏.");
//			break;
//	}
//	DPrint("\n----SD卡修改数据时间:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	ClearWatchdog();
//	TickDelay = 5000;
//	res = SDReadData("SD card","txt",0,TestData,200);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----从[SD card.txt]中读取数据成功");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]文件不存在.");
//			break;
//		case 2:
//			DPrint("\n----SD卡不存在.");
//			break;
//		case 3:
//			DPrint("\n----SD卡损坏.");
//			break;
//	}
//	DPrint("\n----SD卡读数据时间:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	ClearWatchdog();
//	DPrint("\n----测试打开文件后的SD卡操作时间");
//	TickDelay = 5000;
//	res = SDOpenFile("SD card","txt");
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----[SD card.txt]打开成功");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]文件不存在.");
//			break;
//		case 2:
//			DPrint("\n----SD卡不存在.");
//			break;
//		case 3:
//			DPrint("\n----SD卡损坏.");
//			break;
//	}
//	DPrint("\n----文件打开时间:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	TickDelay = 5000;
//	memset(TestData,0x3a,200);
//	res = SDAddData("SD card","txt",TestData,200,rtc);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----[SD card.txt]追加数据成功");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]文件不存在.");
//			break;
//		case 2:
//			DPrint("\n----SD卡已满.");
//			break;
//		case 3:
//			DPrint("\n----SD卡不存在.");
//			break;
//		case 4:
//			DPrint("\n----SD卡损坏.");
//			break;
//	}
//	DPrint("\n----SD卡追加数据时间:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	ClearWatchdog();
//	

//	ClearWatchdog();
//	TickDelay = 5000;
//	memset(TestData,0x33,20);
//	res = SDModifyData("SD card","txt",5,TestData,20,rtc);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----将[SD card.txt]中第5个字节开始的数据修改为0x33修改数据成功");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]文件不存在.");
//			break;
//		case 2:
//			DPrint("\n----SD卡不存在.");
//			break;
//		case 3:
//			DPrint("\n----SD卡损坏.");
//			break;
//	}
//	DPrint("\n----SD卡修改数据时间:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	ClearWatchdog();
//	TickDelay = 5000;
//	res = SDReadData("SD card","txt",0,TestData,200);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----从[SD card.txt]中读取数据成功");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]文件不存在.");
//			break;
//		case 2:
//			DPrint("\n----SD卡不存在.");
//			break;
//		case 3:
//			DPrint("\n----SD卡损坏.");
//			break;
//	}
//	DPrint("\n----SD卡读数据时间:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	TickDelay = 5000;
//	res = SDCloseFile("SD card","txt");
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----[SD card.txt]关闭成功");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]文件不存在.");
//			break;
//		case 2:
//			DPrint("\n----SD卡不存在.");
//			break;
//		case 3:
//			DPrint("\n----SD卡损坏.");
//			break;
//	}
//	DPrint("\n----文件关闭时间:%dms",(5000 - TickDelay));
//	TickDelay = 5000;
//	res = SDFileCheck("SD card","txt",&len);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----[SD card.txt]文件大小读取成功,%x",len);
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]文件不存在.");
//			break;
//		case 2:
//			DPrint("\n----SD卡不存在.");
//			break;
//		case 3:
//			DPrint("\n----SD卡损坏.");
//			break;
//	}
//	DPrint("\n----SD卡检查文件时间:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	ClearWatchdog();
//	TickDelay = 5000;
//	res = SDCheck(&SdStatus);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----SD卡状态读取成功,总容量:%x,可用容量:%x",SdStatus.Capability,SdStatus.AvailableCapability);
//			break;
//		case 1:
//			DPrint("\n----SD卡状态读取失败.");
//			break;
//		case 2:
//			DPrint("\n----SD卡不存在.");
//			break;
//		case 3:
//			DPrint("\n----SD卡损坏.");
//			break;
//	}
//	DPrint("\n----SD卡读状态时间:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	ClearWatchdog();
//	memcpy(XcpCcpFile.StoreName,"170321094231",13);
//	TickDelay = 5000;
//	res = SDDeleteFile((char*)XcpCcpFile.StoreName,"txt");
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----删除文件[170321094231.txt]成功");
//			break;
//		case 1:
//			DPrint("\n----[170321094231.txt]文件不存在.");
//			break;
//		case 2:
//			DPrint("\n----SD卡不存在.");
//			break;
//		case 3:
//			DPrint("\n----SD卡损坏.");
//			break;
//	}
//	res = SDNewFile((char*)XcpCcpFile.StoreName,"txt",rtc);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----新建文件[170321094231.txt]成功");
//			break;
//		case 1:
//			DPrint("\n----[170321094231.txt]文件已存在.");
//			break;
//		case 2:
//			DPrint("\n----SD卡已满.");
//			break;
//		case 3:
//			DPrint("\n----SD卡不存在.");
//			break;
//		case 4:
//			DPrint("\n----SD卡损坏.");
//			break;
//	}
//	
//	TotalLen = 0;
//	for(j = 0;j < 0x60;j ++)
//	{
//		for(i = 0;i < 1053;i++)
//		{
//			Dat[i] = i;
//		}
//		
//		len = (j + 11)%1053;
//		
//		SDFileCheck((char*)XcpCcpFile.StoreName,"txt",&ReadOffset);
//		DPrint("\nXcpCcpFile.StoreName,数据写入前文件大小:%x.\n",ReadOffset);
//		res = SDOpenFile((char*)XcpCcpFile.StoreName,"txt");
//		switch(res)
//		{
//			case 0:
//				DPrint("\n----打开文件[170321094231.txt]成功");
//				break;
//			case 1:
//				DPrint("\n----[170321094231.txt]文件已存在.");
//				break;
//			case 2:
//				DPrint("\n----打开的文件超过最大值.");
//				break;
//			case 3:
//				DPrint("\n----SD卡不存在.");
//				break;
//			case 4:
//				DPrint("\n----SD卡损坏.");
//				break;
//		}
////-		RtcGetTime(&rtc);
//		SDAddData((char*)XcpCcpFile.StoreName,"TXT",Dat,len,rtc);
//		TotalLen += len;
//		DPrint("XcpCcpFile.StoreName,写入数据的总长度:%x.\n",TotalLen);
//		res = SDCloseFile((char*)XcpCcpFile.StoreName,"txt");
//		switch(res)
//		{
//			case 0:
//				DPrint("\n----关闭文件[170321094231.txt]成功");
//				break;
//			case 1:
//				DPrint("\n----[170321094231.txt]文件已存在.");
//				break;
//			case 2:
//				DPrint("\n----打开的文件超过最大值.");
//				break;
//			case 3:
//				DPrint("\n----SD卡不存在.");
//				break;
//			case 4:
//				DPrint("\n----SD卡损坏.");
//				break;
//		}
//		SDFileCheck((char*)XcpCcpFile.StoreName,"txt",&ReadLen);
//		DPrint("XcpCcpFile.StoreName,写入数据后的文件大小:%x.\n",ReadLen);
//		res = SDCloseFile((char*)XcpCcpFile.StoreName,"txt");
//		switch(res)
//		{
//			case 0:
//				DPrint("\n----关闭文件[170321094231.txt]成功");
//				break;
//			case 1:
//				DPrint("\n----[170321094231.txt]文件不存在.");
//				break;
//			case 2:
//				DPrint("\n----文件关闭成功.");
//				break;
//			case 3:
//				DPrint("\n----SD卡不存在.");
//				break;
//			case 4:
//				DPrint("\n----SD卡损坏.");
//				break;
//		}
//		res = SDOpenFile((char*)XcpCcpFile.StoreName,"txt");
//		switch(res)
//		{
//			case 0:
//				DPrint("\n----打开文件[170321094231.txt]成功");
//				break;
//			case 1:
//				DPrint("\n----[170321094231.txt]文件已存在.");
//				break;
//			case 2:
//				DPrint("\n----打开的文件超过最大值.");
//				break;
//			case 3:
//				DPrint("\n----SD卡不存在.");
//				break;
//			case 4:
//				DPrint("\n----SD卡损坏.");
//				break;
//		}
//		memset(Dat1,0x00,1053);
//		res = SDReadData((char*)XcpCcpFile.StoreName,"txt",ReadOffset,Dat1,len);
//		if(res != 0)
//		{
//			DPrint("\n文件读取失败,返回值:%o.\n",res);
//		}
//		
//		for(i = 0;i<len;i++)
//		{
//			if(Dat[i] != Dat1[i])
//			{
//				DPrint("文件不一致.\n");
//				DealDebugSend(1);
//				break;
//			}
//		}
//		DealDebugSend(1);
//	}
//	DPrint("\nSD卡测试完成.\n");
//	DealDebugSend(1);
#endif	
	
	
#if TEST_RTC > 0
	DPrint("\n测试RTC:");
	if(RtcInit()==0)
	{
		DPrint("\n----RTC初始化成功");
	}
	else
	{
		DPrint("\n----RTC初始化失败");
	}
	DealDebugSend(1);
	TestRtcTime.second = 36;
	TestRtcTime.minute = 15;
	TestRtcTime.hour = 10;
	TestRtcTime.day = 29;
	TestRtcTime.month = 10;
	TestRtcTime.year = 16;
	if(RtcSetTime(&TestRtcTime) == 0)
	{
		DPrint("\n----时间设置成功,16年10月29号10:15:36");
	}
	else
	{
		DPrint("\n----时间设置失败,16年10月29号10:15:36");
	}
	DealDebugSend(1);
	if(RtcGetTime(&TestRtcTime) == 0)
	{
		DPrint("\n----时间读取成功,时间为:%d年%d月%d日%d:%d:%d",TestRtcTime.year,TestRtcTime.month,TestRtcTime.day,TestRtcTime.hour,TestRtcTime.minute,TestRtcTime.second);
	}
	else
	{
		DPrint("\n----时间读取失败");
	}
	DPrint("\nRTC测试完成!\n");
	DealDebugSend(1);
#endif	
	
	
#if TEST_GREEN > 0
	S_DEBUGF("\n测试省电模式");
	DealDebugSend(1);
	TimeDelay = 0;
//	while(1)
	{
//		res = GetExtPowerStatus();
//		if(res == 0)
		{
//			if(res1 != res)
//			{
//				S_DEBUGF("\n---外部电源没电,进入休眠\n");
//				DealDebugSend();
//				res1 = res;
//			}
			while(1)
			{
				ClearWatchdog();
				RtcSetAlarm(2);
//				SystemPowerDown();
				CpuPowerDown();
//				DebugClosePort();
//				IoInit();
				Debug_init(115200);
				S_DEBUGF("\n---休眠唤醒\n");
				DealDebugSend(1);
				ClearWatchdog();
//				S_DEBUGF("\n---休眠唤醒\n");
//				DealDebugSend(1);
				delay_ms(10);
//				res = GetExtPowerStatus();
//				if(res != 0)
//				{
//					S_DEBUGF("\n---外部电源上电\n");
//					DealDebugSend();
//					InitSystem();
//					SystemWakeup();
//					RS232ClosePort();
//					IoInit();
//					RS232OpenPort(115200);
//					SysTickConfig(_TICK_1MS);
//					OpenTimer(100);
//					break;
//				}

//				AdcInit();
//				res = BattCoulometry();
//				if(res == 1)
//				{//电池电量低,深度睡眠不唤醒
//					S_DEBUGF("\n---电池电量低.\n");
//					//-return;
//				}
//				else if(res == 0)
//				{
//					S_DEBUGF("\n---电池电量高.\n");
//				}
//				else if(res == 2)
//				{
//					S_DEBUGF("\n---正在充电.\n");
//				}

//				DealDebugSend();
			
				}
		}
//		else
//		{
//				if(res1 != res)
//				{
//					S_DEBUGF("\n---外部电源有电\n");
//					res1 = res;
//				}
//		}
		DealDebugSend(1);
	}
		
#endif
	
	
	
	CreateQueue(&L218_AT_data);	//-实现了队列的初始化
	for(i = 0;i < QUEUEDEEPTH;i++)
	{
			L218_AT_data.queuenodearray[i].len = 0;
			L218_AT_data.queuenodearray[i].rp = 0;
			L218_AT_data.queuenodearray[i].next = 0;
	}
	L218_AT_data.queuenodept_pt = 0;
	L218_AT_data.L218_AT_buf = 0;
	
	while(1)
	{
		ON_GRE_LED();
		delay_ms(300);
		OFF_GRE_LED();
		delay_ms(300);
		ClearWatchdog();
		
		//-len = ReadDebugData(temp_data,20);
		len = ReadDebugData(L218_data,256);
		if(len)
		{
			DPrint("\nDebug接收测试:打印接收指令:%s",temp_data);
			DealDebugSend(1);
			L218_AT_data.L218_AT_buf =(u8*)mymalloc(0,len);						//为fatbuf申请内存
			MemCpy(L218_AT_data.L218_AT_buf,L218_data,len);
			L218_AT_data.queuenodearray[L218_AT_data.queuenodept_pt].len = len;
			L218_AT_data.queuenodearray[L218_AT_data.queuenodept_pt].rp = L218_AT_data.L218_AT_buf;
			AppendQueue(&L218_AT_data, &L218_AT_data.queuenodearray[L218_AT_data.queuenodept_pt]);
			L218_AT_data.queuenodept_pt = (L218_AT_data.queuenodept_pt + 1) % QUEUEDEEPTH;
			
//			FLASH_Unlock();									//解锁 
//			FLASH_ProgramWord(0x08040000,0xaa);
//			FLASH_ProgramWord(0x08040004,0xaa);
////			L218_AT_data.L218_AT_buf =  (u8*)0x08040000;
////			*L218_AT_data.L218_AT_buf = 0x55;
//			FLASH_Lock();										//上锁
			//-以上测试结果说明,当1变为0的时候不需要擦除,但是0变为1必须擦除
			
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
	
	BGprsResetPara();
	
	while(1)
	{
		ON_RED_LED();
		delay_ms(300);
		OFF_RED_LED();
		delay_ms(300);
		//-RS232SendData(0xAA);
//		RS232Print("\n硬件定时器测试:完成时间:%dms",5000);	//-向FIFO中写入数据.
//		DealRS232Send(1);
		ExecuteModuleTask();
	}
}



