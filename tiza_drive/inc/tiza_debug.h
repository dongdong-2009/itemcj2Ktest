#ifndef __TIZA_DEBUG_H
#define __TIZA_DEBUG_H

#include "tiza_include.h"

	#ifdef DEBUG_GLOBAL
		#define EXTERN_DEBUG
	#else
		#define EXTERN_DEBUG extern
	#endif

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

// 2011-11-14修改成为环形FIFO,FIFO满后,丢弃最先入栈数据,但是WriteFIFOs不变
void    InitFIFO(FIFO *fifo, INT8U *array, INT16U deepth);
void    ResetFIFO(FIFO *fifo);
INT8U   *GetFifoRP(FIFO *fifo);
INT8U   *GetFifoWP(FIFO *fifo);
int WriteFIFO(FIFO *fifo, INT8U unit);
int WriteFIFOs(FIFO *fifo, INT8U *units, INT16U unitsize);
int WriteFIFODMA(FIFO *fifo, INT8U *units, INT16U unitsize);
int FIFOEmpty(FIFO *fifo);
int FIFOFull(FIFO *fifo);
INT8U   ReadFIFO(FIFO *fifo);
INT16U  FIFOSpare(FIFO *fifo);
INT16U  FIFOUsed(FIFO *fifo);


	EXTERN_DEBUG uint8 Debug_init(uint32 bound);
	EXTERN_DEBUG uint8 DebugClosePort(void);
	EXTERN_DEBUG uint8 DebugSendData(uint8 Data);
	EXTERN_DEBUG INT16U ReadDebugData(INT8U *data,INT16U LEN);

	EXTERN_DEBUG void DPrint(const char *fmt, ...);
	EXTERN_DEBUG void DealDebugSend(INT8U flag);
	
	/* 潍柴定义 void RS232RecData(uint8 *Data, uint8 Len) */
	
#endif

