/* vim:set ts=4 shiftwidth=4 */

typedef struct tagVIQE_CMD_Deintl 	*pVIQE_CMD_Deintl;
typedef struct tagVIQE_CMD_Dnts 	*pVIQE_CMD_Dnts;
typedef struct tagVIQE_CMD_Dnsp 	*pVIQE_CMD_Dnsp;
typedef struct tagVIQE_CMD_Hpf 		*pVIQE_CMD_Hpf;
typedef struct tagVIQE_CMD_Hiseq 	*pVIQE_CMD_Hiseq;
typedef struct tagVIQE_CMD_Gamut 	*pVIQE_CMD_Gamut;
typedef struct tagTCC_CUI_VIQE 		*pTCC_CUI_VIQE;

typedef struct tagVIQE_CMD_Deintl	{
	int OnOff; // onoff
	int SpatialFlag; // onoff
	int OddFirstFlag; // not defined
	int ManualModeFlag; // not defined
} VIQE_CMD_Deintl;

typedef struct tagVIQE_CMD_Dnts	{
	int OnOff; // onoff
	int SpatialLumaStrength; // spatial luma strength (0 ~ 4)
	int SpatialChromaStrength; // spatial chroma strength (0 ~ 4)
	int TmpLumaStrength; // temporal luma strength (0 ~ 4)
	int TmpChromaStrength; // temporal chroma strength (0 ~ 4)
} VIQE_CMD_Dnts;

typedef struct tagVIQE_CMD_Dnsp	{
	int OnOff; // onoff
	int Strength; // dnsp strength (0 ~ 4)
} VIQE_CMD_Dnsp;

typedef struct tagVIQE_CMD_Hpf	{
	int OnOff; // onoff
	int LumaStrength; // hpf luma strength (0 ~ 4)
	int ChromaStrength; // hpf chroma strength (0 ~ 4)
} VIQE_CMD_Hpf;

typedef struct tagVIQE_CMD_Hiseq	{
	int OnOff; // onoff
	int Strength [16]; // hiseq strength of each segment (0 ~ 255)
	int MultiFrame; // controls multi-frame profiling histogram equalization (0 ~ 5)
} VIQE_CMD_Hiseq;

typedef struct tagAlg01Info {
	double StrDefault;
	double StrYishG;
	double StrYG;
	double StrGishY;
	double StrY;
	double StrYishO;
	double StrO;
	double StrOPin;
	double StrPin;
	double StrPishPin;
	double StrRishO;
	double StrR;
	double StrPishR;
	double StrRP;
	double StrRishP;
	double StrP;
	double StrBishP;
	double StrPishB;
	double StrB;
	double StrGishB;
	double StrBG;
	double StrBishG;
	double StrG;
} Alg01Info;

typedef struct tagAlg23Info {
	int HueStrDefault;
	int HueStrYishG;
	int HueStrYG;
	int HueStrGishY;
	int HueStrY;
	int HueStrYishO;
	int HueStrO;
	int HueStrOPin;
	int HueStrPin;
	int HueStrPishPin;
	int HueStrRishO;
	int HueStrR;
	int HueStrPishR;
	int HueStrRP;
	int HueStrRishP;
	int HueStrP;
	int HueStrBishP;
	int HueStrPishB;
	int HueStrB;
	int HueStrGishB;
	int HueStrBG;
	int HueStrBishG;
	int HueStrG;

	int SatStrDefault;
	int SatStrYishG;
	int SatStrYG;
	int SatStrGishY;
	int SatStrY;
	int SatStrYishO;
	int SatStrO;
	int SatStrOPin;
	int SatStrPin;
	int SatStrPishPin;
	int SatStrRishO;
	int SatStrR;
	int SatStrPishR;
	int SatStrRP;
	int SatStrRishP;
	int SatStrP;
	int SatStrBishP;
	int SatStrPishB;
	int SatStrB;
	int SatStrGishB;
	int SatStrBG;
	int SatStrBishG;
	int SatStrG;

	int ContStrDefault;
	int ContStrYishG;
	int ContStrYG;
	int ContStrGishY;
	int ContStrY;
	int ContStrYishO;
	int ContStrO;
	int ContStrOPin;
	int ContStrPin;
	int ContStrPishPin;
	int ContStrRishO;
	int ContStrR;
	int ContStrPishR;
	int ContStrRP;
	int ContStrRishP;
	int ContStrP;
	int ContStrBishP;
	int ContStrPishB;
	int ContStrB;
	int ContStrGishB;
	int ContStrBG;
	int ContStrBishG;
	int ContStrG;

	int BrightStrDefault;
	int BrightStrYishG;
	int BrightStrYG;
	int BrightStrGishY;
	int BrightStrY;
	int BrightStrYishO;
	int BrightStrO;
	int BrightStrOPin;
	int BrightStrPin;
	int BrightStrPishPin;
	int BrightStrRishO;
	int BrightStrR;
	int BrightStrPishR;
	int BrightStrRP;
	int BrightStrRishP;
	int BrightStrP;
	int BrightStrBishP;
	int BrightStrPishB;
	int BrightStrB;
	int BrightStrGishB;
	int BrightStrBG;
	int BrightStrBishG;
	int BrightStrG;
} Alg23Info;
	
