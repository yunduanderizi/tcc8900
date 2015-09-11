#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "viqe_lib.h"
#include "dev_viqe.h"
#include "gamut_map_internal.h"

#define IlluminantC 0.3101, 0.3162
#define IlluminantD65   0.3127, 0.3291
#define GAMMA_REC709 0.
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

static struct colourSystem const
    NTSCsystem  =  { "NTSC",               0.67,  0.33,  0.21,  0.71,  0.14,  0.08,  IlluminantC,   GAMMA_REC709},
    EBUsystem   =  { "EBU (PAL/SECAM)",    0.64,  0.33,  0.29,  0.60,  0.15,  0.06,  IlluminantD65, GAMMA_REC709},
    SMPTEsystem =  { "SMPTE",             0.630, 0.340, 0.310, 0.595, 0.155, 0.070, IlluminantD65,  GAMMA_REC709},
    HDTVsystem  =  { "HDTV",              0.670, 0.330, 0.210, 0.710, 0.150, 0.060, IlluminantD65,  GAMMA_REC709},
/* Huh? -ajsh
    CIEsystem   =  { "CIE",              0.7355,0.2645,0.2658,0.7243,0.1669,0.0085, 0.3894,0.3324,  GAMMA_REC709},
*/
    CIEsystem   =  { "CIE",              0.7355,0.2645,0.2658,0.7243,0.1669,0.0085, 0.33333333,0.33333333,  GAMMA_REC709},
    Rec709system = { "CIE REC 709",        0.64,  0.33,  0.30,  0.60,  0.15,  0.06,    IlluminantD65, GAMMA_REC709};

/* Customsystem  is a variable that is initialized to CIE Rec 709, but
   we modify it with information specified by the user's options.
*/
static struct colourSystem Customsystem = 
{ "Custom",             0.64,  0.33,  0.30,  0.60,  0.15,  0.06,  
  IlluminantD65, GAMMA_REC709};

static pixel RGBCube [9][9][9];
static xyzpixel XYZCube [9][9][9];
static xyzpixel XYZCube2 [9][9][9];
static yuvpixel YUVCube [9][9][9];
static unsigned char (*HwLUT3D[8])[3];
static int mem_addr_size [8] = { 126, 100, 100, 80, 100, 80, 80, 64 };
static double XYZ_to_RGB_Matrix[3][3];
static double RGB_to_XYZ_Matrix[3][3];

/*
    Given 1976 coordinates u', v', determine 1931 chromaticities x, y
*/
void upvp_to_xy(double up, double vp, double *xc, double *yc)
{
    *xc = 9*up / (6*up - 16*vp + 12);
    *yc = 4*vp / (6*up - 16*vp + 12);
}

/*
    Given 1931 chromaticities x, y, determine 1976 coordinates u', v'
*/
void xy_to_upvp(double xc, double yc, double *up, double *vp)
{
    *up = 4*xc / (- 2*xc + 12*yc + 3);
    *vp = 9*yc / (- 2*xc + 12*yc + 3);
}

void VIQE_CLUTGen (/*VIPET *HwVIPET1, VIPET *HwVIPET, */struct colourSystem *cs, struct colourSystem *LUVSystem, int LUVMode,
		double DefaultWeight, pTriangle TriangleListHead, yuvpixel YUVCube[9][9][9])
{
	double r, g, b;
	int i, j, k;
	GetCTransMatrix (cs, XYZ_to_RGB_Matrix, RGB_to_XYZ_Matrix);
	InitColourCube (YUVCube);
	YUV_to_RGB_CubeConversion (YUVCube);
	RGB_to_XYZConversion (cs, LUVMode);
	GMappingAlgorithm (LUVSystem, DefaultWeight, TriangleListHead);
	XYZ_to_RGB_CubeConversion (cs, LUVMode);
	RGB_to_YUV_CubeConversion (YUVCube);
}

/*int main ()
{
	 CLUTGen_Interface_Example ();
}*/
void LoadHwLUT3D (VIPET *HwVIPET)
{
	int i, j, k;
	int y, cb, cr;
	int lut_sel;
	int msb_y, msb_u, msb_v;
	int luts_addr[8];
	unsigned int	addr;
	unsigned int	wdata;
// avoid clkgating...
HwVIPET->uTOP_CLKGATE.nTOP_CLKGATE = 0xffff;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			for (k = 0; k < 9; k++)
			{
				y = 32*i;
				cb = 32*j;
				cr = 32*k;
				if (y > 255) y = 255;
				if (cb > 255) cb = 255;
				if (cr > 255) cr = 255;
				
				msb_y = i/*y>>5*/;
				msb_u = j/*cb>>5*/;
				msb_v = k/*cr>>5*/;
				lut_sel = (( msb_y&0x1 )<<2) | (( msb_u&0x1 )<<1) | ( msb_v&0x1 );
				gen_luts_addr( luts_addr, msb_y, msb_u, msb_v );
				
				// writing...
				wdata = (YUVCube[i][j][k].Y&0xff)<<16 |
					(YUVCube[i][j][k].Cb&0xff)<<8 |
					(YUVCube[i][j][k].Cr&0xff);
				addr = ( lut_sel * (1<<9) + luts_addr[lut_sel] * 4 );
//				printf ("%d, %d, %d, %d\n", HwVIPET, addr, 0x1000, (uint)HwVIPET + 0x1000 + addr);
				*(uint *)( (uint)HwVIPET + 0x1000 + addr ) = wdata;
			}
		}
	}
HwVIPET->uTOP_CLKGATE.nTOP_CLKGATE = 0;
//exit (0);
    return;
}


// ------------------------------------------------------------------------------
// An example of gamut mapping algorithm
// 	enhancing the saturation of all colors
//
// 		input : 	strength value - floating value( double )
// 		output :	success or fail(-1)
// ------------------------------------------------------------------------------

double SRGetStrength (struct colourSystem *cs, pTriangle TriangleListHead, int i_r, int i_g, int i_b)
{
	double X, Y, Z;
	double x, y, z;
	double x0, y0;
	enum TriangleType CurTriangleType;
	pTriangle CurTriangle;
	rgb_to_xyz_ntsc (cs, i_r, i_g, i_b, &X, &Y, &Z);
	if (X != 0)
		x = X / (X + Y + Z);	
	else
		x = 0;
	if (Y != 0)
		y = Y / (X + Y + Z);
	else
		y = 0;
	z = 1- (x + y);	
	x0 = cs->xWhite;
	y0 = cs->yWhite;
	CurTriangle = GetMyTriRegion (TriangleListHead, x - x0, y - y0);
//	printf ("CurTriangle = %d\n", CurTriangle);
	if (CurTriangle != NULL)
	{
		CurTriangleType = CurTriangle -> Type;
		if (CurTriangleType == eProtect)
		{
			return 0.0;
		}
		else if (CurTriangleType == eAdjust)
		{
//			printf ("return %f\n", CurTriangle -> Strength);
			return (CurTriangle -> Strength);
		}
		else
		{
			return 0.0;
		}
	}
	else
	{
		return 0.0;
	}
}

