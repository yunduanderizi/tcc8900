#ifndef _GAMUT_MAP_H_
#define _GAMUT_MAP_H_
#include <stdio.h>
#include <stdlib.h>
#include "dev_viqe.h"
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned long  LONG;
typedef unsigned char  BYTE;
typedef unsigned int gray;
typedef gray pixval;

struct colourSystem {
	const char *name;
	double xRed, yRed;
	double xGreen, yGreen;
	double xBlue, yBlue;
	double xWhite, yWhite;
	double gamma;
};

typedef struct
{
	BYTE r, g, b;
} pixel;

typedef struct
{
	double x, y, z;
	double Y;
} xyzpixel;

typedef struct
{
	int Y, Cb, Cr;
} yuvpixel;

typedef struct 
{
	double X;
	double Y;
} Vector;

typedef struct tagTriangle *pTriangle;

enum TriangleType { eProtect, eAdjust };

typedef struct tagTriangle { 
	enum TriangleType Type;
	double Strength;
	Vector A;
	Vector B;
	Vector C;
	pTriangle Next;
} Triangle;

void LoadHwLUT3D (VIPET *HwVIPET);
int GamutLUTGenAlg0( /*VIPET *HwVIPET1, VIPET *HwVIPET,*/ 
	int AdvancedMode, 
	int LUVMode,
	double StrDefault,
	double StrYishG, double StrYG, double StrGishY,
	double StrY, double StrYishO, double StrO,
	double StrOPin, double StrPin, double StrPishPin,
	double StrRishO, double StrR, double StrPishR,
	double StrRP, double StrRishP, double StrP,
	double StrBishP, double StrPishB, double StrB,
	double StrGishB, double StrBG, double StrBishG,
	double StrG
	);

int GamutLUTGenAlg1();

void GamutLUTGenAlg2 (
	int AdvancedMode, 
	int LUVMode,
	int BrightStrDefault, 
	int BrightStrYishG, int BrightStrYG, int BrightStrGishY,
	int BrightStrY, int BrightStrYishO, int BrightStrO,
	int BrightStrOPin, int BrightStrPin, int BrightStrPishPin,
	int BrightStrRishO, int BrightStrR, int BrightStrPishR,
	int BrightStrRP, int BrightStrRishP, int BrightStrP,
	int BrightStrBishP, int BrightStrPishB, int BrightStrB,
	int BrightStrGishB, int BrightStrBG, int BrightStrBishG,
	int BrightStrG,

	int ContStrDefault, 
	int ContStrYishG, int ContStrYG, int ContStrGishY,
	int ContStrY, int ContStrYishO, int ContStrO,
	int ContStrOPin, int ContStrPin, int ContStrPishPin,
	int ContStrRishO, int ContStrR, int ContStrPishR,
	int ContStrRP, int ContStrRishP, int ContStrP,
	int ContStrBishP, int ContStrPishB, int ContStrB,
	int ContStrGishB, int ContStrBG, int ContStrBishG,
	int ContStrG,

	int HueStrDefault,
	int HueStrYishG, int HueStrYG, int HueStrGishY,
	int HueStrY, int HueStrYishO, int HueStrO,
	int HueStrOPin, int HueStrPin, int HueStrPishPin,
	int HueStrRishO, int HueStrR, int HueStrPishR,
	int HueStrRP, int HueStrRishP, int HueStrP,
	int HueStrBishP, int HueStrPishB, int HueStrB,
	int HueStrGishB, int HueStrBG, int HueStrBishG,
	int HueStrG,

	int SatStrDefault, 
	int SatStrYishG, int SatStrYG, int SatStrGishY,
	int SatStrY, int SatStrYishO, int SatStrO,
	int SatStrOPin, int SatStrPin, int SatStrPishPin,
	int SatStrRishO, int SatStrR, int SatStrPishR,
	int SatStrRP, int SatStrRishP, int SatStrP,
	int SatStrBishP, int SatStrPishB, int SatStrB,
	int SatStrGishB, int SatStrBG, int SatStrBishG,
	int SatStrG
	);
#endif
