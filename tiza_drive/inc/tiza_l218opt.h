
#ifndef __TIZA_L218OPT_H
#define __TIZA_L218OPT_H

#ifdef TIZA_L218OPT_GLOBAL
	#define TIZA_L218OPT_EXTERN
#else
	#define TIZA_L218OPT_EXTERN extern
#endif
		
	#include <stdint.h>

//=============================================================GPRS=================GPRS=//

	
//	#define TERMINAL_ID_LEN				5			///�ն�ID����
	#define SIM_CARD_IMSI_LEN			15		///SIM��IMSI����
	#define GSM_IMEI_LEN					15		
//	#define GPRS_SEND_BUF_LEN 	512 		///<1460

	typedef struct
	{///3Gģ����ƽṹ��
		uint8   business;  	    //ҵ��
		uint8   at;    		    //����
	}GPRS_CTR_STRUCT;

	typedef struct
	{///3Gģ�����ݽṹ��
		uint8 initStatus;       //��ʼ��״̬
		uint8 netLinkStatus;    //��������״̬
		uint8 MserLinkStatus;		//��IP����״̬
		uint8 sendDataStatus;   //���ݷ���״̬
		uint8 GpsOpenStatus;   	//GPS��״̬
		
		uint8 CheckTCPStatus;		//���TCP����״̬  00��ʼ̬ 01����̬ 02�Ͽ�̬ FF��Ч
		
		uint8  IP[4];						//IP��ַ
		uint16 Port;						//�˿�
		uint8  csq_val;
		uint8  imei[SIM_CARD_IMSI_LEN];	
		uint8* SendData;												//Ҫ���͵�����
		uint16 SendDataLen; 	 									//Ҫ���͵����ݵĳ���
		uint8  setnetparaok_flag;								//��������������˱�־
	}GPRS_DATA_STRUCT;


///״̬ö��
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

//3Gģ���ʼ��״̬
typedef enum{
	GPRS_INIT_OK = 0,
	GPRS_INIT_NO,
	GPRS_INIT_ING,
}GPRS_INIT_STATUS;
//3Gģ����������״̬
typedef enum{
	GPRS_NETLINK_OK = 0,
	GPRS_NETLINK_NO,
	GPRS_NETLINK_ING,
	GPRS_NETLINK_ERR,		//�ر�ʧ��
}GPRS_NETLINK_STATUS;
//����������״̬
typedef enum{
	GPRS_SERLINK_OK = 0,
	GPRS_SERLINK_NO,
	GPRS_SERLINK_ING,
	GPRS_SERLINK_ERR,		//�ر�ʧ��
}GPRS_SERLINK_STATUS;
//���ݷ���״̬
typedef enum{
	GPRS_SENDDATA_ZERO = 0,	//��ʼ״̬
	GPRS_SENDDATA_BUSY,			//æ,���ڷ���
	GPRS_SENDDATA_IDLE,    	//�ɹ��Ϳ���
	GPRS_SENDDATA_OUT,      //Ҫ����
	GPRS_SENDDATA_ERR,     
}GPRS_SENDDATA_STATUS;

//GPSģ���״̬
typedef enum{
	GPS_OPEN_OK = 0,
	GPS_OPEN_NO,
	GPS_OPEN_ING,
	GPS_OPEN_ERR,		//�ر�ʧ��
}GPS_OPEN_STATUS;

///����
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

	#define GPS_INFORM_LEN			71	 		///GPS��ȡ��Ϣ����
	
	typedef union 
	{///GPS��ȡ��Ϣ
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
			union08 statu;			//B0=0��λ��Ч B1=0��γ B2=0����
			
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