int
GamutLUTGenAlg0( /*VIPET *HwVIPET1, VIPET *HwVIPET,*/ 
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
	)	{
	struct colourSystem *cs;
	struct colourSystem CurrentSystem;
	pTriangle TriangleListHead = NULL;
	double xWhite, yWhite;
	int		luts_addr[8];
	//int		addr_lut = 0;
	//int		size_onecomp;
	int		max_val;
	//int		mbits = gamut_map->mbits;
	int		mbits = 3;
	int		y, u, v;
	int		lut_sel;
	int		msb_y, msb_u, msb_v;
	unsigned int		wdata;
	unsigned char		yout, uout, vout;
	unsigned int		addr;
	double				f_r, f_g, f_b;
	double				s, f;
	double				conv_r, conv_g, conv_b;
	double				f_y, f_u, f_v;
	int					i_r, i_g, i_b;
	double				strength_val;
	cs = &NTSCsystem;
	CurrentSystem = *cs;
	xWhite = CurrentSystem.xWhite;
	yWhite = CurrentSystem.yWhite;
	GetCTransMatrix (cs, XYZ_to_RGB_Matrix, RGB_to_XYZ_Matrix);
	MakeTriangleListFromFile (
		LUVMode, NULL, &TriangleListHead, 
		xWhite, yWhite,
		StrYishG, StrYG, StrGishY,
		StrY, StrYishO, StrO,
		StrOPin, StrPin, StrPishPin,
		StrRishO, StrR, StrPishR,
		StrRP, StrRishP, StrP,
		StrBishP, StrPishB, StrB,
		StrGishB, StrBG, StrBishG,
		StrG
	);
	// avoid clkgating...
	// modifying
	//max_val = ( gamut_map->hasLastElem )? 256 : 255;
	max_val = 256;
	for ( y = 0; y <= max_val; y += (1<<(8-mbits)) )	{
		for ( u = 0; u <= max_val; u += (1<<(8-mbits)) )	{
			for ( v = 0; v <= max_val; v += (1<<(8-mbits)) )	{
				msb_y = y>>5;
				msb_u = u>>5;
				msb_v = v>>5;
				lut_sel = (( msb_y&0x1 )<<2) | (( msb_u&0x1 )<<1) | ( msb_v&0x1 );
				gen_luts_addr( luts_addr, msb_y, msb_u, msb_v );
				// convert to RGB
				f_r = 1.164 * ( y - 16 ) + 1.596 * ( v - 128 );
				f_g = 1.164 * ( y - 16 ) - 0.813 * ( v - 128 ) - 0.391 * ( u - 128 );
				f_b = 1.164 * ( y - 16 ) + 2.018 * ( u - 128 );
				// step 1: find f
				f = MAX3( f_r, f_g, f_b ) - MIN3( f_r, f_g, f_b );
				//strength_val = (double)strength / 64;			// 0 ~ 4
				//strength_val = strength;
//				printf ("y = %d, u = %d, v = %d\n", y, u, v);
				i_r = 1.164 * (y - 16) + 1.596 * (v - 128);
				i_g = 1.164 * (y - 16) - 0.813 * (v - 128) - 0.391 * (u - 128);
				i_b = 1.164 * (y - 16) + 2.018 * (u - 128);
//				r = y + 1.371 * (cr - 128);
//				g = y - 0.698 * (cr - 128) - 0.336 * (cb - 128);
//				b = y + 1.732 * (cb - 128);				
				if (i_r > 255) i_r = 255; else if (i_r < 0) i_r = 0;
				if (i_g > 255) i_g = 255; else if (i_g < 0) i_g = 0;
				if (i_b > 255) i_b = 255; else if (i_b < 0) i_b = 0;
//				printf ("r = %d, g = %d, b = %d\n", i_r, i_g, i_b);
				if (AdvancedMode == 1)
					strength_val = SRGetStrength(cs, TriangleListHead, i_r, i_g, i_b);
				else 
					strength_val = StrDefault;
				if ( f > 178 )		s = 0 * strength_val;
				else if ( f > 162 )	s = 0.05 * strength_val;
				else if ( f > 146 ) s = 0.1 * strength_val;
				else if ( f > 130 )	s = 0.15 * strength_val;
				else if ( f > 114 )	s = 0.20 * strength_val;
				else if ( f > 98 )	s = 0.25 * strength_val;
				else if ( f > 82 )	s = 0.3 * strength_val;
				else if ( f > 66 )	s = 0.35 * strength_val;
				else if ( f > 50 )	s = 0.4 * strength_val;
				else if ( f > 34 )	s = 0.45 * strength_val;
				else if ( f > 18 )	s = 0.5 * strength_val;
				else if ( f > 8 )	s = 0.55 * strength_val;
				else 				s = 0.6 * strength_val;
				conv_r = (  1 + s ) * f_r + ( -s / 2 ) * f_g + ( -s / 2 ) * f_b;
				conv_g = ( -s / 2 ) * f_r + (  1 + s ) * f_g + ( -s / 2 ) * f_b;
				conv_b = ( -s / 2 ) * f_r + ( -s / 2 ) * f_g + (  1 + s ) * f_b;
				// convert to YUV
				f_y = (  0.257 * conv_r ) + (  0.504 * conv_g ) + (  0.098 * conv_b ) + 16;
				f_u = ( -0.148 * conv_r ) + ( -0.291 * conv_g ) + (  0.439 * conv_b ) + 128;
				f_v = (  0.439 * conv_r ) + ( -0.368 * conv_g ) + ( -0.071 * conv_b ) + 128;
				// writing...
/*				yout = CLIP3( rint( f_y ), 0, 255 );
				uout = CLIP3( rint( f_u ), 0, 255 );
				vout = CLIP3( rint( f_v ), 0, 255 );*/
				YUVCube[y/32][u/32][v/32].Y = yout;
				YUVCube[y/32][u/32][v/32].Cb = uout;
				YUVCube[y/32][u/32][v/32].Cr = vout;
/*				wdata = yout<<16 | uout<<8 | vout;
				addr = ( lut_sel * (1<<9) + luts_addr[lut_sel] * 4 );
				*(uint *)( (uint)HwVIPET1 + addr ) = wdata;*/
			}
		}
	}
	return 1;
}

