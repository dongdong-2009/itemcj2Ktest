/****************************************************************
**                DTSTRUCT.H                                    *
**               2010-03-10                                     *
****************************************************************/
#include "includes.h"
#include "dtstruct.h"
#include "operator.h"
#include "stdarg.h"

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


BOOLEAN WriteFIFO(FIFO *fifo, INT8U unit)
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

BOOLEAN WriteFIFOs(FIFO *fifo, INT8U *units, INT16U unitsize)  
{
    if (unitsize > fifo->deepth - fifo->occupy) return FALSE;
    for (; unitsize > 0; unitsize--) { 
       *fifo->wp++ = *units++;
       if (fifo->wp >= fifo->limit) fifo->wp = fifo->array;
       OS_ENTER_CRITICAL();
       fifo->occupy++;
       OS_EXIT_CRITICAL();
    }    
    return TRUE;
}

BOOLEAN FIFOEmpty(FIFO *fifo)
{
    if (fifo->occupy == 0) return true;
    else return false;
}    

BOOLEAN FIFOFull(FIFO *fifo)
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

	OS_ENTER_CRITICAL();
	fifo->occupy--;
	OS_EXIT_CRITICAL();
	return ret;
}

INT16U FIFOUsed(FIFO *fifo)
{
    return fifo->occupy;
}

/******************************************************************
   FILO
******************************************************************/
void  InitFILO(FILO *filo, INT8U *array, INT16U deepth)
{
    filo->deepth    = deepth;
    filo->array     = array;
    filo->wp = filo->array;
}
    
void  ResetFILO(FILO *filo)
{
    filo->wp = filo->array;
}

INT8U*  FILOStartPos(FILO *filo)
{
    return filo->array;
}    

BOOLEAN FILOEmpty(FILO *filo)
{
    if (filo->wp == filo->array) 
	{
		return true;
	}
    else 
	{
		return false;
	}
}    
BOOLEAN FILOFull(FILO *filo)
{
    if (filo->wp >= filo->array + filo->deepth) return true;
    else return false;
}    
    
BOOLEAN WriteFILO(FILO *filo, INT8U unit)
{
    if (FILOFull(filo))
    {
		return false;
    }
    else
	{
		*filo->wp = unit;
		filo->wp++;
		return true;
    }
}
    
INT8U  ReadFILO(FILO *filo)
{
    if (FILOEmpty(filo)) return 0xff;
    filo->wp--;
    return *filo->wp;
}

INT16U  FILOOccupy(FILO *filo)
{
    return filo->wp - filo->array;
}

/******************************************************************
   STFIFO
******************************************************************/
void   InitSTFIFO(STFIFO *stfifo, void *array,INT16U stsize,INT16U deepth)
{
    stfifo->deepth    = deepth;
    stfifo->occupy    = 0;
    stfifo->array     = array;
	stfifo->stsize    = stsize;
    stfifo->limit     = (void*)((INT32U)array + stsize*deepth);
    stfifo->wp = stfifo->array;
    stfifo->rp = stfifo->array;
}

void ResetSTFIFO(STFIFO *stfifo)
{
    stfifo->occupy = 0;
    stfifo->rp = stfifo->array;
    stfifo->wp = stfifo->array;
}


BOOLEAN WriteSTFIFO(STFIFO *stfifo, void *stunit)
{
	if (stfifo->occupy >= stfifo->deepth) return FALSE;
    
	memcpy((INT8U*)stfifo->wp,(INT8U*)stunit,stfifo->stsize);
	stfifo->wp = (void*)((INT32U)stfifo->wp + stfifo->stsize);
	if (stfifo->wp >= stfifo->limit) stfifo->wp = stfifo->array;
	OS_ENTER_CRITICAL();
	stfifo->occupy++;
	OS_EXIT_CRITICAL();
	return TRUE;
}

BOOLEAN STFIFOEmpty(STFIFO *stfifo)
{
    if (stfifo->occupy == 0) return true;
    else return false;
}    

INT32U ReadSTFIFO(STFIFO *stfifo,void *stunit)
{
	if (stfifo->occupy == 0)
	{
		return 0x0;
	}
	memcpy((INT8U*)stunit,(INT8U*)stfifo->rp,stfifo->stsize);
	stfifo->rp = (void*)((INT32U)stfifo->rp + stfifo->stsize);
	if (stfifo->rp >= stfifo->limit)
	{
		stfifo->rp = stfifo->array;
	}
	OS_ENTER_CRITICAL();
	stfifo->occupy--;
	OS_EXIT_CRITICAL();
	return stfifo->stsize;
}

INT32U  STFIFOUsed(STFIFO *stfifo)
{
    return stfifo->occupy;
}

/******************************************************************
   QUEUE
******************************************************************/
BOOLEAN CreateQueue(QUEUE *que)
{
    if (que == 0) return FALSE;
    que->head = 0;
    que->tail = 0;
    que->item = 0;
    return TRUE;
}

INT16U QueueItem(QUEUE *que)
{
    if (que == 0) return 0;
    else return (que->item);
}

