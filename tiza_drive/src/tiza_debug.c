/*
   Debug driver for TZ2000
   UART1
*/
#define DEBUG_GLOBAL

#include "tiza_include.h"

#define Debug_Mode_ITorDMA	1	//-0 中断,1 DMA

#define DEBUG_RX_MAX    256
#define DEBUG_TX_MAX    1024

static  INT8U   DebugRx[DEBUG_RX_MAX];
static  INT8U   DebugTx[DEBUG_TX_MAX];

static  FIFO    DebugTxFifo;
static  FIFO    DebugRxFifo;

#define SEND_BUF_SIZE 8200	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.

static  INT8U SendBuff[SEND_BUF_SIZE];	//发送数据缓冲区

////////////////////////////////////////////////////////////////////////////////
/*****************************************************************
*                   FIFO
******************************************************************/
void InitFIFO(FIFO *fifo, INT8U *array, INT16U deepth)
{
    fifo->deepth    = deepth;
    fifo->occupy    = 0;
    fifo->array     = array;
    fifo->limit     = array + deepth;
    fifo->wp = fifo->array;
    fifo->rp = fifo->array;
}

void ResetFIFO(FIFO *fifo)
{
    fifo->occupy = 0;
    fifo->rp = fifo->array;
    fifo->wp = fifo->array;
}


int WriteFIFO(FIFO *fifo, INT8U unit)
{
	if (fifo->occupy >= fifo->deepth)
	{
		ReadFIFO(fifo); // 2011-11-14 改为环回型
		//return FALSE;
	}
    
	*fifo->wp++ = unit;
	if (fifo->wp >= fifo->limit) fifo->wp = fifo->array;

//	OS_ENTER_CRITICAL();
	fifo->occupy++;
//	OS_EXIT_CRITICAL();
	return TRUE;
}

int WriteFIFOs(FIFO *fifo, INT8U *units, INT16U unitsize)  
{
    if (unitsize > fifo->deepth - fifo->occupy) return FALSE;
    for (; unitsize > 0; unitsize--) { 
       *fifo->wp++ = *units++;
       if (fifo->wp >= fifo->limit) fifo->wp = fifo->array;
//-       OS_ENTER_CRITICAL();
       fifo->occupy++;
//-       OS_EXIT_CRITICAL();
    }  

    return TRUE;
}

int WriteFIFODMA(FIFO *fifo, INT8U *units, INT16U unitsize)  
{	
		//-使用DMA时不需要进行数据移动,仅仅修改标志即可
			 if(unitsize >= fifo->deepth)
				 unitsize -= fifo->deepth;
			 else
				 unitsize = fifo->deepth - unitsize;	//-得到接收的有效个数
       fifo->wp = fifo->array + unitsize;
//-       if (fifo->wp >= fifo->limit) fifo->wp = fifo->array + (fifo->wp - fifo->limit);
//-       OS_ENTER_CRITICAL();
			 if(fifo->wp >= fifo->rp)
					fifo->occupy = fifo->wp - fifo->rp;
			 else
					fifo->occupy = fifo->deepth + fifo->wp - fifo->rp;
//-       OS_EXIT_CRITICAL();
		 return TRUE;
}

int FIFOEmpty(FIFO *fifo)
{
    if (fifo->occupy == 0) return true;
    else return false;
}    

int FIFOFull(FIFO *fifo)
{
    if (fifo->occupy == fifo->deepth) return true;
    else return false;
}
    
INT16U  FIFOSpare(FIFO *fifo)
{
    return fifo->deepth - fifo->occupy;
}
    
INT8U ReadFIFO(FIFO *fifo)
{
	int ret;
    
	if (fifo->occupy == 0) return 0xFF;
	ret = *fifo->rp++;
	if (fifo->rp >= fifo->limit) fifo->rp = fifo->array;

//-	OS_ENTER_CRITICAL();
	fifo->occupy--;
//-	OS_EXIT_CRITICAL();
	return ret;
}

INT16U FIFOUsed(FIFO *fifo)
{
    return fifo->occupy;
}



void print_byte(INT8U ch)
{
  WriteFIFO(&DebugTxFifo,ch);
}

