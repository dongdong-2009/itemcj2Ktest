#ifndef __APP_CAN_H
#define __APP_CAN_H

#include "stdint.h"

	#ifdef APP_CAN_GLOBAL
		#define EXTN_APP_CAN
	#else
		#define EXTN_APP_CAN extern
	#endif

	
///����CANID
///�����ڹ����˲������	
	#define CAN1_R_0101 0x1801D1F3		//BMS�����ѹ	1~60
	#define CAN1_R_0102 0x1802D1F3		
	#define CAN1_R_0103 0x1803D1F3		
	#define CAN1_R_0104 0x1804D1F3		
	#define CAN1_R_0105 0x1805D1F3		
	#define CAN1_R_0106 0x1806D1F3		
	#define CAN1_R_0107 0x1807D1F3		
	#define CAN1_R_0108 0x1808D1F3		
	#define CAN1_R_0109 0x1809D1F3		
	#define CAN1_R_0110 0x180AD1F3		
	#define CAN1_R_0111 0x180BD1F3		
	#define CAN1_R_0112 0x180CD1F3		
	#define CAN1_R_0113 0x180DD1F3		
	#define CAN1_R_0114 0x180ED1F3			
	#define CAN1_R_0115 0x180FD1F3		
	#define CAN1_R_0116 0x1810D1F3		
	#define CAN1_R_0117 0x1811D1F3		
	#define CAN1_R_0118 0x1812D1F3		
	#define CAN1_R_0119 0x1813D1F3		
	#define CAN1_R_0120 0x1814D1F3		
	#define CAN1_R_0121 0x1815D1F3		
	#define CAN1_R_0122 0x1816D1F3		
	#define CAN1_R_0123 0x1817D1F3		
	#define CAN1_R_0124 0x1818D1F3		//1~24			��ʱ24
	#define CAN1_R_0125 0x1819D1F3		
	#define CAN1_R_0126 0x181AD1F3		
	#define CAN1_R_0127 0x181BD1F3		
	#define CAN1_R_0128 0x181CD1F3		
	#define CAN1_R_0129 0x181DD1F3			
	#define CAN1_R_0130 0x181ED1F3		
	#define CAN1_R_0131 0x181FD1F3		
	#define CAN1_R_0132 0x1820D1F3		
	#define CAN1_R_0133 0x1821D1F3		
	#define CAN1_R_0134 0x1822D1F3			
	#define CAN1_R_0135 0x1823D1F3		
	#define CAN1_R_0136 0x1824D1F3		
	#define CAN1_R_0137 0x1825D1F3		
	#define CAN1_R_0138 0x1826D1F3		
	#define CAN1_R_0139 0x1827D1F3		
	#define CAN1_R_0140 0x1828D1F3		
	#define CAN1_R_0141 0x1829D1F3		
	#define CAN1_R_0142 0x182AD1F3		
	#define CAN1_R_0143 0x182BD1F3		
	#define CAN1_R_0144 0x182CD1F3			
	#define CAN1_R_0145 0x182DD1F3		
	#define CAN1_R_0146 0x182ED1F3		
	#define CAN1_R_0147 0x182FD1F3		
	#define CAN1_R_0148 0x1830D1F3		
	#define CAN1_R_0149 0x1831D1F3		
	#define CAN1_R_0150 0x1832D1F3		
	#define CAN1_R_0151 0x1833D1F3		
	#define CAN1_R_0152 0x1834D1F3		
	#define CAN1_R_0153 0x1835D1F3		
	#define CAN1_R_0154 0x1836D1F3			
	#define CAN1_R_0155 0x1837D1F3		
	#define CAN1_R_0156 0x1838D1F3		
	#define CAN1_R_0157 0x1839D1F3		
	#define CAN1_R_0158 0x183AD1F3		
	#define CAN1_R_0159 0x183BD1F3		
	#define CAN1_R_0160 0x183CD1F3		//BMS�����ѹ	1~60		
		
	#define CAN1_R_0201 0x183DD1F3		//BMS ģ����	1~30	
	#define CAN1_R_0202 0x183ED1F3		
	#define CAN1_R_0203 0x183FD1F3		//1~3		��ʱ3
	#define CAN1_R_0204 0x1840D1F3		
	#define CAN1_R_0205 0x1841D1F3		
	#define CAN1_R_0206 0x1842D1F3			
	#define CAN1_R_0207 0x1843D1F3		
	#define CAN1_R_0208 0x1844D1F3		
	#define CAN1_R_0209 0x1845D1F3		
	#define CAN1_R_0210 0x1846D1F3		
	#define CAN1_R_0211 0x1847D1F3		
	#define CAN1_R_0212 0x1848D1F3		
	#define CAN1_R_0213 0x1849D1F3		
	#define CAN1_R_0214 0x184AD1F3		
	#define CAN1_R_0215 0x184BD1F3		
	#define CAN1_R_0216 0x184CD1F3			
	#define CAN1_R_0217 0x184DD1F3		
	#define CAN1_R_0218 0x184ED1F3		
	#define CAN1_R_0219 0x184FD1F3		
	#define CAN1_R_0220 0x1850D1F3		
	#define CAN1_R_0221 0x1851D1F3		
	#define CAN1_R_0222 0x1852D1F3		
	#define CAN1_R_0223 0x1853D1F3		
	#define CAN1_R_0224 0x1854D1F3		
	#define CAN1_R_0225 0x1855D1F3		
	#define CAN1_R_0226 0x1856D1F3			
	#define CAN1_R_0227 0x1857D1F3		
	#define CAN1_R_0228 0x1858D1F3		
	#define CAN1_R_0229 0x1859D1F3		
	#define CAN1_R_0230 0x185AD1F3		//BMS ģ����	1~30	
	
	#define CAN1_R_10 0x18F104D0		//VCU����1
	#define CAN1_R_11 0x18F105D0		//VCU����2		
	#define CAN1_R_12 0x18F106D0		//VCU����3
	#define CAN1_R_13 0x18F107D0		//VCU����4
	#define CAN1_R_14 0x18F108D0		//VCU����5
	#define CAN1_R_15 0x18F109D0		//VCU����6
	
	#define CAN1_R_16 0x18FF2880		//��Ʒ���� 
	#define CAN1_R_17 0x18FF2080		//�豸��� 
	//#define CAN1_R_17 0x18FF2E80		//�豸��� 
	#define CAN1_R_18 0x18FF2180		//Ӳ���汾 
	#define CAN1_R_19 0x18FF2280		//����汾 
	#define CAN1_R_20 0x18FF2F80		//ʱ�����
	
	#define CAN1_R_21 0x18FF2583		//�ն˱��
	#define CAN1_R_22 0x18FF2483		//SIM ���� 
	#define CAN1_R_23 0x18FF2783		//�ն������Ϣ 
	#define CAN1_R_24 0x18FF1083		//�ն�״̬ 
	#define CAN1_R_25 0x18FF0583		//�ն���ʱ 
	
	#define CAN1_R_27 0x18F102FB		//T-Box
	