#if 1
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
	)
{
	int		luts_addr[8];
	//int		addr_lut = 0;

	//int		size_onecomp;
	int		max_val;
	//int		mbits = gamut_map->mbits;
	int		mbits = 3;
	int		y, u, v, cur_u, cur_v;
	int		lut_sel;

	int		msb_y, msb_u, msb_v;

	unsigned int		wdata;
	unsigned char		yout, uout, vout;
	unsigned int		addr;

	double				f_r, f_g, f_b;
	double				s, f;
	double				conv_r, conv_g, conv_b;
	double				f_y, f_u, f_v;

	double				strength_val;
	int CurY, CurU, CurV;
	double fBright, fCont, fSat, fHue;
	int Cont, Bright, Sat;
	double Hue;
	int Sin, Cos;
	int x, ux;
	unsigned char LUTU[9][9];
	unsigned char LUTV[9][9];
	struct colourSystem *cs;
	struct colourSystem CurrentSystem;
	pTriangle BrightTriangleListHead = NULL;
	pTriangle ContTriangleListHead = NULL;
	pTriangle HueTriangleListHead = NULL;
	pTriangle SatTriangleListHead = NULL;
	double xWhite, yWhite;
	double BrightCurStrength;
	double ContCurStrength;
	double HueCurStrength;
	double SatCurStrength;
	int					i_r, i_g, i_b;
	cs = &NTSCsystem;
	CurrentSystem = *cs;
	xWhite = CurrentSystem.xWhite;
	yWhite = CurrentSystem.yWhite;
	GetCTransMatrix (cs, XYZ_to_RGB_Matrix, RGB_to_XYZ_Matrix);
	printf ("Config Bright Strength\n");
	MakeTriangleListFromFile (
		LUVMode, NULL, &BrightTriangleListHead, 
		xWhite, yWhite,
		BrightStrYishG, BrightStrYG, BrightStrGishY,
		BrightStrY, BrightStrYishO, BrightStrO,
		BrightStrOPin, BrightStrPin, BrightStrPishPin,
		BrightStrRishO, BrightStrR, BrightStrPishR,
		BrightStrRP, BrightStrRishP, BrightStrP,
		BrightStrBishP, BrightStrPishB, BrightStrB,
		BrightStrGishB, BrightStrBG, BrightStrBishG,
		BrightStrG
	);
	printf ("Config Contrast Strength\n");
	MakeTriangleListFromFile (
		LUVMode, NULL, &ContTriangleListHead, 
		xWhite, yWhite,
		ContStrYishG, ContStrYG, ContStrGishY,
		ContStrY, ContStrYishO, ContStrO,
		ContStrOPin, ContStrPin, ContStrPishPin,
		ContStrRishO, ContStrR, ContStrPishR,
		ContStrRP, ContStrRishP, ContStrP,
		ContStrBishP, ContStrPishB, ContStrB,
		ContStrGishB, ContStrBG, ContStrBishG,
		ContStrG
	);
	printf ("Config Hue Strength\n");
	MakeTriangleListFromFile (
		LUVMode, NULL, &HueTriangleListHead, 
		xWhite, yWhite,
		HueStrYishG, HueStrYG, HueStrGishY,
		HueStrY, HueStrYishO, HueStrO,
		HueStrOPin, HueStrPin, HueStrPishPin,
		HueStrRishO, HueStrR, HueStrPishR,
		HueStrRP, HueStrRishP, HueStrP,
		HueStrBishP, HueStrPishB, HueStrB,
		HueStrGishB, HueStrBG, HueStrBishG,
		HueStrG
	);
	printf ("Config Saturation Strength\n");
	MakeTriangleListFromFile (
		LUVMode, NULL, &SatTriangleListHead, 
		xWhite, yWhite,
		SatStrYishG, SatStrYG, SatStrGishY,
		SatStrY, SatStrYishO, SatStrO,
		SatStrOPin, SatStrPin, SatStrPishPin,
		SatStrRishO, SatStrR, SatStrPishR,
		SatStrRP, SatStrRishP, SatStrP,
		SatStrBishP, SatStrPishB, SatStrB,
		SatStrGishB, SatStrBG, SatStrBishG,
		SatStrG
	);
	// modifying
	//max_val = ( gamut_map->hasLastElem )? 256 : 255;
	max_val = 256;
/*	fBright = (double)iBright * 255.0 / 100.0;
	fCont = (double)iCont / 100.0;
	fHue = (double) iHue;
	fSat = (double) iSat / 100.0;
	Cont = (int) (fCont * 512);
	Bright = (int) fBright;
	Sat = (int) (fSat * 512);
	Hue = (fHue * 3.1415926) / 180.0;
	Sin = (int)(sin(Hue) * 4096);
	Cos = (int)(cos(Hue) * 4096);
	for (y = 0; y <= 256; y+=32)
	{
		for (x = 0; x <= 256; x+=32)
		{
			u = x - 128; 
			v = y - 128;
			ux = (u * Cos + v * Sin) >> 12;
			v = (v * Cos - u * Sin) >> 12;
			u = ((ux * Sat) >> 9) + 128;
			v = ((v * Sat) >> 9) + 128;
			u = min(max(u, 16), 235);
			v = min(max(v, 16), 235);
//			u = min(max(u, 0), 255);
//			v = min(max(v, 0), 255);
			LUTU[y/32][x/32] = u;
			LUTV[y/32][x/32] = v;
		}
	}
	*/
	for ( y = 0; y <= max_val; y += (1<<(8-mbits)) )	{
		for ( u = 0; u <= max_val; u += (1<<(8-mbits)) )	{
			for ( v = 0; v <= max_val; v += (1<<(8-mbits)) )	{
				i_r = 1.164 * (y - 16) + 1.596 * (v - 128);
				i_g = 1.164 * (y - 16) - 0.813 * (v - 128) - 0.391 * (u - 128);
				i_b = 1.164 * (y - 16) + 2.018 * (u - 128);
				if (i_r > 255) i_r = 255; else if (i_r < 0) i_r = 0;
				if (i_g > 255) i_g = 255; else if (i_g < 0) i_g = 0;
				if (i_b > 255) i_b = 255; else if (i_b < 0) i_b = 0;
				if (AdvancedMode == 1)
				{
					BrightCurStrength = SRGetStrength(cs, BrightTriangleListHead, i_r, i_g, i_b);
					ContCurStrength = SRGetStrength(cs, ContTriangleListHead, i_r, i_g, i_b) + 100.0;
					HueCurStrength = SRGetStrength(cs, HueTriangleListHead, i_r, i_g, i_b);
					SatCurStrength = SRGetStrength(cs, SatTriangleListHead, i_r, i_g, i_b) + 100.0;
				}
				else
				{
					BrightCurStrength = BrightStrDefault;
					ContCurStrength = ContStrDefault + 100.0;
					HueCurStrength = HueStrDefault;
					SatCurStrength = SatStrDefault + 100.0;
				}
					
				if (1)//(BrightCurStrength != 0.0 || ContCurStrength != 0.0 || HueCurStrength != 0.0 || SatCurStrength != 0.0)
				{
//					printf ("BrightStr = %lf, ContStr = %lf, HueStr = %lf, SatStr = %lf\n", BrightCurStrength, ContCurStrength, HueCurStrength, SatCurStrength);
					{
						fBright = (double)BrightCurStrength * 255.0 / 100.0;
						fCont = (double)ContCurStrength / 100.0;
						fHue = (double) HueCurStrength;
						fSat = (double) SatCurStrength / 100.0;
						Cont = (int) (fCont * 512);
						Bright = (int) fBright;
						Sat = (int) (fSat * 512);
						Hue = (fHue * 3.1415926) / 180.0;
						/*Sin = (int)(sin(Hue) * 4096);
						Cos = (int)(cos(Hue) * 4096);*/
						cur_u = u - 128; 
						cur_v = v - 128;
						ux = (cur_u * Cos + cur_v * Sin) >> 12;
						cur_v = (cur_v * Cos - cur_u * Sin) >> 12;
						cur_u = ((ux * Sat) >> 9) + 128;
						cur_v = ((cur_v * Sat) >> 9) + 128;
//						cur_u = min(max(cur_u, 16), 235);
//						cur_v = min(max(cur_v, 16), 235);
						cur_u = min(max(cur_u, 0), 255);
						cur_v = min(max(cur_v, 0), 255);
//						LUTU[y/32][x/32] = cur_u;
//						LUTV[y/32][x/32] = v;
					}
					msb_y = y>>5;
					msb_u = u>>5;
					msb_v = v>>5;
					lut_sel = (( msb_y&0x1 )<<2) | (( msb_u&0x1 )<<1) | ( msb_v&0x1 );
					gen_luts_addr( luts_addr, msb_y, msb_u, msb_v );
					CurY = ((Cont * (y - 16)) >> 9) + Bright + 16;
					CurY = min(max(CurY, 0), 255);
					YUVCube[y/32][u/32][v/32].Y = CurY;
					YUVCube[y/32][u/32][v/32].Cb = cur_u;
					YUVCube[y/32][u/32][v/32].Cr = cur_v;
//					printf ("u : %d -> %d, v : %d -> %d\n", u, cur_u, v, cur_v);
				}
/*				else
				{
					YUVCube[y/32][u/32][v/32].Y = min(max(y, 0), 255);
					YUVCube[y/32][u/32][v/32].Cb = u;
					YUVCube[y/32][u/32][v/32].Cr = v;
				}*/
			}
		}
	}
}
#endif


