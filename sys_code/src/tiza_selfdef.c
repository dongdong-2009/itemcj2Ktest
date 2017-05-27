#include "tiza_include.h"

uint8 U8SumCheck(uint8 data[],uint16 len)
{
	uint8 sum = 0;
	uint16 i;
	
	for(i=0;i<len;i++)
	{
		sum += data[i];
	}
	
	return sum;
}
uint8 XorCheck(uint8 data[],uint16 len)
{
	uint8 result = 0;
	uint16 i;
	
	for(i=0;i<len;i++)
	{
		result ^= data[i];
	}
	
	return result;
}
//void RamClear(uint8 *ram_addr,uint16 len)
//{
//	uint16 i;
//	
//	for(i=0;i<len;i++)
//	{
//		ram_addr[i] = INVALID_VAL_FF;
//	}
//}
void RamZero(uint8 *ram_addr,uint16 len)
{
	uint16 i;
	
	for(i=0;i<len;i++)
	{
		ram_addr[i] = 0x00;
	}
}
uint16 CrcCheck(uint8 *p_data,uint16 len)
{
	u16 fcs;
	u16 fcs16_tab[256] = 
	{
		0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
		0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
		0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
		0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
		0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
		0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
		0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
		0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
		0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
		0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
		0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
		0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
		0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
		0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
		0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
		0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
		0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
		0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
		0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
		0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
		0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
		0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
		0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
		0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
		0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
		0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
		0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
		0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
		0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
		0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
		0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
		0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
	};

	fcs = 0xffff;
	while (len --)
	{
		fcs = (fcs >> 8) ^ fcs16_tab[(fcs ^ (*p_data)) & 0xff];
		p_data++;
	}
	fcs ^= 0xffff;
	return fcs;
}
uint8 StrLen(const uint8 *str,uint8 max_count)
{
	uint8 len;
	
	len = 0x00;
	while(*str != '\0')
	{
		str ++;
		len ++;
		if(max_count != 0)
		{
			if(len > max_count)
			{
				len = 0;
				break;
			}
		}
	}
	return len;
}
uint16 Crc16Verify(uint16 check, uint8 *str, uint16 len)
{
	u8 m;
	u16 j;
	
	for(j=0; j<len; j++) 
	{
		check = check ^ (u16) (*str++ << 8);
		for(m=0; m<8; m++) 
		{
			if(check & 0x8000) check = check << 1 ^ 0x1021;
			else 	check = check << 1;
		}
	}
	return check;
}
void LongTimeDly(uint32 dly)
{
    uint32 i,j;
	
	for (i = 0; i < dly; i++) 
	{
//		FeedWtd();
		for (j = 0; j < 200; j++);
	}
}