QUEUEMEM *QueueHead(QUEUE *que)
{
    if (que == 0 || que->item == 0) return 0;
    else return ((QUEUEMEM *)que->head);// + sizeof(NODE));
}

QUEUEMEM *QueueTail(QUEUE *que)
{
    if (que == 0 || que->item == 0) return 0;
    else return ((QUEUEMEM *)que->tail);// + sizeof(NODE));
}

QUEUEMEM *QueueNext(QUEUEMEM *element)
{
    QUEUENODE *curnode;
	
    if (element == 0) return 0;
    curnode = (QUEUENODE *)(element);// - sizeof(NODE));
    if ((curnode = curnode->next) == 0) return 0;
    else return ((QUEUEMEM *)curnode);// + sizeof(NODE));
}

QUEUEMEM *DelQueueElement(QUEUE *que, QUEUEMEM *element)
{
    QUEUENODE *curnode, *prenode, *nextnode;

    if (que == 0 || element == 0) return 0;
    if (que->item == 0) return 0;

    que->item--;
    curnode  = (QUEUENODE *)(element);// - sizeof(NODE));
   
    if (curnode == que->head) {
       que->head = curnode->next; 
       if (que->item == 0) {
          que->tail = 0;
          return 0;
       } else {
          return (QUEUEMEM *)(que->head);// + sizeof(NODE);
       }   
    }  
    
    nextnode = curnode->next;
    prenode = que->head;
    while (prenode != 0) {
       if (prenode->next == curnode) {
          break;
       } else {
          prenode = prenode->next;
       }    
    }
    if (prenode == 0) return 0;
        
    prenode->next = nextnode;
    if (curnode == que->tail) {
       que->tail = prenode;
       return 0;
    } else {
       return ((QUEUEMEM *)nextnode);// + sizeof(NODE));
    }              
}

// Return: Queue head 
QUEUEMEM *DelQueueHead(QUEUE *que)
{
    QUEUEMEM *element;

    if (que == 0 || que->item == 0) return 0;

    element = (QUEUEMEM *)que->head;//+ sizeof(NODE);
    DelQueueElement(que, element);
    return element;
}

// Return: Queue tail 
QUEUEMEM *DelQueueTail(QUEUE *que)
{
    QUEUEMEM *element;

    if (que == 0 || que->item == 0) return 0;

    element = (QUEUEMEM *)que->tail;// + sizeof(NODE);
    DelQueueElement(que, element);
    return element;
}

BOOLEAN AppendQueue(QUEUE *que, QUEUEMEM *element)
{
	QUEUENODE *curnode;

	if(que == 0 || element == 0)
	{
		return FALSE;
	}
	curnode = (QUEUENODE *)(element);// - sizeof(NODE));
	if(que->item == 0)
	{
		que->head = curnode;
	}
	else
	{
		que->tail->next = curnode;
	}
	curnode->next = 0;
	que->tail = curnode;
	que->item++;
	return TRUE;
}

/*
BOOLEAN AllocateQueue(QUEUE *que, QUEUEMEM *mem, INT16U item, INT16U itemsize)
{
    if (!CreateQueue(que)) return FALSE;

    mem += sizeof(NODE);
    for(;item > 0; item--){
        AppendQueue(que, mem);
        mem += itemsize;
    }
    return TRUE;
}
*/
/*****************************************************************
*                  STREAM
******************************************************************/
BOOLEAN InitStream(STREAM *sp, INT8U *buf, INT16U limit)
{
    if (sp == 0) return false;
    sp->limit   = limit;
    sp->optptr   = buf;
    sp->streambuf = buf;
    return true;
}
INT16U GetStreamLimit(STREAM *sp)
{
    return (sp->limit);
}

INT16U GetStreamLen(STREAM *sp)
{
    return ((INT32U)sp->optptr - (INT32U)sp->streambuf);
}

INT8U *GetStreamOpt(STREAM *sp)
{
    return (sp->optptr);
}

INT8U *GetStreamBuf(STREAM *sp)
{
    return (sp->streambuf);
}

void LocateStream(STREAM *sp, INT16U len)
{
	if(sp != 0)
	{
		sp->optptr += len;
		if(sp->optptr > sp->streambuf + sp->limit)
		{
			sp->optptr = sp->streambuf + sp->limit;
		}
	}
}

void StreamWrByte(STREAM *sp, INT8U chb)
{
    if (sp != 0){
        if ((INT32U)sp->optptr < (INT32U)sp->streambuf + sp->limit) {
            *sp->optptr++ = chb;
        }
    }
}

void StreamWrWord(STREAM *sp, INT16U chw)
{
    WORD_UNION temp;
    
    temp.word = chw;
    StreamWrByte(sp, temp.bytes.high);
    StreamWrByte(sp, temp.bytes.low);
}

void StreamWrDword(STREAM *sp, INT32U chdw)
{
    INT16U temp;
    
    temp = chdw >> 16;
	StreamWrWord(sp,temp);
	temp = chdw & 0xffff;
	StreamWrWord(sp,temp);
}

