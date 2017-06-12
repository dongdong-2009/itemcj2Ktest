

#define TIZA_L218DRI_GLOBAL
#define TIZA_L218DRI_DEBUG
#include "tiza_include.h"


//�鿴�汾��AT+CGMR;+ISLKVRSCAN
//170328 tz1000ģ��汾+ISLKVRSCAN: L218v01.01b03.00
//+CGMR: L218v01.01b03, 2017/03/12 18:35
//GTPOS �߰汾����ʹ�ô�������ͬ��AGPS����Ҫ��
///AT ָ��
const uint8 AT_NULL[]			= {""};											///��,�ȴ�����
const uint8 AT[] 					= {"AT\x0d"};								///AT����
const uint8 AT_E0[] 			= {"ATE0\x0d"};							///�رջ���
const uint8 AT_IPR[] 			= {"AT+IPR=115200\x0d"};		///���ò�����Ϊ115200
const uint8 AT_CPIN[] 		= {"AT+CPIN?\x0d"};					///���SIM
//const uint8 AT_CPIN[] 		= {"AT+CGATT=1\x0d"};					///���SIM
const uint8 AT_ICCID[] 		= {"AT+ICCID\x0d"};					///��ȡSIM��ICCID
const uint8 AT_CSQ[] 			= {"AT+CSQ\x0d"};						///��ѯ�ź�ǿ��
const uint8 AT_CREG[] 		= {"AT+CREG?\x0d"};					///��ѯע�����
const uint8 AT_CGATT[] 		= {"AT+CGATT?\x0d"};				///GPRS������� =1

const uint8 AT_GTPOS[] 		= {"AT+GTPOS\x0d"};					///��ȡ��վ��λ��Ϣ 

// �����������							///������ ��������
const uint8 AT_CIPSTATUS[] 		= {"AT+CIPSTATUS\x0d"};           										///��ѯTCP����״̬
const uint8 AT_CIPMUX[] 			= {"AT+CIPMUX=0\x0d"};           											///������ģʽ
const uint8 AT_CSTT[] 				= {"AT+CSTT=\"CMNET\"\x0d"};           								///��������
const uint8 AT_NETOPEN[] 			= {"AT+CIICR\x0d"};                           				///������ ����PDP 0 
const uint8 AT_NETCLOSE[] 		= {"AT+CIPSHUT\x0d"};                         				///�ر�����
//const uint8 AT_CIFSR[] 				= {"AT+CIFSR\x0d"};           												///��ȡ����IP
			uint8 AT_CIPOPEN_TCP[] 	= {"AT+CIPSTART=\"TCP\",\"%u.%u.%u.%u\",\"%u\"\x0d"}; ///��TCP����
			uint8 AT_CIPSEND_TCP[] 	= {"AT+CIPSEND=%u\x0d"};           										///����TCP����
const	uint8 AT_CIPCLOSE[] 		= {"AT+CIPCLOSE\x0d"};               	   							///�ر�����
const	uint8 AT_CIPRXGET[] 		= {"AT+CIPRXGET=0\x0d"};               	   						///�Զ�����TCP���� Ĭ��

// GPS											///����GPS������GPSģ�鲨����
const uint8 AT_GETGPS[]				={"AT+GETGPS=1\x0d"};																	///�رմ�GSM���ڴ�ӡGPS��Ϣ																///�رմ�GSM���ڴ�ӡGPS��Ϣ
const uint8 AT_EGDCONT2[]			={"AT+EGDCONT=2,\"IP\",\"CMNET\"\x0d"};								///���� PDP2����			AGPS
const uint8 AT_EGDCONT1[]			={"AT+EGDCONT=1,\"IP\",\"CMNET\"\x0d"};								///���� PDP1����			AGPS
			uint8 AT_MGPSTS[]				={"AT+MGPSTS%s\x0d"};																	///��������ʱ��ͬ��		AGPS	
			uint8 AT_MGPSEPO[]			={"AT+MGPSEPO%s\x0d"};																///����EPO�ļ����غ͸���		AGPS	
			uint8 AT_EGPSC[]				={"AT+EGPSC%s\x0d"};																	///1/0  ����/�ر�GPSģ��
const	uint8 AT_PMTK314[] 			={"AT+EGPSS=\"PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\"\x0d"}; ///����GPS{"AT\x0d"};	//
const	uint8 AT_PMTK386[] 			={"AT+EGPSS=\"PMTK386,0.4*19\"\x0d"}; 								///���þ�̬�����ٶ���ֵ 0.4m/s

//FTP
//	uint8 AT_SAPBR1[] 				= {"AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\x0d"};
const uint8 AT_SAPBR[] 				= {"AT+SAPBR="};																			///-����FTP�Ļ�������������
const uint8 FTP_TYPEOFINTERCONNECT[] 	= {"3,1,\"CONTYPE\",\"GPRS\"\x0d"};
const uint8 FTP_SETAPN[] 							= {"3,1,\"APN\",\"CMNET\"\x0d"};
const uint8 FTP_OPENBEARER[] 					= {"1,1\x0d"};
const uint8 AT_FTPTYPE[]							= {"AT+FTPTYPE=\"I\"\x0d"};										///-Set the type of data:��A�� for FTP ASCII sessions,��I�� for FTP Binary sessions
const uint8 AT_FTPSERV[] 							= {"AT+FTPSERV=\"%u.%u.%u.%u\"\x0d"};			///IP
const uint8 AT_FTPPORT[]        			= {"AT+FTPPORT=%u\x0d"};											///�������˿�
const uint8 AT_FTPUN[] 								= {"AT+FTPUN=\"%s\"\x0d"};								///USER
const uint8 AT_FTPPW[] 								= {"AT+FTPPW=\"%s\"\x0d"};							///PASSWORD
const uint8 AT_FTPGETNAME[] 					= {"AT+FTPGETNAME=\"%s\"\x0d"};			///-Set the file name in FTP server
const uint8 AT_FTPGETPATH[] 					= {"AT+FTPGETPATH=\"%s\"\x0d"};								///-Set the path of file

const uint8 AT_FTPGET1[] 							= {"AT+FTPGET=1\x0d"};												///-
const uint8 AT_FTPGET2[] 							= {"AT+FTPGET="};															///-
const uint8 AT_FTPQUIT[] 							= {"AT+FTPQUIT\x0d"};													///-Quit FTP connection
const uint8 AT_FTPSIZE[] 							= {"AT+FTPSIZE\x0d"};													///-Get the size of file.




