#include "sms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <time.h>
//#include "phone_msgmanage.h"

unsigned char readbuf[BUFSIZE];


#define LONG_SMS_NUMBER_MAX		1000

typedef struct _LongSms
{
	char content[200];
	int len;
	char encode;
	int id;
	int index;
	int indexMax;
	struct _LongSms *pNext; 
}LongSms;

typedef struct _LongSmsHead
{
	int id;
	LongSms* pHead;
	int number;
	int numberMax;
	struct _LongSmsHead* pNext;	
}LongSmsHead;

static LongSmsHead* gsp_LongSmsHeadArray = NULL;
static int gsi_LongSmsNum = 0;

static void removeAllLongSmsOnHead(LongSmsHead *pLongSmsHead)
{
	LongSms *pTmp = pLongSmsHead->pHead;
	LongSms *pNext = NULL;
	while( pTmp )
	{
		pNext = pTmp->pNext;
		free(pTmp);
		gsi_LongSmsNum--;
		pTmp = pNext;
	}

	pLongSmsHead->pHead = NULL;
	pLongSmsHead->number = 0;

	return;
}

static void removeSmsHead(LongSmsHead *pLongSmsHead)
{
	removeAllLongSmsOnHead(pLongSmsHead);
	LongSmsHead *pTmp = gsp_LongSmsHeadArray;
	LongSmsHead *pNext = NULL;
	LongSmsHead *pPre = NULL;
	while(pTmp)
	{
		pNext = pTmp->pNext;
		if( pTmp == pLongSmsHead )
		{
			free(pTmp);
			if( gsp_LongSmsHeadArray == pTmp )
			{
				gsp_LongSmsHeadArray = pNext;
			}
			else
			{
				pPre->pNext = pNext;
			}
			
			break;
		}
		pPre = pTmp;
		pTmp = pTmp->pNext;
	}

	return;
}

static void removeAllLongSmsHead()
{
	LongSmsHead *pTmp = gsp_LongSmsHeadArray;
	LongSmsHead *pNext = NULL;
	while( pTmp )
	{
		pNext = pTmp->pNext;
		removeAllLongSmsOnHead(pTmp);
		free(pTmp);
		pTmp = pNext;
	}

	gsp_LongSmsHeadArray = NULL;
	gsi_LongSmsNum = 0;

	return;
}

static int appendLongSms(LongSms* pLongSms, char *& pRetContent, int& iLen)
{
	LongSmsHead *pTmp = gsp_LongSmsHeadArray;
	LongSmsHead *pPre = NULL;
	LongSmsHead *pSmsHead = NULL;
	while( pTmp )
	{
		pPre = pTmp;
		if( pTmp->id == pLongSms->id )
		{
			pSmsHead = pTmp;
			break;
		}
		pTmp = pTmp->pNext;
	}

	if( !pSmsHead )
	{
		LongSmsHead *pNew = (LongSmsHead*)malloc(sizeof(LongSmsHead));
		if( !pNew )
		{
			printf("malloc error");
			abort();
		}
		pNew->id = pLongSms->id;
		pNew->number = 1;
		pNew->numberMax = pLongSms->indexMax;
		pNew->pHead = pLongSms;
		pNew->pNext = NULL;
		if( !pPre )
		{
			gsp_LongSmsHeadArray = pNew;
		}
		else
		{
			pPre->pNext = pNew;
		}
		gsi_LongSmsNum++;
		return 0;
	}

	LongSms *pPre1 = pSmsHead->pHead;
	LongSms *pNext1 = pPre1->pNext;
	bool bFindLocation = false;
	while( pNext1 )
	{
		if( pPre1->index > pLongSms->index )
		{
			pSmsHead->pHead = pLongSms;
			pLongSms->pNext = pPre1;
			bFindLocation = true;
			break;
		}
		
		if( pPre1->index < pLongSms->index && pLongSms->index < pNext1->index )
		{
			pPre1->pNext = pLongSms;
			pLongSms->pNext = pNext1;
			bFindLocation = true;
			break;
		}
		pPre1 = pNext1;
		pNext1 = pNext1->pNext;
	}

	if( !bFindLocation )
	{
		if( pLongSms->index > pPre1->index )
		{
			pPre1->pNext = pLongSms;
			pLongSms->pNext = NULL;
		}
		else
		{
			printf("can not find insert location, index[%d]\n", pLongSms->index);
			free(pLongSms);
			return -1;
		}
	}

	pSmsHead->number++;
	gsi_LongSmsNum++;

	//judge whether sms all come here
	if( pSmsHead->number == pSmsHead->numberMax )
	{
		LongSms *pTmp2 = pSmsHead->pHead;
		int len = 0;
		while( pTmp2 )
		{
			len += pTmp2->len;
			pTmp2 = pTmp2->pNext;
		}
		
		pRetContent = (char*)malloc(len+1);
		if( !pRetContent )
		{
			printf("malloc error");
			abort();
		}
		memset(pRetContent, 0, len+1);

		pTmp2 = pSmsHead->pHead;
		len = 0;
		while( pTmp2 )
		{
			memcpy(pRetContent+len, pTmp2->content, pTmp2->len);
			len += pTmp2->len;
			pTmp2 = pTmp2->pNext;
		}

		iLen = len;

		removeSmsHead(pSmsHead);
	}
	
	return 0;
}