///���·�������˲���
	#define CAN1_R_26 0x18F101FB		//VIN��  3
	
	#define CAN1_R_03 0x18F8289E		//BMS�����ѹ��ֵ����֡
	#define CAN1_R_04 0x18F8299E		//BMS�¶�ģ�鼫ֵ����֡
	#define CAN1_R_05 0x18F82B9E		//BMS����1
	#define CAN1_R_06 0x18F82C9E		//BMS����2 
	#define CAN1_R_07 0x18F82D9E		//BMSϵͳ��Ϣ1
	#define CAN1_R_08 0x18F82E9E		//BMSϵͳ��Ϣ2
	
	#define CAN1_R_09 0x10F8159E		//BMSϵͳ��Ϣ3
	
//----------------------------------------------------//
///��CAN1����ID���з���
	#define CAN1R_GROUP0_MATCH  			0x1800D1F3		//�����ѹ�¶�
	#define CAN1R_GROUP0_SHIELD  			0xFF80FFFF		//���μĴ���
	#define CAN1R_GROUP0_INDEX_SATRT  0							//buff����
	#define CAN1R_GROUP0_INDEX_END	  26
	#define CAN1R_GROUP1_MATCH  			0x18F100D0		//VCU����
	#define CAN1R_GROUP1_SHIELD  			0xFFFFF0FF		
	#define CAN1R_GROUP1_INDEX_SATRT  27						
	#define CAN1R_GROUP1_INDEX_END	  32
	#define CAN1R_GROUP2_MATCH  			0x18FF2080		//��Ʒ���
	#define CAN1R_GROUP2_SHIELD  			0xFFFFF0FF		
	#define CAN1R_GROUP2_INDEX_SATRT  33						
	#define CAN1R_GROUP2_INDEX_END	  37
	#define CAN1R_GROUP3_MATCH  			0x18FF0083		//�ն����
	#define CAN1R_GROUP3_SHIELD  			0xFFFFC8FF		
	#define CAN1R_GROUP3_INDEX_SATRT  38						
	#define CAN1R_GROUP3_INDEX_END	  42
	#define CAN1R_GROUP4_MATCH  			0x18F8289E		//BMS     ��BMS��Ϣ3
	#define CAN1R_GROUP4_SHIELD  			0xFFFFF0FF		
	#define CAN1R_GROUP4_INDEX_SATRT  45						
	#define CAN1R_GROUP4_INDEX_END	  50
	
///CAN1����ID�����г�
	#define CAN1R_SIGLE0 CAN1_R_27
	#define CAN1R_SIGLE1 CAN1_R_26
	#define CAN1R_SIGLE2 CAN1_R_09
//----------------------------------------------------------------------//	
///����CANID
//	#define CAN1_T_1  0x00000000



//----------------------------------------------------------------------//	
	#define CAN1_R1_VORT 					0x1800D1F3		//�ж�BMS�����ѹ�¶�&��
	#define CAN1_RX_ID_NUM			 	APP_CAN1ID_ALL_NUM 
	///Ӧ�ò� CANID ��Ӧ��������  �� APP_CAN1ID_ALL_NUM��Ӧ
	#define CAN1_R_VOL_START_INDEX			 0			//��ѹ
	#define CAN1_R_VOL_END_INDEX				 23
	#define CAN1_R_TEM_START_INDEX			 24			//�¶�
	#define CAN1_R_TEM_END_INDEX				 26
	#define CAN1_R_OTH_START_INDEX			 27			//����
	#define CAN1_R_OTH_END_INDEX				 51

	
	
	
	
#endif

