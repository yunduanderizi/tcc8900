#include <stdio.h>
#include <stdlib.h>
#include "gamut_map_internal.h"

pTriangle NewTriangle (
		double wx, double wy,
		enum TriangleType Type,
		double Strength, 
		double Ax, double Ay, 
		double Bx, double By, 
		double Cx, double Cy)
{
	pTriangle TmpTriangle;
	TmpTriangle = (pTriangle) malloc (sizeof (Triangle));
	TmpTriangle -> Type = Type;
	TmpTriangle -> Strength = Strength;
	TmpTriangle -> A.X = Ax - wx;
	TmpTriangle -> A.Y = Ay - wy;
	TmpTriangle -> B.X = Bx - wx;
	TmpTriangle -> B.Y = By - wy;
	TmpTriangle -> C.X = Cx - wx;
	TmpTriangle -> C.Y = Cy - wy;
	TmpTriangle -> Next = NULL;
	return TmpTriangle;
}

void InsertTriangle (pTriangle *Head, pTriangle Tail)
{
	pTriangle Point;
	if (*Head == NULL) 
	{
		*Head = Tail;
		return;
	}
	for (Point = *Head; Point; Point = Point -> Next)
	{
		if (Point -> Next == NULL)
		{
			Point -> Next = Tail;
			break;
		}
	}
	return;
}

void FreeTriangleList (pTriangle PresentPoint)
{
	if (PresentPoint == NULL) return;
	FreeTriangleList (PresentPoint -> Next);
	free (PresentPoint);
	return;
}

pTriangle GetMyTriRegion (pTriangle Head, double X, double Y)
{
	pTriangle Point;
	int IsInTriangle;
//	printf ("X = %f, Y = %f\n", X, Y);
	for (Point = Head; Point; Point = Point -> Next)
	{
		IsInTriangle = PointInTriangle (X, Y, Point);
		if (IsInTriangle == 1)
		{
			return Point;
		}
	}
	return NULL;
}

int PointInTriangle (double cx, double cy, pTriangle CurTriangle)
{
	Vector V0, V1, V2;
	double dot00, dot01, dot02, dot11, dot12;
	double invDenom;
	char Buff [100];
	float X, Y;
	double u, v;
	V0.X = CurTriangle->C.X - CurTriangle->A.X;
	V0.Y = CurTriangle->C.Y - CurTriangle->A.Y;
	V1.X = CurTriangle->B.X - CurTriangle->A.X;
	V1.Y = CurTriangle->B.Y - CurTriangle->A.Y;
	V2.X = cx - CurTriangle->A.X;
	V2.Y = cy - CurTriangle->A.Y;
//	printf ("A.X = %lf, A.Y = %lf, B.X = %lf, B.Y = %lf, C.X = %lf, C.Y = %lf\n", CurTriangle->A.X, CurTriangle->A.Y,
//		CurTriangle->B.X, CurTriangle->B.Y, CurTriangle->C.X, CurTriangle->C.Y);
//	printf ("V0.X = %lf, V0.Y = %lf, V1.X = %lf, V1.Y = %lf, V2.X = %lf, V2.Y = %lf\n", V0.X, V0.Y, V1.X, V1.Y, V2.X, V2.Y);
	dot00 = Dot (V0, V0);
	dot01 = Dot (V0, V1);
	dot02 = Dot (V0, V2);
	dot11 = Dot (V1, V1);
	dot12 = Dot (V1, V2);
	invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	v = (dot00 * dot12 - dot01 * dot02) * invDenom;
//	printf ("cx = %lf, cy = %lf -> ", cx, cy);
//	printf ("invDenom = %lf, u = %lf, v = %lf -> ", invDenom, u, v);
	if ((u > 0) && (v > 0) && (u + v < 1))
	{
//		printf ("Is In Triangle!!\n");
		return 1;
	}
	else 
	{
//		printf ("\n");
		return 0;
	}
}

double Dot (Vector A, Vector B)
{
	return (A.X * B.X + A.Y * B.Y);
}