/*************************************************
*编码与发送短信息
*************************************************/

int NumToCalledPartyBcd (char *phone, LPBYTE pRecData, int len);
inline UCHAR Ascii2Bcd (UCHAR ch);
char* utf8_ucs2 (char *utf8_str, int* ret_len);
u_int16_t * utf8_to_ucs2(const unsigned char *str, int len, int *ucs2_len);
LPSTR BinToHex (LPBYTE p, int len);
void SendStrCmd(int fd, char *buf);
void SendCmd(int fd, void *buf, size_t size);
void HexDump(unsigned char *buf, int size);
void HexDumpLine(unsigned char *buf, int remainder, int offset);
int ReadResp(int fd);
LPSTR Gsm11Enc7bitToUTF8 (LPBYTE pData, int len);
unsigned char  * ucs2_to_utf8(u_int16_t * ucs2_str, int len);
LPSTR PhoneNumToAscii (LPBYTE p, int len);
LPBYTE HexToBin (LPSTR p, int len);
//int SendSMS(int fd, char *to, char *text);

int	ComposeSubmitSms (SmsSendStruct *& pSmsSend, char *dst, char *smsc, char *utf8_str)
{
	char tmpBuf[200];
	char *pResult = tmpBuf;
	char *pSmsType = NULL;
	int smsc_len = 1;
	pSmsSend = NULL;
 /*
       Length of SMSC information. Here the length is 0, 
       which means that the SMSC stored in the phone should be used.
       Note: This octet is optional. On some phones this octet should be omitted! 
       (Using the SMSC stored in phone is thus implicit) 
*/
	if (smsc == NULL)
	*pResult++ = 0;
	else
	{
		smsc_len = NumToCalledPartyBcd (smsc, (LPBYTE)pResult, -1);
		pResult += smsc_len;
	}
/*
       SMS-SUBMIT PDU
       A first octet of "11" (hex) has the following meaning: 
       Bit no   7       6      5      4      3     2      1      0 
       Name TP-RP TP-UDHI TP-SRR TP-VPF TP-VPF TP-RD TP-MTI TP-MTI 
       Value    0       0      0      1      0     0      0      1 
*/
	//*pResult++ = 0x51;
	pSmsType = pResult++;
/*
       TP-Message-Reference. 
       The "00" value here lets the phone set the message reference number itself. 
*/
	*pResult++ = 0;

	int da_len = NumToCalledPartyBcd (dst, (LPBYTE)pResult, -1);
/*
       Address-Length. Length of the sender number 
*/
	if (dst == NULL)
		*pResult = 0;
	else if (pResult[1] == 0xd0)        // for Alphanumeric address type
	{
		//do nothing
	}
	else
		*pResult = strlen (dst) - ((dst[0] == '+') ? 1 : 0);
	pResult += da_len;
/*
       Protocol identifier 0 for text sms
*/
	*pResult++ = 0;
/*
       Data coding scheme 0x08 for UCS2
*/
	UCHAR dc;
//      if( strContaionNotASCII(Content) )
	dc = 0x08;
//      else
//              dc = 00;
	*pResult++ = dc;
/*
       VP Value Validity period value
       0-143
       (VP + 1) x 5 minutes (i.e 5 minutes intervals up to 12 hours)
       144-167
       12 hours + ((VP-143) x 30 minutes)
       168-196
       (VP-166) x 1 day
       197-255
       (VP - 192) x 1 week
*/
	*pResult++ = 0xff;          //max value
	int ud_offset = 4 + da_len + 1 + 1 + smsc_len;
	int ret = 0;
	if (dc == 0x08)
	{
		int iUcs2Len = 0;
		char *pRetUcs2Str = utf8_ucs2 (utf8_str, &iUcs2Len);
		int iSmsHeadLen = (int)(pResult-tmpBuf);
		if( iUcs2Len <= 134 )
		{
			*pSmsType = 0x11;
			char *pSmsContent = (char*)malloc(iSmsHeadLen+1+iUcs2Len);
			if( !pSmsContent )
			{
				printf("malloc error");
				abort();
			}		
			pSmsSend = (SmsSendStruct*)malloc(sizeof(SmsSendStruct));
			if( !pSmsSend )
			{
				printf("malloc error");
				abort();
			}
			pSmsSend->pSendContent = pSmsContent;
			pSmsSend->len = iSmsHeadLen+1+iUcs2Len;
			pSmsSend->pNext = NULL;
			memcpy(pSmsSend->pSendContent, tmpBuf, iSmsHeadLen);
			pResult = &(pSmsSend->pSendContent[iSmsHeadLen]);
			*pResult++ = iUcs2Len;
			memcpy(pResult, pRetUcs2Str, iUcs2Len);
		}
		else
		{
			*pSmsType = 0x51;
			char cSectionNum = (iUcs2Len+134)/134;
			char cCurrentSection = 1;
			int iCurrentUcs2Len = 0;
			char *pUcs2StrTmp = pRetUcs2Str;
			int iUcs2LenTmp = iUcs2Len;
			static char cSmsId = 0;
			SmsSendStruct* pSmsSendTmp = NULL;
			SmsSendStruct* pSmsSendTmpPre = NULL;
			while( iUcs2LenTmp > 0 )
			{
				if( iUcs2LenTmp > 134 )
					iCurrentUcs2Len = 134;
				else
					iCurrentUcs2Len = iUcs2LenTmp;
				char *pSmsContent = (char*)malloc(iSmsHeadLen+1+6+iCurrentUcs2Len);
				if( !pSmsContent )
				{
					printf("malloc error");
					abort();
				}		
				pSmsSendTmp = (SmsSendStruct*)malloc(sizeof(SmsSendStruct));
				if( !pSmsSendTmp )
				{
					printf("malloc error");
					abort();
				}
				
				pSmsSendTmp->pSendContent = pSmsContent;
				pSmsSendTmp->len = iSmsHeadLen+1+6+iCurrentUcs2Len;
				pSmsSendTmp->pNext = NULL;
				memcpy(pSmsSendTmp->pSendContent, tmpBuf, iSmsHeadLen);
				pResult = &(pSmsSendTmp->pSendContent[iSmsHeadLen]);
				*pResult++ = iCurrentUcs2Len+6;
				*pResult++ = 5;
				*pResult++ = 0;
				*pResult++ = 3;
				*pResult++ = cSmsId;
				*pResult++ = cSectionNum;
				*pResult++ = cCurrentSection;
				cCurrentSection++;
				memcpy(pResult, pUcs2StrTmp, iCurrentUcs2Len);
				iUcs2LenTmp -= iCurrentUcs2Len;
				pUcs2StrTmp += iCurrentUcs2Len;

				if( !pSmsSend )
					pSmsSend = pSmsSendTmp;
				else
					pSmsSendTmpPre->pNext = pSmsSendTmp;
				pSmsSendTmpPre = pSmsSendTmp;
			}
			cSmsId++;
		}
		free(pRetUcs2Str);
	}
	else
	{
		fprintf (stderr, "not implement now!\n");
		assert (1);
	}

	return 0;
}