const uint8 RECV_IPD_ACK[] = "+RECEIVE,";       		// RECV DATA LEN, ADD "+IPD" HEADER
//const uint8 RECV_FROM_ACK[] = "RECV FROM:";       // RECV FORM: STRING IF SET SHOW "RECV FROM" HEADER
const uint8 OK_ACK[] 						= "OK";
const uint8 CRLF_ACK[] 					= "\x0d\x0a";
//const uint8 CIPOPEN_EXTRA_OK[]	= "CONNECT OK";		// IP IS OPENED SUCCESS
const uint8 CIPSEND_EXTRA_OK[] 	= "SEND OK";			// SEND SUCCESS
const uint8 CIPCLOSE_EXTRA_OK[] = "CLOSE OK";			// IP IS CLOSED SUCCESS
const uint8 GPRS_HAVE_RX_DATA[] = "+RECEIVE,";		// �н�������
const uint8 CONST_DATA_1[] = "1\x0d";							// �н�������
//const uint8 FTPGET_OK_ACK[] = "+FTPGET:1,1";			// Open FTP session SUCCESS
//const uint8 FTPGET_FINISH_ACK[] = "+FTPGET: 1,0";	// means finish read DATA

uint8  TEMPF_DATA_FROM_FTP[4096];
uint16 FLASH_DATA_PT = 0;