int
GamutLUTGenAlg1( void ) {
	int                       luts_addr[8];
	int                       max_val;
	int                       mbits = 3;
	int                       y, u, v;
	int                       lut_sel;
	int                       msb_y, msb_u, msb_v;
	unsigned int                     wdata;
	unsigned char                 yout, uout, vout;
	unsigned int                     addr;
	double                                          f_r, f_g, f_b;
	double                                          s, f;
	double                                          conv_r, conv_g, conv_b;
	double                                          f_y, f_u, f_v;
	double                                          strength_val;
              // avoid clkgating...
	// modifying
	//max_val = ( gamut_map->hasLastElem )? 256 : 255;
	max_val = 256;
	for ( y = 0; y <= max_val; y += (1<<(8-mbits)) )          {
		for ( u = 0; u <= max_val; u += (1<<(8-mbits)) )         {
			for ( v = 0; v <= max_val; v += (1<<(8-mbits)) )           {
				msb_y = y>>5;
				msb_u = u>>5;
				msb_v = v>>5;
				lut_sel = (( msb_y&0x1 )<<2) | (( msb_u&0x1 )<<1) | ( msb_v&0x1 );
				gen_luts_addr( luts_addr, msb_y, msb_u, msb_v );
// f_y = (  1.000 * (double)y ) + (  0.1314 * (double)u ) + (  -0.1661 * (double)v );
//f_u = (  0.000 * (double)y ) + (  0.7151 * (double)u ) + (  0.0958 * (double)v );
//f_v = (  0.000 * (double)y ) + ( -0.0958 * (double)u ) + ( 1.3856 * (double)v );
//				f_y = (  1.000 * (double)y ) + (  -0.2541 * (double)u ) + (  0.3213 * (double)v );
//				f_u = (  0.000 * (double)y ) + (  0.9375 * (double)u ) + (  -0.1853 * (double)v );
//				f_v = (  0.000 * (double)y ) + (  0.1853 * (double)u ) + ( 1.0301 * (double)v );
				f_y = (  1.000 * ( y - 16 ) ) + (-0.2541 * ( u - 128 ) ) + (  0.3213 * ( v - 128 ) ) + 16;
				f_u = (  0.000 * ( y - 16 ) ) + (  0.9375 * ( u - 128 ) ) + (-0.1853 * ( v - 128 ) ) + 128;
				f_v = (  0.000 * ( y - 16 ) ) + (  0.1853 * ( u - 128 ) ) + (  1.0301 * ( v - 128 ) ) + 128;
				// writing...
/*				yout = CLIP3( rint( f_y ), 0, 255 );
				uout = CLIP3( rint( f_u ), 0, 255 );
				vout = CLIP3( rint( f_v ), 0, 255 );*/
				YUVCube[y/32][u/32][v/32].Y = yout;
				YUVCube[y/32][u/32][v/32].Cb = uout;
				YUVCube[y/32][u/32][v/32].Cr = vout;
/*				wdata = yout<<16 | uout<<8 | vout;
				addr = ( lut_sel * (1<<9) + luts_addr[lut_sel] * 4 );
				*(uint *)( (uint)HwVIPET1 + addr ) = wdata;*/
			}
		}
	}
	return 1;
}

#if 0
void GamutLUTGenAlg2 (int iBright, int iCont, int iHue, int iSat)
{
	int		luts_addr[8];
	//int		addr_lut = 0;

	//int		size_onecomp;
	int		max_val;
	//int		mbits = gamut_map->mbits;
	int		mbits = 3;
	int		y, u, v;
	int		lut_sel;

	int		msb_y, msb_u, msb_v;

	unsigned int		wdata;
	unsigned char		yout, uout, vout;
	unsigned int		addr;

	double				f_r, f_g, f_b;
	double				s, f;
	double				conv_r, conv_g, conv_b;
	double				f_y, f_u, f_v;

	double				strength_val;
	int CurY, CurU, CurV;
	double fBright, fCont, fSat, fHue;
	int Cont, Bright, Sat;
	double Hue;
	int Sin, Cos;
	int x, ux;
	unsigned char LUTU[9][9];
	unsigned char LUTV[9][9];
	// modifying
	//max_val = ( gamut_map->hasLastElem )? 256 : 255;
	max_val = 256;
	fBright = (double)iBright * 255.0 / 100.0;
	fCont = (double)iCont / 100.0;
	fHue = (double) iHue;
	fSat = (double) iSat / 100.0;
	Cont = (int) (fCont * 512);
	Bright = (int) fBright;
	Sat = (int) (fSat * 512);
	Hue = (fHue * 3.1415926) / 180.0;
	Sin = (int)(sin(Hue) * 4096);
	Cos = (int)(cos(Hue) * 4096);
	for (y = 0; y <= 256; y+=32)
	{
		for (x = 0; x <= 256; x+=32)
		{
			u = x - 128; 
			v = y - 128;
			ux = (u * Cos + v * Sin) >> 12;
			v = (v * Cos - u * Sin) >> 12;
			u = ((ux * Sat) >> 9) + 128;
			v = ((v * Sat) >> 9) + 128;
			u = min(max(u, 16), 235);
			v = min(max(v, 16), 235);
//			u = min(max(u, 0), 255);
//			v = min(max(v, 0), 255);
			LUTU[y/32][x/32] = u;
			LUTV[y/32][x/32] = v;
		}
	}
	for ( y = 0; y <= max_val; y += (1<<(8-mbits)) )	{
		for ( u = 0; u <= max_val; u += (1<<(8-mbits)) )	{
			for ( v = 0; v <= max_val; v += (1<<(8-mbits)) )	{

				msb_y = y>>5;
				msb_u = u>>5;
				msb_v = v>>5;
				lut_sel = (( msb_y&0x1 )<<2) | (( msb_u&0x1 )<<1) | ( msb_v&0x1 );
				gen_luts_addr( luts_addr, msb_y, msb_u, msb_v );
				CurY = ((Cont * (y - 16)) >> 9) + Bright + 16;
				CurY = min(max(CurY, 0), 255);
				YUVCube[y/32][u/32][v/32].Y = CurY;
				YUVCube[y/32][u/32][v/32].Cb = LUTU[v/32][u/32];
				YUVCube[y/32][u/32][v/32].Cr = LUTV[v/32][u/32];
			}
		}
	}
}
#endif

