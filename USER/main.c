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
  INT8U   	StoreName[13];				//���ڴ洢���ݵ��ļ����ƣ�
  INT32U 		StoreFileIndexOffset;	//���ڴ洢���ļ��������ļ��е�ƫ�Ƶ�ַ
  INT8U   	ReadName[13];			//���ڶ�ȡ���ݵ��ļ����ƣ�
  INT32U		ReadFileSize;			//���ڶ�ȡ���ļ��Ĵ�С
  INT32U		ReadFileOffset;		//���ڶ�ȡ���ļ���ƫ�Ƶ�ַ;
	INT32U		ReadFileIndexOffset;	//���ڶ�ȡ���ļ���INDEX�е�ƫ�Ƶ�ַ;
	INT16U		LastSendLen;			//��һ�η��͵����ݳ���
}XCPCCP_FILE;
XCPCCP_FILE  XcpCcpFile;

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
	
//	while(1)
//		ClearWatchdog();
	
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
//-	OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	 				   
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
} 

//LED0����
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
	//-DPrint("\nӲ����ʱ������:���ʱ��:%dms",5000);	//-��FIFO��д������.
	//-DealDebugSend(1);
//	RS232_init(115200);
//	RS485_init(115200);
	
#if TEST_SD > 0
	DPrint("\nSD��ģ�����:");
	DealDebugSend(1);
	ClearWatchdog();
	TickDelay = 5000;
	res = SdInit();
	switch(res)
	{
		case 0:
			DPrint("\n----SD����ʼ���ɹ�.");
			break;
		case 1:
			DPrint("\n----SD����ʼ��ʧ��.");
			break;
		case 2:
			DPrint("\n----��SD��.");
			break;
		case 3:
			DPrint("\n----SD����.");
			break;
	}
	DPrint("\n----SD����ʼ��ʱ��:%dms",(5000 - TickDelay));
	DealDebugSend(1);
	
	TickDelay = 5000;
	DPrint("\n----SD����ʽ��");
	DealDebugSend(1);
	res = SDFormat();
	switch(res)
	{
		case 0:
			DPrint("\n----SD����ʽ���ɹ�");
			break;
		case 1:
			DPrint("\n----SD����ʽ��ʧ��.");
			break;
		case 2:
			DPrint("\n----SD��������.");
			break;
		case 3:
			DPrint("\n----SD����.");
			break;
	}
	DPrint("\n----SD����ʽ��ʱ��:%dms",(5000 - TickDelay));
	DealDebugSend(1);
	
	ClearWatchdog();
	DPrint("\n----SD���½��ļ�");
	DealDebugSend(1);
	TickDelay = 5000;
	res = SDNewFile("SD card","txt",rtc);
	switch(res)
	{
		case 0:
			DPrint("\n----�½��ļ�[SD card.txt]�ɹ�");
			break;
		case 1:
			DPrint("\n----[SD card.txt]�ļ��Ѵ���.");
			break;
		case 2:
			DPrint("\n----SD������.");
			break;
		case 3:
			DPrint("\n----SD��������.");
			break;
		case 4:
			DPrint("\n----SD����.");
			break;
	}
	DPrint("\n----SD���½��ļ�ʱ��:%dms",(5000 - TickDelay));
	DealDebugSend(1);
	ClearWatchdog();
	
//	DPrint("\n�½�300���ļ�");
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
//			DPrint("\n----�½��ļ�%m�ɹ�",TestData,3);
//		}
//		else
//		{
//			DPrint("\n----�½��ļ�%mʧ�ܣ�����ֵ = %d",TestData,3,res);
//		}
//		DealDebugSend(1);
//		ClearWatchdog();
//	}
//	DPrint("\n----���Բ����ļ�ֱ�Ӳ���������ʱ��");
//	DealDebugSend(1);
	
//	TickDelay = 5000;
//	memset(TestData,0x3a,200);
//	res = SDAddData("SD card","txt",TestData,200,rtc);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----[SD card.txt]׷�����ݳɹ�");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]�ļ�������.");
//			break;
//		case 2:
//			DPrint("\n----SD������.");
//			break;
//		case 3:
//			DPrint("\n----SD��������.");
//			break;
//		case 4:
//			DPrint("\n----SD����.");
//			break;
//	}
//	DPrint("\n----SD��׷������ʱ��:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	ClearWatchdog();
//	TickDelay = 5000;
//	res = SDFileCheck("SD card","txt",&len);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----[SD card.txt]�ļ���С��ȡ�ɹ�,%x",len);
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]�ļ�������.");
//			break;
//		case 2:
//			DPrint("\n----SD��������.");
//			break;
//		case 3:
//			DPrint("\n----SD����.");
//			break;
//	}
//	DPrint("\n----SD������ļ�ʱ��:%dms",(5000 - TickDelay));
//	DealDebugSend(1);