///ָ��ṹ��ʼ��
AT_CMD_STRUCT g_at_cmd_struct[] = 
{
		{(uint8 *)AT_NULL,						10,	 1*SEND_1T,	EXE_NO,	AtNullFun},
		{(uint8 *)AT, 							 100,	10*SEND_1T,	EXE_NO,	AtFun},
		{(uint8 *)AT_E0,						 100,	10*SEND_1T,	EXE_NO,	AtE0Fun},
		{(uint8 *)AT_IPR,						 100,	 3*SEND_1T,	EXE_NO,	AtIprFun},
		{(uint8 *)AT_CPIN,					 1000,	 3*SEND_1T,	EXE_NO,	AtCpinFun},
		{(uint8 *)AT_ICCID,					 100,	 3*SEND_1T,	EXE_NO,	AtIccidFun},
		{(uint8 *)AT_CSQ,					   100,	 3*SEND_1T,	EXE_NO,	AtCsqFun},
		{(uint8 *)AT_CREG,					 100,	 3*SEND_1T,	EXE_NO,	AtCregFun},
		{(uint8 *)AT_CGATT,					5000,	 3*SEND_1T,	EXE_NO,	AtCgattFun},
		{(uint8 *)AT_GTPOS,					 500,	 1*SEND_1T,	EXE_NO,	AtGtposFun},
	///����
		{(uint8 *)AT_CIPSTATUS,	   	 500,	 1*SEND_1T,	EXE_NO,	AtCipstatusFun},
		{(uint8 *)AT_CIPMUX,	    	 100,	 1*SEND_1T,	EXE_NO,	AtCipmuxFun},
		{(uint8 *)AT_CSTT,	    		 100,	 1*SEND_1T,	EXE_NO,	AtCsttFun},
		{(uint8 *)AT_NETOPEN,	   	 	 500,	 1*SEND_1T,	EXE_NO,	AtCiicrFun},
		{(uint8 *)AT_NETCLOSE,			 500,	 1*SEND_1T,	EXE_NO,	AtNetCloseFun},
		//{(uint8 *)AT_CIFSR,	         500,	 3*SEND_1T,	EXE_NO,	AtCifsrFun},
		{(uint8 *)AT_CIPOPEN_TCP,	   500,	 3*SEND_1T,	EXE_NO,	AtCipOpenTcpFun},
		{(uint8 *)AT_CIPSEND_TCP,    500,	 1*SEND_1T,	EXE_NO,	AtCipSendTcpFun},
		{(uint8 *)AT_CIPCLOSE,       100,	 1*SEND_1T,	EXE_NO,	AtCipCloseFun},
		{(uint8 *)AT_CIPRXGET,	     100,	 1*SEND_1T,	EXE_NO,	AtCipRxGetFun},
	///GPS
		{(uint8 *)AT_GETGPS,	       100,	 1*SEND_1T,	EXE_NO,	AtGetgpsFun},
		{(uint8 *)AT_EGDCONT2,	     100,	 1*SEND_1T,	EXE_NO,	AtEgdcont2Fun},
		{(uint8 *)AT_EGDCONT1,       100,	 1*SEND_1T,	EXE_NO,	AtEgdcont1Fun},
		{(uint8 *)AT_MGPSTS,         100,	 1*SEND_1T,	EXE_NO,	AtMgpstsFun},
		{(uint8 *)AT_MGPSEPO,	       100,	 1*SEND_1T,	EXE_NO,	AtMgpsepoFun},
		{(uint8 *)AT_EGPSC,	     		 100,	 1*SEND_1T,	EXE_NO,	AtEgpscFun},
		{(uint8 *)AT_PMTK314,	     	 100,	 1*SEND_1T,	EXE_NO,	AtPmtk314Fun},
		{(uint8 *)AT_PMTK386,	     	 100,	 1*SEND_1T,	EXE_NO,	AtPmtk386Fun},
	///FTP	
		{(uint8 *)AT_SAPBR,	    	   300,	 1*SEND_1T,	EXE_NO,	AtSAPBRFun},
		{(uint8 *)AT_FTPTYPE,    	   100,	 1*SEND_1T,	EXE_NO,	AtFTPTYPEFun},
		{(uint8 *)AT_FTPSERV,    	   100,	 1*SEND_1T,	EXE_NO,	AtFTPSERVFun},
		{(uint8 *)AT_FTPPORT,    	   100,	 1*SEND_1T,	EXE_NO,	AtFTPPORTFun},
		{(uint8 *)AT_FTPUN,    		   100,	 1*SEND_1T,	EXE_NO,	AtFTPUNFun},
		{(uint8 *)AT_FTPPW,    		   100,	 1*SEND_1T,	EXE_NO,	AtFTPPWFun},
		{(uint8 *)AT_FTPGETNAME,     100,	 1*SEND_1T,	EXE_NO,	AtFTPGETNAMEFun},
		{(uint8 *)AT_FTPGETPATH,     100,	 1*SEND_1T,	EXE_NO,	AtFTPGETPATHFun},
		{(uint8 *)AT_FTPGET1,    	  3000,	 1*SEND_1T,	EXE_NO,	AtFTPGET1Fun},
		{(uint8 *)AT_FTPGET2,    	  3000,	 1*SEND_1T, EXE_NO,	AtFTPGET2Fun},
		{(uint8 *)AT_FTPQUIT,    	   100,	 1*SEND_1T,	EXE_NO,	AtFTPQUITFun},
		{(uint8 *)AT_FTPSIZE,    	  1000,	 1*SEND_1T,	EXE_NO,	AtFTPSIZEFun},

		
};
void ReadOverTailIndex(uint16 len)
{
	g_gprs_rxstruct.tail = (len+g_gprs_rxstruct.tail) % GPRS_UART_RXBUF_LEN;
}
static uint32 CharFloatDataToHex(uint8 gps_data[],uint8 len,uint8 type)
{
	uint8 data[12];
	uint32 u32_val = 0;
	double d32_val = 0;
	
	if(IsValidCharFloatNum(gps_data,len) == 0){
		return 0;
	}
	memcpy(data,gps_data,len);
	data[len] = '\0';
	d32_val = atof((const char*)data);
	u32_val = d32_val * 1000000;
	
	return u32_val;
}
///ATָ�����---start
void AtNullFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag){
		g_at_cmd_struct[AT_NULL_INDEX].exe_flag = EXE_OK;
	}
	else{
		g_at_cmd_struct[AT_NULL_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);
}							
void AtFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag){
		g_at_cmd_struct[AT_INDEX].exe_flag = EXE_OK;
	}
	else{
		g_at_cmd_struct[AT_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);
}
void AtE0Fun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag){
		g_at_cmd_struct[AT_E0_INDEX].exe_flag = EXE_OK;
	}
	else{
		g_at_cmd_struct[AT_E0_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);	
}
void AtIprFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag){
		g_at_cmd_struct[AT_IPR_INDEX].exe_flag = EXE_OK;
	}
	else{
		g_at_cmd_struct[AT_IPR_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);
}							
void AtCpinFun(uint8 *data,uint16 len,uint8 flag)
{	
	// +CPIN: READY
	if(flag) {
		g_at_cmd_struct[AT_CPIN_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CPIN_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);
}
void AtIccidFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 mat_index;
	uint8 cmp_data[] = {"+ICCID: "};///+ICCID: 89860058101650410360
	
	g_at_cmd_struct[AT_ICCID_INDEX].exe_flag = EXE_FAIL;
	
	if(flag) {
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		
		if(mat_index > 0) {
			MemCpy(g_dri_ICCID, data+mat_index, 20);
			g_at_cmd_struct[AT_ICCID_INDEX].exe_flag = EXE_OK;
		}	
	}
	ReadOverTailIndex(len);
}
void AtCsqFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 res,mat_index,tmp_val;
	uint8 cmp_data[] = {"+CSQ: "};
	
	g_at_cmd_struct[AT_CSQ_INDEX].exe_flag = EXE_FAIL;
	
	if(flag) {
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		if(mat_index > 0) {
			res = FALSE;
			if(data[mat_index+1] == ',') {
				if(IsValidNum(data+mat_index,1))
				{
					tmp_val = data[mat_index] - '0';
					res = TRUE;
				}
			}
			else {
				if(IsValidNum(data+mat_index,2))
				{
					tmp_val = data[mat_index] - '0';
					tmp_val *= 10;
					tmp_val += data[mat_index+1] - '0';
					res = TRUE;
				}
			}
			
			if(res) {
				if(tmp_val <= 31) {
/*					g_gprs_data_struct.csq_val = tmp_val;*/
				}
			}
			g_at_cmd_struct[AT_CSQ_INDEX].exe_flag = EXE_OK;
		}	
	}
	ReadOverTailIndex(len);
}
void AtCregFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 mat_index;
	uint8 cmp_data[] = {"+CREG: "};
	
	g_at_cmd_struct[AT_CREG_INDEX].exe_flag = EXE_FAIL;
	
	if(flag) {
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		
		if(mat_index > 0) {
			if((data[mat_index+2] == '1')||(data[mat_index+2] == '5')) {
				g_at_cmd_struct[AT_CREG_INDEX].exe_flag = EXE_OK;
			}
		}
	}	
	ReadOverTailIndex(len);
}
void AtCgattFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 mat_index;
	uint8 cmp_data[] = {"+CGATT: "};
	
	g_at_cmd_struct[AT_CGATT_INDEX].exe_flag = EXE_FAIL;
	
	if(flag) {
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		
		if(mat_index > 0) {
			if(data[mat_index]=='1')//|| data[mat_index]=='0')
			{
				g_at_cmd_struct[AT_CGATT_INDEX].exe_flag = EXE_OK;
			}
		}
	}	
	ReadOverTailIndex(len);
}

void AtGtposFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 mat_index,dot_index[3],i,j,lat_len,long_len;
	uint8 cmp_data[] = {"+GTPOS: "};
	uint32 lat_val,long_val;
	
	g_at_cmd_struct[AT_GTPOS_INDEX].exe_flag = EXE_FAIL;
	
	if(flag) {
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);		
		if(mat_index > 0) {
			i = 0;
			for(j=mat_index; j<len; j++){
				if(data[j] == ','){
					dot_index[i++] = j;
				}
				if(i >= 2){	
					//�ѻ�û�վ��λ��Ϣ	  
					//ʹ�û�վλ�ã�֮������ʹ�û�վλ��
					lat_len  = dot_index[1] - dot_index[0] - 1;
					long_len = dot_index[0] - mat_index;
					lat_val  = CharFloatDataToHex(&data[dot_index[0]+1], lat_len,  LAT_TYPE );
					long_val = CharFloatDataToHex(&data[mat_index],      long_len, LONG_TYPE);
					if(long_val){
						g_gps_struct.gpsinform.subitem.latitude  = lat_val;
						g_gps_struct.gpsinform.subitem.longitude = long_val;
					}
					
					g_at_cmd_struct[AT_GTPOS_INDEX].exe_flag = EXE_OK;
					break;
				}
			}
		}
		else if(g_gps_struct.gpsinform.subitem.statu.bit.B0 == 0){
			//�ڻ�ȡ��վλ��֮ǰ�Ѿ���λ���óɹ���־ ���ڻ�ȡ��վλ��
			g_at_cmd_struct[AT_GTPOS_INDEX].exe_flag = EXE_OK;
		}
	}	
	ReadOverTailIndex(len);
}
			
