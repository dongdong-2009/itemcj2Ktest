#ifndef __TIZA_RS485_H
#define __TIZA_RS485_H

#include "tiza_include.h"

	#ifdef RS485_GLOBAL
		#define EXTERN_RS485
	#else
		#define EXTERN_RS485 extern
	#endif

/*****************************************************************
*                  DEFINE FIFO STRUCT
******************************************************************/
//typedef struct {
//    INT16U      deepth;
//    INT16U      occupy;
//    INT8U       *array;
//    INT8U       *limit;
//    INT8U       *wp;
//    INT8U       *rp;
//} FIFO;

//// 2011-11-14修改成为环形FIFO,FIFO满后,丢弃最先入栈数据,但是WriteFIFOs不变
//void    InitFIFO(FIFO *fifo, INT8U *array, INT16U deepth);
//void    ResetFIFO(FIFO *fifo);
//INT8U   *GetFifoRP(FIFO *fifo);
//INT8U   *GetFifoWP(FIFO *fifo);
//int WriteFIFO(FIFO *fifo, INT8U unit);
//int WriteFIFOs(FIFO *fifo, INT8U *units, INT16U unitsize);
//int WriteFIFODMA(FIFO *fifo, INT8U *units, INT16U unitsize);
//int FIFOEmpty(FIFO *fifo);
//int FIFOFull(FIFO *fifo);
//INT8U   ReadFIFO(FIFO *fifo);
//INT16U  FIFOSpare(FIFO *fifo);
//INT16U  FIFOUsed(FIFO *fifo);


	EXTERN_RS485 uint8 RS485_init(uint32 bound);
	EXTERN_RS485 uint8 RS485ClosePort(void);
	EXTERN_RS485 uint8 RS485SendData(uint8 Data);
	EXTERN_RS485 INT16U ReadRS485Data(INT8U *data,INT16U LEN);

	EXTERN_RS485 void RS485Print(const char *fmt, ...);
	EXTERN_RS485 void DealRS485Send(INT8U flag);
	
	/* 潍柴定义 void RS485RecData(uint8 *Data, uint8 Len) */
	EXTERN_RS485 void RS485RecData(INT8U* Dat,INT16U Len);	
	
#endif

