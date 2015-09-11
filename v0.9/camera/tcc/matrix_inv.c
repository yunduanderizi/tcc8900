  
#include <stdio.h>   
#include <stdlib.h> // malloc   
#include "gamut_map_internal.h"
  
static double  
Determinant(double** matrixA, int uesrN);   
  
static int  
InverseMatrix(double** matrixA, double** inversedMatrixA, int userN,   
                double determinantResult);   
  
static int  
MultiplicationMatrix(double** matrixA, double** matrixB, double** resultMatrix, int userN);   
static void MatrixInv(double matrix_in [3][3], double matrix_out [3][3]);
static void Get_XYZ_to_RGB_Matrix (
	struct colourSystem *cs, 
	double RGB_to_XYZ_Matrix[3][3]
);

/*int main ()
{
	struct colourSystem *cs;
	double XYZ_to_RGB_Matrix[3][3];
	double RGB_to_XYZ_Matrix[3][3];
	int i, j;
	cs = &NTSCsystem;
	GetCTransMatrix (cs, XYZ_to_RGB_Matrix, RGB_to_XYZ_Matrix);
	return 0;
}
*/
void GetCTransMatrix (
	struct colourSystem *cs, 
	double XYZ_to_RGB_Matrix[3][3],
	double RGB_to_XYZ_Matrix[3][3] 
)
{
	int i, j;
	Get_XYZ_to_RGB_Matrix (cs, XYZ_to_RGB_Matrix);
	MatrixInv (XYZ_to_RGB_Matrix, RGB_to_XYZ_Matrix);
/*	printf ("XYZ_to_RGB Matrix = \n");
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			printf ("\t%f", XYZ_to_RGB_Matrix[i][j]);
		}
		printf ("\n");
	}
	printf ("RGB_to_XYZ Matrix = \n");
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			printf ("\t%f", RGB_to_XYZ_Matrix[i][j]);
		}
		printf ("\n");
	}*/
	return;
}

static void Get_XYZ_to_RGB_Matrix (
	struct colourSystem *cs, 
	double RGB_to_XYZ_Matrix[3][3]
)
{
    double xr, yr, zr, xg, yg, zg, xb, yb, zb;
    double xw, yw, zw;
    double rx, ry, rz, gx, gy, gz, bx, by, bz;
    double rw, gw, bw;

    xr = cs->xRed;    yr = cs->yRed;    zr = 1 - (xr + yr);
    xg = cs->xGreen;  yg = cs->yGreen;  zg = 1 - (xg + yg);
    xb = cs->xBlue;   yb = cs->yBlue;   zb = 1 - (xb + yb);

    xw = cs->xWhite;  yw = cs->yWhite;  zw = 1 - (xw + yw);

    /* xyz -> rgb matrix, before scaling to white. */
    rx = yg*zb - yb*zg;  ry = xb*zg - xg*zb;  rz = xg*yb - xb*yg;
    gx = yb*zr - yr*zb;  gy = xr*zb - xb*zr;  gz = xb*yr - xr*yb;
    bx = yr*zg - yg*zr;  by = xg*zr - xr*zg;  bz = xr*yg - xg*yr;

    /* White scaling factors.
       Dividing by yw scales the white luminance to unity, as conventional. */
    rw = (rx*xw + ry*yw + rz*zw) / yw;
    gw = (gx*xw + gy*yw + gz*zw) / yw;
    bw = (bx*xw + by*yw + bz*zw) / yw;

    /* xyz -> rgb matrix, correctly scaled to white. */
    rx = rx / rw;  ry = ry / rw;  rz = rz / rw;
    gx = gx / gw;  gy = gy / gw;  gz = gz / gw;
    bx = bx / bw;  by = by / bw;  bz = bz / bw;
    RGB_to_XYZ_Matrix[0][0] = rx;
    RGB_to_XYZ_Matrix[0][1] = ry;
    RGB_to_XYZ_Matrix[0][2] = rz;
    RGB_to_XYZ_Matrix[1][0] = gx;
    RGB_to_XYZ_Matrix[1][1] = gy;
    RGB_to_XYZ_Matrix[1][2] = gz;
    RGB_to_XYZ_Matrix[2][0] = bx;
    RGB_to_XYZ_Matrix[2][1] = by;
    RGB_to_XYZ_Matrix[2][2] = bz;
}