/////���������غ���
void AtCipstatusFun(uint8 *data,uint16 len,uint8 flag)
{
	//STATE: IP GPRSACT
	//STATE: CONNECT OK
	//STATE: TCP CLOSE
	
	if(flag) {
/*		if(SubMatch("IP GPRSACT", 10,data,len)){
			g_gprs_data_struct.CheckTCPStatus = 0;			
		}
		else if(SubMatch("CONNECT OK", 10,data,len)){
			g_gprs_data_struct.CheckTCPStatus = 1;			
		}
		else if(SubMatch("TCP CLOSE", 9,data,len)){
			g_gprs_data_struct.CheckTCPStatus = 2;			
		}
		else{
			#ifdef L218_DEBUG
				LocalUartFixedLenSend((uint8*)"Cipstatus no SubMatch\r\n",23);
			#endif
		}*/
		g_at_cmd_struct[AT_CIPSTATUS_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CIPSTATUS_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtCipmuxFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CIPMUX_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CIPMUX_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtCsttFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CSTT_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CSTT_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtCiicrFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CIICR_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CIICR_INDEX].exe_flag = EXE_FAIL;
		L218Reset();
	}	
	ReadOverTailIndex(len);
}
void AtNetCloseFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_NETCLOSE_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_NETCLOSE_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
//void AtCifsrFun(uint8 *data,uint16 len,uint8 flag)
//{
//	if(flag) {
//		g_at_cmd_struct[AT_CIFSR_INDEX].exe_flag = EXE_OK;
//	}
//	else {
//		g_at_cmd_struct[AT_CIFSR_INDEX].exe_flag = EXE_FAIL;
//	}	
//	ReadOverTailIndex(len);
//}
void AtCipOpenTcpFun(uint8 *data,uint16 len,uint8 flag)
{
	//�ɴ�data��һ��У��CONNECT OK  �� ALREADY CONNECT
	char Aleaddy_open[] = {"ALREADY"};
	
	if(flag) {
		g_at_cmd_struct[AT_CIPOPEN_TCP_INDEX].exe_flag = EXE_OK;
	}
	else {
		if(SubMatch((uint8 *)Aleaddy_open,7,data,len) > 0){
			g_at_cmd_struct[AT_CIPOPEN_TCP_INDEX].exe_flag = EXE_OK;
		}
		g_at_cmd_struct[AT_CIPOPEN_TCP_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtCipSendTcpFun(uint8 *data,uint16 len,uint8 flag)
{
	#ifdef L218_DEBUG
		char str_ch[10];
		uint8 str_len;
		uint16 i;
	#endif	
	
	if(flag) {
		g_at_cmd_struct[AT_CIPSEND_TCP_INDEX].exe_flag = EXE_OK;
	}
	else {
		#ifdef L218_DEBUG
			LocalUartFixedLenSend((uint8*)"AT_CIPSEND_TCP_INDEX = fail \r\n", 30);
			for(i=0;i<len;i++){
				str_len = sprintf(str_ch, "%.2X ", data[i]);
				LocalUartFixedLenSend((uint8*)str_ch,str_len);
			}	
			str_len = sprintf(str_ch, "i=%d\r\n", i);
			LocalUartFixedLenSend((uint8*)str_ch,str_len);
		#endif
		g_at_cmd_struct[AT_CIPSEND_TCP_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtCipCloseFun(uint8 *data, uint16 len, uint8 flag)
{    
	if(flag) {
		g_at_cmd_struct[AT_CIPCLOSE_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CIPCLOSE_INDEX].exe_flag = EXE_FAIL;
	}	    
	ReadOverTailIndex(len);
}
void AtCipRxGetFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CIPRXGET_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CIPRXGET_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
////GPS��ػص�����
void AtGetgpsFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_GETGPS_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_GETGPS_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtEgdcont2Fun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_EGDCONT2_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_EGDCONT2_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtEgdcont1Fun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_EGDCONT1_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_EGDCONT1_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtMgpstsFun(uint8 *data,uint16 len,uint8 flag)
{
	char temp_ack[9]={"+MGPSTS: "};
	uint16 mat_index;
	
	mat_index = SubMatch((uint8 *)temp_ack,9,data,len);
	if(mat_index > 0){//  ?
		if(flag){
			if(data[mat_index] == '1'){
				g_at_cmd_struct[AT_MGPSTS_INDEX].exe_flag = EXE_03_ON;
			}
			else if(data[mat_index] == '0'){
				g_at_cmd_struct[AT_MGPSTS_INDEX].exe_flag = EXE_04_OFF;
			}
		}
		else{
			g_at_cmd_struct[AT_MGPSTS_INDEX].exe_flag = EXE_FAIL;
		}
	}
	else{//   1/0
		if(flag) {
			g_at_cmd_struct[AT_MGPSTS_INDEX].exe_flag = EXE_OK;
		}
		else {
			g_at_cmd_struct[AT_MGPSTS_INDEX].exe_flag = EXE_FAIL;
		}	
	}
	ReadOverTailIndex(len);
}
void AtMgpsepoFun(uint8 *data,uint16 len,uint8 flag)
{
	char temp_ack[10]={"+MGPSEPO: "};
	uint16 mat_index;
	
	mat_index = SubMatch((uint8 *)temp_ack,10,data,len);
	if(mat_index > 0){//  ?
		if(flag){
			if(data[mat_index] == '1'){
				g_at_cmd_struct[AT_MGPSEPO_INDEX].exe_flag = EXE_03_ON;
			}
			else if(data[mat_index] == '0'){
				g_at_cmd_struct[AT_MGPSEPO_INDEX].exe_flag = EXE_04_OFF;
			}
		}
		else{
			g_at_cmd_struct[AT_MGPSEPO_INDEX].exe_flag = EXE_FAIL;
		}
	}
	else{//   1/0
		if(flag) {
			g_at_cmd_struct[AT_MGPSEPO_INDEX].exe_flag = EXE_OK;
		}
		else {
			g_at_cmd_struct[AT_MGPSEPO_INDEX].exe_flag = EXE_FAIL;
		}	
	}
	ReadOverTailIndex(len);
}

void AtEgpscFun(uint8 *data,uint16 len,uint8 flag)
{
	char temp_ack[8]={"+EGPSC: "};
	uint16 mat_index;
	
	mat_index = SubMatch((uint8 *)temp_ack,8,data,len);
	if(mat_index > 0){//  ?
		if(flag){
			if(data[mat_index] == '1'){
				g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag = EXE_03_ON;
			}
			else if(data[mat_index] == '0'){
				g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag = EXE_04_OFF;
			}
		}
		else{
			g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag = EXE_FAIL;
		}
	}
	else{//   1/0
		if(flag) {
			g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag = EXE_OK;
		}
		else {
			g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag = EXE_FAIL;
		}	
	}
	ReadOverTailIndex(len);
}
void AtPmtk314Fun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_PMTK314_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_PMTK314_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtPmtk386Fun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_PMTK386_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_PMTK386_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
////FTP
void AtSAPBRFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_SAPBR_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_SAPBR_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPTYPEFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPTYPE_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPTYPE_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPSERVFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPSERV_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPSERV_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPPORTFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPPORT_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPPORT_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPUNFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPUN_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPUN_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPPWFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPPW_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPPW_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPGETNAMEFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPGETNAME_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPGETNAME_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPGETPATHFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPGETPATH_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPGETPATH_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPGET1Fun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPGET1_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPGET1_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}


void AtFTPGET2Fun(uint8 *data,uint16 len,uint8 flag)
{	
	/*
  uint8  i = 0,j = 0,cmp_data[20];// = {"+FTPGET: 2,"},cmp_data3[20] = {"+FTPGET:1,1"};
	uint8  ack_flag = FALSE,rrx_data[L218_MAX_BUF_LEN];
	uint16 flash_len = 0,mat_index = 0,rx_len = 0,source_head = 0,end_head = 0,time;
	
	cmp_data[i++] = '+';
	cmp_data[i++] = 'F';
	cmp_data[i++] = 'T';
	cmp_data[i++] = 'P';
	cmp_data[i++] = 'G';
	cmp_data[i++] = 'E';
	cmp_data[i++] = 'T';
	cmp_data[i++] = ':';
	cmp_data[i++] = ' ';	//8
	cmp_data[i++] = '2';
	cmp_data[i++] = ',';
	cmp_data[i++] = 0;
	i = 0;
	
	g_at_cmd_struct[AT_FTPGET2_INDEX].exe_flag = EXE_FAIL;

	mat_index = SubMatch(cmp_data, 11, data, len);	//-��ȡ�ֽڳ�������,���ұ����и��Ӵ���
	if(mat_index > 0)
	{
		while((data[mat_index + i] >= 0x30)&&(data[mat_index + i] <= 0x39))
			i++;
		while(j < i)
		{
			flash_len *= 10;
			flash_len += (data[mat_index + j] - 0x30);
			j++;
		}
		if(flash_len == 0) 
			goto COMPARA_LAB;
		source_head = mat_index + i + 2;	//-ָ�����ݿ�ʼ�ĵط�
		end_head = source_head + flash_len + 2 + 6;	//-ָ����+FTPGET:1,�Ŀ�ʼλ��
		if((end_head > L218_MAX_BUF_LEN) || (flash_len > 1024))
		{
			ReadOverTailIndex(len);
			return;	
		}	
			
		if(len < end_head + 11)	//-˵��û�н���ȫ��Ҫ����ֱ�����ս���(+FTPGET:1,1)
		{
			for(time=0;time<g_at_cmd_struct[AT_FTPGET2_INDEX].max_ms_wait;time++)
			{
				OSTimeDlyHMSM(0, 0, 0, 100);
				rx_len = L218UartIsRxDone(rrx_data,L218_MAX_BUF_LEN);
				if(rx_len > end_head + 11)
				{
					ack_flag = TRUE;
					data = rrx_data;
					break;
				}
			}
			if(!ack_flag)
			{
				ReadOverTailIndex(rx_len);
				return;
			}
		}

		MemCpy(TEMPF_DATA_FROM_FTP + ftp_struct.ftp_rx_len,data + source_head,flash_len);
		ftp_struct.ftp_rx_file_byte_counter += flash_len;
		ftp_struct.ftp_rx_len += flash_len;

//printf("\r\n boot_struct.program_total_size = %d ftp_struct.ftp_rx_file_byte_counter = %d ftp_struct.ftp_rx_len = %d\r\n",boot_struct.program_total_size,ftp_struct.ftp_rx_file_byte_counter,ftp_struct.ftp_rx_len);
		if(ftp_struct.ftp_rx_len >= 2048)
		{
			CpuFlashWrite(FTP_BLIND_FLASH_START+((ftp_struct.ftp_rx_file_byte_counter / 2048) - 1) * 2048,TEMPF_DATA_FROM_FTP, 2048);
			ftp_struct.ftp_rx_len -= 2048;
			if(ftp_struct.ftp_rx_len)
				MemCpy(TEMPF_DATA_FROM_FTP,TEMPF_DATA_FROM_FTP + 2048,ftp_struct.ftp_rx_len);
		}

		if(ftp_struct.ftp_rx_file_byte_counter == boot_struct.program_total_size)	//-�ж��Ǽ������ǽ���
		{//-�������ӽ������ж�
			cmp_data[8] = ' ';cmp_data[9] = '1';cmp_data[10] = ',';cmp_data[11] = '0';cmp_data[12] = 0;
			if(SubMatch(cmp_data, 12, data + end_head, len - end_head) > 0){
				g_at_cmd_struct[AT_FTPGET2_INDEX].exe_flag = EXE_OK;
				if(ftp_struct.ftp_rx_len){
					CpuFlashWrite(FTP_BLIND_FLASH_START+(ftp_struct.ftp_rx_file_byte_counter / 2048) * 2048,TEMPF_DATA_FROM_FTP, ftp_struct.ftp_rx_len);
					ftp_struct.ftp_rx_len = 0;
					ftp_struct.ftp_rx_file_byte_counter = 0;
				}
			}
		}
		else if(ftp_struct.ftp_rx_file_byte_counter > boot_struct.program_total_size)	//-����
		{//-
			ReadOverTailIndex(rx_len);
			return;
		}
		else
		{
			cmp_data[8] = '1';cmp_data[9] = ',';cmp_data[10] = '1';cmp_data[11] = 0;
			mat_index = SubMatch(cmp_data, 11, data + end_head, len - end_head);	//-��֤��β�����������ݲ���
			if(mat_index > 0){
COMPARA_LAB:				
				g_at_cmd_struct[AT_FTPGET2_INDEX].exe_flag = EXE_CONTINUE;	//-��Ҫ���ӳ�ʱ�˳�
			}
		}
		
	}
	*/
	ReadOverTailIndex(len);
}

void AtFTPQUITFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPQUIT_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPQUIT_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);
}

void AtFTPSIZEFun(uint8 *data,uint16 len,uint8 flag)
{
/*	uint8 mat_index,i = 0,j = 0;
	uint8 cmp_data[] = {"+FTPSIZE: 1,0,"};
	uint32 temp_len = 0;
		
	g_at_cmd_struct[AT_FTPSIZE_INDEX].exe_flag = EXE_FAIL;
	
	if(flag) {
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		
		if(mat_index > 0) {
			while((data[mat_index + i] >= 0x30)&&(data[mat_index + i] <= 0x39))
				i++;
			while(j < i)
			{
				temp_len *= 10;
				temp_len += (data[mat_index + j] - 0x30);
				j++;
			}
			boot_struct.program_total_size = temp_len;			
			g_at_cmd_struct[AT_FTPSIZE_INDEX].exe_flag = EXE_OK;
		}
	}	
	*/
	ReadOverTailIndex(len);
}
///ATָ�����---end

//--------------------------------------------------------------------------------------------------------------------------------//
/******************************************************
//ATָ���
******************************************************/
//                    ATָ������      ��������      �������ݳ���     ƥ������      ƥ�����ݳ���		�Ƿ�(1/0)���0D0A 
void L218SendAtCmd(uint8 cmd_index,uint8 app_data[],uint8 app_len,uint8 mat_data[],uint8 mat_len, uint8 flag)
{
	uint8 count,ack_flag = FALSE,rx_data[L218_MAX_BUF_LEN];
	uint16 rx_len = 0,mat_index = 0,mat_0d0a = 0,time;
  uint8  i = 0,cmp_data[20];// = {"+FTPGET: 1,0"};
	cmp_data[i++] = '+';
	cmp_data[i++] = 'F';
	cmp_data[i++] = 'T';
	cmp_data[i++] = 'P';
	cmp_data[i++] = 'G';
	cmp_data[i++] = 'E';
	cmp_data[i++] = 'T';
	cmp_data[i++] = ':';
	cmp_data[i++] = ' ';	//8
	cmp_data[i++] = '1';	
	cmp_data[i++] = ',';
	cmp_data[i++] = '0';
	cmp_data[i++] = 0;
	
	g_at_cmd_struct[cmd_index].exe_flag = EXE_NO;
	//ָ��ʹ���
	for(count=0; count<g_at_cmd_struct[cmd_index].tx_max_count; count++)
	{
		//����ָ��
		L218_GPRSUART_Send(g_at_cmd_struct[cmd_index].cmd_text,StrLen(g_at_cmd_struct[cmd_index].cmd_text,0));
		if(app_len > 0){
			L218_GPRSUART_Send(app_data,app_len);
		}		
		L218_GPRSUARTSend_Ctr(1);
		#ifdef TIZA_L218DRI_DEBUG
			if(count == 0){
				DebugSendDatas(g_at_cmd_struct[cmd_index].cmd_text,StrLen(g_at_cmd_struct[cmd_index].cmd_text,0));
				if(app_len > 0){
					DebugSendDatas(app_data,app_len);
				}
				DealDebugSend(1);
			}
		#endif
		//��ʱ�ȴ�Ӧ��
		for(time=0;time<g_at_cmd_struct[cmd_index].max_ms_wait;time++)
		{
			OSTimeDlyHMSM(0, 0, 0, WAIT_10MS);
			rx_len = L218UartIsRxDone(rx_data,L218_MAX_BUF_LEN);
			if(rx_len > 0)
			{
				mat_index = SubMatch(mat_data,mat_len,rx_data,rx_len);
				mat_0d0a  = SubMatch((uint8*)"\x0d\x0a", 2,&rx_data[mat_index],rx_len-mat_index);
				if(mat_index>0 && (mat_0d0a>0 || flag==0)){
					#ifdef TIZA_L218DRI_DEBUG
						DebugSendDatas(rx_data,rx_len);
						DealDebugSend(1);
					#endif
					ack_flag = TRUE;
					break;
				}
	/*			else if(cmd_index == AT_FTPGET2_INDEX){
					mat_index = SubMatch(cmp_data, 12, rx_data, rx_len);
					mat_0d0a  = SubMatch((uint8*)"\x0d\x0a", 2,&rx_data[mat_index],rx_len-mat_index);
					if(mat_index>0 && mat_0d0a>0){
						if((ftp_struct.ftp_rx_file_byte_counter+rx_len) >= boot_struct.program_total_size){
						#ifdef L218_DEBUG
							LocalUartFixedLenSend((uint8*)"FTP DOWNLOAD FILE END\r\n", 23);
						#endif
						ack_flag = TRUE;
						break;
						}
					}
					else {
						cmp_data[9] = '2';
						if(SubMatch(cmp_data, 12, rx_data, rx_len) > 0){
							break;
						}
						
						cmp_data[9] = '1';
					}
				}
				*/
			}
		}
		if(ack_flag){
			break;
		}
	}
	
	(g_at_cmd_struct[cmd_index].fun)(rx_data,rx_len,ack_flag);
}

uint16 L218UartIsRxDone(uint8 data[],uint16 len)
{
	uint16 count=0,index;
	
	#if GPRSUART_Mode_ITorDMA == 1
	g_gprs_rxstruct.head = (GPRS_UART_RXBUF_LEN - DMA_GetCurrDataCounter(DMA1_Stream1)) % GPRS_UART_RXBUF_LEN;
	#endif
	if(g_gprs_rxstruct.head != g_gprs_rxstruct.tail)
	{//������
		count = (GPRS_UART_RXBUF_LEN + g_gprs_rxstruct.head - g_gprs_rxstruct.tail) % GPRS_UART_RXBUF_LEN;
		if(count > len){//����ָ��Խ��
			return 0;
		}
		for(index=0; index < count; index++){
			data[index] = g_gprs_rxstruct.buf[((index+g_gprs_rxstruct.tail) % GPRS_UART_RXBUF_LEN)];
		}
		
	}
	return count;
}

/******************************************************
//DMA ���ú���
//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:�����ַ
//mar:�洢����ַ
//ndtr:���ݴ�����  
******************************************************/
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

/******************************************************
//������->�洢��ģʽ
******************************************************/
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

/******************************************************
//����һ��DMA����
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:���ݴ�����  
******************************************************/
static  void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	DMA_Cmd(DMA_Streamx, DISABLE);                      //�ر�DMA���� 	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����  		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          	//���ݴ�����   
	DMA_Cmd(DMA_Streamx, ENABLE);                      	//����DMA���� 
}	  
/******************************************************
//ģ��GPRS���ڳ�ʼ��
******************************************************/
void L218_GPRSUART_Init(uint32 bound)
{	
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
#if GPRSUART_Mode_ITorDMA == 0	
	NVIC_InitTypeDef NVIC_InitStructure;
#endif	

	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);				//�ر�����ж�
	USART_DeInit(USART3);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 				//ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);				//ʹ��USART3ʱ��
 
	//����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); 		//GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 		//GPIOB11����ΪUSART3	
	//USART3�˿�����
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10 | GPIO_Pin_11; 	//GPIOB10��GPIOB11
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;								//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 							//���츴�����
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP; 							//����
	GPIO_Init(GPIOB,&GPIO_InitStructure); 											//��ʼ��PB10��PB11
  //USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate 		= bound;							//����������
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;		//һ��ֹͣλ
	USART_InitStructure.USART_Parity 			= USART_Parity_No;		//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode 				= USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); 									//��ʼ������3
	
	USART_Cmd(USART3, ENABLE); 																	//ʹ�ܴ���3
	USART_ClearFlag(USART3, USART_FLAG_TC);

#if GPRSUART_Mode_ITorDMA == 0
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);															//��������ж�
	//Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel 									= USART3_IRQn;					//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;											//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			=1;											//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd	 							= ENABLE;								//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
#else
	//DMA1,STEAM3,CH4,����Ϊ����3,GprsDMA_SendBuff,����Ϊ:GPRS_DMA_SENDBUF_LEN.
	MYDMA_Config(DMA1_Stream3,DMA_Channel_4,(u32)&USART3->DR,(u32)GprsDMA_SendBuff,GPRS_DMA_SENDBUF_LEN);
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);  			//ʹ�ܴ���3��DMA���� 
	MYDMA_Enable(DMA1_Stream3,1);     									//��ʼһ��DMA���䣡	
	//DMA1,STEAM1,CH4,����Ϊ����3,g_gprs_rxbuff,����Ϊ:GPRS_UART_RXBUF_LEN.
	MYDMA_Config_Circular(DMA1_Stream1,DMA_Channel_4,(u32)&USART3->DR,(u32)g_gprs_rxbuff,GPRS_UART_RXBUF_LEN);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);  			//ʹ�ܴ���3��DMA���� 
	DMA_Cmd(DMA1_Stream1, ENABLE);                      //����DMA���� 
