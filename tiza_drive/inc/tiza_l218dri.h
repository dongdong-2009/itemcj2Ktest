#ifndef __TIZA_L218DRI_H
#define __TIZA_L218DRI_H
	
#include <stdint.h>

#ifdef TIZA_L218DRI_GLOBAL
	#define TIZA_L218DRI_EXTERN
#else
	#define TIZA_L218DRI_EXTERN extern
#endif
				
		#define WAIT_10MS 10	///延时
		#define SEND_1T 1				///发送1次
				
		#define EXE_NO	 			0X00 ///未执行
		#define EXE_OK	 			0X01
		#define EXE_FAIL 			0X02
		#define EXE_03_ON 		0X03
		#define EXE_04_OFF 		0X04
		#define EXE_CONTINUE 	0X05	//FTP
				
		#define L218_MAX_BUF_LEN 		2048
		
		typedef struct
		{///AT命令结构体
			uint8  *cmd_text;																	/// 指令内容
			uint16 max_ms_wait;																///最长等待n个100毫秒
			uint8  tx_max_count;															///最大发送次数
			uint8  exe_flag;																	///执行结果
			void  (*fun)(uint8 *data,uint16 len,uint8 flag);	///函数处理
		}AT_CMD_STRUCT;

		typedef enum 
		{///指令序列
			AT_NULL_INDEX=0,
			AT_INDEX, 
			AT_E0_INDEX,
			AT_IPR_INDEX,
			AT_CPIN_INDEX,
			AT_ICCID_INDEX,
			AT_CSQ_INDEX,
			AT_CREG_INDEX,
			AT_CGATT_INDEX,
			AT_GTPOS_INDEX,
			
			AT_CIPSTATUS_INDEX,			//10
			AT_CIPMUX_INDEX,
			AT_CSTT_INDEX,
			AT_CIICR_INDEX,
			AT_NETCLOSE_INDEX,
			//AT_CIFSR_INDEX,
			AT_CIPOPEN_TCP_INDEX,
			AT_CIPSEND_TCP_INDEX,
			AT_CIPCLOSE_INDEX,
			AT_CIPRXGET_INDEX,
			
			AT_GETGPS_INDEX,	
			AT_EGDCONT2_INDEX,		//20
			AT_EGDCONT1_INDEX,
			AT_MGPSTS_INDEX,
			AT_MGPSEPO_INDEX,
			AT_EGPSC_INDEX,
			AT_PMTK314_INDEX,		
			AT_PMTK386_INDEX,		

      AT_SAPBR_INDEX,
      AT_FTPTYPE_INDEX,
      AT_FTPSERV_INDEX,
      AT_FTPPORT_INDEX,
      AT_FTPUN_INDEX,
      AT_FTPPW_INDEX,
      AT_FTPGETNAME_INDEX,
      AT_FTPGETPATH_INDEX,
      AT_FTPGET1_INDEX,
      AT_FTPGET2_INDEX,
      AT_FTPQUIT_INDEX,
      AT_FTPSIZE_INDEX,			
			
		}AT_CMD_INDEX;
	///AT指令处理函数---start
		void AtNullFun(uint8 *data,uint16 len,uint8 flag);   
		void AtFun(uint8 *data,uint16 len,uint8 flag);
		void AtE0Fun(uint8 *data,uint16 len,uint8 flag);
		void AtIprFun(uint8 *data,uint16 len,uint8 flag);
		void AtCpinFun(uint8 *data,uint16 len,uint8 flag);
		void AtIccidFun(uint8 *data,uint16 len,uint8 flag);
		void AtCsqFun(uint8 *data,uint16 len,uint8 flag);
		void AtCregFun(uint8 *data,uint16 len,uint8 flag);
		void AtCgattFun(uint8 *data,uint16 len,uint8 flag);
		void AtGtposFun(uint8 *data,uint16 len,uint8 flag);   
		// 网络服务相关
    void AtCipstatusFun(uint8 *data,uint16 len,uint8 flag);
    void AtCipmuxFun(uint8 *data,uint16 len,uint8 flag);
		void AtCsttFun(uint8 *data,uint16 len,uint8 flag);
		void AtCiicrFun(uint8 *data,uint16 len,uint8 flag);
		void AtNetCloseFun(uint8 *data,uint16 len,uint8 flag);
		//void AtCifsrFun(uint8 *data,uint16 len,uint8 flag);
		void AtCipOpenTcpFun(uint8 *data,uint16 len,uint8 flag);
		void AtCipSendTcpFun(uint8 *data,uint16 len,uint8 flag);
		void AtCipCloseFun(uint8 *data,uint16 len,uint8 flag);
		void AtCipRxGetFun(uint8 *data,uint16 len,uint8 flag);
		void AtCipSripFun(uint8 *data,uint16 len,uint8 flag);
		// GPS 相关
		void AtGetgpsFun(uint8 *data,uint16 len,uint8 flag);
		void AtEgdcont2Fun(uint8 *data,uint16 len,uint8 flag);
		void AtEgdcont1Fun(uint8 *data,uint16 len,uint8 flag);
		void AtMgpstsFun(uint8 *data,uint16 len,uint8 flag);
		void AtMgpsepoFun(uint8 *data,uint16 len,uint8 flag);
		void AtEgpscFun(uint8 *data,uint16 len,uint8 flag);
		void AtPmtk314Fun(uint8 *data,uint16 len,uint8 flag);
		void AtPmtk386Fun(uint8 *data,uint16 len,uint8 flag);
		//FTP		