uint16 SubMatch(uint8 sub[],uint8 sub_len,uint8 tar[],uint16 tar_len) 
{ 
	uint16 i = 0, j = 0,index = 0; 
	
	while((j<sub_len)&&((i+sub_len)<=tar_len))
	{
		if(tar[i+j] == sub[j]) 
		{
			j++;
			if(j == sub_len)
			{
				index = i+j;
				break;
			}
		}
		else 
		{ 
			i ++; 
			j = 0; 
		}
	}
	
	return index; 
} 
uint8 IsValidChar(uint8 data[],uint8 len)
{
	uint8 i,res = TRUE;
	
	for(i=0;i<len;i++)
	{
		if(!(((data[i] >= 'A')&&(data[i] <= 'Z'))||
		    ((data[i] >= 'a')&&(data[i] <= 'z'))))
		{
			break;
		}
	}
	
	if(i != len)
	{
		res = FALSE;
	}
	
	return res;
}
uint8 IsValidAscii(uint8 data[],uint8 len)
{
	uint8 i,res = TRUE;
	
	for(i=0;i<len;i++)
	{
		if(!(((data[i] >= 'A')&&(data[i] <= 'Z'))||
		    ((data[i] >= 'a')&&(data[i] <= 'z'))||
			((data[i] >= '0')&&(data[i] <= '9'))))
		{
			break;
		}
	}
	
	if(i != len)
	{
		res = FALSE;
	}
	
	return res;
}
uint8 IsValidNum(uint8 data[],uint8 len)
{
	uint8 i,res = TRUE;
	
	for(i=0;i<len;i++)
	{
		if((data[i] < 0x30)||(data[i] > 0x39))
		{
			break;
		}
	}
	
	if(i != len)
	{
		res = FALSE;
	}

	return res;
}
uint8 IsValidCharFloatNum(uint8 data[],uint8 len)
{
	uint8 i,res = TRUE;
	
	for(i=0;i<len;i++)
	{
		if(((data[i] >= 0x30)&&(data[i] <= 0x39))||(data[i] == '.'))
		{
			
		}
		else
		{
			break;
		}
	}
	
	if(i != len)
	{
		res = FALSE;
	}

	return res;
}
uint8 IsValidNumOrChar(uint8 data[],uint8 len)
{
	uint8 i,res = TRUE;
	
	for(i=0;i<len;i++)
	{
		if(!(((data[i] >= '0')&&(data[i] <= '9'))||
		     ((data[i] >= 'A')&&(data[i] <= 'Z'))||
		     ((data[i] >= 'a')&&(data[i] <= 'z'))))
		{
			break;
		}
	}
	
	if(i != len)
	{
		res = FALSE;
	}

	return res;
}
uint8 IsValidNumOrCharOrDot(uint8 data[],uint8 len)
{
	uint8 i,res = TRUE;
	
	for(i=0;i<len;i++)
	{
		if(!(((data[i] >= '0')&&(data[i] <= '9'))||
		     ((data[i] >= 'A')&&(data[i] <= 'Z'))||
		     ((data[i] >= 'a')&&(data[i] <= 'z'))||(data[i] == '.')))
		{
			break;
		}
	}
	
	if(i != len)
	{
		res = FALSE;
	}

	return res;
}
uint32 LittleBigSwap(uint32 val)
{
	uint32 tmp_val;
	
	tmp_val = val & 0xff;
	tmp_val = (tmp_val << 8) + ((val >> 8) & 0xff);
	tmp_val = (tmp_val << 8) + ((val >> 16) & 0xff);
	tmp_val = (tmp_val << 8) + ((val >> 24) & 0xff);
	
	return tmp_val;
}
void MemCpy(uint8 dst[],uint8 src[],uint16 len)
{
	uint16 i;
	
	for(i=0;i<len;i++)
	{
		dst[i]= src[i];
	}
}
uint8 MemCmp(uint8 dst[],uint8 src[],uint16 len)
{
	uint8 res = TRUE;
	uint16 i;
	
	for(i=0;i<len;i++)
	{
		if(dst[i] != src[i])
		{
			res = FALSE;
			break;
		}
	}
	return res;
}