//	ClearWatchdog();
//	TickDelay = 5000;
//	memset(TestData,0x33,20);
//	res = SDModifyData("SD card","txt",5,TestData,20,rtc);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----��[SD card.txt]�е�5���ֽڿ�ʼ�������޸�Ϊ0x33�޸����ݳɹ�");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]�ļ�������.");
//			break;
//		case 2:
//			DPrint("\n----SD��������.");
//			break;
//		case 3:
//			DPrint("\n----SD����.");
//			break;
//	}
//	DPrint("\n----SD���޸�����ʱ��:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	ClearWatchdog();
//	TickDelay = 5000;
//	res = SDReadData("SD card","txt",0,TestData,200);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----��[SD card.txt]�ж�ȡ���ݳɹ�");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]�ļ�������.");
//			break;
//		case 2:
//			DPrint("\n----SD��������.");
//			break;
//		case 3:
//			DPrint("\n----SD����.");
//			break;
//	}
//	DPrint("\n----SD��������ʱ��:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	ClearWatchdog();
//	DPrint("\n----���Դ��ļ����SD������ʱ��");
//	TickDelay = 5000;
//	res = SDOpenFile("SD card","txt");
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----[SD card.txt]�򿪳ɹ�");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]�ļ�������.");
//			break;
//		case 2:
//			DPrint("\n----SD��������.");
//			break;
//		case 3:
//			DPrint("\n----SD����.");
//			break;
//	}
//	DPrint("\n----�ļ���ʱ��:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	TickDelay = 5000;
//	memset(TestData,0x3a,200);
//	res = SDAddData("SD card","txt",TestData,200,rtc);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----[SD card.txt]׷�����ݳɹ�");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]�ļ�������.");
//			break;
//		case 2:
//			DPrint("\n----SD������.");
//			break;
//		case 3:
//			DPrint("\n----SD��������.");
//			break;
//		case 4:
//			DPrint("\n----SD����.");
//			break;
//	}
//	DPrint("\n----SD��׷������ʱ��:%dms",(5000 - TickDelay));
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
//			DPrint("\n----��[SD card.txt]�е�5���ֽڿ�ʼ�������޸�Ϊ0x33�޸����ݳɹ�");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]�ļ�������.");
//			break;
//		case 2:
//			DPrint("\n----SD��������.");
//			break;
//		case 3:
//			DPrint("\n----SD����.");
//			break;
//	}
//	DPrint("\n----SD���޸�����ʱ��:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	ClearWatchdog();
//	TickDelay = 5000;
//	res = SDReadData("SD card","txt",0,TestData,200);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----��[SD card.txt]�ж�ȡ���ݳɹ�");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]�ļ�������.");
//			break;
//		case 2:
//			DPrint("\n----SD��������.");
//			break;
//		case 3:
//			DPrint("\n----SD����.");
//			break;
//	}
//	DPrint("\n----SD��������ʱ��:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	TickDelay = 5000;
//	res = SDCloseFile("SD card","txt");
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----[SD card.txt]�رճɹ�");
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]�ļ�������.");
//			break;
//		case 2:
//			DPrint("\n----SD��������.");
//			break;
//		case 3:
//			DPrint("\n----SD����.");
//			break;
//	}
//	DPrint("\n----�ļ��ر�ʱ��:%dms",(5000 - TickDelay));
//	TickDelay = 5000;
//	res = SDFileCheck("SD card","txt",&len);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----[SD card.txt]�ļ���С��ȡ�ɹ�,%x",len);
//			break;
//		case 1:
//			DPrint("\n----[SD card.txt]�ļ�������.");
//			break;
//		case 2:
//			DPrint("\n----SD��������.");
//			break;
//		case 3:
//			DPrint("\n----SD����.");
//			break;
//	}
//	DPrint("\n----SD������ļ�ʱ��:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	ClearWatchdog();
//	TickDelay = 5000;
//	res = SDCheck(&SdStatus);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----SD��״̬��ȡ�ɹ�,������:%x,��������:%x",SdStatus.Capability,SdStatus.AvailableCapability);
//			break;
//		case 1:
//			DPrint("\n----SD��״̬��ȡʧ��.");
//			break;
//		case 2:
//			DPrint("\n----SD��������.");
//			break;
//		case 3:
//			DPrint("\n----SD����.");
//			break;
//	}
//	DPrint("\n----SD����״̬ʱ��:%dms",(5000 - TickDelay));
//	DealDebugSend(1);
//	ClearWatchdog();
//	memcpy(XcpCcpFile.StoreName,"170321094231",13);
//	TickDelay = 5000;
//	res = SDDeleteFile((char*)XcpCcpFile.StoreName,"txt");
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----ɾ���ļ�[170321094231.txt]�ɹ�");
//			break;
//		case 1:
//			DPrint("\n----[170321094231.txt]�ļ�������.");
//			break;
//		case 2:
//			DPrint("\n----SD��������.");
//			break;
//		case 3:
//			DPrint("\n----SD����.");
//			break;
//	}
//	res = SDNewFile((char*)XcpCcpFile.StoreName,"txt",rtc);
//	switch(res)
//	{
//		case 0:
//			DPrint("\n----�½��ļ�[170321094231.txt]�ɹ�");
//			break;
//		case 1:
//			DPrint("\n----[170321094231.txt]�ļ��Ѵ���.");
//			break;
//		case 2:
//			DPrint("\n----SD������.");
//			break;
//		case 3:
//			DPrint("\n----SD��������.");
//			break;
//		case 4:
//			DPrint("\n----SD����.");
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
//		DPrint("\nXcpCcpFile.StoreName,����д��ǰ�ļ���С:%x.\n",ReadOffset);
//		res = SDOpenFile((char*)XcpCcpFile.StoreName,"txt");
//		switch(res)
//		{
//			case 0:
//				DPrint("\n----���ļ�[170321094231.txt]�ɹ�");
//				break;
//			case 1:
//				DPrint("\n----[170321094231.txt]�ļ��Ѵ���.");
//				break;
//			case 2:
//				DPrint("\n----�򿪵��ļ��������ֵ.");
//				break;
//			case 3:
//				DPrint("\n----SD��������.");
//				break;
//			case 4:
//				DPrint("\n----SD����.");
//				break;
//		}
////-		RtcGetTime(&rtc);
//		SDAddData((char*)XcpCcpFile.StoreName,"TXT",Dat,len,rtc);
//		TotalLen += len;
//		DPrint("XcpCcpFile.StoreName,д�����ݵ��ܳ���:%x.\n",TotalLen);
//		res = SDCloseFile((char*)XcpCcpFile.StoreName,"txt");
//		switch(res)
//		{
//			case 0:
//				DPrint("\n----�ر��ļ�[170321094231.txt]�ɹ�");
//				break;
//			case 1:
//				DPrint("\n----[170321094231.txt]�ļ��Ѵ���.");
//				break;
//			case 2:
//				DPrint("\n----�򿪵��ļ��������ֵ.");
//				break;
//			case 3:
//				DPrint("\n----SD��������.");
//				break;
//			case 4:
//				DPrint("\n----SD����.");
//				break;
//		}
//		SDFileCheck((char*)XcpCcpFile.StoreName,"txt",&ReadLen);
//		DPrint("XcpCcpFile.StoreName,д�����ݺ���ļ���С:%x.\n",ReadLen);
//		res = SDCloseFile((char*)XcpCcpFile.StoreName,"txt");
//		switch(res)
//		{
//			case 0:
//				DPrint("\n----�ر��ļ�[170321094231.txt]�ɹ�");
//				break;
//			case 1:
//				DPrint("\n----[170321094231.txt]�ļ�������.");
//				break;
//			case 2:
//				DPrint("\n----�ļ��رճɹ�.");
//				break;
//			case 3:
//				DPrint("\n----SD��������.");
//				break;
//			case 4:
//				DPrint("\n----SD����.");
//				break;
//		}
//		res = SDOpenFile((char*)XcpCcpFile.StoreName,"txt");
//		switch(res)
//		{
//			case 0:
//				DPrint("\n----���ļ�[170321094231.txt]�ɹ�");
//				break;
//			case 1:
//				DPrint("\n----[170321094231.txt]�ļ��Ѵ���.");
//				break;
//			case 2:
//				DPrint("\n----�򿪵��ļ��������ֵ.");
//				break;
//			case 3:
//				DPrint("\n----SD��������.");
//				break;
//			case 4:
//				DPrint("\n----SD����.");
//				break;
//		}
//		memset(Dat1,0x00,1053);
//		res = SDReadData((char*)XcpCcpFile.StoreName,"txt",ReadOffset,Dat1,len);
//		if(res != 0)
//		{
//			DPrint("\n�ļ���ȡʧ��,����ֵ:%o.\n",res);
//		}
//		
//		for(i = 0;i<len;i++)
//		{
//			if(Dat[i] != Dat1[i])
//			{
//				DPrint("�ļ���һ��.\n");
//				DealDebugSend(1);
//				break;
//			}
//		}
//		DealDebugSend(1);
//	}
//	DPrint("\nSD���������.\n");
//	DealDebugSend(1);
#endif	
	
	
#if TEST_RTC > 0
	DPrint("\n����RTC:");
	if(RtcInit()==0)
	{
		DPrint("\n----RTC��ʼ���ɹ�");
	}
	else
	{
		DPrint("\n----RTC��ʼ��ʧ��");
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
		DPrint("\n----ʱ�����óɹ�,16��10��29��10:15:36");
	}
	else
	{
		DPrint("\n----ʱ������ʧ��,16��10��29��10:15:36");
	}
	DealDebugSend(1);
	if(RtcGetTime(&TestRtcTime) == 0)
	{
		DPrint("\n----ʱ���ȡ�ɹ�,ʱ��Ϊ:%d��%d��%d��%d:%d:%d",TestRtcTime.year,TestRtcTime.month,TestRtcTime.day,TestRtcTime.hour,TestRtcTime.minute,TestRtcTime.second);
	}
	else
	{
		DPrint("\n----ʱ���ȡʧ��");
	}
	DPrint("\nRTC�������!\n");
	DealDebugSend(1);