void MakeTriangleListFromFile (int LUVMode, FILE *Input, pTriangle *TriangleListHead, 
		double xWhite, double yWhite,
		double StrYishG, double StrYG, double StrGishY,
		double StrY, double StrYishO, double StrO,
		double StrOPin, double StrPin, double StrPishPin,
		double StrRishO, double StrR, double StrPishR,
		double StrRP, double StrRishP, double StrP,
		double StrBishP, double StrPishB, double StrB,
		double StrGishB, double StrBG, double StrBishG,
		double StrG
		)
{
	char TriangleInput[22][100] = 
	{
		"-square -name:G -A:0.15:0.81 -B:0.01:0.51 -C:0.26:0.36 -D:0.28:0.38 -XSTR:100.0\0",
		"-square -name:BishG -A:0.01:0.51 -B:0.03:0.37 -C:0.23:0.33 -D:0.26:0.36 -XSTR:100.0\0",
		"-square -name:BG -A:0.03:0.37 -B:0.06:0.25 -C:0.22:0.30 -D:0.23:0.33 -XSTR:100.0\0",
		"-square -name:GishB -A:0.06:0.25 -B:0.08:0.13 -C:0.20:0.23 -D:0.22:0.30 -XSTR:100.0\0",
		"-square -name:B -A:0.08:0.13 -B:0.13:0.04 -C:0.21:0.20 -D:0.20:0.23 -XSTR:100.0\0",
		"-square -name:PishB -A:0.13:0.04 -B:0.15:0.01 -C:0.23:0.19 -D:0.21:0.20 -XSTR:100.0\0",
		"-square -name:BishP -A:0.15:0.01 -B:0.21:0.02 -C:0.25:0.19 -D:0.23:0.19 -XSTR:100.0\0",
		"-square -name:P -A:0.21:0.02 -B:0.28:0.05 -C:0.29:0.20 -D:0.25:0.19 -XSTR:100.0\0",
		"-square -name:RishP -A:0.28:0.05 -B:0.32:0.07 -C:0.32:0.22 -D:0.29:0.20 -XSTR:100.0\0",
		"-square -name:PishPin -A:0.31:0.26 -B:0.32:0.19 -C:0.48:0.26 -D:0.37:0.30 -XSTR:100.0\0",
		"-square -name:Pin -A:0.37:0.30 -B:0.48:0.26 -C:0.54:0.33 -D:0.40:0.33 -XSTR:100.0\0",
		"-square -name:OPin -A:0.40:0.33 -B:0.54:0.33 -C:0.52:0.38 -D:0.43:0.38 -XSTR:100.0\0",
		"-square -name:RP -A:0.32:0.19 -B:0.32:0.07 -C:0.44:0.12 -D:0.38:0.215 -XSTR:100.0\0",
		"-square -name:PishR -A:0.38:0.215 -B:0.44:0.12 -C:0.63:0.22 -D:0.48:0.26 -XSTR:100.0\0",
		"-square -name:R -A:0.48:0.26 -B:0.63:0.22 -C:0.73:0.28 -D:0.54:0.33 -XSTR:100.0\0",
		"-square -name:RishO -A:0.54:0.33 -B:0.73:0.28 -C:0.61:0.39 -D:0.52:0.38 -XSTR:100.0\0",
		"-square -name:O -A:0.48:0.38 -B:0.61:0.39 -C:0.55:0.44 -D:0.48:0.42 -XSTR:100.0\0",
		"-square -name:YishO -A:0.48:0.42 -B:0.55:0.44 -C:0.53:0.47 -D:0.46:0.44 -XSTR:100.0\0",
		"-square -name:Y -A:0.46:0.44 -B:0.53:0.47 -C:0.48:0.52 -D:0.41:0.45 -XSTR:100.0\0",
		"-square -name:GishY -A:0.41:0.45 -B:0.48:0.52 -C:0.43:0.56 -D:0.38:0.43 -XSTR:100.0\0",
		"-square -name:YG -A:0.38:0.43 -B:0.43:0.56 -C:0.34:0.66 -D:0.33:0.41 -XSTR:100.0\0",
		"-square -name:YishG -A:0.33:0.41 -B:0.34:0.66 -C:0.15:0.81 -D:0.28:0.38 -XSTR:100.0\0",
	};
	char *Buffer;
	char ColorName[10];
	double Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
	double Strength;
	int NameFlag;
	int SquareFlag;
	int i;
	pTriangle TmpTriangle;
	enum TriangleType Type;
	char *StartPoint;

	for (i = 0; i < 22; i++)
	{
		Buffer = TriangleInput[i];
		if (!strncmp (Buffer, "//", 2))
		{
			continue;
		}
//		printf ("Buffer = %s\n", Buffer);
		StartPoint = strstr (Buffer, "-square");
		if (StartPoint) SquareFlag = 1;
		else SquareFlag = 0;
		StartPoint = strstr (Buffer, "-name:");
		if (StartPoint)
		{
			sscanf (StartPoint + 6, "%s", &ColorName);
//			printf ("color name = %s\n", ColorName);
			NameFlag = 1;
		}
		else
		{
			NameFlag = 0;
		}
		StartPoint = strstr (Buffer, "-A:");
		if (StartPoint)
			sscanf (StartPoint + 3, "%lf:%lf", &Ax, &Ay);
		StartPoint = strstr (Buffer, "-B:");
		if (StartPoint)
			sscanf (StartPoint + 3, "%lf:%lf", &Bx, &By);
		StartPoint = strstr (Buffer, "-C:");
		if (StartPoint)
			sscanf (StartPoint + 3, "%lf:%lf", &Cx, &Cy);
		StartPoint = strstr (Buffer, "-D:");
		if (StartPoint)
			sscanf (StartPoint + 3, "%lf:%lf", &Dx, &Dy);
		Strength = 0.0;
		if (NameFlag == 1)
		{
			printf ("ColorName = %s\n", ColorName);
			if (!strcmp (ColorName, "YishG")) Strength = StrYishG;
			else if (!strcmp (ColorName, "YG")) Strength = StrYG;
			else if (!strcmp (ColorName, "GishY")) Strength = StrGishY;
			else if (!strcmp (ColorName, "Y")) Strength = StrY;
			else if (!strcmp (ColorName, "YishO")) Strength = StrYishO;
			else if (!strcmp (ColorName, "O")) Strength = StrO;
			else if (!strcmp (ColorName, "OPin")) Strength = StrOPin;
			else if (!strcmp (ColorName, "Pin")) Strength = StrPin;
			else if (!strcmp (ColorName, "PishPin")) Strength = StrPishPin;
			else if (!strcmp (ColorName, "RishO")) Strength = StrRishO;
			else if (!strcmp (ColorName, "R")) Strength = StrR;
			else if (!strcmp (ColorName, "PishR")) Strength = StrPishR;
			else if (!strcmp (ColorName, "RP")) Strength = StrRP;
			else if (!strcmp (ColorName, "RishP")) Strength = StrRishP;
			else if (!strcmp (ColorName, "P")) Strength = StrP;
			else if (!strcmp (ColorName, "BishP")) Strength = StrBishP;
			else if (!strcmp (ColorName, "PishB")) Strength = StrPishB;
			else if (!strcmp (ColorName, "B")) Strength = StrB;
			else if (!strcmp (ColorName, "GishB")) Strength = StrGishB;
			else if (!strcmp (ColorName, "BG")) Strength = StrBG;
			else if (!strcmp (ColorName, "BishG")) Strength = StrBishG;
			else if (!strcmp (ColorName, "G")) Strength = StrG;
		}
		StartPoint = strstr (Buffer, "-STR:");
		if (StartPoint && Strength == 0.0)
		{
			sscanf (StartPoint + 5, "%lf", &Strength);
		}
		printf ("-Square = %d, -A:%lf:%lf -B%lf:%lf -C:%lf:%lf -D:%lf:%lf -STR:%lf\n", SquareFlag, Ax, Ay, Bx, By, Cx, Cy, Dx, Dy, Strength);
/*		Ax /= 10; Ay /= 10;
		Bx /= 10; By /= 10;
		Cx /= 10; Cy /= 10;
		Dx /= 10; Dy /= 10;*/
		if (LUVMode == 1)
		{
			xy_to_upvp (Ax, Ay, &Ax, &Ay);
			xy_to_upvp (Bx, By, &Bx, &By);
			xy_to_upvp (Cx, Cy, &Cx, &Cy);
			xy_to_upvp (Dx, Dy, &Dx, &Dy);
		}
				
		if (Strength == 0.0) Type = eProtect;
		else Type = eAdjust;
		TmpTriangle = NewTriangle 
		(
			xWhite, yWhite,
		 	Type, Strength, 
			Ax, Ay,
			Bx, By,
			Cx, Cy
		);
		InsertTriangle (TriangleListHead, TmpTriangle);
		TmpTriangle = NewTriangle 
		(
			xWhite, yWhite,
		 	Type, Strength, 
			Cx, Cy,
			Dx, Dy,
			Ax, Ay
		);
		InsertTriangle (TriangleListHead, TmpTriangle);
	}
	return;
}

void GamutLUTGenAlg3 (
	/*VIPET *HwVIPET1, VIPET *HwVIPET,*/ int LUVMode,
	double StrDefault,
	double StrYishG, double StrYG, double StrGishY,
	double StrY, double StrYishO, double StrO,
	double StrOPin, double StrPin, double StrPishPin,
	double StrRishO, double StrR, double StrPishR,
	double StrRP, double StrRishP, double StrP,
	double StrBishP, double StrPishB, double StrB,
	double StrGishB, double StrBG, double StrBishG,
	double StrG
	)
{
//	int LUVMode = 1;
	pTriangle TriangleListHead = NULL;
	pTriangle TmpTriangle;
	double DefaultWeight = StrDefault;
	double xBlue, yBlue, xGreen, yGreen, xRed, yRed;
	double xWhite, yWhite;
	double xCyan1, yCyan1, xCyan2, yCyan2;
	double xYellow1, yYellow1, xYellow2, yYellow2;
	double xMagen1, yMagen1, xMagen2, yMagen2;
	struct colourSystem *cs;
	struct colourSystem CurrentSystem;
	FILE *Input;
//	Input = fopen ("triangle.cie", "rb");
	cs = &NTSCsystem;
	CurrentSystem = *cs;
	if (LUVMode == 1)
	{
		xy_to_upvp (CurrentSystem.xBlue, CurrentSystem.yBlue, 
			&(CurrentSystem.xBlue), &(CurrentSystem.yBlue));
		xy_to_upvp (CurrentSystem.xRed, CurrentSystem.yRed, 
			&(CurrentSystem.xRed), &(CurrentSystem.yRed));
		xy_to_upvp (CurrentSystem.xGreen, CurrentSystem.yGreen, 
			&(CurrentSystem.xGreen), &(CurrentSystem.yGreen));
		xy_to_upvp (CurrentSystem.xWhite, CurrentSystem.yWhite, 
			&(CurrentSystem.xWhite), &(CurrentSystem.yWhite));
	}
	
	xBlue = CurrentSystem.xBlue;
	yBlue = CurrentSystem.yBlue;
	xGreen = CurrentSystem.xGreen;
	yGreen = CurrentSystem.yGreen;
	xRed = CurrentSystem.xRed;
	yRed = CurrentSystem.yRed;
	xWhite = CurrentSystem.xWhite;
	yWhite = CurrentSystem.yWhite;
	MakeTriangleListFromFile (
		LUVMode, Input, &TriangleListHead, 
		xWhite, yWhite,
		StrYishG, StrYG, StrGishY,
		StrY, StrYishO, StrO,
		StrOPin, StrPin, StrPishPin,
		StrRishO, StrR, StrPishR,
		StrRP, StrRishP, StrP,
		StrBishP, StrPishB, StrB,
		StrGishB, StrBG, StrBishG,
		StrG
	);
	VIQE_CLUTGen (/*HwVIPET1, HwVIPET,*/ cs, &CurrentSystem, LUVMode, DefaultWeight, TriangleListHead, YUVCube);
	FreeTriangleList (TriangleListHead);
	TriangleListHead = NULL;
//	fclose (Input);
}

