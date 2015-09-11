#ifndef _GAMUT_MAP_INTERNAL_H_
#define _GAMUT_MAP_INTERNAL_H_
#include "dev_viqe.h"
#include "gamut_map.h"
void upvp_to_xy(double up, double vp, double *xc, double *yc);
void xy_to_upvp(double xc, double yc, double *up, double *vp);
void VIQE_CLUTGen (/*VIPET *HwVIPET1, VIPET *HwVIPET, */struct colourSystem *cs, struct colourSystem *LUVSystem, int LUVMode,
		double DefaultWeight, pTriangle TriangleListHead, yuvpixel YUVCube[9][9][9]);
void YUV_to_RGB_CubeConversion (yuvpixel YUVCube[9][9][9]);
void RGB_to_YUV_CubeConversion (yuvpixel YUVCube[9][9][9]);
void GMappingAlgorithm (struct colourSystem *cs, double DefaultWeight, pTriangle TriangleListHead);
double Calculate_dt (double Weight, double d1, double d2, double dc);
void CalculateTarget (double xc, double yc, double dt, double dc, double *xt, double *yt, double *zt, double x0, double y0);
double AbsF (double X);
double Calculate_xy (double xc, double yc, double m, double n, double x0, double y0);
int GetRegion (struct colourSystem *cs, double x, double y, double x0, double y0);
double ATan2 (double y, double x);
void XYZ_to_RGB_CubeConversion (struct colourSystem *cs, int LUVMode);
void linear_to_srgb (double *r, double *g, double *b);
void srgb_trans (double *c);
void gamma_correct_rgb_gamut (const struct colourSystem *cs, double *r, double *g, double *b);
void gamma_correct_gamut (const struct colourSystem *cs, double *c);
void RGB_to_XYZConversion (struct colourSystem *cs, int LUVMode);
void InitColourCube (yuvpixel YUVCube[9][9][9]);
void HoSepia (int R, int G, int B, double *HR, double *HG, double *HB);
void HoBluish (int R, int G, int B, double *HR, double *HG, double *HB);
void rgb_to_xyz_ntsc (const struct colourSystem *cs, int R, int G, int B, double *X, double *Y, double *Z);
int constrain_rgb_gamut(double *r, double *g, double *b);
void nonlinear_to_linear_rgb (double gamma, double *c);
void GetCTransMatrix (
	struct colourSystem *cs, 
	double XYZ_to_RGB_Matrix[3][3],
	double RGB_to_XYZ_Matrix[3][3] 
);
pTriangle NewTriangle (
		double wx, double wy,
		enum TriangleType Type,
		double Strength, 
		double Ax, double Ay, 
		double Bx, double By, 
		double Cx, double Cy);
void InsertTriangle (pTriangle *Head, pTriangle Tail);
void FreeTriangleList (pTriangle PresentPoint);
pTriangle GetMyTriRegion (pTriangle Head, double X, double Y);
int PointInTriangle (double cx, double cy, pTriangle CurTriangle);
double Dot (Vector A, Vector B);
#endif
