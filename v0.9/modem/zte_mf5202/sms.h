#ifndef SMS_H
#define	SMS_H

typedef unsigned char UCHAR;
typedef unsigned char* LPBYTE;
typedef unsigned char* LPSTR;
typedef unsigned short u_int16_t;
typedef unsigned int	uint32_t;

// Latin-1 undefined character (code 172 (Latin-1 boolean not, "¬"))
#define NOP 172

// GSM undefined character (code 16 (GSM Delta))
#define GSM_NOP 16

#define LOG stdout
#define REALLOC	realloc
#define BUFSIZE (65536+100)

#define BCDTOASCII(a,b) {*b++ = bcdtable[a&0xf]; *b++ = bcdtable[a >> 4];}
#define HEXTOBIN(x) ( (x) >= '0' && (x) <= '9' ? ((x)-'0') : \
                    (x) >= 'A' && (x) <= 'F' ? ((x)-'A'+10) : ((x)-'a'+10))
                    
#define TOHEX(a, b)	{*b++ = hextable[a >> 4];*b++ = hextable[a&0xf];}

static unsigned char gsmToLatin1Table[] = {
    //  0 '@', '£', '$', '¥', 'è', 'é', 'ù', 'ì', 
    '@', 163, '$', 165, 232, 233, 249, 236,
    //  8 'ò', 'Ç',  LF, 'Ø', 'ø',  CR, 'Å', 'å', 
    242, 199, 10, 216, 248, 13, 197, 229,
    // 16 '¬', '_', '¬', '¬', '¬', '¬', '¬', '¬',
    NOP, '_', NOP, NOP, NOP, NOP, NOP, NOP,
    // 24 '¬', '¬', '¬', '¬', 'Æ', 'æ', 'ß', 'É',
    NOP, NOP, NOP, NOP, 198, 230, 223, 201,
    // 32 ' ', '!', '"', '#', '¤', '%', '&', ''',
    ' ', '!', '"', '#', 164, '%', '&', '\'',
    // 40 '(', ')', '*', '+', ',', '-', '.', '/',
    '(', ')', '*', '+', ',', '-', '.', '/',
    // 48 '0', '1', '2', '3', '4', '5', '6', '7',
    '0', '1', '2', '3', '4', '5', '6', '7',
    // 56 '8', '9', ':', ';', '<', '=', '>', '?', 
    '8', '9', ':', ';', '<', '=', '>', '?',
    // 64 '¡', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 
    161, 'A', 'B', 'C', 'D', 'E', 'F', 'G',
    // 72 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    // 80 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    // 88 'X', 'Y', 'Z', 'Ä', 'Ö', 'Ñ', 'Ü', '§', 
    'X', 'Y', 'Z', 196, 214, 209, 220, 167,
    // 96 '¿', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
    191, 'a', 'b', 'c', 'd', 'e', 'f', 'g',
    // 104 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 
    'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    // 112 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
    // 120 'x', 'y', 'z', 'ä', 'ö', 'ñ', 'ü', 'à', 
    'x', 'y', 'z', 228, 246, 241, 252, 224
};

static const uint32_t encoding_mask[] = {
    ~0x7ff, ~0xffff, ~0x1fffff, ~0x3ffffff
};

static const unsigned char encoding_byte[] = {
    0xc0, 0xe0, 0xf0, 0xf8, 0xfc
};

static UCHAR hextable[16] ={ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
static UCHAR bcdtable[16] ={ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '#', 'a', 'b', 'c', '\0'};

struct SmsSendStruct
{
	char *pSendContent;
	int len;
	struct SmsSendStruct* pNext;
};

LPBYTE HexToBin (LPSTR p, int len);
int DecodeSMS (LPBYTE pRecData, LPSTR *smsc, LPSTR *oa, LPSTR *text, LPSTR *date);
//int SendSMS(int fd, char *to, char *text);
int	ComposeSubmitSms (SmsSendStruct *& pSmsSend, char *dst, char *smsc, char *utf8_str);
LPSTR BinToHex (LPBYTE p, int len);
void HexDump(unsigned char *buf, int size);
#endif