void YUV_to_RGB_CubeConversion (yuvpixel YUVCube[9][9][9])
{
	int r, g, b;
	int i, j, k;
	int y, cb, cr;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			for (k = 0; k < 9; k++)
			{
				y = YUVCube[i][j][k].Y;
				cb = YUVCube[i][j][k].Cb;
				cr = YUVCube[i][j][k].Cr;
				r = 1.164 * (y - 16) + 1.596 * (cr - 128);
				g = 1.164 * (y - 16) - 0.813 * (cr - 128) - 0.391 * (cb - 128);
				b = 1.164 * (y - 16) + 2.018 * (cb - 128);
//				r = y + 1.371 * (cr - 128);
//				g = y - 0.698 * (cr - 128) - 0.336 * (cb - 128);
//				b = y + 1.732 * (cb - 128);				
				if (r > 255) r = 255; else if (r < 0) r = 0;
				if (g > 255) g = 255; else if (g < 0) g = 0;
				if (b > 255) b = 255; else if (b < 0) b = 0;
				RGBCube[i][j][k].r = r;
				RGBCube[i][j][k].g = g;
				RGBCube[i][j][k].b = b;
			}
		}
	}
	return;
}

void RGB_to_YUV_CubeConversion (yuvpixel YUVCube[9][9][9])
{
	int i, j, k;
	double dY, dCb, dCr;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			for (k = 0; k < 9; k++)
			{
				dY = 0.257*RGBCube[i][j][k].r + 0.504*RGBCube[i][j][k].g + 0.098*RGBCube[i][j][k].b + 16;	
				dCb = (-0.148)*RGBCube[i][j][k].r - 0.291*RGBCube[i][j][k].g + 0.439*RGBCube[i][j][k].b + 128;	
				dCr = 0.439*RGBCube[i][j][k].r - 0.368*RGBCube[i][j][k].g - 0.071*RGBCube[i][j][k].b + 128;
//				dY = 0.299*RGBCube[i][j][k].r + 0.587*RGBCube[i][j][k].g + 0.114*RGBCube[i][j][k].b;	
//				dCb = (-0.172)*RGBCube[i][j][k].r - 0.339*RGBCube[i][j][k].g + 0.511*RGBCube[i][j][k].b + 128;	
//				dCr = 0.511*RGBCube[i][j][k].r - 0.428*RGBCube[i][j][k].g - 0.083*RGBCube[i][j][k].b + 128;

				if (dY > 255.0) dY = 255.0; 
				else if (dY < 0.0) dY = 0.0;
				if (dCb > 255.0) dCb = 255.0; 
				else if (dCb < 0.0) dCb = 0.0;
				if (dCr > 255.0) dCr = 255.0;	
				else if (dCr < 0.0) dCr = 0.0;
				YUVCube[i][j][k].Y = (int) dY;
				YUVCube[i][j][k].Cb = (int) dCb;
				YUVCube[i][j][k].Cr = (int) dCr;
			}
		}
	}
	return;
}

void GMappingAlgorithm (struct colourSystem *cs, double DefaultWeight, pTriangle TriangleListHead)
{
	int i, j, k;
	int Region;
	double m1, m2, m3;
	double n1, n2, n3;
	double x0, y0, xc, yc, xr, yr, xg, yg, xb, yb;
	double dc, di, d1, d2, dt;
	double Weight;
	double xy, xt, yt, zt;
	int IsInTriangle;
	pTriangle CurTriangle;
	enum TriangleType CurTriangleType;

	double Tmp1;
	double Tmp2;
	double tmpx;
	double tmpy;
	double xi;
	double yi;
	double nx;
	double mx;
	double s;
	
	x0 = cs->xWhite;
	y0 = cs->yWhite;
	xr = cs->xRed - x0;
	yr = cs->yRed - y0;
	xg = cs->xGreen - x0;
	yg = cs->yGreen - y0;
	xb = cs->xBlue - x0;
	yb = cs->yBlue - y0;
	m1 = (yg - yr) / (xg - xr);
	m2 = (yb - yg) / (xb - xg);
	m3 = (yr - yb) / (xr - xb);
	n1 = yr - (xr)*m1;
	n2 = yg - (xg)*m2;
	n3 = yb - (xb)*m3;
	
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			for (k = 0; k < 9; k++)
			{
				xc = XYZCube[i][j][k].x - x0;
				yc = XYZCube[i][j][k].y - y0;
				CurTriangle = GetMyTriRegion (TriangleListHead, xc, yc);
				if (CurTriangle != NULL)
				{
					CurTriangleType = CurTriangle -> Type;
					if (CurTriangleType == eProtect)
					{
						XYZCube2[i][j][k].x = XYZCube[i][j][k].x;
						XYZCube2[i][j][k].y = XYZCube[i][j][k].y;
						XYZCube2[i][j][k].z = XYZCube[i][j][k].z;
						XYZCube2[i][j][k].Y = XYZCube[i][j][k].Y;
						continue;

					}
					else if (CurTriangleType == eAdjust)
					{
						Weight = CurTriangle -> Strength;
					}
					else
					{
						Weight = DefaultWeight;
						if (Weight == 0.0)
						{
							XYZCube2[i][j][k].x = XYZCube[i][j][k].x;
							XYZCube2[i][j][k].y = XYZCube[i][j][k].y;
							XYZCube2[i][j][k].z = XYZCube[i][j][k].z;
							XYZCube2[i][j][k].Y = XYZCube[i][j][k].Y;
							continue;
						}
					}
				}
				else
				{
					Weight = DefaultWeight;
					if (Weight == 0.0)
					{
						XYZCube2[i][j][k].x = XYZCube[i][j][k].x;
						XYZCube2[i][j][k].y = XYZCube[i][j][k].y;
						XYZCube2[i][j][k].z = XYZCube[i][j][k].z;
						XYZCube2[i][j][k].Y = XYZCube[i][j][k].Y;
						continue;
					}
				}
				Region = GetRegion (cs, xc, yc, x0, y0);
//				dc = sqrt (xc*xc + yc*yc);
				if (xc != 0)
				{
					switch (Region)
					{
						case 1 : xy = Calculate_xy (xc, yc, m1, n1, x0, y0); break;
						case 2 : xy = Calculate_xy (xc, yc, m2, n2, x0, y0); break;
						case 3 : xy = Calculate_xy (xc, yc, m3, n3, x0, y0); break;
					}
					
//					Tmp1 = sqrt (1.0 + (yc*yc)/(xc*xc));
					Tmp2 = AbsF(xy);
					di = Tmp1 * Tmp2;
					
					s = yc / xc;
					switch (Region)
					{
						case 1 : nx = n1; mx = m1; break;
						case 2 : nx = n2; mx = m2; break;
						case 3 : nx = n3; mx = m3; break;
					}
					xi = nx / (s - mx);
					yi = (s*nx) / (s - mx);
//					di = sqrt (xi*xi + yi*yi);
				}
				else
				{
					switch (Region)
					{
						case 1 : di = AbsF(n1); break;
						case 2 : di = AbsF(n2); break;
						case 3 : di = AbsF(n3); break;
					}
				}
				d1 = 0.0;
				d2 = di;
				dt = Calculate_dt (Weight, d1, d2, dc);
				CalculateTarget (xc, yc, dt, dc, &xt, &yt, &zt, x0, y0); 
	//			printf ("%d >> di = %f, xc = %f, yc = %f, xt = %f, yt = %f\n", Region, di, xc+x0, yc+y0, xt, yt);
				XYZCube2[i][j][k].x = xt;
				XYZCube2[i][j][k].y = yt;
				XYZCube2[i][j][k].z = zt;
				XYZCube2[i][j][k].Y = XYZCube[i][j][k].Y;
			}
		}
	}
	return;
}