static void MatrixInv(double matrix_in [3][3], double matrix_out [3][3])   
{   
    int userN = 3;   
    int userM = 2;   
    int row = 0;   
    int column = 0;   
  
    double** matrixA;   
    double** inversedMatrixA;   
    double** multiplicationalMatrixA;   
    double determinantResult = 0;   
  
    matrixA = (double**)malloc(sizeof(double*) * userN);   
    for (row = 0; row < userN; row++)   
    {   
        matrixA[row] = (double*)malloc(sizeof(double) * userN);   
    } /* for (row = 0; row < userN; row++) */  
  
    for (row = 0; row < userN; row++)   
    {   
        for (column = 0; column < userN; column++)   
        {   
            matrixA[row][column] = matrix_in[row][column];
        } /* for (column = 0; column < userN; column++) */  
    } /* for (row = 0; row < userN; row++) */  
 
    determinantResult = Determinant(matrixA, userN);   
  
    if (!determinantResult)   
    {   
        printf("\nNo determinant(A)\n");   
    }
    else  
    {   
        inversedMatrixA = (double**)malloc(sizeof(double*) * userN);   
        multiplicationalMatrixA = (double**)malloc(sizeof(double*) * userN);   
  
        for (row = 0; row < userN; row++)   
        {   
            inversedMatrixA[row] = (double*)malloc(sizeof(double) * userN);   
            multiplicationalMatrixA[row] =   
                                        (double*)malloc(sizeof(double) * userN);   
        }
  
        InverseMatrix(matrixA, inversedMatrixA, userN, determinantResult);   
  
        for (row = 0; row < userN; row++)   
        {   
            for (column = 0; column < userN; column++)   
            {   
		matrix_out[row][column] = inversedMatrixA[row][column];
            }
        }
  
/*        MultiplicationMatrix(matrixA, inversedMatrixA,   
                                multiplicationalMatrixA, userN);   
  
        printf("\nA'A = \n");   
        for (row = 0; row < userN; row++)   
        {   
            for (column = 0; column < userN; column++)   
            {   
                printf("%15.5lf ", multiplicationalMatrixA[row][column]);   
                if (column % userN == userM)   
                {   
                    printf("\n");   
                }
            }
        }
*/  
        for(row = 0; row < userN; row++)   
        {   
            free(multiplicationalMatrixA[row]);   
            free(inversedMatrixA[row]);   
        } /* for(row = 0; row < userN; row++) */  
  
        free(multiplicationalMatrixA);   
        free(inversedMatrixA);   
    }   
  
    for(row = 0; row < userN; row++)   
    {   
        free(matrixA[row]);   
    } /* for(row = 0; row < userN; row++) */  
    free(matrixA);   
}   
  
static double  
Determinant (double** matrixA, int userN)   
{   
    int userM = 0;   
    int counter = 0;   
    int determinantACheck = 0;   
    int determinantARow = 0;   
    int determinantAColumn = 0;   
    int sign = 1;   
  
    double determinantResult = 0;   
    double** matrixDeterminantA;   
  
    userM = userN-1;   
  
    matrixDeterminantA = (double**)malloc(sizeof(double*) * userM);   
    for (counter = 0; counter < userM; counter++)   
    {   
        matrixDeterminantA[counter] = (double*)malloc(sizeof(double) * userM);   
    } /* for (counter = 0; counter < userM; counter++) */  
  
    if (userN == 2)   
    {   
        determinantResult = (matrixA[0][0] * matrixA[1][1]) -   
                                (matrixA[0][1] * matrixA[1][0]);   
    } /* if (userN == 2) */  
  
    else  
    {   
        for (counter = 0; counter < userN; counter++)   
        {   
            for (determinantARow = 0; determinantARow < userM;   
                    determinantARow++)   
            {   
                determinantACheck = 0;   
                for (determinantAColumn = 0; determinantAColumn < userM;   
                        determinantAColumn++)   
                {   
                    if (determinantAColumn == counter)   
                    {   
                        determinantACheck++;   
                    } /* if (determinantAColumn == counter) */  
                    matrixDeterminantA[determinantARow][determinantAColumn] =   
                            matrixA[determinantARow + 1][determinantACheck];   
                    determinantACheck++;   
                } /* for (determinantAColumn = 0; determinantAColumn < userM;  
                        determinantAColumn++) */  
            } /* for (determinantARow = 0; determinantARow < userM;  
                    determinantARow++) */  
  
            if(counter % 2 == 1)   
            {   
                sign = -1;   
            } /* if(counter % 2 == 1) */  
            else  
            {   
                sign = 1;   
            } /* else */  
  
            determinantResult += sign * matrixA[0][counter] *   
                                    Determinant(matrixDeterminantA,userM);   
        } /* for (counter = 0; counter < userN; counter++) */  
    } /* else */  
  
  
    for(counter = 0; counter < userM; counter++)   
    {   
        free(matrixDeterminantA[counter]);   
    } /* for(counter = 0; counter < userM; counter++) */  
    free(matrixDeterminantA);   
  
    return determinantResult;   
}   
  
