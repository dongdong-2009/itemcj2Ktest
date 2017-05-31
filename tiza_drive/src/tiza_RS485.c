/*
   RS485 driver for TZ2000
   UART2
   485_RX	PA3
   485_RE	PG5
   485_TX PA2
*/
#define RS485_GLOBAL

#include "tiza_include.h"

#define RS485_Mode_ITorDMA	1	//-0 中断,1 DMA

#define RS485_RX_MAX    256
#define RS485_TX_MAX    1024

INT8U   RS485Rx[RS485_RX_MAX];
INT8U   RS485Tx[RS485_TX_MAX];

static  FIFO    RS485TxFifo;
static  FIFO    RS485RxFifo;

#define SEND_BUF_SIZE 8200	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.

static  INT8U SendBuff[SEND_BUF_SIZE];	//发送数据缓冲区

#define DISABLE_485_RECV()	(GPIO_SetBits(GPIOG, GPIO_Pin_5));
#define ENABLE_485_RECV()	(GPIO_ResetBits(GPIOG, GPIO_Pin_5));
////////////////////////////////////////////////////////////////////////////////
/*****************************************************************
*                   FIFO
******************************************************************/
static  void InitFIFO(FIFO *fifo, INT8U *array, INT16U deepth)
{
    fifo->deepth    = deepth;
    fifo->occupy    = 0;
    fifo->array     = array;
    fifo->limit     = array + deepth;
    fifo->wp = fifo->array;
    fifo->rp = fifo->array;
}

static  void ResetFIFO(FIFO *fifo)
{
    fifo->occupy = 0;
    fifo->rp = fifo->array;
    fifo->wp = fifo->array;
}


static  int WriteFIFO(FIFO *fifo, INT8U unit)
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

static  int WriteFIFOs(FIFO *fifo, INT8U *units, INT16U unitsize)  
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

static  int WriteFIFODMA(FIFO *fifo, INT8U *units, INT16U unitsize)  
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

static  int FIFOEmpty(FIFO *fifo)
{
    if (fifo->occupy == 0) return true;
    else return false;
}    

static  int FIFOFull(FIFO *fifo)
{
    if (fifo->occupy == fifo->deepth) return true;
    else return false;
}
    
static  INT16U  FIFOSpare(FIFO *fifo)
{
    return fifo->deepth - fifo->occupy;
}
    
static  INT8U ReadFIFO(FIFO *fifo)
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

static  INT16U FIFOUsed(FIFO *fifo)
{
    return fifo->occupy;
}



static  void print_byte(INT8U ch)
{
  WriteFIFO(&RS485TxFifo,ch);
}

static  void print_bytehex(INT8U ch)
{
  WriteFIFO(&RS485TxFifo,SemiHexToChar(ch >> 4));
  WriteFIFO(&RS485TxFifo,SemiHexToChar(ch));
  WriteFIFO(&RS485TxFifo,' ');
}

static  void print_mem(INT8U *mem, INT16U memsize)
{
  WriteFIFOs(&RS485TxFifo,mem,memsize);
}

static  void print_memhex(INT8U *mem, INT16U memsize)
{
  for (; memsize > 0; memsize--)
  {
    print_bytehex(*mem++);
  }
}

static  void print(char *str)
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

static  void sprint_byte(INT8U ch)
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
void RS485Print(const char *fmt, ...)
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
static  void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
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
static  void MYDMA_Config_Circular(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
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
static  void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输 
}	  


uint8 RS485_init(uint32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
#if RS485_Mode_ITorDMA == 0	
	NVIC_InitTypeDef NVIC_InitStructure;
#endif	
	
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//关闭相关中断
	USART_DeInit(USART2);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //使能GPIOG时钟
	// 485_en pin
  GPIO_InitStructure.GPIO_Pin= GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL; 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //告诉GPIO
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	ENABLE_485_RECV();
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOC2与GPIOC3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART2, ENABLE);  //使能串口2 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);