#endif	
	
	
#if TEST_GREEN > 0
	S_DEBUGF("\n����ʡ��ģʽ");
	DealDebugSend(1);
	TimeDelay = 0;
//	while(1)
	{
//		res = GetExtPowerStatus();
//		if(res == 0)
		{
//			if(res1 != res)
//			{
//				S_DEBUGF("\n---�ⲿ��Դû��,��������\n");
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
				S_DEBUGF("\n---���߻���\n");
				DealDebugSend(1);
				ClearWatchdog();
//				S_DEBUGF("\n---���߻���\n");
//				DealDebugSend(1);
				delay_ms(10);
//				res = GetExtPowerStatus();
//				if(res != 0)
//				{
//					S_DEBUGF("\n---�ⲿ��Դ�ϵ�\n");
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
//				{//��ص�����,���˯�߲�����
//					S_DEBUGF("\n---��ص�����.\n");
//					//-return;
//				}
//				else if(res == 0)
//				{
//					S_DEBUGF("\n---��ص�����.\n");
//				}
//				else if(res == 2)
//				{
//					S_DEBUGF("\n---���ڳ��.\n");
//				}

//				DealDebugSend();
			
				}
		}
//		else
//		{
//				if(res1 != res)
//				{
//					S_DEBUGF("\n---�ⲿ��Դ�е�\n");
//					res1 = res;
//				}
//		}
		DealDebugSend(1);
	}
		