uint8 AsciiToHexVal(uint8 h_b,uint8 l_b)
{
	uint8 hex_val;
	
	if((h_b >= '0')&&(h_b <= '9'))
	{
		hex_val = h_b - '0';
	}
	else if((h_b >= 'A')&&(h_b <= 'F'))
	{
		hex_val = h_b - 'A' + 10;
	}
	else if((h_b >= 'a')&&(h_b <= 'f'))
	{
		hex_val = h_b - 'a' + 10;
	}
	
	hex_val <<= 4;
	
	if((l_b >= '0')&&(l_b <= '9'))
	{
		hex_val |= l_b - '0';
	}
	else if((l_b >= 'A')&&(l_b <= 'F'))
	{
		hex_val |= l_b - 'A' + 10;
	}
	else if((l_b >= 'a')&&(l_b <= 'f'))
	{
		hex_val |= l_b - 'a' + 10;
	}

	return hex_val;
}
uint16 AsciiToHex(uint8 *p_src,uint16 len,uint8 *p_dst)   
{   
    uint16 i,ret_len;

    ret_len = len >> 1;	
    for(i=0;i<ret_len;i++)   
    {   
      p_dst[i] = AsciiToHexVal(p_src[2*i],p_src[2*i+1]);
    }   
        
    return ret_len;       
} 
uint16 HexToAscii(uint8 *p_src,uint16 len,uint8 *p_dst)   
{     
    uint8 tab[]="0123456789ABCDEF";    /// 0x0-0xf的字符查找表    
    uint16 i;
	
    for(i=0;i<len;i++)   
    {    
        *p_dst++ = tab[(*p_src) >> 4];      
        *p_dst++ = tab[(*p_src) & 0x0f];   
        p_src++;   
    }   
        
    return (len << 1);   
}
uint32 U8ToUint32(uint8 data[])
{
	uint32 temp_u32;
	
	temp_u32  = data[0] << 24;
	temp_u32 |= data[1] << 16;
	temp_u32 |= data[2] << 8;
	temp_u32 |= data[3];
	
	return temp_u32;
}
uint8 U16ToAscii(uint8 in_data[],uint8 in_len,uint8 out_data[])
{
	uint8 i,j,k;
	uint16 temp_u16;
	
	j = 0;
	for(i=0;i<in_len/2;i++)
	{
		temp_u16 = in_data[2*i+1] << 8;
		temp_u16  += in_data[2*i];
		for(k=0;k<4;k++)
		{
			out_data[j +3 - k] = temp_u16 % 10;
			out_data[j +3 - k] += 0x30;
			temp_u16 /= 10;
		}
		j += 4;
	}
	
	return j;
}
void Uint32ToU8(uint8 data[],uint32 val)
{
	data[0] = val >> 24;
	data[1] = val >> 16;
	data[2] = val >> 8;
	data[3] = val;
}
uint32 U8ToUint32Rev(uint8 data[])
{
	uint32 temp_u32;
	
	temp_u32  = data[3] << 24;
	temp_u32 |= data[2] << 16;
	temp_u32 |= data[1] << 8;
	temp_u32 |= data[0];
	
	return temp_u32;
}
void Uint32ToU8Rev(uint8 data[],uint32 val)
{
	data[3] = val >> 24;
	data[2] = val >> 16;
	data[1] = val >> 8;
	data[0] = val;
}

////////////////////////////////////////////////////////////////////////////////

INT16U  CopyInt2Str(char *str,INT8U *int8,INT16U int8len,INT16U maxstrlen)
{
	if (int8len > maxstrlen - 1) return 0;
	memcpy(str,int8,int8len);
	str += int8len;
	*str = 0;
	return int8len;
}

INT16U  CopyInt32U(INT32U *dptr,INT32U *sptr,INT16U len)
{
	INT16U i;
	for (i=0;i<len;i++)
	{
		*dptr++ = *sptr++;
	}
	return len; 
}


INT16U  DByte2Word(	INT8U high,INT8U low)
{
    INT16U result;
	result = high;
	result <<= 8;
	result += low;
	return result;
}

INT32U QByte2DWord(INT8U *bt)
{
	INT32U  result;
	INT8U i;
	
	result = 0;
	for (i=0;i<4;i++)
	{
		result <<= 8;
		result += *bt++;	  
	}
	return result;
}

/******************************************************************************************
   CHECKSUM CACULATE
*****************************************************************************************/
INT8U GetChkSum(INT8U *dptr, INT16U len)
{
	INT8U result;	
	result = GetChkSum_16(dptr,len);	
	return result;
}

INT8U GetChkSum_N(INT8U *dptr, INT16U len)
{
	return (~GetChkSum(dptr, len));
}

INT8U GetChkSum_XOR(INT8U *dptr, INT16U len)
{
    INT8U result;
	
    result = 0;
    for (; len > 0; len--) {
        result ^= *dptr++;
    }
    return result;
}

INT8U GetChkSum_Carry(INT8U *dptr, INT16U len)
{
	WORD_UNION result;	
	result.word = 0;
	for (; len > 0; len--)
	{
		result.word += *dptr++;
		if(result.bytes.high > 0)
		{
			result.bytes.low += result.bytes.high;
			result.bytes.high = 0;
		}
	}
	return (result.bytes.low);
}

INT16U GetChkSum_16(INT8U *dptr, INT16U len)
{
    INT16U result;	
    result = 0;
    for (; len > 0; len--) {
        result += *dptr++;
    }
    return result;
}