int NumToCalledPartyBcd (char *phone, LPBYTE pRecData, int len)
{
	int i = 0;
	int j = 0;
	int num_len = strlen(phone);
	int bcd_len;
	if (phone != NULL)
	{
		if (phone[0] == '+')
		{
			bcd_len = num_len / 2;
			*pRecData++ = bcd_len + 1;
			*pRecData++ = 0x91;
			i = 1;
		}
		else
		{
			bcd_len = (num_len + 1) / 2;
			*pRecData++ = bcd_len + 1;
			*pRecData++ = 0x81;
			i = 0;
		}
		j = 2;
		int max_len;
		if (len == -1)
		{
			len = bcd_len + 2;
			max_len = len;
		}
		else
			max_len = len - 2;
		for (; i < num_len && j < max_len; i += 2)
		{
			UCHAR chL = Ascii2Bcd (phone[i]);
			UCHAR chH;
			if (i + 1 < num_len)
				chH = Ascii2Bcd (phone[i + 1]);
			else
				chH = 0xf;
			*pRecData++ = ((chH & 0xf) << 4) | (chL & 0xf);
			j++;
			if (i + 3 == num_len)
			{
				UCHAR chL = Ascii2Bcd (phone[i + 2]);
				UCHAR chH = 0xf;
				*pRecData++ = ((chH & 0xf) << 4) | (chL & 0xf);
				j++;
				i++;
			}
		}
	}
	for (; j < len; j++)
		*pRecData++ = 0xff;
	
	return len;
}