#endif
////////////////////////////////////////////////////////////////////////////////
	g_gprs_txstruct.buf 	= g_gprs_txbuff;
	g_gprs_txstruct.head 	= 0;
	g_gprs_txstruct.tail 	= 0;
	g_gprs_rxstruct.buf 	= g_gprs_rxbuff;
	g_gprs_rxstruct.head 	= 0;
	g_gprs_rxstruct.tail 	= 0;
////////////////////////////////////////////////////////////////////////////////	
}

/******************************************************
//ģ��GPRS���� �رպ���
******************************************************/
void L218_GPRSUART_Close(void)
{
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);					//�ر�����ж�
	USART_DeInit(USART3);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3, DISABLE);	//�ر�USART3ʱ��
}
/******************************************************
//ģ��GPRS���� ���ͻ�ѹbuff����
******************************************************/
void L218_GPRSUART_Send(uint8 data[],uint16 len) 
{
	uint16 i;
	
	if(len ==0 || data == NULL) return ;
	
#if GPRSUART_Mode_ITorDMA == 0	
	for(i=0;i<len;i++){
		USART_SendData(USART3, data[i]);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
	}	
	
#else		
	for(i=0; i < len; i++){
		g_gprs_txstruct.buf[g_gprs_txstruct.head] = data[i];
		g_gprs_txstruct.head = (g_gprs_txstruct.head+1) % GPRS_UART_TXBUF_LEN;		
		if(g_gprs_txstruct.head == g_gprs_txstruct.tail)
		{//buff����βҲ��ǰ�ƶ�
			g_gprs_txstruct.tail = (g_gprs_txstruct.tail+1) % GPRS_UART_TXBUF_LEN;
		}
	}
#endif	
}

