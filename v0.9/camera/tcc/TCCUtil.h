/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : TCCUtil.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef	_TCC_UTIL_H__
#define	_TCC_UTIL_H__

/******************************************************************************
* include 
******************************************************************************/
#include "globals.h"

/******************************************************************************
* typedefs & structure
******************************************************************************/
typedef unsigned char	U8;
typedef unsigned short	U16;
typedef unsigned int	U32;

/******************************************************************************
* defines 
******************************************************************************/

#ifndef BITSET
#define	BITSET(X, MASK)				( (X) |= (U32)(MASK) )
#endif

#ifndef BITSET
#define	BITSET(X, MASK)				( (X) |= (U32)(MASK) )
#endif
#ifndef BITSCLR
#define	BITSCLR(X, SMASK, CMASK)	( (X) = ((((U32)(X)) | ((U32)(SMASK))) & ~((U32)(CMASK))) )
#endif
#ifndef BITCSET
#define	BITCSET(X, CMASK, SMASK)	( (X) = ((((U32)(X)) & ~((U32)(CMASK))) | ((U32)(SMASK))) )
#endif
#ifndef BITCLR
#define	BITCLR(X, MASK)				( (X) &= ~((U32)(MASK)) )
#endif
#ifndef BITXOR
#define	BITXOR(X, MASK)				( (X) ^= (U32)(MASK) )
#endif
#ifndef ISZERO
#define	ISZERO(X, MASK)				(  ! (((U32)(X)) & ((U32)(MASK))) )
#endif
#ifndef BYTE_OF
#define	BYTE_OF(X)					( *(volatile unsigned char *)(&(X)) )
#endif
#ifndef HWORD_OF
#define	HWORD_OF(X)					( *(volatile unsigned short *)(&(X)) )
#endif
#ifndef WORD_OF
#define	WORD_OF(X)					( *(volatile unsigned int *)(&(X)) )
#endif

#ifndef MCHw1
#define MCHw1(x)  ( (0x00000001<<(x)))
#endif

#ifndef MCHw0
#define	MCHw0(x)  (~(0x00000001<<(x)))
#endif



/******************************************************************************
* globals
******************************************************************************/

/******************************************************************************
* locals
******************************************************************************/


/******************************************************************************
* declarations
******************************************************************************/



#endif //_TCC_UTIL_H___