inline UCHAR Ascii2Bcd (UCHAR ch)
{
	if (ch > '0' && ch < '9')
		ch -= '0';
	else if (ch == '*')
		ch = 10;
	else if (ch == '#')
		ch = 11;
	return ch;
}

char* utf8_ucs2 (char *utf8_str, int* ret_len)
{
	int ucs2_len = 0;
	u_int16_t *ucs2_str = NULL;
	int i;
	int utf8_len = strlen (utf8_str);
	
	ucs2_str = utf8_to_ucs2 ((const unsigned char *)utf8_str, utf8_len, &ucs2_len);
	char *pRetUcs2Str = (char*)malloc(ucs2_len*sizeof(u_int16_t));
	if( !pRetUcs2Str )
	{
		printf("malloc error");
		abort();
	}
	*ret_len = ucs2_len*sizeof(u_int16_t);

	for (i = 0; i < ucs2_len*sizeof(u_int16_t); i += 2)
	{
		pRetUcs2Str[i] = *ucs2_str >> 8;
		pRetUcs2Str[i + 1] = *ucs2_str & 0xff;
		ucs2_str++;
	}

	return pRetUcs2Str;
}


u_int16_t * utf8_to_ucs2(const unsigned char *str, int len, int *ucs2_len)
{
	static u_int16_t *ucs2_str = NULL;
	static int      max_len_ucs2_str = 0;
	static uint32_t ch;
	static int      cnt = 0;
	int             i_str, i_ucs2_str;

	if (len == 0)
		return NULL;
	if (max_len_ucs2_str < len + 1) {
		max_len_ucs2_str = len + 1;
		ucs2_str = (u_int16_t *)REALLOC(ucs2_str, (sizeof(u_int16_t) * max_len_ucs2_str));
	}

	for (i_ucs2_str = 0, i_str = 0; i_str < len; i_str++) {
		if (cnt > 0) {
			uint32_t        byte = str[i_str];

			if ((byte & 0xc0) != 0x80) {
				i_str--;
				cnt = 0;
			} else {
				ch <<= 6;
				ch |= byte & 0x3f;
				if (--cnt == 0) {
				ucs2_str[i_ucs2_str++] = ch;
				}
			}
		} else {
			ch = str[i_str];
			if (ch < 0x80) {
			/* One byte sequence.  */
				ucs2_str[i_ucs2_str++] = ch;
			} else {
				if (ch >= 0xc2 && ch < 0xe0) {
				/* We expect two bytes.  The first byte cannot be 0xc0
				* or 0xc1, otherwise the wide character could have been
				* represented using a single byte.  */
					cnt = 2;
					ch &= 0x1f;
                		} else if ((ch & 0xf0) == 0xe0) {
				/* We expect three bytes.  */
					cnt = 3;
					ch &= 0x0f;
				} else if ((ch & 0xf8) == 0xf0) {
				/* We expect four bytes.  */
					cnt = 4;
					ch &= 0x07;
				} else if ((ch & 0xfc) == 0xf8) {
				/* We expect five bytes.  */
					cnt = 5;
					ch &= 0x03;
				} else if ((ch & 0xfe) == 0xfc) {
				/* We expect six bytes.  */
					cnt = 6;
					ch &= 0x01;
				} else {
					cnt = 1;
				}
				--cnt;
			}
		}
	}

	*ucs2_len = i_ucs2_str;
	
	return ucs2_str;
}