void print_bytehex(INT8U ch)
{
  WriteFIFO(&DebugTxFifo,SemiHexToChar(ch >> 4));
  WriteFIFO(&DebugTxFifo,SemiHexToChar(ch));
  WriteFIFO(&DebugTxFifo,' ');
}

void print_mem(INT8U *mem, INT16U memsize)
{
  WriteFIFOs(&DebugTxFifo,mem,memsize);
}

void print_memhex(INT8U *mem, INT16U memsize)
{
  for (; memsize > 0; memsize--)
  {
    print_bytehex(*mem++);
  }
}

void print(char *str)
{
  if(str == NULL)
  {
    return;
  }
  while(*str)
  {
    if(*str == '\n') 
    {
      print_byte(0x0D);
      print_byte(0x0A);
      str++;
    }
    else
    {
      print_byte(*str++);
    }
  }
}

void sprint_byte(INT8U ch)
{
  if(ch == '\n') 
  {
    print_byte(0x0D);
    print_byte(0x0A);
  }
  else
  {
    print_byte(ch);
  }
}

/*
%d :INT16 
%l :HEX-INT16
%t :INT16 string
%o :HEX - INT8U
%x :HEX - INT32
%c :char
%s :string;
%h :HEX string；  
%m : memory
%p : Pointer
*/
void DPrint(const char *fmt, ...)
{
  char *s;
  INT8U *ptr;
  INT32U d,i,sht;
  INT8U op,temp;
  INT8U buf[16];
  va_list ap;

  va_start(ap, fmt);
  sht = 0;
  while (*fmt)
  {
    if(*fmt != '%') 
    {
      sht = 0;
      sprint_byte(*fmt++);
      continue;
    }
    switch (*++fmt)
    {
      case 'd':
				d = va_arg(ap,int);
        temp = IntToString(buf,d,sht);
        print_mem(buf,temp);
        break;
				
      case 'l':
				d = va_arg(ap,int);
        for (i=0;i<2;i++)
        {
          temp = d >> ((1-i)*8);
          print_byte(SemiHexToChar(temp >> 4));
          print_byte(SemiHexToChar(temp));
        }
        break;
      case 't':
        ptr = va_arg(ap,INT8U *);
        d = va_arg(ap,INT32U);
        for (i=0;i<d;i++)
          {
        temp = IntToString(buf,*ptr++,0);    
        print_mem(buf,temp);
        print_byte(' ');
      }      
        break;
      case 'o':
        op = va_arg(ap,int);
        print_bytehex(op);
        break;
      case 'x':
        d = va_arg(ap,int);
        for (i=0;i<4;i++)
        {
          temp = d >> ((3-i)*8);
          print_byte(SemiHexToChar(temp >> 4));
          print_byte(SemiHexToChar(temp));
        }
        break;
      case 'c':
        op = va_arg(ap,int);
        print_byte((INT8U)op);
        break;
      case 'h':
        ptr = va_arg(ap,INT8U *);
        d = va_arg(ap,INT32U);
        print_memhex(ptr,d);
        break;
      case 'm':
        ptr = va_arg(ap,INT8U *);
        d = va_arg(ap,INT32U);
        print_mem(ptr,d);
        break;     
      case 's':
        s = va_arg(ap, char *);
        print(s);
        break;
      case 'p':
        ptr = va_arg(ap, void *);
        d = (INT32U)ptr;
        print("0x");
        for (i=0;i<4;i++)
        {
          temp = d >> ((3-i)*8);
          print_byte(SemiHexToChar(temp >> 4));
          print_byte(SemiHexToChar(temp));
        }
        break;   
      default: 
        sht = *fmt;
        if(sht > '0' && sht < '9') 
        {
          sht -= '0';
        }
        else
        {
          sht = 0;
          print_byte('%');
        }
        break;   
    }
    fmt++;
  }
  va_end(ap);
}



////////////////////////////////////////////////////////////////////////////////

//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量  
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
  DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待DMA可配置 
	
  /* 配置 DMA Stream */
  DMA_InitStructure.DMA_Channel = chx;  //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
  DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//初始化DMA Stream
	

}

//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量  
void MYDMA_Config_Circular(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
  DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待DMA可配置 
	
  /* 配置 DMA Stream */
  DMA_InitStructure.DMA_Channel = chx;  //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到存储器模式
  DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// 使用循环模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//初始化DMA Stream
	

}