#if RS485_Mode_ITorDMA == 0
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
#else
	MYDMA_Config(DMA1_Stream6,DMA_Channel_4,(u32)&USART2->DR,(u32)SendBuff,SEND_BUF_SIZE);//DMA2,STEAM7,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);  //使能串口2的DMA发送 
	MYDMA_Enable(DMA1_Stream6,1);     //开始一次DMA传输！	
	
	MYDMA_Config_Circular(DMA1_Stream5,DMA_Channel_4,(u32)&USART2->DR,(u32)RS485Rx,RS485_RX_MAX);//DMA2,STEAM5,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);  //使能串口2的DMA接收 
	DMA_Cmd(DMA1_Stream5, ENABLE);                      //开启DMA传输 
	
	//-中断配置
	//-DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);
#endif
////////////////////////////////////////////////////////////////////////////////
	InitFIFO(&RS485TxFifo,RS485Tx,sizeof(RS485Tx));
	InitFIFO(&RS485RxFifo,RS485Rx,sizeof(RS485Rx));
////////////////////////////////////////////////////////////////////////////////	
	return 0;
}

// 0 : success    1: failed
uint8 RS485ClosePort(void)
{
	// disble recv
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//关闭相关中断
	USART_DeInit(USART2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);//关闭USART2时钟

	return 0;
}

uint8 RS485SendData(uint8 Data)
{
	while((USART2->SR&0X40)==0);
	USART_SendData(USART2,Data);
	return 0;
}

void RS485RecData(INT8U* Dat,INT16U Len)
{
#if	RS485_Mode_ITorDMA == 0
  for(;Len > 0;Len--,Dat++)
  {
    WriteFIFO(&RS485RxFifo,*Dat);
  }
#else
	INT16U temp_len;
	
	temp_len = DMA_GetCurrDataCounter(DMA1_Stream5);	//-得到当前还剩余多少个数据
	WriteFIFODMA(&RS485RxFifo,Dat,temp_len);
#endif	
}

//串口1中断服务程序
void USART2_IRQHandler(void)
{
	uint8 Res;
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)  
	{//接收中断
		Res =USART_ReceiveData(USART2);//读取接收到的数据
		RS485RecData(&Res,1);
	}
}


//////////////////////////////////////////////////////////////////////////////////

INT16U ReadRS485Data(INT8U *data,INT16U LEN)
{
	int res,i = 0;
	
	if(FIFOUsed(&RS485RxFifo))
	{
		while(LEN--)
		{
			res = ReadFIFO(&RS485RxFifo);
			if(res != 0xff)
				data[i++] = res;
			else
				return i;	
		}
	}
	
	return i;	
}

//-0 非阻塞发送,1 阻塞发送
void DealRS485Send(INT8U flag)
{
  INT8U Data;	
	INT16U i = 0;
  if(FIFOEmpty(&RS485TxFifo))
  {//发送缓存空
    return;
  }
#if RS485_Mode_ITorDMA == 1	
	if(flag)
	{
		while(1)
		{
			if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET)//等待DMA1_Steam6传输完成
			{ 
				DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);//清除DMA1_Steam6传输完成标志 
				break;
			}
		}
	}
	else
	{
		if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET)//等待DMA1_Steam6传输完成
		{ 
			DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);//清除DMA1_Steam6传输完成标志 
		}
		else
			return;
	}	
	
#endif	
	
	
  while(FIFOUsed(&RS485TxFifo))
  {
    Data = ReadFIFO(&RS485TxFifo);
#if EN_RS485_RS485 > 0
    RS485SendData(&Data,1);
#endif

#if RS485_Mode_ITorDMA == 0
    RS485SendData(Data);
#else		
		//-写入DMA缓存中
		SendBuff[i++] = Data;
#endif
	
  }
	
#if RS485_Mode_ITorDMA == 1	
	MYDMA_Enable(DMA1_Stream6,i);     //开始一次DMA传输！	 
#endif
	
}

////////////////////////////////////////////////////////////////////////////////