LPSTR BinToHex (LPBYTE p, int len)
{
	int i;
	LPSTR str = (LPSTR) malloc (len * 3 + 1);
	LPSTR basestr = str;
	for (i = 0; i < len; i++)
	{
		TOHEX(p[i], str);
	}
	*str = '\0';

	return basestr;
}

void SendStrCmd(int fd, char *buf)
{
	SendCmd(fd, buf, strlen(buf));
}

void SendCmd(int fd, void *buf, size_t size)
{
	fprintf(LOG, "Sending:\n");
	HexDump((unsigned char*)buf, size);

	if(write(fd, buf, size) == -1) {
		fprintf(stderr, "Shit. %s\n", strerror(errno));
		exit(1);
  	}
}

void HexDump(unsigned char *buf, int size)
{
	int i = 0;

	for (i = 0; i < size; i += 16)
		HexDumpLine(buf + i, size - i, i);
	fprintf(LOG, "%08x\n", size);
}

void HexDumpLine(unsigned char *buf, int remainder, int offset)
{
	int i = 0;
	char c = 0;

	// Print the hex part
	fprintf(LOG, "%08x | ", offset);
	for (i = 0; i < 16; ++i) {
		if (i < remainder)
			fprintf(LOG, "%02x%s", buf[i], (i == 7) ? "  " : " ");
		else
			fprintf(LOG, "  %s", (i == 7) ? "  " : " ");
	}
  // Print the ascii part
  
	fprintf(LOG, " | ");
	for (i = 0; i < 16 && i < remainder; ++i) {
		c = buf[i];
		if (c >= 0x20 && c <= 0x7e)
			fprintf(LOG, "%c%s", c, (i == 7) ? " " : "");
		else
			fprintf(LOG, ".%s", (i == 7) ? " " : "");
	}

	fprintf(LOG, "\n");

	return;
}


int ReadResp(int fd)
{
	int len = 0;
	struct timeval timeout;
	int nfds = fd + 1;
	fd_set readfds;
	int select_ret;

	FD_ZERO(&readfds);
	FD_SET(fd, &readfds);

// Wait a second
	timeout.tv_sec = 1;
	timeout.tv_usec = 500000;

	fprintf(stderr,"s");
	while ((select_ret = select(nfds, &readfds, NULL, NULL, &timeout)) > 0)
	{
		fprintf(stderr,"1");
		fprintf(stderr,"select_ret = %d\n",select_ret);
		len += read(fd, readbuf + len, BUFSIZE - len);
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);
		timeout.tv_sec = 0;
		timeout.tv_usec = 500000;
	}
	if (len > 0) {
		fprintf(stderr,"2");
		fprintf(LOG, "Read:\n");
		HexDump(readbuf, len);
	}
	
	return len;
}