//		void AtCPINFun(uint8 *data,uint16 len,uint8 flag);
		void AtSAPBRFun(uint8 *data,uint16 len,uint8 flag);
		void AtFTPTYPEFun(uint8 *data,uint16 len,uint8 flag);
		void AtFTPSERVFun(uint8 *data,uint16 len,uint8 flag);
		void AtFTPPORTFun(uint8 *data,uint16 len,uint8 flag);
		void AtFTPUNFun(uint8 *data,uint16 len,uint8 flag);
		void AtFTPPWFun(uint8 *data,uint16 len,uint8 flag);
		void AtFTPGETNAMEFun(uint8 *data,uint16 len,uint8 flag);
		void AtFTPGETPATHFun(uint8 *data,uint16 len,uint8 flag);
		void AtFTPGET1Fun(uint8 *data,uint16 len,uint8 flag);
		void AtFTPGET2Fun(uint8 *data,uint16 len,uint8 flag);
		void AtFTPQUITFun(uint8 *data,uint16 len,uint8 flag);
		void AtFTPSIZEFun(uint8 *data,uint16 len,uint8 flag);
	///AT指令处理函数---end
	
	///对外申明 变量
		TIZA_L218DRI_EXTERN uint8 AT_CIPOPEN_TCP[];
		TIZA_L218DRI_EXTERN uint8 AT_CIPSEND_TCP[];
		TIZA_L218DRI_EXTERN uint8 AT_MGPSTS[];
		TIZA_L218DRI_EXTERN uint8 AT_MGPSEPO[];
		TIZA_L218DRI_EXTERN uint8 AT_EGPSC[];


	TIZA_L218DRI_EXTERN const uint8 FTP_TYPEOFINTERCONNECT[];
	TIZA_L218DRI_EXTERN	const uint8 FTP_SETAPN[];
	TIZA_L218DRI_EXTERN const uint8 FTP_OPENBEARER[] ;
	TIZA_L218DRI_EXTERN const uint8 AT_FTPSERV[];
	TIZA_L218DRI_EXTERN const uint8 AT_FTPPORT[];
	TIZA_L218DRI_EXTERN const uint8 AT_FTPUN[];
	TIZA_L218DRI_EXTERN const uint8 AT_FTPPW[] ;
	TIZA_L218DRI_EXTERN const uint8 AT_FTPGETNAME[];
	TIZA_L218DRI_EXTERN const uint8 AT_FTPGETPATH[]; 

	TIZA_L218DRI_EXTERN const uint8 RECV_IPD_ACK[];       	// RECV DATA LEN, ADD "+IPD" HEADER
	//TIZA_L218DRI_EXTERN const uint8 RECV_FROM_ACK[];      // RECV FORM: STRING IF SET SHOW "RECV FROM" HEADER
	TIZA_L218DRI_EXTERN const uint8 OK_ACK[];
	TIZA_L218DRI_EXTERN const uint8 CRLF_ACK[];
	//TIZA_L218DRI_EXTERN const uint8 CIPOPEN_EXTRA_OK[];		// IP IS OPENED SUCCESS
	TIZA_L218DRI_EXTERN const uint8 CIPSEND_EXTRA_OK[];			// SEND SUCCESS
	TIZA_L218DRI_EXTERN const uint8 CIPCLOSE_EXTRA_OK[];		// IP IS CLOSED SUCCESS
	TIZA_L218DRI_EXTERN const uint8 GPRS_HAVE_RX_DATA[];		// 有接收数据
	TIZA_L218DRI_EXTERN const uint8 CONST_DATA_1[];					// 有接收数据