/******************************************************
//ģ��GPRS���� DMA���Ϳ���
//-0 ����������,1 ��������
******************************************************/
void L218_GPRSUARTSend_Ctr(uint8 flag)
{
#if GPRSUART_Mode_ITorDMA == 1	
	uint16 count, i;
	
	if(g_gprs_txstruct.head == g_gprs_txstruct.tail) 		{		return;		}			//û�з�������

	if(flag){
		while(1){
			if(DMA_GetFlagStatus(DMA1_Stream3,DMA_FLAG_TCIF3)!=RESET)	{					//�ȴ� DMA1_Stream3 �������
				DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);												//��� DMA1_Stream3 ������ɱ�־ 
				break;
			}
		}
	}
	else{
		if(DMA_GetFlagStatus(DMA1_Stream3,DMA_FLAG_TCIF3)!=RESET)	{					//�ȴ� DMA1_Stream3 �������
			DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);												//��� DMA1_Stream3 ������ɱ�־ 
		}
		else
			return;
	}	
	
	count = (GPRS_UART_TXBUF_LEN + g_gprs_txstruct.head - g_gprs_txstruct.tail) % GPRS_UART_TXBUF_LEN;
	for(i=0; i < count; i++){
		GprsDMA_SendBuff[i] = g_gprs_txstruct.buf[g_gprs_txstruct.tail];
		g_gprs_txstruct.tail = (g_gprs_txstruct.tail+1) % GPRS_UART_TXBUF_LEN;
	}
	MYDMA_Enable(DMA1_Stream3, count);     																//��ʼһ��DMA���䣡	 