INT32U GetChkSum_32(INT8U *dptr, INT16U len)
{
    INT32U result;	
    result = 0;
    for (; len > 0; len--) {
        result += *dptr++;
    }
    return result;
}
//计算系统参数的校验和
INT16U GetChkSum_PAR(INT8U *dptr, INT16U len)
{
	INT16U i,result;
	INT8U  *ptr;
	
	result = 0;
	ptr = dptr;
	
	for (i=0;i<len;i++)
	{
		result += *dptr++;
	}
	result += len;	
	result += GetChkSum_Carry(ptr,len);  
	return result;
}


/***********************************************************************************
    字符模式变换函数
***********************************************************************************/
INT8U Hex_2_Bcd(INT8U hex)
{
    INT8U bcd;
	hex %= 100;
	bcd = hex / 10;
	bcd <<= 4;
	bcd += hex % 10;
	return bcd;
}

INT8U Bcd2Hex(INT8U bcd)
{
    INT8U hex;
	hex = (bcd >> 4) * 10;
	hex += bcd & 0x0F;
	return hex;
}


// data 转换为BCD,如果fillcout!=0，则需要填充满fillcount,不足字段补0
INT8U IntToBcd(INT32U  data, INT8U *bcd, INT8U fillcount)
{
   INT8U i,index,count;
   INT8U  buf[6];
   
   if (fillcount > sizeof(buf)) fillcount = sizeof(buf);
   
   memset(buf,0x00,sizeof(buf));   
   for (i=1;i<=sizeof(buf);i++) {
      index = sizeof(buf) - i;
      buf[index] = Hex_2_Bcd(data % 100);
      data /= 100;
      if (data == 0) break;
   }
   
   if (fillcount == 0) {
      count = i;
   } else {
      count = fillcount;
   }
   
   for (i=0;i<count;i++) {
      index = sizeof(buf) - count + i;
      *bcd++ = buf[index]; 
   }
   return count;
}

void  HexsToBcds(INT8U *bcds,INT8U *hexs,INT16U len)
{
    for (;len > 0;len--) {
       *bcds++ = Hex_2_Bcd(*hexs++);
	}
}

void BcdsToHexs(INT8U *hexs,INT8U *bcds,INT16U len)
{
    for (;len > 0;len--) {
	   *hexs++ = Bcd2Hex(*bcds++);
    }
}

INT8U SemiHexToChar(INT8U sbyte)
{
    sbyte &= 0x0F;
    if (sbyte < 0x0A) return (sbyte + '0');
    else return (sbyte - 0x0A + 'A');
}

INT8U CharToSemiHex(INT8U schar)
{
    if (schar >= '0' && schar <= '9') return (schar - '0');
    else if (schar >= 'A' && schar <= 'F') return (schar - 'A' + 0x0A);
    else if (schar >= 'a' && schar <= 'f') return (schar - 'a' + 0x0A);
    else return 0;
}

INT16U HexsToString(INT8U *str, INT8U *hexs, INT16U len)
{
    INT16U i;
    INT8U  stemp;
    
    for (i=1; i<=len; i++) {
       stemp = *hexs++;
       *str++ = SemiHexToChar(stemp >> 4);
       *str++ = SemiHexToChar(stemp & 0x0F);
    }
    return (2 * len);
}

INT16U StringToHexs(INT8U *hexs, INT8U *str, INT16U len)
{
    INT16U i,slen;
    INT8U  dtemp, stemp;

	slen = len;	
    if (len % 2) len++;
    len /= 2;	
    for (i=1; i<=len; i++) {
       stemp   = CharToSemiHex(*str++);
       dtemp   = stemp << 4;
       *hexs++ = CharToSemiHex(*str++) | dtemp;
    }
	if (slen % 2) {		
       hexs--;
       *hexs |= 0x0F; 
	}
    return len;
}

INT32U StringToHex(INT8U *str, INT16U len)
{
    INT32U result;
	INT8U  i,temp;	

	result = 0;
	if (len > sizeof(INT32U)) len = sizeof(INT32U);	
	for (i=1; i<=len; i++) {
       temp   = CharToSemiHex(*str++);
	   result <<= 4;
	   result |= temp;
    }
    return result;
}