//	TIZA_L218DRI_EXTERN const uint8 FTPGET_OK_ACK[];				// Open FTP session SUCCESS
//	TIZA_L218DRI_EXTERN const uint8 FTPGET_FINISH_ACK[];		// means finish read DATA

	TIZA_L218DRI_EXTERN	AT_CMD_STRUCT g_at_cmd_struct[];
	
	

	#define GPRSUART_Mode_ITorDMA	1	//-0 中断,1 DMA
	#define GPSUART_Mode_ITorDMA	1	//-0 中断,1 DMA
		
	#define GPS_USART_BPR						115200
	#define GPRS_USART_BPR					115200
	#define GPS_UART_TXBUF_LEN   		10
	#define GPS_UART_RXBUF_LEN   		1024
	#define GPRS_UART_TXBUF_LEN  		1024
	#define GPRS_UART_RXBUF_LEN  		4096
	
	
	#define GPRS_DMA_SENDBUF_LEN  	1024
		
	typedef struct
	{
		uint8  flag;
		uint16 head;
		uint16 tail;
		uint8 *buf;
	}USART_STRUCT;
	
	
	TIZA_L218DRI_EXTERN uint8 g_gps_txbuff[GPS_UART_TXBUF_LEN];
	TIZA_L218DRI_EXTERN uint8 g_gps_rxbuff[GPS_UART_RXBUF_LEN];
	TIZA_L218DRI_EXTERN uint8 g_gprs_txbuff[GPRS_UART_TXBUF_LEN];
	TIZA_L218DRI_EXTERN uint8 g_gprs_rxbuff[GPRS_UART_RXBUF_LEN];
	TIZA_L218DRI_EXTERN USART_STRUCT g_gps_txstruct;
	TIZA_L218DRI_EXTERN USART_STRUCT g_gps_rxstruct;
	TIZA_L218DRI_EXTERN USART_STRUCT g_gprs_txstruct;
	TIZA_L218DRI_EXTERN USART_STRUCT g_gprs_rxstruct;
	
	TIZA_L218DRI_EXTERN uint8 GprsDMA_SendBuff[GPRS_DMA_SENDBUF_LEN];		//GPRSDMA发送数据缓冲区
	
	
	TIZA_L218DRI_EXTERN uint8 g_dri_ICCID[20];
	
	
	TIZA_L218DRI_EXTERN	void L218SendAtCmd(uint8 cmd_index,uint8 app_data[],uint8 app_len,uint8 mat_data[],uint8 mat_len, uint8 flag);
	TIZA_L218DRI_EXTERN	uint16 L218UartIsRxDone(uint8 data[],uint16 len);
	TIZA_L218DRI_EXTERN	void ReadOverTailIndex(uint16 len);
	TIZA_L218DRI_EXTERN	void L218_Init(void);
	TIZA_L218DRI_EXTERN	void L218Reset(void);
	
	TIZA_L218DRI_EXTERN	void L218_GPRSUART_Init(uint32 bound);
	TIZA_L218DRI_EXTERN void L218_GPRSUART_Send(uint8 data[],uint16 len);
	TIZA_L218DRI_EXTERN void L218_GPRSUARTSend_Ctr(uint8 flag);


#endif













