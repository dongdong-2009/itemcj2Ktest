#ifndef __SELFDEF_H
#define __SELFDEF_H

#include <stdint.h>

	uint8 U8SumCheck(uint8 data[],uint16 len);
	uint8 XorCheck(uint8 data[],uint16 len);
	void RamClear(uint8 *ram_addr,uint16 len);
	void RamZero(uint8 *ram_addr,uint16 len);
	uint16 CrcCheck(uint8 *p_data,uint16 len);
	uint16 Crc16Verify(uint16 check, uint8 *str, uint16 len);
	uint8 StrLen(const uint8 *str,uint8 max_count);
	void LongTimeDly(uint32 dly);
	uint16 SubMatch(uint8 sub[],uint8 sub_len,uint8 tar[],uint16 tar_len);
	uint8 IsValidChar(uint8 data[],uint8 len);
	uint8 IsValidAscii(uint8 data[],uint8 len);
	uint8 IsValidNum(uint8 data[],uint8 len);
	uint8 IsValidCharFloatNum(uint8 data[],uint8 len);
	uint8 IsValidNumOrChar(uint8 data[],uint8 len);
	uint8 IsValidNumOrCharOrDot(uint8 data[],uint8 len);
	uint8 IsValidSimCardNum(uint8 data[]);
	uint32 LittleBigSwap(uint32 val);
	void MemCpy(uint8 dst[],uint8 src[],uint16 len);
	uint8 MemCmp(uint8 dst[],uint8 src[],uint16 len);
	uint8 AsciiToHexVal(uint8 h_b,uint8 l_b);
	uint16 AsciiToHex(uint8 *p_src,uint16 len,uint8 *p_dst);
	uint16 HexToAscii(uint8 *p_src,uint16 len,uint8 *p_dst);
	uint32 U8ToUint32(uint8 data[]);
	uint8 U16ToAscii(uint8 in_data[],uint8 in_len,uint8 out_data[]);
	void Uint32ToU8(uint8 data[],uint32 val);
	uint32 U8ToUint32Rev(uint8 data[]);
	void Uint32ToU8Rev(uint8 data[],uint32 val);
	
////////////////////////////////////////////////////////////////////////////////	
INT16U  CopyInt2Str(char *str,INT8U *int8,INT16U int8len,INT16U maxstrlen);
INT16U  CopyInt32U(INT32U *dptr,INT32U *sptr,INT16U len);
INT16U  DByte2Word(	INT8U high,INT8U low);
INT32U  QByte2DWord(INT8U *bt);

INT8U GetChkSum(INT8U *dptr, INT16U len);
INT8U GetChkSum_N(INT8U *dptr, INT16U len);
INT8U GetChkSum_XOR(INT8U *dptr, INT16U len);
INT8U GetChkSum_Carry(INT8U *dptr, INT16U len);

INT16U GetChkSum_16(INT8U *dptr, INT16U len);
INT16U GetChkSum_PAR(INT8U *dptr, INT16U len);
INT32U GetChkSum_32(INT8U *dptr, INT16U len);



INT16U sprint(INT8U *sptr, INT16U maxlen,const char *fmt, ...);
void  XOR_Opt(INT8U *dptr,INT8U *sptr,INT8U xorbase,INT16U len);



//int ExecEntry(INT16U index, const EXECENTRY_ST *funcentry, INT16U num);
INT32U  AsciiFloat2Int(INT8U *af,INT8U len,INT8U fractsize); // ??¨¨¡¤¦Ì?D?¨ºy¦Ì?o¨®fractsize??;

INT8U   SemiHexToChar(INT8U sbyte);
INT8U   CharToSemiHex(INT8U schar);

INT8U Hex_2_Bcd(INT8U hex);
INT8U Bcd2Hex(INT8U bcd);
INT8U IntToBcd(INT32U  data, INT8U *bcd, INT8U fillcount);
void  HexsToBcds(INT8U *bcds,INT8U *hexs,INT16U len);
void  BcdsToHexs(INT8U *hexs,INT8U *bcds,INT16U len);


INT16U HexsToString(INT8U *str, INT8U *hexs, INT16U len);
INT16U StringToHexs(INT8U *hexs, INT8U *str, INT16U len);

INT32U  StringToHex(INT8U *str, INT16U len);
INT8U   IntToString(INT8U *dptr, INT32U data,INT8U reflen);
INT32U  StringToInt(INT8U *sptr, INT8U len);


void    Mass_AsciiToDec(INT8U *dptr, INT8U *sptr, INT8U len);
void    UpCaseChar(INT8U *ptr, INT16U len);

INT16U  KeyLocation(INT8U *sptr, char key, INT8U keyindex,INT16U limit);
INT16U  SearchDigitalString(INT8U *ptr, INT16U maxlen, INT8U flagchar, INT8U numflag);
INT16U  SearchString(INT8U *dptr, INT16U limitlen, INT8U *sptr, INT16U maxlen, INT8U flagchar, INT8U numflag);
BOOLEAN SearchKeyWord(INT8U *ptr, INT16U maxlen, char *sptr);
BOOLEAN MatchStringBackword(INT8U *bptr, INT8U blen, INT8U *sptr,INT8U slen);


INT16U ConvertAscii2unicode(INT8U *dptr,INT8U *ascii,INT8U asclen);


INT8U  FormatTermID(INT8U *newID, char *orinID);
INT8U  FormatDeviceID(INT8U *newID, char *orinID);
////////////////////////////////////////////////////////////////////////////////
#endif