/*
int SendSMS(int fd, char *to, char *text)
{
	UCHAR buf[200]; 
	UCHAR at_cmd[1024];
	LPSTR strHex;
	int len;
	int pdu_len;
	int retry = 10;
   
//    fd = InitConn(115200); 
//    SetPDUMode(fd);
	len = ComposeSubmitSms(buf, sizeof(buf), to, NULL, text);
	strHex = BinToHex(buf, len);
	pdu_len = strlen((char*)strHex)/2-1;
//printf("%d: %s\n",strlen(strHex)/2-1, strHex);
	sprintf((char*)at_cmd,"AT+CMGS=%d\r",pdu_len);
	fprintf(stderr,"S");
	SendStrCmd(fd, (char*)at_cmd);
	do{
		len = ReadResp(fd);
	}while(!strstr((char*)readbuf,">") && retry-- > 0);
	if(retry > 0)
	{
		fprintf(stderr,">");
		sprintf((char*)at_cmd,"%s\032",strHex); //CTRL-Z
		SendStrCmd(fd, (char*)at_cmd);
		do{
			fprintf(stderr,".");
			len = ReadResp(fd);
		}while(!strstr((char*)readbuf,"+CMGS:") && retry-- > 0);
	}
	if(retry > 0)
	{
		fprintf(stderr,"OK\n");
		free(strHex);
//	CloseConn(fd);
		return 0;//success
	}
	else{
		fprintf(stderr,"%s: %s\n",at_cmd,readbuf);
		free(strHex);
	}
//    CloseConn(fd);
	return -1;//failed
}
*/

/**************************************************
*解码函数
**************************************************/
LPBYTE HexToBin (LPSTR p, int len)
{
	int i;
	LPBYTE out = (LPBYTE) malloc (len >> 1);
	LPBYTE out_org = out;

	for (i = 0; i < len; i += 2)
	{
		*out = (HEXTOBIN (p[i]) << 4) | HEXTOBIN (p[i + 1]);
		out++;
	}
	
	return out_org;
}

LPSTR Gsm11Enc7bitToUTF8 (LPBYTE pData, int len)
{
	LPSTR str = (LPSTR) malloc (len * 8 / 7 + 1);
	LPSTR base_str = str;
	unsigned int shift_buf;
	int remanent_bits, consume_len;

	shift_buf = *pData++;
	remanent_bits = 8;
	consume_len = 1;

	do
	{
		*str++ = (unsigned char) (shift_buf & 0x7f);
		shift_buf >>= 7;
		remanent_bits -= 7;
		if (remanent_bits < 7)
		{
			if (consume_len >= len)
				break;
			shift_buf |= (*pData++ << remanent_bits);
			remanent_bits += 8;
			consume_len++;
		}
	}while (1);

	*str = 0;

	int str_len = str - base_str;
	unsigned short *wstr =
		(unsigned short *) malloc (sizeof (unsigned short) * str_len + 1);

	int i;
	for (i = 0; i < str_len; i++)
	{
		wstr[i] = gsmToLatin1Table[base_str[i]]<<8;
	}
	wstr[i] = 0;
	unsigned char *utf8_str = ucs2_to_utf8 (wstr, str_len);

	free (base_str);
	free (wstr);
	
	return utf8_str;
}

unsigned char  * ucs2_to_utf8(u_int16_t * ucs2_str, int len)
{
	unsigned char *utf8 = NULL;
	int      max_len_utf8 = 0;
	unsigned char  *outptr;
	int i;
	unsigned char *ucs2_str_char = (unsigned char *)ucs2_str;
 
	max_len_utf8 = len * 6 + 1;
	//utf8 = (unsigned char *)REALLOC(utf8, max_len_utf8);
	utf8 = (unsigned char *)malloc(max_len_utf8);
	
	outptr = utf8;
	for (i = 0; i < len; i++) {
		//uint32_t        wc1 = ucs2_str[i];
		unsigned char lo = ucs2_str_char[i*2];
		unsigned char hi = ucs2_str_char[i*2+1];
		uint32_t        wc = ((lo<<8) | hi);

		if (wc < 0x80)
            /* It's an one byte sequence.  */
			*outptr++ = (unsigned char)wc;
		else {
			size_t          step;
			unsigned char           *start;

			for (step = 2; step < 6; ++step)
				if ((wc & encoding_mask[step - 2]) == 0)
					break;

			start = outptr;
			*outptr = encoding_byte[step - 2];
			outptr += step;
			--step;
			do {
				start[step] = 0x80 | (wc & 0x3f);
				wc >>= 6;
			}while (--step > 0);
			start[0] |= wc;
		}
	}
	*outptr = '\0';
	
	return utf8;
}