#endif	
}

/******************************************************
//ģ��GPRS����3�жϷ������
******************************************************/
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)  
	{
		#if GPRSUART_Mode_ITorDMA == 0
		g_gprs_rxstruct.buf[g_gprs_rxstruct.head] = USART_ReceiveData(USART3);	//��ȡ���յ�������
		g_gprs_rxstruct.head = (g_gprs_rxstruct.head+1) % GPRS_UART_RXBUF_LEN;		
		if(g_gprs_rxstruct.head == g_gprs_rxstruct.tail)
		{//buff����βҲ��ǰ�ƶ�
			g_gprs_rxstruct.tail = (g_gprs_rxstruct.tail+1) % GPRS_UART_RXBUF_LEN;
		}
		#endif
	}
}

/******************************************************
//ģ��GPS���ڳ�ʼ��
//ģ��û�з���
******************************************************/
void L218_GPSUART_Init(uint32 bound){
	
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
#if GPSUART_Mode_ITorDMA == 0	
	NVIC_InitTypeDef NVIC_InitStructure;
#endif	

	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);				//�ر�����ж�
	USART_DeInit(UART4);
	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 			//ʹ��GPIAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);				//ʹ��UART2ʱ��
 
	//����4��Ӧ���Ÿ���ӳ��
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_USART4); 		//GPIOA0����ΪUART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4); 		//GPIOA1����ΪUART2	
	//UART4�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 									//GGPIOA1
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 		//GPIOA0��GPIOA1
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;								//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 							//���츴�����
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP; 							//����
	GPIO_Init(GPIOA,&GPIO_InitStructure); 											//��ʼ��PA0��PA1
  //UART4 ��ʼ������
	USART_InitStructure.USART_BaudRate 		= bound;							//����������
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;		//һ��ֹͣλ
	USART_InitStructure.USART_Parity 			= USART_Parity_No;		//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode 				= USART_Mode_Rx ;			//�շ�ģʽ