double Calculate_dt (double Weight, double d1, double d2, double dc)
{
	double TmpDt;
/*	if (dc <= d1 || dc > d2) return dc;
	else return (d2 - ((((d2 - dc)/(2*(d2 - d1)) + Weight) * (d2 - dc))/(Weight + 1)));*/
/*	if (dc > 0)
	{
		return (d2 - ((((d2 - dc)/(d2 - d1) + Weight) * (d2 - dc))/(Weight + 1)));
	}
	else
	{
		return (d2 - ((((dc - d2)/(d2 - d1) + Weight) * (d2 - dc))/(Weight + 1)));
	}*/
	if (dc <= d1 || dc > d2) return dc;
	else //if (Weight >= 0)
	{
		TmpDt = (d2 - ((((d2 - dc)/((d2 - d1)) + 4) * (d2 - dc))/(5 + Weight*dc)));
		return TmpDt;
	}
/*	else if (Weight < 0)
	{
		TmpDt = (d2 - ((((d2 - dc)/((d2 - d1)) + 4) * (d2 - dc))/(5 + (-Weight)*dc)));
		TmpDt -= dc;
		TmpDt = dc - TmpDt;
		return TmpDt;
	}*/
}

void CalculateTarget (double xc, double yc, double dt, double dc, double *xt, double *yt, double *zt, double x0, double y0)
{
	if (dc != 0)
	{
		*xt = ((xc*dt)/dc) + x0;
		*yt = ((yc*dt)/dc) + y0;
		*zt = 1 - (*xt) - (*yt);
	}
	else
	{
		*xt = xc + x0;
		*yt = yc + y0;
		*zt = 1 - (*xt) - (*yt);
	}
	return;
}

double AbsF (double X)
{
	if (X < 0) return -X;
	else return X;
}

double Calculate_xy (double xc, double yc, double m, double n, double x0, double y0)
{
	double s;
	s = yc / xc;
	if (AbsF(yc) > AbsF(xc))
	{
		return ((m*x0 + n - y0)/(1 - m*s));
	}
	else
	{
		return ((m*x0 + n - y0)/(s - m));
	}
}

int GetRegion (struct colourSystem *cs, double x, double y, double x0, double y0)
{
	double PI = 3.14;
	double AngleC, AngleR, AngleG, AngleB;
	AngleC = ATan2 (y, x);
	AngleR = ATan2 (cs->yRed - y0, cs->xRed - x0);
	AngleG = ATan2 (cs->yGreen - y0, cs->xGreen - x0);
	AngleB = ATan2 (cs->yBlue - y0, cs->xBlue - x0);
	if (AngleC >= AngleR && AngleC < AngleG) return 1;
	else if (AngleC >= AngleG && AngleC < AngleB) return 2;
	else if ((AngleC >= AngleB && AngleC <= 2*PI) || (AngleC >=0 && AngleC < AngleR)) return 3;
}

double ATan2 (double y, double x)
{
	double PI = 3.14;
//	if (y >= 0) return atan2 (y, x);
//	else return 2*PI + atan2 (y, x);
	return 0.0;
}

void XYZ_to_RGB_CubeConversion (struct colourSystem *cs, int LUVMode)
{
	int i, j, k;
	int tmp_r, tmp_g, tmp_b;
	double jr, jg, jb, jmax;
	int mx;
	double xtgt, ytgt, ztgt, Y;
	double Xo, Yo, Zo;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			for (k = 0; k < 9; k++)
			{
				if (LUVMode == 1)
				{
//					upvp_to_xy (XYZCube2[i][j][k].x, XYZCube2[i][j][k].y, &(XYZCube2[i][j][k].x), &(XYZCube2[i][j][k].y));
					upvp_to_xy (XYZCube2[i][j][k].x, XYZCube2[i][j][k].y, &xtgt, &ytgt);
				}
				else
				{
					xtgt = XYZCube2[i][j][k].x;
					ytgt = XYZCube2[i][j][k].y;
				}
				ztgt = 1 - xtgt - ytgt;
//				ztgt = XYZCube2[i][j][k].z;
/*				xtgt = XYZCube[i][j][k].x;
				ytgt = XYZCube[i][j][k].y;
				ztgt = XYZCube[i][j][k].z;*/
//				Y = (XYZCube2[i][j][k].Y);
				Y = (XYZCube2[i][j][k].Y);
				Xo = (xtgt / ytgt) * Y;
				Yo = Y;
				Zo = (ztgt / ytgt) * Y;
/*				jr = 1.9100 * Xo - 0.5325 * Yo - 0.2882 * Zo;
				jg = -0.9847 * Xo + 1.9992 * Yo - 0.0283 * Zo;
				jb = 0.0583 * Xo - 0.1184 * Yo + 0.8976 * Zo;*/
/*				jr = 1.9099961 * Xo -0.5324542 * Yo - 0.2882091 * Zo;
				jg = -0.9846663 * Xo + 1.9991710 * Yo - 0.0283082 * Zo;
				jb = 0.0583056 * Xo - 0.1183781 * Yo + 0.8975535 * Zo;*/
				jr = XYZ_to_RGB_Matrix[0][0] * Xo + XYZ_to_RGB_Matrix[0][1] * Yo + XYZ_to_RGB_Matrix[0][2] * Zo;
				jg = XYZ_to_RGB_Matrix[1][0] * Xo + XYZ_to_RGB_Matrix[1][1] * Yo + XYZ_to_RGB_Matrix[1][2] * Zo;
				jb = XYZ_to_RGB_Matrix[2][0] * Xo + XYZ_to_RGB_Matrix[2][1] * Yo + XYZ_to_RGB_Matrix[2][2] * Zo;
//				linear_to_srgb (&jr, &jg, &jb);
				constrain_rgb_gamut (&jr, &jg, &jb);
				gamma_correct_rgb_gamut (cs, &jr, &jg, &jb);
				tmp_r = (int) 255*jr;
				tmp_g = (int) 255*jg;
				tmp_b = (int) 255*jb;
/*				RGBCube[i][j][k].r = (BYTE) 255*jr;
				RGBCube[i][j][k].g = (BYTE) 255*jg;
				RGBCube[i][j][k].b = (BYTE) 255*jb;*/
				if (tmp_r > 255) tmp_r = 255;
				else if (tmp_r < 0) tmp_r = 0;
				if (tmp_g > 255) tmp_g = 255;
				else if (tmp_g < 0) tmp_g = 0;
				if (tmp_b > 255) tmp_b = 255;
				else if (tmp_b < 0) tmp_b = 0;
				RGBCube[i][j][k].r = (BYTE)tmp_r;
				RGBCube[i][j][k].g = (BYTE)tmp_g;
				RGBCube[i][j][k].b = (BYTE)tmp_b;
			}
		}
	}
	return;
}

void linear_to_srgb (double *r, double *g, double *b)
{
	srgb_trans (r);
	srgb_trans (g);
	srgb_trans (b);
	return;
}

void srgb_trans (double *c)
{
	double a = 0.055;
	if (*c <= 0.0031308) *c = 12.92 * (*c);
//	else *c = (1+a)*(pow(*c, 1/2.4)) - a;
	if (*c > 255) *c = 255;
	else if (*c < 0) *c = 0;
	return;
}