void StreamWrWord_Little(STREAM *sp, INT16U chw)
{
    StreamWrByte(sp, chw & 0xFF);
    StreamWrByte(sp, chw >> 8);
}

void StreamWrDword_Little(STREAM *sp, INT32U chdw)
{
    INT16U temp;
    
    temp = chdw & 0xffff;
    StreamWrWord_Little(sp,temp);
    temp = chdw >> 16;
    StreamWrWord_Little(sp,temp);
}

INT16U   StreamRdWord_Little(STREAM *sp)
{
    INT16U tmp,tmp1;
    tmp  = StreamRdByte(sp);
    tmp1 = StreamRdByte(sp);
    tmp1 <<= 8;
    tmp += tmp1;
    return tmp;
}

INT32U   StreamRdDword_Little(STREAM *sp)
{
    INT32U tmp,tmp1;
    tmp = StreamRdWord_Little(sp);
    tmp1 = StreamRdWord_Little(sp);
    tmp1 <<= 16;
    tmp += tmp1;
    return tmp;
}

void StreamWrLF(STREAM *sp)
{
    StreamWrByte(sp, CRR);
    StreamWrByte(sp, LF);
}

void StreamWrCR(STREAM *sp)
{
    StreamWrByte(sp, CRR);
}

void StreamWrStr(STREAM *sp, char *str)
{
	while(*str)
	{
		StreamWrByte(sp, *str++);
	}
	StreamWrByte(sp,0);
}

void  StreamWrData(STREAM *sp, INT8U *data, INT16U len)
{
    while(len--) { 
       StreamWrByte(sp, *data++);
    }
}


void StreamWrRTC(STREAM *sp, RTC_ST *prtc)
{
    StreamWrite(sp,"%o%o%o%o%o%o",prtc->year,prtc->month,prtc->day,prtc->hour,prtc->minute,prtc->second);
}

void StreamWrBcdRTC(STREAM *sp, RTC_ST *prtc)
{
	BCDRTC_ST  brtc;
	RtcToBcd(prtc,&brtc);
	StreamWrite(sp,"%o%o%o%o%o%o",brtc.year,brtc.month,brtc.day,brtc.hour,brtc.minute,brtc.second);
}
	
INT8U StreamRdByte(STREAM *sp)
{
    return (*sp->optptr++);
}

INT16U StreamRdWord(STREAM *sp)
{
    WORD_UNION temp;
	
    temp.bytes.high = StreamRdByte(sp);
    temp.bytes.low  = StreamRdByte(sp);
    return temp.word;
}


INT32U StreamRdDword(STREAM *sp)
{
    INT32U temp;
	temp = StreamRdWord(sp);
	temp <<= 16;
	temp |= StreamRdWord(sp);
	return temp;
}

void StreamRdData(STREAM *sp, INT8U *data, INT16U len)
{
	while(len--)
	{
		*data++ = *sp->optptr++;
	}
}			  
void StreamRdStr(STREAM *sp, char *str)
{
	INT8U Dat;
	
	do
	{
		Dat = StreamRdByte(sp);
		*str = Dat;
		str++;
	}while(Dat);
}



/*
%b :HEX - BCD //0x32 -> 0x50
%d :INT32 
%o :HEX - INT8U
%x :HEX - INT16U 
%w :HEX - INT32
%s :string;
%m : memory
*/
/*
INT8U Hex2Bcd(INT8U hex)
{
    INT8U bcd;
	hex %= 100;
	bcd = hex / 10;
	bcd <<= 4;
	bcd += hex % 10;
	return bcd;
}
  */
void StreamWrite(STREAM *sp,const char *fmt, ...)
{
	char *s;
	INT8U *ptr;
	INT32U d;
	INT8U op,sht;
	INT8U buf[10];
	va_list ap;

	va_start(ap,fmt);
	sht = 0;
	while (*fmt)
	{
		if(*fmt != '%') 
		{
			sht = 0;
			if(*fmt != '\n')
			{
				StreamWrByte(sp,*fmt++);
			}
			else
			{
				StreamWrLF(sp);
				fmt++;
			}
			continue;
		}
		switch (*++fmt)
		{
			case 'd':
		  case 'l':
				d = va_arg(ap,int);
				op = IntToString(buf,d,sht);
				StreamWrData(sp,buf,op);
				break;
			case 'o':
				op = va_arg(ap,int);
				StreamWrByte(sp,op);
				break;
			case 'x':
				d = va_arg(ap,int);
				StreamWrWord(sp,d);
				break;
			case 'w':
				d = va_arg(ap,int);
				StreamWrDword(sp,d);
				break;
		  case 'm':
				ptr = va_arg(ap,INT8U *);
				d = va_arg(ap,INT32U);
				StreamWrData(sp,ptr,d);
				break;           
			case 's':
				s = va_arg(ap, char *);
				StreamWrStr(sp,s);
				break;
		  case 'b':
				d = va_arg(ap,int);			
				StreamWrByte(sp,Hex_2_Bcd(d));
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
					StreamWrByte(sp,'%'); 
				}
				break;
		}
		fmt++;
	}
	va_end(ap);
}