static int  
InverseMatrix(double** matrixA, double** inversedMatrixA, int userN,   
                double determinantResult)   
{   
    int userM = 0;   
    int row = 0;   
    int column = 0;   
    int cofactorRowCheck=0;   
    int cofactorColumnCheck=0;   
    int cofactorRow=0;   
    int cofactorColumn=0;   
    int sign = 1;   
  
    double temporaryValue = 0;   
    double** determinantCofactorMatrixA;   
    double** cofactorMatrixA;   
  
    userM = userN - 1;   
  
    cofactorMatrixA = (double**)malloc(sizeof(double*) * userN);   
    for(row = 0; row < userN; row++)   
    {   
        cofactorMatrixA[row] = (double*)malloc(sizeof(double) * userN);   
    } /* for(row = 0; row < userN; row++) */  
  
    determinantCofactorMatrixA = (double**)malloc(sizeof(double*) * userM);   
    for(row = 0; row < userM; row++)   
    {   
        determinantCofactorMatrixA[row] =   
                (double*)malloc(sizeof(double) * userM);   
    } /* for(row = 0; row < userM; row++) */  
  
    /* Find the Cofactor */  
  
    if (userN == 2)   
    {   
        cofactorMatrixA[0][0] = (matrixA[0][0] * matrixA[1][1]) -   
                                (matrixA[0][1] * matrixA[1][0]);   
    } /* if (userN == 2) */  
    else  
    {   
        for (row = 0; row < userN; row++)   
        {   
            for (column = 0; column < userN; column++)   
            {   
                cofactorColumnCheck=0;   
                for (cofactorRow = 0; cofactorRow < userM; cofactorRow++)   
                {   
                    if(cofactorColumnCheck == column)   
                    {   
                        cofactorColumnCheck++;   
                    } /* if(cofactorColumnCheck == column) */  
                    cofactorRowCheck=0;   
                    for (cofactorColumn = 0; cofactorColumn <userM;   
                            cofactorColumn++)   
                    {   
                        if (cofactorColumn == row)   
                        {   
                            cofactorRowCheck++;   
                        } /* if (cofactorColumn == row) */  
                        determinantCofactorMatrixA[cofactorRow]   
                            [cofactorColumn] =   
                            matrixA[cofactorColumnCheck][cofactorRowCheck];   
                        cofactorRowCheck++;   
                    } /* for (cofactorColumn = 0; cofactorColumn <userM;  
                            cofactorColumn++) */  
                    cofactorColumnCheck++;   
                } /* for (cofactorRow = 0; cofactorRow < userM;  
                            cofactorRow++) */  
                if( (row+column) % 2 == 1)   
                {   
                    sign = -1;   
                } /* if( (row+column) % 2 == 1) */  
                else  
                {   
                    sign = 1;   
                } /* else */  
                cofactorMatrixA[row][column] = sign *   
                        Determinant(determinantCofactorMatrixA, userM);   
            } /* for (column = 0; column < userN; column++) */  
        } /* for (row = 0; row < userN; row++) */  
    } /* else */  
  
  
    for(row = 0; row < userM; row++)   
    {   
        free(determinantCofactorMatrixA[row]);   
    } /* for(row = 0; row < userM; row++) */  
    free(determinantCofactorMatrixA);   
  
    // adjoint   
    for (row = 0; row < userN; row++)   
    {   
        for (column = 0; column < userN; column++)   
        {   
            temporaryValue = cofactorMatrixA[row][column];   
            inversedMatrixA[row][column] = cofactorMatrixA[column][row];   
            inversedMatrixA[column][row] = temporaryValue;   
        } /* for (column = 0; column < userN; column++) */  
    } /* for (row = 0; row < userN; row++) */  
  
    // adjoint / determinant = inverse   
    for (row = 0; row < userN;row++)   
    {   
        for (column = 0; column < userN; column++)   
        {   
            inversedMatrixA[row][column] =   
                            cofactorMatrixA[row][column] / determinantResult;   
        } /*for (column = 0; column < userN; column++) */  
    } /* for (row = 0; row < userN;row++) */  
  
    for(row = 0; row < userN; row++)   
    {   
        free(cofactorMatrixA[row]);   
    } /* for(row = 0; row < userN; row++) */  
    free(cofactorMatrixA);   
  
    return 0;   
}   
  
static int  
MultiplicationMatrix(double** matrixA, double** matrixB, double** resultMatrix,   
                        int userN)   
{   
    int row = 0;   
    int column = 0;   
    int check = 0;   
  
    for (row = 0; row < userN; row++)   
    {   
        for (column = 0; column < userN; column++)   
        {   
            for (check = 0; check < userN; check++)   
            {   
                resultMatrix[row][column] += matrixA[row][check] *   
                                                matrixB[check][column];   
            } /* for (check = 0; check < userN; check++) */  
        } /* for (column = 0; column < userN; column++) */  
    } /* for (row = 0; row < userN; row++) */  
  
    return 0;   
}  
 
