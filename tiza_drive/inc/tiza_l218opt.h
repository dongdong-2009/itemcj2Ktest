
#ifndef __TIZA_L218OPT_H
#define __TIZA_L218OPT_H

#ifdef TIZA_L218OPT_GLOBAL
	#define TIZA_L218OPT_EXTERN
#else
	#define TIZA_L218OPT_EXTERN extern
#endif
		
	#include <stdint.h>

//=============================================================GPRS=================GPRS=//

	
//	#define TERMINAL_ID_LEN				5			///终端ID长度
	#define SIM_CARD_IMSI_LEN			15		///SIM卡IMSI长度
	#define GSM_IMEI_LEN					15		
//	#define GPRS_SEND_BUF_LEN 	512 		///<1460

	typedef struct
	{///3G模块控制结构体
		uint8   business;  	    //业务
		uint8   at;    		    //服务
	}GPRS_CTR_STRUCT;

	typedef struct
	{///3G模块数据结构体
		uint8 initStatus;       //初始化状态
		uint8 netLinkStatus;    //网络连接状态
		uint8 MserLinkStatus;		//主IP连接状态
		uint8 sendDataStatus;   //数据发送状态
		uint8 GpsOpenStatus;   	//GPS打开状态
		
		uint8 CheckTCPStatus;		//检查TCP连接状态  00初始态 01连接态 02断开态 FF无效
		
		uint8  IP[4];						//IP地址
		uint16 Port;						//端口
		uint8  csq_val;
		uint8  imei[SIM_CARD_IMSI_LEN];	
		uint8* SendData;												//要发送的数据
		uint16 SendDataLen; 	 									//要发送的数据的长度
		uint8  setnetparaok_flag;								//设置网络参数好了标志
	}GPRS_DATA_STRUCT;


///状态枚举
typedef enum
{
  GPRS_NULL = 0,
  GPRS_INIT,
  GPRS_CONNECT_NET,
  GPRS_CONNECT_SERVER,
  GPRS_DISCONNECT_SERVER,
	GPRS_DISCONNECT_NET,
	GPRS_SEND_DATA,
  GPRS_RESET,
  GPRS_CHECK_TCPSTATUS,
	
	GPS_OPEN_NET,
	GPS_CLOSE_NET,
}GPRS_BUSINESS;

//3G模块初始化状态
typedef enum{
	GPRS_INIT_OK = 0,
	GPRS_INIT_NO,
	GPRS_INIT_ING,
}GPRS_INIT_STATUS;
//3G模块网络连接状态
typedef enum{
	GPRS_NETLINK_OK = 0,
	GPRS_NETLINK_NO,
	GPRS_NETLINK_ING,
	GPRS_NETLINK_ERR,		//关闭失败
}GPRS_NETLINK_STATUS;
//服务器连接状态
typedef enum{
	GPRS_SERLINK_OK = 0,
	GPRS_SERLINK_NO,
	GPRS_SERLINK_ING,
	GPRS_SERLINK_ERR,		//关闭失败
}GPRS_SERLINK_STATUS;
//数据发送状态
typedef enum{
	GPRS_SENDDATA_ZERO = 0,	//初始状态
	GPRS_SENDDATA_BUSY,			//忙,正在发送
	GPRS_SENDDATA_IDLE,    	//成功就空闲
	GPRS_SENDDATA_OUT,      //要发送
	GPRS_SENDDATA_ERR,     
}GPRS_SENDDATA_STATUS;

//GPS模块打开状态
typedef enum{
	GPS_OPEN_OK = 0,
	GPS_OPEN_NO,
	GPS_OPEN_ING,
	GPS_OPEN_ERR,		//关闭失败
}GPS_OPEN_STATUS;

///变量
TIZA_L218OPT_EXTERN GPRS_CTR_STRUCT g_gprs_ctr_struct;
TIZA_L218OPT_EXTERN GPRS_DATA_STRUCT g_gprs_data_struct;

TIZA_L218OPT_EXTERN void ModlueCalledProcess(void);

//=============================================================GPS===================GPS=//
	
	#define RMC_TYPE 		1
	#define GGA_TYPE 		2
	
	#define LAT_TYPE 		0
	#define LONG_TYPE 	1
	#define SPEED_TYPE 	2
	#define DIR_TYPE 		3
	#define AMP_TYPE 		4

	#define GPS_INFORM_LEN			71	 		///GPS截取信息长度
	
	typedef union 
	{///GPS截取信息
		uint8 array[GPS_INFORM_LEN];
		struct 
		{
			uint32 latitude;
			uint32 longitude;
			uint16 high;
			uint8  speedk;
			uint8  direction;
			uint8  year;
			uint8  month;
			uint8  day;
			uint8  hour;
			uint8  minute;
			uint8  second;
			uint8  starnum;
			union08 statu;			//B0=0定位有效 B1=0北纬 B2=0东经
			
		}subitem;
	}GPS_INFORM_UNION;
	
	typedef struct
	{
		GPS_INFORM_UNION gpsinform;
	}GPS_STRUCT;

TIZA_L218OPT_EXTERN 	GPS_STRUCT g_gps_struct;



//---------------------------------------------------------------------------//



	#define _YES 	1
	#define _NO 	0
	
	TIZA_L218OPT_EXTERN uint8  GPRStestdata[600];
	
	
	typedef struct
	{
		uint8 usemodule;
		uint8 needlinkser;
		uint8 needopengps;
	}BUSINESS_STRUCT;
	
	
	TIZA_L218OPT_EXTERN BUSINESS_STRUCT g_business_struct;
	
	
	TIZA_L218OPT_EXTERN void ExecuteModuleTask(void);
	TIZA_L218OPT_EXTERN void BGprsResetPara(void);
	TIZA_L218OPT_EXTERN uint8 BusiSendReady(void);
	TIZA_L218OPT_EXTERN void BusiDisconGps(void);
	TIZA_L218OPT_EXTERN void BusiConGps(void);
	TIZA_L218OPT_EXTERN void BusiConNet(void);
	TIZA_L218OPT_EXTERN void BusiDisconNet(void);
	TIZA_L218OPT_EXTERN void BusiDisconSer(void);
	TIZA_L218OPT_EXTERN void BusiConSer(void);
	TIZA_L218OPT_EXTERN void BusiCheckTCPStatus(void);
	TIZA_L218OPT_EXTERN void BusiResetModule(void);
	
	TIZA_L218OPT_EXTERN void GPRStestfun(void);


		
#endif