// SAMPLE: 1234 -----> '1','2','3','4'
INT8U IntToString(INT8U *str,INT32U data,INT8U reflen)
{
    INT8U len;
    INT8U *ptr;
    INT8U tempbuf[16]; 
    
    len     = 0;
    if (reflen > sizeof(tempbuf)) return 0;
    for (;;) {
       tempbuf[len] = SemiHexToChar(data % 10);
       len++;
       if (data < 10) break;
       else data /= 10;
    }
    if (reflen == 0) reflen = len; // 为0时，以实际值；
    if (len >= reflen) len = reflen;
    else {
       for (;len<reflen;len++) tempbuf[len] = '0';
    }
    ptr = tempbuf + reflen;  //point to the valid of next char
    ptr--;
    len = reflen;
    for (;reflen>0;reflen--) *str++ = *ptr--;
    return len;    
}

// SAMPLE: '1','2','3','4' -----> 1234 
INT32U StringToInt(INT8U *sptr, INT8U len)
{
    INT32U result;    
    result = 0;
    for (; len > 0; len--) {
       result = result * 10 + CharToSemiHex(*sptr++);
    }
    return result;
}

void  Mass_AsciiToDec(INT8U *dptr, INT8U *sptr, INT8U len)
{
    // MUST BE 2
    for (; len > 0; len--) {
        *dptr++ = StringToInt(sptr,2);
        sptr += 2;
    }
}

INT32U  AsciiFloat2Int(INT8U *af,INT8U len,INT8U fractsize) // 精确到小数点后fractsize位
{
    INT32U  result;
	INT16U multi,fraction;
    INT8U   i,finddot;	

	finddot = false;
	multi = 1;
	for (i=0;i<fractsize;i++) {
	   multi *= 10;
    }

	result = 0;
	for (i=0;i<len;i++) {
	   if (*af == '.') {          
		  result *= multi;
		  finddot = true;
          i++;
          af++;
		  break;		  
	   } else {
          result *= 10;
		  result += *af++ - '0';
	   }
	}
    fraction = 0;
	if (finddot == false) {
       result *= multi;
	} else {
	   
	   for (;fractsize>0;fractsize--) {
          fraction *= 10;
	      if (i<len) {
		     fraction += *af - '0';
		     af++;
		     i++;
	      }
	   }  
	}
	result += fraction;
	return result;
}

void UpCaseChar(INT8U *ptr, INT16U len)
{
	INT8U ch;
	for (;len>0;len--)
	{
		ch = *ptr;
		if((ch >= 'a') && (ch <= 'z')) 
		{
			ch += 'A' - 'a';
		  *ptr = ch;
		}
		ptr++;
	}
}

/***********************************************************************************
    搜索定位比较函数
***********************************************************************************/
void  XOR_Opt(INT8U *dptr,INT8U *sptr,INT8U xorbase,INT16U len)
{
    INT16U i;
	for (i=0;i<len;i++) {
       *dptr++ = *sptr++ ^ xorbase;
	}
}

//int ExecEntry(INT16U index, const EXECENTRY_ST *funcentry, INT16U num)
//{
//	for (; num > 0; num--, funcentry++)
//	{
//		if (index == funcentry->index)
//		{
//			if (funcentry->entryproc != 0)
//			{
//				funcentry->entryproc();
//				return TRUE;
//			}
//		}
//	}
//	return FALSE;
//}


INT16U KeyLocation(INT8U *sptr, char key, INT8U keyindex,INT16U limit)
{
	INT16U loc;    
	loc = 0;
	for (;;)
	{
		if(*sptr++ == key)
		{
			if(keyindex == 1)
			{
				break;
			}
			else
			{
				keyindex--;
			}
		}
		if(limit == 0)
		{
			break;
		}
		else
		{
			loc++;
		}
		limit--;
	}
	return loc;
}

INT16U SearchDigitalString(INT8U *ptr, INT16U maxlen, INT8U flagchar, INT8U numflag)
{
    BOOLEAN find;
    INT16U  result;
    INT8U *bptr, *eptr;
    
    find = FALSE;
	bptr = ptr;
	eptr = ptr;
    for(;;) {
        if (maxlen == 0) {
            find = FALSE;
            break;
        }
        if (*ptr == flagchar) {
            numflag--;
            if (numflag == 0) {
                eptr = ptr;
                break;
            }
        }
        if (*ptr >= '0' && *ptr <= '9') {
            if (!find) {
                find = TRUE;
                bptr = ptr;
            }
        } else {
            find = FALSE;
        }
        ptr++;
        maxlen--;
    }
    
    if (find) {
        result = 0;
        for(;;) {
            result = result * 10 + CharToSemiHex(*bptr++);
            if (bptr == eptr) break;
        }
    } else {
        result = 0xffff;
    }
    return result;
}

