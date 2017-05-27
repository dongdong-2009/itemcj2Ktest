/*
   RS232 driver for TZ2000
   UART6
*/
#define RS232_GLOBAL

#include "tiza_include.h"

#define RS232_Mode_ITorDMA	1	//-0 �ж�,1 DMA

#define RS232_RX_MAX    256
#define RS232_TX_MAX    1024

INT8U   RS232Rx[RS232_RX_MAX];
INT8U   RS232Tx[RS232_TX_MAX];

static  FIFO    RS232TxFifo;
static  FIFO    RS232RxFifo;

#define SEND_BUF_SIZE 8200	//�������ݳ���,��õ���sizeof(TEXT_TO_SEND)+2��������.

INT8U SendBuff[SEND_BUF_SIZE];	//�������ݻ�����

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
		ReadFIFO(fifo); // 2011-11-14 ��Ϊ������
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
		//-ʹ��DMAʱ����Ҫ���������ƶ�,�����޸ı�־����
			 if(unitsize >= fifo->deepth)
				 unitsize -= fifo->deepth;
			 else
				 unitsize = fifo->deepth - unitsize;	//-�õ����յ���Ч����
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
  WriteFIFO(&RS232TxFifo,ch);
}

static  void print_bytehex(INT8U ch)
{
  WriteFIFO(&RS232TxFifo,SemiHexToChar(ch >> 4));
  WriteFIFO(&RS232TxFifo,SemiHexToChar(ch));
  WriteFIFO(&RS232TxFifo,' ');
}

static  void print_mem(INT8U *mem, INT16U memsize)
{
  WriteFIFOs(&RS232TxFifo,mem,memsize);
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
%h :HEX string��  
%m : memory
%p : Pointer
*/
void RS232Print(const char *fmt, ...)
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

//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:�����ַ
//mar:�洢����ַ
//ndtr:���ݴ�����  
static  void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
  DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//�ȴ�DMA������ 
	
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel = chx;  //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = ndtr;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//��ʼ��DMA Stream
	

}

//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:�����ַ
//mar:�洢����ַ
//ndtr:���ݴ�����  
static  void MYDMA_Config_Circular(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
  DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//�ȴ�DMA������ 
	
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel = chx;  //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//���赽�洢��ģʽ
  DMA_InitStructure.DMA_BufferSize = ndtr;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ʹ��ѭ��ģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//��ʼ��DMA Stream
	

}

//����һ��DMA����
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:���ݴ�����  
static  void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //�ر�DMA���� 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //���ݴ�����  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //����DMA���� 
}	  


uint8 RS232_init(uint32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
#if RS232_Mode_ITorDMA == 0	
	NVIC_InitTypeDef NVIC_InitStructure;
#endif	
	
	USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);//�ر�����ж�
	USART_DeInit(USART6);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART6ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOC6����ΪUSART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOC7����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIOC6��GPIOC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PC6��PC7

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���6 
	
	USART_ClearFlag(USART6, USART_FLAG_TC);

#if RS232_Mode_ITorDMA == 0
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
#else
	MYDMA_Config(DMA2_Stream6,DMA_Channel_5,(u32)&USART6->DR,(u32)SendBuff,SEND_BUF_SIZE);//DMA2,STEAM7,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.
	USART_DMACmd(USART6,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA���� 
	MYDMA_Enable(DMA2_Stream6,1);     //��ʼһ��DMA���䣡	
	
	MYDMA_Config_Circular(DMA2_Stream2,DMA_Channel_5,(u32)&USART6->DR,(u32)RS232Rx,RS232_RX_MAX);//DMA2,STEAM5,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.
	USART_DMACmd(USART6,USART_DMAReq_Rx,ENABLE);  //ʹ�ܴ���1��DMA���� 
	DMA_Cmd(DMA2_Stream2, ENABLE);                      //����DMA���� 
#endif
////////////////////////////////////////////////////////////////////////////////
	InitFIFO(&RS232TxFifo,RS232Tx,sizeof(RS232Tx));
	InitFIFO(&RS232RxFifo,RS232Rx,sizeof(RS232Rx));
////////////////////////////////////////////////////////////////////////////////	
	return 0;
}

// 0 : success    1: failed
uint8 RS232ClosePort(void)
{
	// disble recv
	USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);//�ر�����ж�
	USART_DeInit(USART6);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, DISABLE);//�ر�USART2ʱ��

	return 0;
}

uint8 RS232SendData(uint8 Data)
{
	while((USART6->SR&0X40)==0);
	USART_SendData(USART6,Data);
	return 0;
}

void RS232RecData(INT8U* Dat,INT16U Len)
{
#if	RS232_Mode_ITorDMA == 0
  for(;Len > 0;Len--,Dat++)
  {
    WriteFIFO(&RS232RxFifo,*Dat);
  }
#else
	INT16U temp_len;
	
	temp_len = DMA_GetCurrDataCounter(DMA2_Stream2);	//-�õ���ǰ��ʣ����ٸ�����
	WriteFIFODMA(&RS232RxFifo,Dat,temp_len);
#endif	
}

//����1�жϷ������
void USART6_IRQHandler(void)
{
	uint8 Res;
	
	if(USART_GetITStatus(USART6,USART_IT_RXNE) != RESET)  
	{//�����ж�
		Res =USART_ReceiveData(USART6);//��ȡ���յ�������
		RS232RecData(&Res,1);
	}
}


//////////////////////////////////////////////////////////////////////////////////

INT16U ReadRS232Data(INT8U *data,INT16U LEN)
{
	int res,i = 0;
	
	if(FIFOUsed(&RS232RxFifo))
	{
		while(LEN--)
		{
			res = ReadFIFO(&RS232RxFifo);
			if(res != 0xff)
				data[i++] = res;
			else
				return i;	
		}
	}
	
	return i;	
}

//-0 ����������,1 ��������
void DealRS232Send(INT8U flag)
{
  INT8U Data;	
	INT16U i = 0;
  if(FIFOEmpty(&RS232TxFifo))
  {//���ͻ����
    return;
  }
#if RS232_Mode_ITorDMA == 1	
	if(flag)
	{
		while(1)
		{
			if(DMA_GetFlagStatus(DMA2_Stream6,DMA_FLAG_TCIF6)!=RESET)//�ȴ�DMA2_Steam7�������
			{ 
				DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);//���DMA2_Steam7������ɱ�־ 
				break;
			}
		}
	}
	else
	{
		if(DMA_GetFlagStatus(DMA2_Stream6,DMA_FLAG_TCIF6)!=RESET)//�ȴ�DMA2_Steam7�������
		{ 
			DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);//���DMA2_Steam7������ɱ�־ 
		}
		else
			return;
	}	
	
#endif	
	
	
  while(FIFOUsed(&RS232TxFifo))
  {
    Data = ReadFIFO(&RS232TxFifo);
#if EN_RS232_RS485 > 0
    RS485SendData(&Data,1);
#endif

#if RS232_Mode_ITorDMA == 0
    RS232SendData(Data);
#else		
		//-д��DMA������
		SendBuff[i++] = Data;
#endif
	
  }
	
#if RS232_Mode_ITorDMA == 1	
	MYDMA_Enable(DMA2_Stream6,i);     //��ʼһ��DMA���䣡	 
#endif
	
}

////////////////////////////////////////////////////////////////////////////////