//	USART_InitStructure.USART_Mode 				= USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(UART4, &USART_InitStructure); 									//��ʼ������4
	
	USART_Cmd(UART4, ENABLE); 																	//ʹ�ܴ���4
	USART_ClearFlag(UART4, USART_FLAG_TC);

#if GPSUART_Mode_ITorDMA == 0
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);																//��������ж�
	//Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel 									= USART4_IRQHandler;		//����4�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;											//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			=3;											//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd	 							= ENABLE;								//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
#else
	//DMA1,STEAM5,CH4,����Ϊ����3,g_gps_rxbuff,����Ϊ:GPS_UART_RXBUF_LEN.
	MYDMA_Config_Circular(DMA1_Stream2,DMA_Channel_4,(u32)&UART4->DR,(u32)g_gps_rxbuff,GPS_UART_RXBUF_LEN);
	USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);  			//ʹ�ܴ���3��DMA����
	DMA_Cmd(DMA1_Stream2, ENABLE);                      //����DMA���� 
#endif
////////////////////////////////////////////////////////////////////////////////
	g_gps_txstruct.buf 		= g_gps_txbuff;
	g_gps_txstruct.head 	= 0;
	g_gps_txstruct.tail 	= 0;
	g_gps_rxstruct.buf 		= g_gps_rxbuff;
	g_gps_rxstruct.head 	= 0;
	g_gps_rxstruct.tail 	= 0;
////////////////////////////////////////////////////////////////////////////////	
}
/******************************************************
//ģ��GPS����4�жϷ������
******************************************************/
void USART4_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)  
	{
		#if L218GPSUART_Mode_ITorDMA == 0
		g_gps_rxstruct.buf[g_gps_rxstruct.head] = USART_ReceiveData(USART3);	//��ȡ���յ�������
		g_gps_rxstruct.head = (g_gps_rxstruct.head+1) % GPS_UART_RXBUF_LEN;		
		if(g_gps_rxstruct.head == g_gps_rxstruct.tail)
		{//buff����βҲ��ǰ�ƶ�
			g_gps_rxstruct.tail = (g_gps_rxstruct.tail+1) % GPS_UART_RXBUF_LEN;
		}
		#endif
	}
}
/******************************************************
//ģ��GPS���� �رպ���
******************************************************/
void L218_GPSUART_Close(void)
{
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);					//�ر�����ж�
	USART_DeInit(UART4);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_UART4, DISABLE);	//�ر�USART4ʱ��
}

/******************************************************
//ģ���ʼ��
******************************************************/
void L218_Init(void)
{
	L218_GPSUART_Init(GPS_USART_BPR);
//	L218_GPRSUART_Init(GPRS_USART_BPR);
	
	HIGH_GPRS_DTR();

	L218Reset();
}
/******************************************************
//ģ�鸴λ&������
******************************************************/
void L218Reset(void)
{
	uint8 i;
	uint32 bpr[3] = {115200,38400,9600};
	
	HIGH_GPRS_IGT();
	OFF_GPRS_PWR();///�ȹػ�
	OFF_REST_L218();
	OSTimeDlyHMSM(0, 0, 6, 0);
	ON_GPRS_PWR(); ///�ٿ���
	ON_REST_L218();
	OSTimeDlyHMSM(0, 0, 2, 500);
	LOW_GPRS_IGT();
	OSTimeDlyHMSM(0, 0, 2, 0);
	HIGH_GPRS_IGT();
	OSTimeDlyHMSM(0, 0, 2, 500);
	
//ģ���Զ���ȡ������	
	for(i=0;i<3;i++){
		L218_GPRSUART_Init(bpr[i]);
		L218SendAtCmd(AT_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);
		if(g_at_cmd_struct[AT_INDEX].exe_flag == EXE_OK){
			break;
		}
	}
	
	if(i == 3){//  AT����Ӧ��������
		DPrint("AT no response,wo will restart terminal now!!\n");
		DealDebugSend(1);	
		SysReset();						//����ä��&��λ
	}
	if(bpr[i] != GPRS_USART_BPR){
		L218SendAtCmd(AT_IPR_INDEX,NULL,0,(uint8 *)OK_ACK,2, 1);///����ģ��Ĳ�����Ϊ115200
		
		L218_GPRSUART_Init(GPRS_USART_BPR);
	}
}

