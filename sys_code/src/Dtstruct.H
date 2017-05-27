/****************************************************************
**                DTSTRUCT.H                                    *
**               2010-03-10                                     *
****************************************************************/
#ifndef __DTSTRUCT_H_
#define __DTSTRUCT_H_
#include "..\system\rtc.h"
/*****************************************************************
*                  DEFINE FIFO STRUCT
******************************************************************/
typedef struct {
    INT16U      deepth;
    INT16U      occupy;
    INT8U       *array;
    INT8U       *limit;
    INT8U       *wp;
    INT8U       *rp;
} FIFO;


#define  LP_FIFO                 FIFO
// 2011-11-14修改成为环形FIFO,FIFO满后,丢弃最先入栈数据,但是WriteFIFOs不变
void    InitFIFO(FIFO *fifo, INT8U *array, INT16U deepth);
void    ResetFIFO(FIFO *fifo);
INT8U   *GetFifoRP(FIFO *fifo);
INT8U   *GetFifoWP(FIFO *fifo);
BOOLEAN WriteFIFO(FIFO *fifo, INT8U unit);
BOOLEAN WriteFIFOs(FIFO *fifo, INT8U *units, INT16U unitsize);
BOOLEAN FIFOEmpty(FIFO *fifo);
BOOLEAN FIFOFull(FIFO *fifo);
INT8U   ReadFIFO(FIFO *fifo);
INT16U  FIFOSpare(FIFO *fifo);
INT16U  FIFOUsed(FIFO *fifo);

/****************************************************************
               DEFINE FILO STRUCT
****************************************************************/
typedef struct {
    INT16U      deepth;
    INT8U       *array;
    INT8U       *wp;    
} FILO;

void  InitFILO(FILO *filo, INT8U *array, INT16U deepth);
void  ResetFILO(FILO *filo);
INT8U*  FILOStartPos(FILO *filo);
BOOLEAN FILOEmpty(FILO *filo);
BOOLEAN FILOFull(FILO *filo);
BOOLEAN WriteFILO(FILO *filo, INT8U unit);
INT8U  ReadFILO(FILO *filo);
INT16U  FILOOccupy(FILO *filo);


/*****************************************************************
*                  DEFINE STRUCT FIFO STRUCT
******************************************************************/
typedef struct {
    INT16U       deepth;
    INT16U       occupy;
	INT16U       stsize;
    void         *array;
    void         *limit;
    void         *wp;
    void         *rp;
} STFIFO;

void    InitSTFIFO(STFIFO *stfifo, void *array,INT16U stsize,INT16U deepth);
void    ResetSTFIFO(STFIFO *stfifo);
BOOLEAN WriteSTFIFO(STFIFO *stfifo, void *stunit);
BOOLEAN STFIFOEmpty(STFIFO *stfifo);
BOOLEAN STFIFOFull(STFIFO *stfifo);
INT32U  ReadSTFIFO(STFIFO *stfifo,void *stunit);

/****************************************************************
            DEFINE QUEUE STRUCT
****************************************************************/
#define QUEUEMEM     INT8U
#define QUEUENODE    struct node

typedef struct node{
    QUEUENODE  *next;
} NODE;

typedef struct {
    QUEUENODE  *head;
    QUEUENODE  *tail;
    INT16U     item;
} QUEUE;

BOOLEAN     CreateQueue(QUEUE *que);
INT16U      QueueItem(QUEUE *que);
QUEUEMEM    *QueueHead(QUEUE *que);
QUEUEMEM    *QueueTail(QUEUE *que);
QUEUEMEM    *QueueNext(QUEUEMEM *element);
QUEUEMEM    *DelQueueElement(QUEUE *que, QUEUEMEM *element);
QUEUEMEM    *DelQueueHead(QUEUE *que);
QUEUEMEM    *DelQueueTail(QUEUE *que);
BOOLEAN     AppendQueue(QUEUE *que, QUEUEMEM *element);
BOOLEAN     AllocateQueue(QUEUE *que, QUEUEMEM *mem, INT16U item, INT16U itemsize);


/****************************************************************
            DEFINE STREAM STRUCT
****************************************************************/
typedef struct {
    INT8U	   *streambuf;
    INT8U      *optptr;
    INT16U     limit;
}STREAM;

BOOLEAN  InitStream(STREAM *sp, INT8U *buf, INT16U limit);
INT16U 	GetStreamLimit(STREAM *sp);
INT16U   GetStreamLen(STREAM *sp);
INT8U    *GetStreamOpt(STREAM *sp);
INT8U    *GetStreamBuf(STREAM *sp);
void     LocateStream(STREAM *sp, INT16U len);
void     StreamWrByte(STREAM *sp, INT8U chb);
void     StreamWrWord(STREAM *sp, INT16U chw);
void     StreamWrDword(STREAM *sp, INT32U chdw);
void     StreamWrLF(STREAM *sp);
void     StreamWrCR(STREAM *sp);
void     StreamWrStr(STREAM *sp, char *str);
void     StreamWrData(STREAM *sp, INT8U *data, INT16U len);
void     StreamWrRTC(STREAM *sp, RTC_ST *prtc);
void     StreamWrBcdRTC(STREAM *sp, RTC_ST *prtc);
INT8U    StreamRdByte(STREAM *sp);
INT16U   StreamRdWord(STREAM *sp);
INT32U   StreamRdDword(STREAM *sp);
void StreamRdStr(STREAM *sp, char *str);
void     StreamRdData(STREAM *sp, INT8U *data, INT16U len);
void     StreamWrite(STREAM *sp,const char *fmt, ...);

void     StreamWrWord_Little(STREAM *sp, INT16U chw);
void     StreamWrDword_Little(STREAM *sp, INT32U chdw);
INT16U   StreamRdWord_Little(STREAM *sp);
INT32U   StreamRdDword_Little(STREAM *sp);
INT32U  STFIFOUsed(STFIFO *stfifo);

#endif