//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输 
}	  


uint8 Debug_init(uint32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
#if Debug_Mode_ITorDMA == 0	
	NVIC_InitTypeDef NVIC_InitStructure;
#endif	
	
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//关闭相关中断
	USART_DeInit(USART1);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	//-USART_ClearFlag(USART1, USART_FLAG_TC);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC的时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;	// 232_pwr
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL; 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_2); //232 pwr

#if Debug_Mode_ITorDMA == 0
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
#else
	MYDMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);//DMA2,STEAM7,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送 
	MYDMA_Enable(DMA2_Stream7,1);     //开始一次DMA传输！	
	
	MYDMA_Config_Circular(DMA2_Stream5,DMA_Channel_4,(u32)&USART1->DR,(u32)DebugRx,DEBUG_RX_MAX);//DMA2,STEAM5,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);  //使能串口1的DMA发送 
	DMA_Cmd(DMA2_Stream5, ENABLE);                      //开启DMA传输 
#endif
////////////////////////////////////////////////////////////////////////////////
	InitFIFO(&DebugTxFifo,DebugTx,sizeof(DebugTx));
	InitFIFO(&DebugRxFifo,DebugRx,sizeof(DebugRx));
////////////////////////////////////////////////////////////////////////////////	
	return 0;
}

// 0 : success    1: failed
uint8 DebugClosePort(void)
{
	// disble recv
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//关闭相关中断
	USART_DeInit(USART1);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);//关闭USART2时钟

	return 0;
}

uint8 DebugSendData(uint8 Data)
{
	while((USART1->SR&0X40)==0);
	USART_SendData(USART1,Data);
	return 0;
}

void DebugRecData(INT8U* Dat,INT16U Len)
{
#if	Debug_Mode_ITorDMA == 0
  for(;Len > 0;Len--,Dat++)
  {
    WriteFIFO(&DebugRxFifo,*Dat);
  }
#else
	INT16U temp_len;
	
	temp_len = DMA_GetCurrDataCounter(DMA2_Stream5);	//-得到当前还剩余多少个数据
	WriteFIFODMA(&DebugRxFifo,Dat,temp_len);
#endif	
}

//串口1中断服务程序
void USART1_IRQHandler(void)
{
	uint8 Res;
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)  
	{//接收中断
		Res =USART_ReceiveData(USART1);//读取接收到的数据
		DebugRecData(&Res,1);
	}
}


////////////////////////////////////////////////////////////////////////////////

INT16U ReadDebugData(INT8U *data,INT16U LEN)
{
	int res,i = 0;
	
	if(FIFOUsed(&DebugRxFifo))
	{
		while(LEN--)
		{
			res = ReadFIFO(&DebugRxFifo);
			if(res != 0xff)
				data[i++] = res;
			else
				return i;	
		}
	}
	
	return i;	
}

//-0 非阻塞发送,1 阻塞发送
void DealDebugSend(INT8U flag)
{
  INT8U Data;	
	INT16U i = 0;
  if(FIFOEmpty(&DebugTxFifo))
  {//发送缓存空
    return;
  }
#if Debug_Mode_ITorDMA == 1	
	if(flag)
	{
		while(1)
		{
			if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//等待DMA2_Steam7传输完成
			{ 
				DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//清除DMA2_Steam7传输完成标志 
				break;
			}
		}
	}
	else
	{
		if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//等待DMA2_Steam7传输完成
		{ 
			DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//清除DMA2_Steam7传输完成标志 
		}
		else
			return;
	}	
	
#endif	
	
	
  while(FIFOUsed(&DebugTxFifo))
  {
    Data = ReadFIFO(&DebugTxFifo);
#if EN_DEBUG_RS485 > 0
    RS485SendData(&Data,1);
#endif

#if Debug_Mode_ITorDMA == 0
    DebugSendData(Data);
#else		
		//-写入DMA缓存中
		SendBuff[i++] = Data;
#endif
	
  }
	
#if Debug_Mode_ITorDMA == 1	
	MYDMA_Enable(DMA2_Stream7,i);     //开始一次DMA传输！	 
#endif
	
}

////////////////////////////////////////////////////////////////////////////////