INT16U SearchString(INT8U *dptr, INT16U limitlen, INT8U *sptr, INT16U maxlen, INT8U flagchar, INT8U numflag)
{
    BOOLEAN find;
    INT16U  len;
    INT8U *bptr, *eptr;
    
    find = FALSE;
	bptr = dptr;
	eptr = dptr;
    for(;;) {
        if (maxlen == 0) {
            find = FALSE;
            break;
        }
        if (*sptr == flagchar) {
            if (!find) {
                find = TRUE;
                bptr = sptr;
            } else {
                numflag--;
                if (numflag == 0) {
                    eptr = sptr;
                    break;
                } else {
                    find = FALSE;
                }
            }
        }
        sptr++;
        maxlen--;
    }
    
    if (find) {
        len = 0;
        bptr++;
        while(bptr != eptr) {
            if (len >= limitlen) {
                len = 0;
                break;
            }
            *dptr++ = *bptr++;
            len++;
        }
        return len;
    } else {
        return 0;
    }
}

BOOLEAN SearchKeyWord(INT8U *ptr, INT16U maxlen, char *sptr)
{
	char *tmpptr;

	if (*sptr == 0) return FALSE;
	tmpptr = sptr;
	while(maxlen > 0)
	{
		if (*ptr++ == *tmpptr++)
		{
			if (*tmpptr == 0) return TRUE;
		}
		else
		{
			tmpptr = sptr;
		}
		maxlen--;
	}
	return FALSE;
}

BOOLEAN MatchStringBackword(INT8U *bptr, INT8U blen, INT8U *sptr,INT8U slen)
{
	INT8U *ptr1,*ptr2;
	if (blen < slen) return false;
	ptr1 = bptr + blen;
	ptr2 = sptr + slen;
	ptr1--;//point to the last byte
	ptr2--;
	while(slen > 0)
	{
		if(*ptr1-- != *ptr2--)
		{
			return false;
		}
		slen--;
	}
	return true;   
}    


/***********************************************************************************
    搜索定位比较函数
***********************************************************************************/
   



INT16U ConvertAscii2unicode(INT8U *dptr,INT8U *ascii,INT8U asclen)
{
    INT8U  len;
    for(len=asclen;len>0;len--) {
       *dptr++ = 0x00;
       *dptr++ = *ascii++;
    }   
    return 2*asclen;
}

INT8U  FormatTermID(INT8U *newID, char *orinID)
{
	INT8U i,len;
	INT8U *sptr,*dptr;
	
	len = strlen(orinID);
	memset(newID,0x00,10);
	dptr = newID + 10;
	sptr = (INT8U*)(orinID + len);
	i = 0;
	while (len > 0)
	{
		*--dptr = *--sptr;
		len--;
		i++;
		if(i >= 10)
		{
			break;
		}
	}	
	return 10;
}





INT8U LatiLogiToBcd(INT8U *bcd,INT8U *latlog)
{
    INT8U i,tmp,buf[4];    
	HexsToBcds(buf,latlog,4);
    if (latlog[0]  > 100) {
	   tmp = 0x10;
	} else {
	   tmp = 0x00;
	}
	for (i=0;i<4;i++) {
       tmp |= buf[i] >> 4;
	   *bcd++ = tmp;
	   tmp = 0x00;
	   tmp |= buf[i] << 4;
	}
	return 4;
}

INT32U degree_change(INT8U *str)
{
    INT32U degree;
    INT32U brach;

    degree = str[0]*1000000;
    brach = (str[1]*10000+str[2]*100+str[3])*5;
    if (2 == brach%3){
        brach += 1;
    }
    degree += brach/3;
    return degree;
}



////////////////////////////////////////////////////////////////////////////////