#endif
	
	
	
	CreateQueue(&L218_AT_data);	//-ʵ���˶��еĳ�ʼ��
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
			DPrint("\nDebug���ղ���:��ӡ����ָ��:%s",temp_data);
			DealDebugSend(1);
			L218_AT_data.L218_AT_buf =(u8*)mymalloc(0,len);						//Ϊfatbuf�����ڴ�
			MemCpy(L218_AT_data.L218_AT_buf,L218_data,len);
			L218_AT_data.queuenodearray[L218_AT_data.queuenodept_pt].len = len;
			L218_AT_data.queuenodearray[L218_AT_data.queuenodept_pt].rp = L218_AT_data.L218_AT_buf;
			AppendQueue(&L218_AT_data, &L218_AT_data.queuenodearray[L218_AT_data.queuenodept_pt]);
			L218_AT_data.queuenodept_pt = (L218_AT_data.queuenodept_pt + 1) % QUEUEDEEPTH;
			
//			FLASH_Unlock();									//���� 
//			FLASH_ProgramWord(0x08040000,0xaa);
//			FLASH_ProgramWord(0x08040004,0xaa);
////			L218_AT_data.L218_AT_buf =  (u8*)0x08040000;
////			*L218_AT_data.L218_AT_buf = 0x55;
//			FLASH_Lock();										//����
			//-���ϲ��Խ��˵��,��1��Ϊ0��ʱ����Ҫ����,����0��Ϊ1�������
			
			mat_index = SubMatch("ok",2,temp_data,len);
			if(mat_index)
			{
				DPrint("\nDebug���ղ���:��ӡ����:%d��",j);
				j++;
				DealDebugSend(1);
			}
		}
		//-for(i=0;i<len;i++)
			//-DebugSendData(temp_data[i]);
//		DPrint("\nDebug����:��ӡ����:%d��",j);
//		j++;
//		DealDebugSend(1);
		
		len = ReadRS232Data(temp_data,20);
		if(len)
		{
			RS232Print("\nRS232���ղ���:��ӡ����ָ��:%s",temp_data);
			DealRS232Send(1);
			mat_index = SubMatch("ok",2,temp_data,len);
			if(mat_index)
			{
				RS232Print("\nRS232���ղ���:��ӡ����:%d��",j);
				j++;
				DealRS232Send(1);
			}
		}
	}
}

//LED1����
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
//		RS232Print("\nӲ����ʱ������:���ʱ��:%dms",5000);	//-��FIFO��д������.
//		DealRS232Send(1);
		ExecuteModuleTask();
	}
}