void gamma_correct_rgb_gamut (const struct colourSystem *cs, double *r, double *g, double *b)
{
	gamma_correct_gamut (cs, r);
	gamma_correct_gamut (cs, g);
	gamma_correct_gamut (cs, b);
	return;
}

void gamma_correct_gamut (const struct colourSystem *cs, double *c)
{
	double gamma;
	gamma = cs -> gamma;
/*	if (gamma == 0.)
	{
		double cc = 0.018;
		if (*c < cc)
			*c *= (1.099 * pow(cc, 0.45) - 0.099) / cc;
		else 
			*c = 1.099 * pow (*c, 0.45) - 0.099;
	}
	else
	{
		*c = pow (*c, 1./gamma);
	}*/
	return;
}

void RGB_to_XYZConversion (struct colourSystem *cs, int LUVMode)
{
	int i, j, k;
	double X, Y, Z;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			for (k = 0; k < 9; k++)		
			{
				rgb_to_xyz_ntsc (cs, RGBCube[i][j][k].r, RGBCube[i][j][k].g, RGBCube[i][j][k].b, &X, &Y, &Z);
				if (X != 0)
					XYZCube[i][j][k].x = X / (X + Y + Z);	
				else
					XYZCube[i][j][k].x = 0;
				if (Y != 0)
					XYZCube[i][j][k].y = Y / (X + Y + Z);
				else
					XYZCube[i][j][k].y = 0;
				XYZCube[i][j][k].z = 1- (XYZCube[i][j][k].x + XYZCube[i][j][k].y);	
				XYZCube[i][j][k].Y = Y;
				if (LUVMode == 1)
				{
					xy_to_upvp (XYZCube[i][j][k].x, XYZCube[i][j][k].y, &(XYZCube[i][j][k].x), &(XYZCube[i][j][k].y));
				}
			}
		}
	}
	return;
}

void InitColourCube (yuvpixel YUVCube[9][9][9])
{
	int i, j, k;
	int tmp_r, tmp_g, tmp_b;
	for (i = 0; i < 9; i ++)
	{
		for (j = 0; j < 9; j++)
		{
			for (k = 0; k < 9; k++)
			{
				YUVCube [i][j][k].Y = i * 32;
				YUVCube [i][j][k].Cb = j * 32;
				YUVCube [i][j][k].Cr = k * 32;
				if (YUVCube [i][j][k].Y > 255) YUVCube [i][j][k].Y = 255;
				if (YUVCube [i][j][k].Cb > 255) YUVCube [i][j][k].Cb = 255;
				if (YUVCube [i][j][k].Cr > 255) YUVCube [i][j][k].Cr = 255;
				tmp_r = i * 32;
				tmp_g = j * 32;
				tmp_b = k * 32;
/*				RGBCube [i][j][k].r = i * 32;
				RGBCube [i][j][k].g = j * 32;
				RGBCube [i][j][k].b = k * 32;
				if (RGBCube [i][j][k].r > 255) RGBCube [i][j][k].r = 255;
				if (RGBCube [i][j][k].g > 255) RGBCube [i][j][k].g = 255;
				if (RGBCube [i][j][k].b > 255) RGBCube [i][j][k].b = 255;*/
				if (tmp_r > 255) tmp_r = 255;
				if (tmp_g > 255) tmp_g = 255;
				if (tmp_b > 255) tmp_b = 255;
				RGBCube [i][j][k].r = tmp_r;
				RGBCube [i][j][k].g = tmp_g;
				RGBCube [i][j][k].b = tmp_b;
			}
		} 
	}
	return;
}

void HoSepia (int R, int G, int B, double *HR, double *HG, double *HB)
{
	double incR, incG, incB;
	*HR   = R*0.393 + G*0.769 + B*0.189;
	*HG = R*0.349 + G*0.686 + B*0.168;
	*HB  = R*0.272 + G*0.534 + B*0.131;
	return;
}

void HoBluish (int R, int G, int B, double *HR, double *HG, double *HB)
{
	double TmpH;
	double TmpS;
	double TmpI;
	int MinC;

	TmpH = 240.0;
	if (R <= G) MinC = R;
	else MinC = G;
	if (MinC <= B) MinC = MinC;
	else MinC = B;
	if (R == 0 && G == 0 && B == 0)
	{
		TmpS = 0.0;
	}
	else
	{
		TmpS = 1.0 - (((double) 3 / (double) (R + G + B))*(double) MinC);
	}
	TmpI = (double) (R +  G + B) / (double) 3;
	TmpI += (double) 100;
	*HG = TmpI * (1 - TmpS);
	*HB = TmpI * (1+ (TmpS*2));
	*HR = 3 * TmpI - (*HG + *HB);
	if (*HR > 255) *HR = 255.0;
	else if (*HR < 0) *HR = 0;
	if (*HG > 255) *HG = 255.0;
	else if (*HG < 0) *HG = 0;
	if (*HB > 255) *HB = 255.0;
	else if (*HB < 0) *HB = 0;
	return;
}

void rgb_to_xyz_ntsc (const struct colourSystem *cs, int R, int G, int B, double *X, double *Y, double *Z)
{
	double dr, dg, db;
	double r, g, b, ciex, ciey, ciez;
	double gamma;
	double cc;
	gamma = cs -> gamma;
	cc = 0.018;
	double HR;
	double HG;
	double HB;
	
//	HoSepia (R, G, B, &HR, &HG, &HB);
//	HoBluish (R, G, B, &HR, &HG, &HB);
	
	dr = (double) R / 255.0;
	dg = (double) G / 255.0;
	db = (double) B / 255.0;

	nonlinear_to_linear_rgb (gamma, &dr);
	nonlinear_to_linear_rgb (gamma, &dg);
	nonlinear_to_linear_rgb (gamma, &db);
	constrain_rgb_gamut (&dr, &dg, &db);

/*	r = pow ((R / 255.0 + 0.055) / 1.055, 2.4);
	g = pow ((G / 255.0 + 0.055) / 1.055, 2.4);
	b = pow ((B / 255.0 + 0.055) / 1.055, 2.4);*/
/*	*X = 0.6069 * r + 0.1735 * g + 0.2003 * b;
	*Y = 0.2989 * r + 0.5866 * g + 0.1145 * b;
	*Z = 0.0000 * r + 0.0661 * g + 1.1162 * b;*/
	*X = RGB_to_XYZ_Matrix[0][0] * dr + RGB_to_XYZ_Matrix[0][1] * dg + RGB_to_XYZ_Matrix[0][2] * db;
	*Y = RGB_to_XYZ_Matrix[1][0] * dr + RGB_to_XYZ_Matrix[1][1] * dg + RGB_to_XYZ_Matrix[1][2] * db;
	*Z = RGB_to_XYZ_Matrix[2][0] * dr + RGB_to_XYZ_Matrix[2][1] * dg + RGB_to_XYZ_Matrix[2][2] * db;
	return;
}

int constrain_rgb_gamut(r, g, b)
  double *r, *g, *b;
{
    double w;

    /* Amount of white needed is w = - min(0, *r, *g, *b) */
    w = (0 < *r) ? 0 : *r;
    w = (w < *g) ? w : *g;
    w = (w < *b) ? w : *b;
    w = - w;

    /* Add just enough white to make r, g, b all positive. */
    if (w > 0) {
        *r += w;  *g += w; *b += w;

        return 1;                     /* Colour modified to fit RGB gamut */
    }

    return 0;                         /* Colour within RGB gamut */
}

void nonlinear_to_linear_rgb (double gamma, double *c)
{
/*	if (gamma == 0.)
	{
		double cc = 0.018;
		if (*c < cc)
			*c *= cc / (1.099 * pow(cc, 0.45) - 0.099);
		else 
			*c = pow ((*c + 0.099)/1.099, 1/0.45);
	}
	else
	{
		*c = pow (*c, gamma);
	}*/
	return;
}