LPSTR PhoneNumToAscii (LPBYTE p, int len)
{
	int i;
	LPSTR str = (LPSTR) malloc (len * 2 + 10);
	LPSTR basestr = str;
	if (*p++ & 0x50)
		*str++ = '+';
	for (i = 0; i < len; i++)
	{
		BCDTOASCII (p[i], str);
		if (*(str - 1) == '\0')
			break;
	}
	*str = '\0';
	
	return basestr;
}

LPSTR Gsm11SCTStoStr (LPBYTE p, int len)
{
    //e.g 50 70 90 71 81 12 23 to 05-07-09 17:18:12
    LPSTR str = (LPSTR) malloc (32);
    LPSTR basestr = str;

    BCDTOASCII (*p, str);
    p++;
    *str++ = '-';
    BCDTOASCII (*p, str);
    p++;
    *str++ = '-';
    BCDTOASCII (*p, str);
    p++;
    *str++ = ' ';
    BCDTOASCII (*p, str);
    p++;
    *str++ = ':';
    BCDTOASCII (*p, str);
    p++;
    *str++ = ':';
    BCDTOASCII (*p, str);
    *str = '\0';
    return basestr;
}


int DecodeSMS (LPBYTE pRecData, LPSTR *smsc, LPSTR *oa, LPSTR *text, LPSTR *date)
{
	pRecData--;
    	//SMS and the PDU format http://www.dreamfabric.com/sms/
	int smsc_len = pRecData[1];
//    if (smsc_len < 12)          //have meaning data.
	{
        //process smsc
		UCHAR smsc_toa = pRecData[2];
		LPSTR smsc_num = NULL;
		if (smsc_len)
		{
			if (smsc_toa == 0xd0)       //for Alphanumeric address type
			{
				smsc_num = Gsm11Enc7bitToUTF8 (&pRecData[3], smsc_len - 1);
			}
			else
			{
				smsc_num = PhoneNumToAscii (&pRecData[2], smsc_len - 1);
			}
		}
//		fprintf (stderr, "smsc: %s\n", smsc_num);
		//free (smsc_num);
		*smsc = smsc_num;

		UCHAR mr = 0;
		UCHAR vp = 7;           //
		UCHAR pdu = *(pRecData + 2 + smsc_len);
		if ((pdu & 0x03) == 1)  //SMS-SUBMIT (MS ==> SMSC)
		{
			mr = 1;
			if ((pdu & 0x18) == 0)
				vp = 0;
			else if ((pdu & 0x18) == 0x10)
				vp = 1;
		}
		UCHAR smsHead = 0;
		if( (pdu & 0x40) == 0x40 )
		{
			smsHead = 1;
		}
		LPBYTE oa_a = pRecData + 2 + smsc_len + mr + 2;
		int oa_len;
		LPSTR oa_phonenum;
		if (*oa_a == 0xd0)      //for Alphanumeric address type
		{
			oa_len = pRecData[2 + smsc_len + mr + 1] / 2;
			oa_phonenum = (unsigned char*)strdup((char*)Gsm11Enc7bitToUTF8 (oa_a + 1, oa_len));
			oa_len += 1;
		}
		else
		{
			oa_len = (pRecData[2 + smsc_len + mr + 1] + 1) / 2 + 1;
			oa_phonenum = PhoneNumToAscii (oa_a, oa_len - 1);
		}
//		fprintf (stderr, "oa_phonenum: %s\n", oa_phonenum);
		*oa = oa_phonenum;

		LPBYTE udl_a = oa_a + oa_len + 2 + vp;
		int udl = *udl_a;
		if( udl == 0 )
		{
			*text = (unsigned char*)malloc(2);
			if( !(*text) )
			{
				printf("malloc error");
				abort();
			}
			(*text)[0] = 32;
			(*text)[1] = 0;
		}
		else
		{
		
		int smsHeadLen = 0;
		UCHAR smsId = 0;
		UCHAR smsIndexMax = 0;
		UCHAR smsIndex = 0;
		if( smsHead == 1 )
		{
			smsHeadLen = 1+*(udl_a+1);

			if( *(udl_a+2) == 0 )
			{
				smsId = *(udl_a+4);
				smsIndexMax = *(udl_a+5);
				smsIndex = *(udl_a+6);
			}
		}

		if( smsIndexMax > 0 )
		{
			char* pContent = (char*)(udl_a + smsHeadLen + 1);
			int iLen = udl-6;
			UCHAR encoding = *(oa_a + oa_len + 1);
			unsigned char *utf8_str;

			LongSms* pLongSms = (LongSms*)malloc(sizeof(LongSms));
			if( !pLongSms )
			{
				printf("malloc error");
				abort();
			}
			
			memcpy(pLongSms->content, pContent, iLen);
			pLongSms->len = iLen;
			pLongSms->encode = encoding;
			//free(utf8_str);
			utf8_str = NULL;
			pLongSms->id = smsId;
			pLongSms->index = smsIndex;
			pLongSms->indexMax = smsIndexMax;
			pLongSms->pNext = NULL;
			char *pRetContent = NULL;

			int iSmsContentLen = 0;
			appendLongSms(pLongSms, pRetContent, iSmsContentLen);
			
			if( pRetContent )
			{
				if ((encoding & 0xe8) == 0x08)
				{                       //UCS2
					utf8_str = ucs2_to_utf8 ((u_int16_t *)(pRetContent), (iSmsContentLen) / 2);
				}
				else if ((encoding & 0xec) == 0x00 || (encoding & 0xf4) == 0xf0)
				{                       //7bit default alphabet
					utf8_str = Gsm11Enc7bitToUTF8 ((unsigned char*)(pRetContent+1), ((iSmsContentLen-1) * 7) / 8);
					unsigned char* myutf8str = (unsigned char*)malloc(1+iSmsContentLen);
					if( !myutf8str )
					{
						printf("malloc error");
						abort();
					}
					myutf8str[0] = (*pRetContent)>>1;
					myutf8str[1] = 0;
					strcat((char*)myutf8str, (char*)utf8_str);
					free(utf8_str);
					
					utf8_str = myutf8str;
				}
				else if ((encoding & 0xec) == 0x40 || (encoding & 0xf4) == 0xf4)
				{                       //append a space as 8 bit ascii
					*(udl_a + udl + 1) = 0;
					utf8_str = (unsigned char*)malloc(strlen(pRetContent)+1);
					if( !utf8_str )
					{
						printf("malloc error");
						abort();
					}

					strcpy((char*)utf8_str, pRetContent);
				}
				free(pRetContent);
				*text = (unsigned char*)utf8_str;
					
			}
			else
			{
				*text = NULL;
			}
			
		}
		else
		{
			UCHAR encoding = *(oa_a + oa_len + 1);
			unsigned char *utf8_str = NULL;
			if ((encoding & 0xe8) == 0x08)
			{                       //UCS2
				utf8_str = ucs2_to_utf8 ((u_int16_t * )(udl_a + smsHeadLen + 1), udl / 2);
			}
			else if ((encoding & 0xec) == 0x00 || (encoding & 0xf4) == 0xf0)
			{                       //7bit default alphabet
				utf8_str = Gsm11Enc7bitToUTF8 (udl_a + smsHeadLen + 1, (udl * 7 + 7) / 8);
			}
			else if ((encoding & 0xec) == 0x40 || (encoding & 0xf4) == 0xf4)
			{                       //append a space as 8 bit ascii
				*(udl_a + udl + 1) = 0;

				utf8_str = (unsigned char*)malloc(strlen((char*)(udl_a + smsHeadLen + 1))+1);
				if( !utf8_str )
				{
					printf("malloc error");
					abort();
				}

				strcpy((char*)utf8_str, (char*)(udl_a + smsHeadLen + 1));
			}
			 if( !utf8_str )                            
			{                                
				utf8_str = (unsigned char*)malloc(32);                               
				strcpy((char*)utf8_str, "invalid message");                            
			}

			*text = utf8_str;
		}

		}
		
		if (vp == 7)
		{
			LPSTR cstr_date = Gsm11SCTStoStr (udl_a - 7, 7);
			*date = cstr_date;
		}
	}
return 0;
}