typedef struct tagVIQE_CMD_Gamut	{
	int OnOff; 
	int AlgorithmNumber; 
	int LUVMode; 
	int LUTUpdated; 
	Alg01Info Alg01;
	Alg23Info Alg23;
} VIQE_CMD_Gamut;

typedef struct tagVIQE_CMD_Huff	{
	int OnOff;
	int Mode;
} VIQE_CMD_Huff;

typedef struct tagTCC_CUI_VIQE	{
	int RandomMode;
	int HalfProcessing;
	int StepOnOff;
	int StepEvent;
	int Monitor;
	int ScaleOnOff;
	int DeblockOnOff;
	VIQE_CMD_Deintl Deintl;
	VIQE_CMD_Dnts Dnts;
	VIQE_CMD_Dnsp Dnsp;
	VIQE_CMD_Hpf Hpf;
	VIQE_CMD_Hiseq Hiseq;
	VIQE_CMD_Gamut Gamut;
	VIQE_CMD_Huff Huff;
} TCC_CUI_VIQE;

extern TCC_CUI_VIQE Tcc_Cui_VIQE;

/*
enum	
{
	TCC_CUI_NULL= 0,
	TCC_CUI_VIQE_SHOW_STATE,
	TCC_CUI_VIQE_TURN,
	TCC_CUI_VIQE_CONF_DEINTL,
	TCC_CUI_VIQE_CONF_DNTS,
	TCC_CUI_VIQE_CONF_DNSP,
	TCC_CUI_VIQE_CONF_HPF,
	TCC_CUI_VIQE_CONF_HISEQ,
	TCC_CUI_VIQE_CONF_GAMUT,
	TCC_CUI_VIQE_CONF_HELP,
	TCC_CUI_NUM
};

typedef struct TCC_CUI_CMD_structure
{
	char *Cmd;
	int	Cmd_Size;
	int  	Index;
}TCC_CUI_CMD_t;
*/
pTCC_CUI_VIQE 		Get_Tcc_Cui_VIQE( void );
void 				TCC_CUI_VIQE_Menu_Display( void );
int 				TCC_CUI_Parse_CMD( char* Buffer );
void 				TCC_CUI_VIQE_ShowDeintl( int En );
void 				TCC_CUI_VIQE_ShowDnts( int En );
void 				TCC_CUI_VIQE_ShowDnsp( int En );
void 				TCC_CUI_VIQE_ShowHpf( int En );
void 				TCC_CUI_VIQE_ShowHiseq( int En );
void 				TCC_CUI_VIQE_ShowGamut( int En );
void 				TCC_CUI_VIQE_ShowRandomMode( int En );
void 				TCC_CUI_VIQE_ShowHalfProcessing( int En );
void 				TCC_CUI_VIQE_ShowState( char *Buffer );
void 				TCC_CUI_VIQE_StepOn( char *Buffer );
void 				TCC_CUI_VIQE_Turn( char *Buffer );
void 				TCC_CUI_VIQE_ConfDeintl( char *Buffer );
void 				TCC_CUI_VIQE_ConfDnts( char *Buffer );
void 				TCC_CUI_VIQE_ConfDnsp( char *Buffer );
void 				TCC_CUI_VIQE_ConfHpf( char *Buffer );
void 				TCC_CUI_VIQE_ConfHiseq( char *Buffer );
void 				TCC_CUI_VIQE_ConfGamut( char *Buffer );
void 				TCC_CUI_VIQE_ConfRandomMode( char *Buffer );
void 				TCC_CUI_VIQE_ConfHalfProcessing(char *Buffer );
