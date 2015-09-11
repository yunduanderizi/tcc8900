/****************************************************************************
 *   FileName    : TCC89x_Structures.h
 *   Description : 
 ****************************************************************************
*
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
*
 ****************************************************************************/


/************************************************************************
*	TCC89x Internal Register Definition File
************************************************************************/
#ifndef	__TCC89xSTRUCTURES_H__
#define	__TCC89xSTRUCTURES_H__


/*******************************************************************************
*	 TCC8900_DataSheet_PART 2 SMU & PMU_V0.00 Dec.11 2008
********************************************************************************/
/************************************************************************
*	1. Clock Controller Register Define			(Base Addr = 0xF0400000) // R/W
************************************************************************/
//---------------------------------------------------------------------------------------------
//31  | 30  | 29  | 28  | 27  | 26  | 25  | 24  | 23  | 22  | 21  | 20  | 19  | 18  | 17  | 16 |
//  												 		|CFGEN|MODE | NCKOE/DPRD           |
//15  | 14  | 13  | 12  | 11  | 10  |  9  |  8  |  7  |   6 |   5 |   4 |   3 |   2 |   1 |  0 |
//   NCKOE/DMIN  		|		NCKOE/DMAX      |		  NCKOE/DCDIV   |     |	    CKSEL      |
//----------------------------------------------------------------------------------------------   
//CLK_BASE					*(volatile unsigned long *)0xF0400000

typedef struct _CKC{
	volatile unsigned int	CLK0CTRL;					//   0x00  R/W  0x2FFFF4  CPU & Bus Clock0 Control Register 
	volatile unsigned int	CLK1CTRL;					//   0x04  R/W  0x2FFFF4  CPU & Bus Clock1 Control Register 
	volatile unsigned int	CLK2CTRL;					//   0x08  R/W  0x2FFFF4  CPU & Bus Clock2 Control Register 
	volatile unsigned int	CLK3CTRL;					//   0x0C  R/W  0x2FFFF4  CPU & Bus Clock3 Control Register 
	volatile unsigned int	CLK4CTRL;					//   0x10  R/W  0x2FFFF4  CPU & Bus Clock4 Control Register 
	volatile unsigned int	CLK5CTRL;					//   0x14  R/W  0x2FFFF4  CPU & Bus Clock5 Control Register 
	volatile unsigned int	CLK6CTRL;					//   0x18  R/W  0x2FFFF4  CPU & Bus Clock6 Control Register 
	volatile unsigned int	CLK7CTRL;					//   0x1C  R/W  0x2FFFF4  CPU & Bus Clock 7Control Register 
	volatile unsigned int	PLL0CFG;					//   0x20  R/W  0x8010FA03  PLL0 Configuration Register 
	volatile unsigned int	PLL1CFG;					//   0x24  R/W  0x80009603  PLL1 Configuration Register 
	volatile unsigned int	PLL2CFG;					//   0x28  R/W  0x80007D03  PLL2 Configuration Register 
	volatile unsigned int	PLL3CFG;					//   0x2C  R/W  0x80009603  PLL3 Configuration Register 
	volatile unsigned int	CLKDIVC;					//   0x30  R/W  0x81818181  PLL Divider Configuration Register 
	volatile unsigned int	CLKDIVC1;					//   0x34  R/W  0x00008181  External Clock Divider Configuration Register 
	volatile unsigned int	CLKDIVC2;					//-  0x38     Reserved 
	volatile unsigned int	CLKDIVC3;					//-  0x3C      Reserved 
	volatile unsigned int	SWRESETPRD;					//   0x40  R/W  0x000000FF  Software Reset Period Register 
	volatile unsigned int	SWRESET;					//   0x44  R/W  0x00000000  Software Reset Control Register 
	volatile unsigned int	NOTDEFINE0[14];			//-  0x48-0x7C      Reserved 
	volatile unsigned int	PCLK_TCX;					//   0x80  R/W  0x00014000  Timer Counter 0 Oscillator-Clock Control Register   
	volatile unsigned int	PCLK_TCT;					//   0x84  R/W  0x00014000  Timer Counter 0 Clock Control Register   
	volatile unsigned int	PCLK_TCZ;					//   0x88  R/W  0x00014000  Timer Counter 1 Clock Control Register   
	volatile unsigned int	PCLK_LCD0;					//   0x8C  R/W  0x00014000  LCD0 Clock Control Register 
	volatile unsigned int	PCLK_LCD1;					//   0x90  R/W  0x00014000  LCD1 Clock Control Register 
	volatile unsigned int	PCLK_LCDSI;				//   0x94  R/W  0x00014000  LCDSI Clock Control Register 
	volatile unsigned int	PCLK_CIFMC;				//   0x98  R/W  0x00014000  Control Register for CIF Internal Clock
	volatile unsigned int	PCLK_CIFSC;				//   0x9C  R/W  0x00014000  Control Register for CIF Scaler Clock
	volatile unsigned int	PCLK_OUT0;					//   0xA0  R/W  0x00014000  Control Register for External Clock Output 0
	volatile unsigned int	PCLK_OUT1;					//   0xA4  R/W  0x00014000  Control Register for External Clock Output 1 
	volatile unsigned int	PCLK_HDMI;					//   0xA8  R/W  0x00014000  Control Register for HDMI PHY Input Clock 
	volatile unsigned int	PCLK_USB11H;				//   0xAC  R/W  0x00014000  Control Register for USB 1.1 Host 
	volatile unsigned int	PCLK_SDMMC0;				//   0xB0  R/W  0x00014000  Control Register for SD/MMC Channel 0
	volatile unsigned int	PCLK_MSTICK;				//   0xB4  R/W  0x00014000  Memory Stick Clock Control Register 
	volatile unsigned int	PCLK_I2C;					//   0xB8  R/W  0x00014000  I2C Clock Control Register 
	volatile unsigned int	PCLK_UART0;				//   0xBC  R/W  0x00014000  UART0 Clock Control Register 
	volatile unsigned int	PCLK_UART1;				//   0xC0  R/W  0x00014000  UART1 Clock Control Register 
	volatile unsigned int	PCLK_UART2;				//   0xC4  R/W  0x00014000  UART2 Clock Control Register 
	volatile unsigned int	PCLK_UART3;				//   0xC8  R/W  0x00014000  UART3 Clock Control Register 
	volatile unsigned int	PCLK_UART4;				//   0xCC  R/W  0x00014000  UART4 Clock Control Register 
	volatile unsigned int	PCLK_UART5;				//   0xD0  R/W  0x00014000  UART5 Clock Control Register 
	volatile unsigned int	PCLK_GPSB0;				//   0xD4  R/W  0x00014000  Control Register for GPSB Channel 0
	volatile unsigned int	PCLK_GPSB1;				//   0xD8  R/W  0x00014000  Control Register for GPSB Channel 1
	volatile unsigned int	PCLK_GPSB2;				//   0xDC  R/W  0x00014000  Control Register for GPSB Channel 2
	volatile unsigned int	PCLK_GPSB3;				//   0xE0  R/W  0x00014000  Control Register for GPSB Channel 3
	volatile unsigned int   PCLK_GPSB4; 				//	0x0E4 R/W  0x14000000  Control Register for GPSB Channel 4
	volatile unsigned int	PCLK_GPSB5;				//   0xE8  R/W  0x00014000  Control Register for GPSB Channel 5 
	volatile unsigned int	PCLK_ADC;					//   0xEC  R/W  0x00014000  Control Register for ADC (Touch Screen)
	volatile unsigned int	PCLK_SPDIF;				//   0xF0  R/W  0x00140000  Control Register for SPDIF 
	volatile unsigned int	PCLK_EHI0;					//   0xF4  R/W  0x00140000  Control Register for EHI Channel 0
	volatile unsigned int   PCLK_EHI1;					//	0x0F8 R/W  0x14000000 Control Register for EHI Channel 1 
	volatile unsigned int	PCLK_AUD;					//   0xFC  R/W  0x00014000  Control Register for Audio DMA
	volatile unsigned int	PCLK_CAN ;					//   0x100  R/W  0x00014000  Control Register for CAN 
	volatile unsigned int	NOTDEFINE1;				//   0x104  R/W  0x00140000  Reserved 
	volatile unsigned int	PCLK_SDMMC1;				//   0x108  R/W  0x00014000  Control Register for SD/MMC Channel 1
	volatile unsigned int	NOTDEFINE2;				//   0x10C  R/W  0x00014000  Reserved 
	volatile unsigned int	PCLK_DAI ;					//   0x110  R/W  0x00014000  Control Register for DAI (DAI Only) 
}sCKCReg,CKC, *PCKC;

/************************************************************************
*	2. Vectored Priority Interrupt Controller Register Map(Base Addr = 0xF0401000)
************************************************************************/
//#define	HwPIC_BASE					*(volatile unsigned long *)0xF0401000
typedef struct _PIC{
	volatile unsigned int	IEN0;					//     0x000  R/W  0x00000000  Interrupt Enable0 Register 
	volatile unsigned int	IEN1;					//     0x004  R/W  0x00000000  Interrupt Enable1 Register 
	volatile unsigned int	CLR0;					//     0x008  R/W  0x00000000  Interrupt Clear0 Register 
	volatile unsigned int	CLR1;					//     0x00C  R/W  0x00000000  Interrupt Clear1 Register 
	volatile unsigned int	STS0;					//     0x010  R  Unknown  Interrupt Status0 Register 
	volatile unsigned int	STS1;					//     0x014  R  Unknown  Interrupt Status1 Register 
	volatile unsigned int	SEL0;					//     0x018  R/W  0x00000000  IRQ or FIR Selection0 Register 
	volatile unsigned int	SEL1;					//     0x01C  R/W  0x00000000  IRQ or FIR Selection1 Register 
	volatile unsigned int	SRC0;					//     0x020  R  Unknown  Source Interrupt Status0 Register 
	volatile unsigned int	SRC1;					//     0x024  R  Unknown  Source Interrupt Status1 Register 
	volatile unsigned int	MSTS0;					//     0x028  R  0x00000000  Masked Status0 Register 
	volatile unsigned int	MSTS1;					//     0x02C  R  0x00000000  Masked Status1 Register 
	volatile unsigned int	TIG0;					//     0x030  R/W  0x00000000  Test Interrupt Generation0 Register 
	volatile unsigned int	TIG1;					//     0x034  R/W  0x00000000  Test Interrupt Generation1 Register 
	volatile unsigned int	POL0;					//     0x038  R/W  0x00000000  Interrupt Polarity0 Register 
	volatile unsigned int	POL1;					//     0x03C  R/W  0x00000000  Interrupt Polarity1 Register 
	volatile unsigned int	IRQ0;					//     0x040  R  0x00000000  IRQ Raw Status0 Register 
	volatile unsigned int	IRQ1;					//     0x044  R  0x00000000  IRQ Raw Status1 Register 
	volatile unsigned int	FIQ0;					//     0x048  R  Unknown  FIQ Status0 Register 
	volatile unsigned int	FIQ1;					//     0x04C  R  Unknown  FIQ Status1 Register 
	volatile unsigned int	MIRQ0;					//     0x050  R  0x00000000  Masked IRQ Status0 Register 
	volatile unsigned int	MIRQ1;					//     0x054  R  0x00000000  Masked IRQ Status1 Register 
	volatile unsigned int	MFIQ0;					//     0x058  R  0x00000000  Masked FIQ Status0 Register 
	volatile unsigned int	MFIQ1;					//     0x05C  R  0x00000000  Masked FIQ Status1 Register 
	volatile unsigned int	MODE0;					//     0x060  R/W  0x00000000  Trigger Mode0 Register ? Level or Edge 
	volatile unsigned int	MODE1;					//     0x064  R/W  0x00000000  Trigger Mode1 Register ? Level or Edge 
	volatile unsigned int	SYNC0;					//     0x068  R/W  0xFFFFFFFF  Synchronization Enable0 Register 
	volatile unsigned int	SYNC1;					//     0x06C  R/W  0xFFFFFFFF  Synchronization Enable1 Register 
	volatile unsigned int	WKEN0;					//     0x070  R/W  0x00000000  Wakeup Event Enable0 Register 
	volatile unsigned int	WKEN1;					//     0x074  R/W  0x00000000  Wakeup Event Enable1 Register 
	volatile unsigned int	MODEA0;					//     0x078  R/W  0x00000000  Both Edge or Single Edge0 Register 
	volatile unsigned int	MODEA1;					//     0x07C  R/W  0x00000000  Both Edge or Single Edge1 Register 
	volatile unsigned int	NOTDEFINE0[32];		//-  0x80-0xFC      Reserved 
	volatile unsigned int	INTMSK0;				//     0x100  R/W  0xFFFFFFFF  Interrupt Output Masking0 Register 
	volatile unsigned int	INTMSK1;				//     0x104  R/W  0xFFFFFFFF  Interrupt Output Masking1 Register 
	volatile unsigned int	ALLMSK;					//     0x108  R/W  0x00000003  All Mask Register 
}sHwINT,PIC, *PPIC;

//#define	HwVIC_BASE					*(volatile unsigned long *)0xF0401200
typedef struct _VIC{
	volatile unsigned int	VAIRQ;					//       0x200  R  0x800000XX  IRQ Vector Register 
	volatile unsigned int	VAFIQ;					//       0x204  R  0x800000XX  FIQ Vector Register 
	volatile unsigned int	VNIRQ;					//       0x208  R  0x800000XX  IRQ Vector Number Register 
	volatile unsigned int	VNFIQ;					//       0x20C  R  0x800000XX  FIQ Vector Number Register 
	volatile unsigned int	VCTRL;					//       0x210  R/W  0x00000000  Vector Control Register 
	volatile unsigned int	NOTDEFINE0[3];			//  		0x214-0x218-0x21c      Reserved 
	volatile unsigned int	PRIO0;					//       0x220  R/W  0x03020100  Priorities for Interrupt 0 ~ 3 
	volatile unsigned int	PRIO1;					//       0x224  R/W  0x07060504  Priorities for Interrupt 4 ~ 7 
	volatile unsigned int	PRIO2;					//       0x228  R/W  0x0B0A0908  Priorities for Interrupt 8 ~ 11 
	volatile unsigned int	PRIO3;					//       0x22C  R/W  0x0F0E0D0C  Priorities for Interrupt 12 ~ 15 
	volatile unsigned int	PRIO4;					//       0x230  R/W  0x13121110  Priorities for Interrupt 16 ~ 19 
	volatile unsigned int	PRIO5;					//       0x234  R/W  0x17161514  Priorities for Interrupt 20 ~ 23 
	volatile unsigned int	PRIO6;					//       0x238  R/W  0x1B1A1918  Priorities for Interrupt 24 ~ 27 
	volatile unsigned int	PRIO7;					//       0x23C  R/W  0x1F1E1D1C  Priorities for Interrupt 28 ~ 31 
	volatile unsigned int	PRIO8;					//       0x220  R/W  0x23222120  Priorities for Interrupt 32 ~ 35 
	volatile unsigned int	PRIO9;					//       0x224  R/W  0x27262524  Priorities for Interrupt 36 ~ 39 
	volatile unsigned int	PRIO10;					//       0x228  R/W  0x2B2A2928  Priorities for Interrupt 40 ~ 43 
	volatile unsigned int	PRIO11;					//       0x22C  R/W  0x2F2E2D2C  Priorities for Interrupt 44 ~ 47 
	volatile unsigned int	PRIO12;					//       0x230  R/W  0x33323130  Priorities for Interrupt 48 ~ 51 
	volatile unsigned int	PRIO13;					//       0x234  R/W  0x37363534  Priorities for Interrupt 52 ~ 55 
	volatile unsigned int	PRIO14;					//       0x238  R/W  0x3B3A3938  Priorities for Interrupt 56 ~ 59 
	volatile unsigned int	PRIO15;					//       0x23C  R/W  0x3F3E3D3C  Priorities for Interrupt 60 ~ 63 

}VIC, *PVIC;

/***********************************************************************
*	3. Timer/Counter Register Map (Base Address = 0xF0403000) 
************************************************************************/
//#define	HwTMR_BASE					*(volatile unsigned long *)0xF0403000	// Timer/Counter Base Register
typedef struct _TIMER{
	volatile unsigned int	TCFG0;					//         0x00  R/W  0x00  Timer/Counter 0 Configuration Register 
	volatile unsigned int	TCNT0;					//         0x04  R/W  0x0000  Timer/Counter 0 Counter Register 
	volatile unsigned int	TREF0;					//         0x08  R/W  0xFFFF  Timer/Counter 0 Reference Register 
	volatile unsigned int	TMREF0;					//         0x0C  R/W  0x0000  Timer/Counter 0 Middle Reference Register 
	volatile unsigned int	TCFG1;					//         0x10  R/W  0x00  Timer/Counter 1 Configuration Register 
	volatile unsigned int	TCNT1;					//         0x14  R/W  0x0000  Timer/Counter 1 Counter Register 
	volatile unsigned int	TREF1;					//         0x18  R/W  0xFFFF  Timer/Counter 1 Reference Register 
	volatile unsigned int	TMREF1;					//         0x1C  R/W  0x0000  Timer/Counter 1 Middle Reference Register 
	volatile unsigned int	TCFG2;					//         0x20  R/W  0x00  Timer/Counter 2 Configuration Register 
	volatile unsigned int	TCNT2;					//         0x24  R/W  0x0000  Timer/Counter 2 Counter Register 
	volatile unsigned int	TREF2;					//         0x28  R/W  0xFFFF  Timer/Counter 2 Reference Register 
	volatile unsigned int	TMREF2;					//         0x2C  R/W  0x0000  Timer/Counter 2 Middle Reference Register 
	volatile unsigned int	TCFG3;					//         0x30  R/W  0x00  Timer/Counter 3 Configuration Register 
	volatile unsigned int	TCNT3;					//         0x34  R/W  0x0000  Timer/Counter 3 Counter Register 
	volatile unsigned int	TREF3;					//         0x38  R/W  0xFFFF  Timer/Counter 3 Reference Register 
	volatile unsigned int	TMREF3;					//         0x3C  R/W  0x0000  Timer/Counter 3 Middle Reference Register 
	volatile unsigned int	TCFG4;					//         0x40  R/W  0x00  Timer/Counter 4 Configuration Register 
	volatile unsigned int	TCNT4;					//         0x44  R/W  0x00000  Timer/Counter 4 Counter Register 
	volatile unsigned int	TREF4;					//         0x48  R/W  0xFFFFF  Timer/Counter 4 Reference Register 
	volatile unsigned int   NOTDEFINE0;
	volatile unsigned int	TCFG5;					//         0x50  R/W  0x00  Timer/Counter 5 Configuration Register 
	volatile unsigned int	TCNT5;					//         0x54  R/W  0x00000  Timer/Counter 5 Counter Register 
	volatile unsigned int	TREF5;					//         0x58  R/W  0xFFFFF  Timer/Counter 5 Reference Register 
	volatile unsigned int   NOTDEFINE1;
	volatile unsigned int	TIREQ;					//         0x60  R/W  0x0000  Timer/Counter n Interrupt Request Register 
	volatile unsigned int   NOTDEFINE2[3];
	volatile unsigned int	TWDCFG;					//         0x70  R/W  0x0000  Reserved 
	volatile unsigned int	TWDCLR;					//         0x74  W  -  Reserved 
	volatile unsigned int   NOTDEFINE3[2];
	volatile unsigned int	TC32EN;					//         0x80  R/W  0x00007FFF  32-bit Counter Enable / Pre-scale Value 
	volatile unsigned int	TC32LDV;				//         0x84  R/W  0x00000000  32-bit Counter Load Value 
	volatile unsigned int	TC32CMP0;				//         0x88  R/W  0x00000000  32-bit Counter Match Value 0 
	volatile unsigned int	TC32CMP1;				//         0x8C  R/W  0x00000000  32-bit Counter Match Value 1 
	volatile unsigned int	TC32PCNT;				//         0x90  R/W  -  32-bit Counter Current Value (pre-scale counter) 
	volatile unsigned int	TC32MCNT;				//         0x94  R/W  -  32-bit Counter Current Value (main counter) 
	volatile unsigned int	TC32IRQ;				//         0x98  R/W  0x0000----  32-bit Counter Interrupt Control 

}TIMER, *PTIMER;

typedef struct _TIMERN{
	volatile unsigned int	TCFG;					// 0x000   R/W	Timer/Counter Configuration Register 
	volatile unsigned int	TCNT;					// 0x004   R/W  Timer/Counter Counter Register 
	volatile unsigned int	TREF;					// 0x008   R/W  Timer/Counter Reference Register 
	volatile unsigned int	TMREF;					// 0x00C   R/W  Timer/Counter Middle Reference Register 
} TIMERN, *PTIMERN;

/***********************************************************************
*	4. PMU(POWER MANAGEMENT UNIT) Register Map (Base Address = 0xF0404000) 
************************************************************************/
//#define	HwCONTROL  				*(volatile unsigned long *)0xF0404000 	//R/W   PMU Control Register 
typedef struct _PMU{
	volatile unsigned int	CONTROL;				// 	0x00 R/W   PMU Control Register 
	volatile unsigned int	WKUPEN ;				//	0x04  R/W    Wakeup Enable Configuration Register 
	volatile unsigned int	WKUPPOL ;				//	0x08 R/W   Wakeup Polarity Configuration Register 
	volatile unsigned int	WATCHDOG;				//  0x0C  R/W    Watchdog Control Register 
	volatile unsigned int	CONFIG0 ;				//	0x10 R/W   Boot Configuration Register 
	volatile unsigned int	USERSTS ;				//	0x14 R/W   Status Register 
	volatile unsigned int	PWROFF  ;				//	0x18  R/W    Power-Off Control Register 
} PMU, *PPMU;



/*******************************************************************************
*	5. SMUI2C Controller Register Define   (Base Addr = 0xF0405000)
********************************************************************************/
//#define HwSMU_I2CMASTER0_BASE     *(volatile unsigned long*)0xF0405000
//#define HwSMU_I2CMASTER1_BASE     *(volatile unsigned long*)0xF0405040
//#define HwSMU_I2CICLK_BASE     	*(volatile unsigned long*)0xF0405080	//I2C_SCL divider Regist
typedef struct _SMUI2CMASTER{
    volatile unsigned int  PRES;                // 0x00 R/W 0xFFFF Clock Prescale register
    volatile unsigned int  CTRL;                // 0x04 R/W 0x0000 Control Register
    volatile unsigned int  TXR;                 // 0x08 W 0x0000 Transmit Register
    volatile unsigned int  CMD;                 // 0x0C W 0x0000 Command Register
    volatile unsigned int  RXR;                 // 0x10 R 0x0000 Receive Register
    volatile unsigned int  SR;                  // 0x14 R 0x0000 Status register
    volatile unsigned int  TIME;                // 0x18 R/W 0x0000 Timing Control Register    
} SMUI2CMASTER, *PSMUI2CMASTER;

typedef struct _SMUI2CICLK{
    volatile unsigned int  ICLK;                // 0x00 R/W 0xFFFF Clock Prescale register
} SMUI2CICLK, *PSMUI2CICLK;

/*******************************************************************************
*	 TCC8900_DataSheet_PART 3 GPIO_V0.00 Dec.11 2008
********************************************************************************/
/************************************************************************
*	1. GPIO Register Map (Base Address = 0xF0102000) 
************************************************************************/
/*
#define	HwGPIO_BASE  			*(volatile unsigned long *)0xF0102000  	// 
#define	HwGPIOA_BASE  			*(volatile unsigned long *)0xF0102000  	// 
#define	HwGPIOB_BASE  			*(volatile unsigned long *)0xF0102040  	// 
#define	HwGPIOC_BASE  			*(volatile unsigned long *)0xF0102080  	// 
#define	HwGPIOD_BASE  			*(volatile unsigned long *)0xF01020C0  	// 
#define	HwGPIOE_BASE  			*(volatile unsigned long *)0xF0102100  	// 
#define	HwGPIOF_BASE  			*(volatile unsigned long *)0xF0102140  	// 
#define	HwEINTSEL_BASE  		*(volatile unsigned long *)0xF0102180  	// 
*/
typedef struct _GPIO{
	volatile unsigned int	GPADAT;					//   0x000  R/W  0x00000000  GPA Data Register 
	volatile unsigned int	GPAEN;					//   0x004  R/W  0x00000000  GPA Output Enable Register 
	volatile unsigned int	GPASET;					//   0x008  W  -  OR function on GPA Output Data 
	volatile unsigned int	GPACLR;					//   0x00C  W  -  BIC function on GPA Output Data 
	volatile unsigned int	GPAXOR;					//   0x010  W  -  XOR function on GPA Output Data 
	volatile unsigned int	GPACD0;					//   0x014  W  0x55555555  Driver strength Control 0 on GPA Output Data 
	volatile unsigned int	GPACD1;					//   0x018  W  0x00000000  Driver strength Control 1 on GPA Output Data 
	volatile unsigned int	GPAPD0;					//   0x01C  W  0x55555555  Pull-Up/Down function on GPA Output Data 
	volatile unsigned int	GPAPD1;					//   0x020  W  0x00000000  Pull-Up/Down function on GPA Output Data 
	volatile unsigned int	GPAFN0;					//   0x024  W  0x00000000  Port Configuration on GPA Output Data 
	volatile unsigned int	GPAFN1;					//   0x028  W  0x00000000  Port Configuration on GPA Output Data 
	volatile unsigned int	GPAFN2;					//   0x02C  W  0x00000000  Port Configuration on GPA Output Data 
	volatile unsigned int	GPAFN3;					//   0x030  W  0x00000000  Port Configuration on GPA Output Data 
	volatile unsigned int	NOTDEFINE0[3];			//	 0x034-0x03C     Reserved 
	volatile unsigned int	GPBDAT;					//   0x040  R/W  0x00000000  GPB Data Register 
	volatile unsigned int	GPBEN;					//   0x044  R/W  0x00000000  GPB Output Enable Register 
	volatile unsigned int	GPBSET;					//   0x048  W  -  OR function on GPB Output Data 
	volatile unsigned int	GPBCLR;					//   0x04C  W  -  BIC function on GPB Output Data 
	volatile unsigned int	GPBXOR;					//   0x050  W  -  XOR function on GPB Output Data 
	volatile unsigned int	GPBCD0;					//   0x054  W  0x55555555  Driver strength Control 0 on GPB Output Data 
	volatile unsigned int	GPBCD1;					//   0x058  W  0x00000000  Driver strength Control 1 on GPB Output Data 
	volatile unsigned int	GPBPD0;					//   0x05C  W  0x55555555  Pull-Up/Down function on GPB Output Data 
	volatile unsigned int	GPBPD1;					//   0x060  W  0x00000000  Pull-Up/Down function on GPB Output Data 
	volatile unsigned int	GPBFN0;					//   0x064  W  0x00000000  Port Configuration on GPB Output Data 
	volatile unsigned int	GPBFN1;					//   0x068  W  0x00000000  Port Configuration on GPB Output Data 
	volatile unsigned int	GPBFN2;					//   0x06C  W  0x00000000  Port Configuration on GPB Output Data 
	volatile unsigned int	GPBFN3;					//   0x070  W  0x00000000  Port Configuration on GPB Output Data 
	volatile unsigned int	NOTDEFINE1[3];			// 	 0x074-0x07C     Reserved 
	volatile unsigned int	GPCDAT;					//   0x080  R/W  0x00000000  GPC Data Register 
	volatile unsigned int	GPCEN;					//   0x084  R/W  0x00000000  GPC Output Enable Register 
	volatile unsigned int	GPCSET;					//   0x088  W  -  OR function on GPC Output Data 
	volatile unsigned int	GPCCLR;					//   0x08C  W  -  BIC function on GPC Output Data 
	volatile unsigned int	GPCXOR;					//   0x090  W  -  XOR function on GPC Output Data 
	volatile unsigned int	GPCCD0;					//   0x094  W  0x55555555  Driver strength Control 0 on GPC Output Data 
	volatile unsigned int	GPCCD1;					//   0x098  W  0x00000000  Driver strength Control 1 on GPC Output Data 
	volatile unsigned int	GPCPD0;					//   0x09C  W  0x55555555  Pull-Up/Down function on GPC Output Data 
	volatile unsigned int	GPCPD1;					//   0x0A0  W  0x00000000  Pull-Up/Down function on GPC Output Data 
	volatile unsigned int	GPCFN0;					//   0x0A4  W  0x00000000  Port Configuration on GPC Output Data 
	volatile unsigned int	GPCFN1;					//   0x0A8  W  0x00000000  Port Configuration on GPC Output Data 
	volatile unsigned int	GPCFN2;					//   0x0AC  W  0x00000000  Port Configuration on GPC Output Data 
	volatile unsigned int	GPCFN3;					//   0x0B0  W  0x00000000  Port Configuration on GPC Output Data 
	volatile unsigned int	NOTDEFINE2[3];			// 	 0x0B4-0x0BC Reserved 
	volatile unsigned int	GPDDAT;					//   0x0C0  R/W  0x00000000  GPD Data Register 
	volatile unsigned int	GPDEN;					//   0x0C4  R/W  0x00000000  GPD Output Enable Register 
	volatile unsigned int	GPDSET;					//   0x0C8  W  -  OR function on GPD Output Data 
	volatile unsigned int	GPDCLR;					//   0x0CC  W  -  BIC function on GPD Output Data 
	volatile unsigned int	GPDXOR;					//   0x0D0  W  -  XOR function on GPD Output Data 
	volatile unsigned int	GPDCD0;					//   0x0D4  W  0x55555555  Driver strength Control 0 on GPD Output Data 
	volatile unsigned int	GPDCD1;					//   0x0D8  W  0x00000000  Driver strength Control 1 on GPD Output Data 
	volatile unsigned int	GPDPD0;					//   0x0DC  W  0x55555555  Pull-Up/Down function on GPD Output Data 
	volatile unsigned int	GPDPD1;					//   0x0E0  W  0x00000000  Pull-Up/Down function on GPD Output Data 
	volatile unsigned int	GPDFN0;					//   0x0E4  W  0x00000000  Port Configuration on GPD Output Data 
	volatile unsigned int	GPDFN1;					//   0x0E8  W  0x00000000  Port Configuration on GPD Output Data 
	volatile unsigned int	GPDFN2;					//   0x0EC  W  0x00000000  Port Configuration on GPD Output Data 
	volatile unsigned int	GPDFN3;					//   0x0F0  W  0x00000000  Port Configuration on GPD Output Data 
	volatile unsigned int	NOTDEFINE3[3];			// 	 0x0F4-0x0FC     Reserved 
	volatile unsigned int	GPEDAT;					//   0x100  R/W  0x00000000  GPE Data Register 
	volatile unsigned int	GPEEN;					//   0x104  R/W  0x00000000  GPE Output Enable Register 
	volatile unsigned int	GPESET;					//   0x108  W  -  OR function on GPE Output Data 
	volatile unsigned int	GPECLR;					//   0x10C  W  -  BIC function on GPE Output Data 
	volatile unsigned int	GPEXOR;					//   0x110  W  -  XOR function on GPE Output Data 
	volatile unsigned int	GPECD0;					//   0x114  W  0x55555555  Driver strength Control 0 on GPE Output Data 
	volatile unsigned int	GPECD1;					//   0x118  W  0x00000000  Driver strength Control 1 on GPE Output Data 
	volatile unsigned int	GPEPD0;					//   0x11C  W  0x55555555  Pull-Up/Down function on GPE Output Data 
	volatile unsigned int	GPEPD1;					//   0x120  W  0x00000000  Pull-Up/Down function on GPE Output Data 
	volatile unsigned int	GPEFN0;					//   0x124  W  0x00000000  Port Configuration on GPE Output Data 
	volatile unsigned int	GPEFN1;					//   0x128  W  0x00000000  Port Configuration on GPE Output Data 
	volatile unsigned int	GPEFN2;					//   0x12C  W  0x00000000  Port Configuration on GPE Output Data 
	volatile unsigned int	GPEFN3;					//   0x130  W  0x00000000  Port Configuration on GPE Output Data 
	volatile unsigned int	NOTDEFINE4[3];			// 	 0x134-0x13C     Reserved 
	volatile unsigned int	GPFDAT;					//   0x140  R/W  0x00000000  GPF Data Register 
	volatile unsigned int	GPFEN;					//   0x144  R/W  0x00000000  GPF Output Enable Register 
	volatile unsigned int	GPFSET;					//   0x148  W  -  OR function on GPF Output Data 
	volatile unsigned int	GPFCLR;					//   0x14C  W  -  BIC function on GPF Output Data 
	volatile unsigned int	GPFXOR;					//   0x150  W  -  XOR function on GPF Output Data 
	volatile unsigned int	GPFCD0;					//   0x154  W  0x55555555  Driver strength Control 0 on GPF Output Data 
	volatile unsigned int	GPFCD1;					//   0x158  W  0x00000000  Driver strength Control 1 on GPF Output Data 
	volatile unsigned int	GPFPD0;					//   0x15C  W  0x55555555  Pull-Up/Down function on GPF Output Data 
	volatile unsigned int	GPFPD1;					//   0x160  W  0x00000000  Pull-Up/Down function on GPF Output Data 
	volatile unsigned int	GPFFN0;					//   0x164  W  0x00000000  Port Configuration on GPF Output Data 
	volatile unsigned int	GPFFN1;					//   0x168  W  0x00000000  Port Configuration on GP Output Data 
	volatile unsigned int	GPFFN2;					//   0x16C  W  0x00000000  Port Configuration on GPF Output Data 
	volatile unsigned int	GPFFN3;					//   0x170  W  0x00000000  Port Configuration on GPF Output Data 
	volatile unsigned int	NOTDEFINE5[4];			// 	 0x174-0x17C     Reserved 
	volatile unsigned int	EINTSEL0;				//   0x184  R/W  0x00000000  External Interrupt Select Register 01
	volatile unsigned int	EINTSEL1;				//   0x188  R/W  0x00000000  External Interrupt Select Register 1 
	volatile unsigned int	EINTSEL2;				//   0x18C  R/W  0x00000000  External Interrupt Select Register 2 
	volatile unsigned int	MON;					//   0x190  R/W  0x00000000  System Monitor Enable Register   
	volatile unsigned int	ECID0;					//   0x194  R/W  0x00000000  CID output Register 
	volatile unsigned int	ECID1;					//   0x198  R  -  CID serial input Register 
	volatile unsigned int	ECID2;					//   0x19C  R  -  CID parallel input 0 Register 
	volatile unsigned int	ECID3;					//   0x1A0  R  -  CID parallel input 1 Register     
}GPIO, *PGPIO;

typedef struct _GPION{
	volatile unsigned int	GPDAT;					// 0x000  R/W	GPA Data Register 
	volatile unsigned int	GPEN;					// 0x004  R/W	GPA Output Enable Register 
	volatile unsigned int	GPSET;					// 0x008  W  	OR function on GPA Output Data 
	volatile unsigned int	GPCLR;					// 0x00C  W  	BIC function on GPA Output Data 
	volatile unsigned int	GPXOR;					// 0x010  W  	XOR function on GPA Output Data 
	volatile unsigned int	GPCD0;					// 0x014  W  	Driver strength Control 0 on GPA Output Data 
	volatile unsigned int	GPCD1;					// 0x018  W  	Driver strength Control 1 on GPA Output Data 
	volatile unsigned int	GPPD0;					// 0x01C  W  	Pull-Up/Down function on GPA Output Data 
	volatile unsigned int	GPPD1;					// 0x020  W  	Pull-Up/Down function on GPA Output Data 
	volatile unsigned int	GPFN0;					// 0x024  W  	Port Configuration on GPA Output Data 
	volatile unsigned int	GPFN1;					// 0x028  W  	Port Configuration on GPA Output Data 
	volatile unsigned int	GPFN2;					// 0x02C  W  	Port Configuration on GPA Output Data 
	volatile unsigned int	GPFN3;					// 0x030  W  	Port Configuration on GPA Output Data 
} GPION, *PGPION;

/*******************************************************************************
*	 TCC8900_DataSheet_PART 4 CORE & MEMORY BUS_V0.00 Dec.11 2008
********************************************************************************/
/************************************************************************
*	3. DRAM CONTROLLER Register Map (Base Address = 0xF0301000) 
************************************************************************/
/*
#define	HwDRAM_BASE  			*(volatile unsigned long *)0xF0301000  	//
#define	HwDRAMM0_BASE  		*(volatile unsigned long *)0xF0301000  	//
#define	HwDRAMM1_BASE  		*(volatile unsigned long *)0xF0302000  	//
#define	HwDRAMMISC_BASE  		*(volatile unsigned long *)0xF0303000  	//
#define	HwDRAMPHY_BASE  		*(volatile unsigned long *)0xF0304000  	//
#define	HwDRAMMEMBUS_BASE  	*(volatile unsigned long *)0xF0305004  	//
*/
typedef struct _DRAM{
	volatile unsigned int	STAT;					//  0x000 RO  -  Status Register 
	volatile unsigned int	CMD;					//  0x004 WO  -  Command Register 
	volatile unsigned int	DCMD;					//   0x008  WO  -  Direct COmmnad Register 
	volatile unsigned int	CFG;					//  0x00C R/W 0x00010020 Configuration Register 
	volatile unsigned int	REF;					//   0x010  R/W  0x00000A60  Refresh Period Register 
	volatile unsigned int	CAS;					//  0x014 R/W 0x00000006 CAS Latency Register 
	volatile unsigned int	DQSS;					//  0x018 R/W 0x00000001 t_dqss Register 
	volatile unsigned int	MRD;					//  0x01C R/W 0x00000002 t_mrd Register 
	volatile unsigned int	RAS;					//  0x020 R/W 0x00000007 t_ras Register 
	volatile unsigned int	RC;						//  0x024 R/W 0x0000000B t_rc Register 
	volatile unsigned int	RCD;					//  0x028 R/W 0x0000001D t_rcd Register 
	volatile unsigned int	RFC;					//  0x02C R/W 0x000000212 t_rfc Register 
	volatile unsigned int	RP;						//  0x030 R/W 0x0000001D t_rp Register 
	volatile unsigned int	RRD;					//  0x034 R/W 0x00000002 t_rrd Register 
	volatile unsigned int	WR;						//  0x038 R/W 0x00000003 t_wr Register 
	volatile unsigned int	WTR;					//  0x03C R/W 0x00000002 t_wtr Register 
	volatile unsigned int	XP;						//  0x040 R/W 0x00000001 t_xp Register 
	volatile unsigned int	XSR;					//  0x044 R/W 0x0000000A t_xsr Register 
	volatile unsigned int	ESR;					//  0x048 R/W 0x00000014 t_esr Register 
	volatile unsigned int	CFG2;					//  0x04C R/W  -  Memory_cfg2 Register 
	volatile unsigned int	CFG3;					//  0x050 R/W 0x00000007 Memory_cfg3 Register 
	volatile unsigned int	NOTDEFINE0[43];		//	 - 0x054- 0x0FC   Reserved 
	volatile unsigned int	ID0;					//  0x100 R/W 0x00000000 AXI ID0 configuration Register 
	volatile unsigned int	ID1;					//  0x104 R/W 0x00000000 AXI ID1 configuration Register 
	volatile unsigned int	ID2;					//  0x108 R/W 0x00000000 AXI ID2 configuration Register 
	volatile unsigned int	ID3;					//  0x10C R/W 0x00000000 AXI ID3 configuration Register 
	volatile unsigned int	ID4;					//  0x110 R/W 0x00000000 AXI ID4 configuration Register 
	volatile unsigned int	ID5;					//  0x114 R/W 0x00000000 AXI ID5 configuration Register 
	volatile unsigned int	ID6;					//  0x118 R/W 0x00000000 AXI ID6 configuration Register 
	volatile unsigned int	ID7;					//  0x11C R/W 0x00000000 AXI ID7 configuration Register 
	volatile unsigned int	ID8;					//  0x120 R/W 0x00000000 AXI ID8 configuration Register 
	volatile unsigned int	ID9;					//  0x124 R/W 0x00000000 AXI ID9 configuration Register 
	volatile unsigned int	ID10;					//   0x128  R/W  0x00000000  AXI ID10 configuration Register 
	volatile unsigned int	ID11;					//   0x12C  R/W  0x00000000  AXI ID11 configuration Register 
	volatile unsigned int	ID12;					//   0x130  R/W  0x00000000  AXI ID12 configuration Register 
	volatile unsigned int	ID13;					//   0x134  R/W  0x00000000  AXI ID13 configuration Register 
	volatile unsigned int	ID14;					//   0x138  R/W  0x00000000  AXI ID14 configuration Register 
	volatile unsigned int	ID15;					//   0x13C  R/W  0x00000000  AXI ID15 configuration Register 
	volatile unsigned int	NOTDEFINE1[48];			//	 - 0x140- 0x1FC   Reserved 
	volatile unsigned int	CH0;					//  0x200 R/W 0x0000FF00 CHIP ID0 configuration Register 
	volatile unsigned int	CH1;					//  0x204 R/W 0x0000FF00 CHIP ID1 configuration Register 
	volatile unsigned int	CH2;					//  0x208 R/W 0x0000FF00 CHIP ID2 configuration Register 
	volatile unsigned int	CH3;					//  0x20C R/W 0x0000FF00 CHIP ID3 configuration Register 
}DRAM, *PDRAM;

typedef struct _DRAMMX{
	volatile unsigned int	M0STAT;					//  0x000 RO  -  Status Register 
	volatile unsigned int	M0CMD;					//  0x004 WO  -  Command Register 
	volatile unsigned int	M0DCMD;					//   0x008  WO  -  Direct COmmnad Register 
	volatile unsigned int	M0CFG;					//  0x00C R/W 0x00010020 Configuration Register 
	volatile unsigned int	M0REF;					//   0x010  R/W  0x00000A60  Refresh Period Register 
	volatile unsigned int	M0CAS;					//  0x014 R/W 0x00000006 CAS Latency Register 
	volatile unsigned int	M0DQSS;					//  0x018 R/W 0x00000001 t_dqss Register 
	volatile unsigned int	M0MRD;					//  0x01C R/W 0x00000002 t_mrd Register 
	volatile unsigned int	M0RAS;					//  0x020 R/W 0x00000007 t_ras Register 
	volatile unsigned int	M0RC;						//  0x024 R/W 0x0000000B t_rc Register 
	volatile unsigned int	M0RCD;					//  0x028 R/W 0x0000001D t_rcd Register 
	volatile unsigned int	M0RFC;					//  0x02C R/W 0x000000212 t_rfc Register 
	volatile unsigned int	M0RP;						//  0x030 R/W 0x0000001D t_rp Register 
	volatile unsigned int	M0RRD;					//  0x034 R/W 0x00000002 t_rrd Register 
	volatile unsigned int	M0WR;						//  0x038 R/W 0x00000003 t_wr Register 
	volatile unsigned int	M0WTR;					//  0x03C R/W 0x00000002 t_wtr Register 
	volatile unsigned int	M0XP;						//  0x040 R/W 0x00000001 t_xp Register 
	volatile unsigned int	M0XSR;					//  0x044 R/W 0x0000000A t_xsr Register 
	volatile unsigned int	M0ESR;					//  0x048 R/W 0x00000014 t_esr Register 
	volatile unsigned int	M0CFG2;					//  0x04C R/W  -  Memory_cfg2 Register 
	volatile unsigned int	M0CFG3;					//  0x050 R/W 0x00000007 Memory_cfg3 Register 
	volatile unsigned int	NOTDEFINE0[43];		//	 - 0x054- 0x0FC   Reserved 
	volatile unsigned int	M0ID0;					//  0x100 R/W 0x00000000 AXI ID0 configuration Register 
	volatile unsigned int	M0ID1;					//  0x104 R/W 0x00000000 AXI ID1 configuration Register 
	volatile unsigned int	M0ID2;					//  0x108 R/W 0x00000000 AXI ID2 configuration Register 
	volatile unsigned int	M0ID3;					//  0x10C R/W 0x00000000 AXI ID3 configuration Register 
	volatile unsigned int	M0ID4;					//  0x110 R/W 0x00000000 AXI ID4 configuration Register 
	volatile unsigned int	M0ID5;					//  0x114 R/W 0x00000000 AXI ID5 configuration Register 
	volatile unsigned int	M0ID6;					//  0x118 R/W 0x00000000 AXI ID6 configuration Register 
	volatile unsigned int	M0ID7;					//  0x11C R/W 0x00000000 AXI ID7 configuration Register 
	volatile unsigned int	M0ID8;					//  0x120 R/W 0x00000000 AXI ID8 configuration Register 
	volatile unsigned int	M0ID9;					//  0x124 R/W 0x00000000 AXI ID9 configuration Register 
	volatile unsigned int	M0ID10;					//   0x128  R/W  0x00000000  AXI ID10 configuration Register 
	volatile unsigned int	M0ID11;					//   0x12C  R/W  0x00000000  AXI ID11 configuration Register 
	volatile unsigned int	M0ID12;					//   0x130  R/W  0x00000000  AXI ID12 configuration Register 
	volatile unsigned int	M0ID13;					//   0x134  R/W  0x00000000  AXI ID13 configuration Register 
	volatile unsigned int	M0ID14;					//   0x138  R/W  0x00000000  AXI ID14 configuration Register 
	volatile unsigned int	M0ID15;					//   0x13C  R/W  0x00000000  AXI ID15 configuration Register 
	volatile unsigned int	NOTDEFINE1[48];			//	 - 0x140- 0x1FC   Reserved 
	volatile unsigned int	M0CH0;					//  0x200 R/W 0x0000FF00 CHIP ID0 configuration Register 
	volatile unsigned int	M0CH1;					//  0x204 R/W 0x0000FF00 CHIP ID1 configuration Register 
	volatile unsigned int	M0CH2;					//  0x208 R/W 0x0000FF00 CHIP ID2 configuration Register 
	volatile unsigned int	M0CH3;					//  0x20C R/W 0x0000FF00 CHIP ID3 configuration Register 
	volatile unsigned int	NOTDEFINE2[892];		//	 - 0x1210- 0x1FFC   Reserved 
	//0xF0302000
	volatile unsigned int	M1STAT;					//  0x000 RO  -  Status Register 
	volatile unsigned int	M1CMD;					//  0x004 WO  -  Command Register 
	volatile unsigned int	M1DCMD;					//   0x008  WO  -  Direct COmmnad Register 
	volatile unsigned int	M1CFG;					//  0x00C R/W 0x00010020 Configuration Register 
	volatile unsigned int	M1REF;					//   0x010  R/W  0x00000A60  Refresh Period Register 
	volatile unsigned int	M1CAS;					//  0x014 R/W 0x00000006 CAS Latency Register 
	volatile unsigned int	M1DQSS;					//  0x018 R/W 0x00000001 t_dqss Register 
	volatile unsigned int	M1MRD;					//  0x01C R/W 0x00000002 t_mrd Register 
	volatile unsigned int	M1RAS;					//  0x020 R/W 0x00000007 t_ras Register 
	volatile unsigned int	M1RC;						//  0x024 R/W 0x0000000B t_rc Register 
	volatile unsigned int	M1RCD;					//  0x028 R/W 0x0000001D t_rcd Register 
	volatile unsigned int	M1RFC;					//  0x02C R/W 0x000000212 t_rfc Register 
	volatile unsigned int	M1RP;						//  0x030 R/W 0x0000001D t_rp Register 
	volatile unsigned int	M1RRD;					//  0x034 R/W 0x00000002 t_rrd Register 
	volatile unsigned int	M1WR;						//  0x038 R/W 0x00000003 t_wr Register 
	volatile unsigned int	M1WTR;					//  0x03C R/W 0x00000002 t_wtr Register 
	volatile unsigned int	M1XP;						//  0x040 R/W 0x00000001 t_xp Register 
	volatile unsigned int	M1XSR;					//  0x044 R/W 0x0000000A t_xsr Register 
	volatile unsigned int	M1ESR;					//  0x048 R/W 0x00000014 t_esr Register 
	volatile unsigned int	M1CFG2;					//  0x04C R/W  -  Memory_cfg2 Register 
	volatile unsigned int	M1CFG3;					//  0x050 R/W 0x00000007 Memory_cfg3 Register 
	volatile unsigned int	NOTDEFINE3[43];		//	 - 0x054- 0x0FC   Reserved 
	volatile unsigned int	M1ID0;					//  0x100 R/W 0x00000000 AXI ID0 configuration Register 
	volatile unsigned int	M1ID1;					//  0x104 R/W 0x00000000 AXI ID1 configuration Register 
	volatile unsigned int	M1ID2;					//  0x108 R/W 0x00000000 AXI ID2 configuration Register 
	volatile unsigned int	M1ID3;					//  0x10C R/W 0x00000000 AXI ID3 configuration Register 
	volatile unsigned int	M1ID4;					//  0x110 R/W 0x00000000 AXI ID4 configuration Register 
	volatile unsigned int	M1ID5;					//  0x114 R/W 0x00000000 AXI ID5 configuration Register 
	volatile unsigned int	M1ID6;					//  0x118 R/W 0x00000000 AXI ID6 configuration Register 
	volatile unsigned int	M1ID7;					//  0x11C R/W 0x00000000 AXI ID7 configuration Register 
	volatile unsigned int	M1ID8;					//  0x120 R/W 0x00000000 AXI ID8 configuration Register 
	volatile unsigned int	M1ID9;					//  0x124 R/W 0x00000000 AXI ID9 configuration Register 
	volatile unsigned int	M1ID10;					//   0x128  R/W  0x00000000  AXI ID10 configuration Register 
	volatile unsigned int	M1ID11;					//   0x12C  R/W  0x00000000  AXI ID11 configuration Register 
	volatile unsigned int	M1ID12;					//   0x130  R/W  0x00000000  AXI ID12 configuration Register 
	volatile unsigned int	M1ID13;					//   0x134  R/W  0x00000000  AXI ID13 configuration Register 
	volatile unsigned int	M1ID14;					//   0x138  R/W  0x00000000  AXI ID14 configuration Register 
	volatile unsigned int	M1ID15;					//   0x13C  R/W  0x00000000  AXI ID15 configuration Register 
	volatile unsigned int	NOTDEFINE4[48];			//	 - 0x140- 0x1FC   Reserved 
	volatile unsigned int	M1CH0;					//  0x200 R/W 0x0000FF00 CHIP ID0 configuration Register 
	volatile unsigned int	M1CH1;					//  0x204 R/W 0x0000FF00 CHIP ID1 configuration Register 
	volatile unsigned int	M1CH2;					//  0x208 R/W 0x0000FF00 CHIP ID2 configuration Register 
	volatile unsigned int	M1CH3;					//  0x20C R/W 0x0000FF00 CHIP ID3 configuration Register 
}DRAMMX, *PDRAMMX;

typedef struct _DRAMPHY{
	volatile unsigned int	REG0; 	//  0x400  R/W  0x00000000  PHY Mode Control Register 
	volatile unsigned int	REG1; 	// 0x404 R/ RW 0x00000018  DLL Control & Status Register 
	volatile unsigned int	REG2; 	// 0x408 R/W 0x00000000 DLL Phase Detector configuration Register 
	volatile unsigned int	REG3; 	//  0x40C  R/W  0x00000000  Gate Control Register 
	volatile unsigned int	REG4; 	// 0x410  R/W  0x00000000  Read Data Slice 0 Control Register 
	volatile unsigned int	REG5; 	//  0x414  R/W  0x00000000  Read Data Slice 1 Control Register 
	volatile unsigned int	REG6; 	//  0x418  RO  0x00000000  Read Data Slice 2 Control Register 
	volatile unsigned int	REG7; 	//  0x41C  R/W  0x00000000  Read Data Slice 3 Control Register 
	volatile unsigned int	REG8; 	//  0x420  R/W  0x00000000  CLK Delay Register 
	volatile unsigned int	REG9; 	//  0x424  R/W  0x00000000  DLL Force Lock Value Register 
	volatile unsigned int	REG10; 	// 0x428 R/W 0x00000000 ZQ Calibration Control Register 
	volatile unsigned int	REG11; 	//  0x42C  RO  0x00000000  ZQ Calibration Status Register 
	volatile unsigned int	REG12; 	// 0x430 R/W 0x00000000 Read Delay Register 
}DRAMPHY, *PDRAMPHY;

typedef struct _DRAMMISC{
	volatile unsigned int	M0CFG0; 		//0x00 R/W 0x80400000 SDR/DDR SDRAM Controller Configuration Register 0 
	volatile unsigned int	M0CFG1; 		//0x04 R/RW 0x00000018 SDR/DDR SDRAM Controller Configuration Register 1 
	volatile unsigned int	NOTDEFINE0[2];	//	 0x08-0x0C   Reserved 
	volatile unsigned int	M1CFG0; 		//0x10 R/W 0x80000000 DDR2 SDRAM Controller Configuration Register 0 
	volatile unsigned int	M1CFG1; 		//0x14 R/W 0x00000000 DDR2 SDRAM Controller Configuration Register 1 
	volatile unsigned int	NOTDEFINE1[2];	//	- 0x18- 0x1C   Reserved 
	volatile unsigned int	COMMON; 		//0x20 R/W 0x00010103  Common Control Register 
	volatile unsigned int	PHYCTRL; 		//0x24  R/W  0x00000000  SDRAM PHY Control Register 
	volatile unsigned int	PHYTSTS; 		//0x28  RO  0x00000000  SDRAM PHY Status Register 
	volatile unsigned int	IOCFG;  		//0x2C  R/W  0x00000000  SDRAM IO Control Register 
}DRAMMISC, *PDRAMMISC;

typedef struct _DRAMMEMBUS{
	volatile unsigned int	CKDOWN;				//0x04 RW 0x00000000 Clock Enable control over memory bus com-ponents
}DRAMMEMBUS, *PDRAMMEMBUS;

/************************************************************************
*	4-1. MISC CORE BUS CONFIGURATION REGISTERS				(Base Addr = 0xF0101000)
************************************************************************/
//#define	HwCORECFG_BASE				*(volatile unsigned long *)0xF0101000	************************************************************************/
typedef struct _MISCCOREBUS{
	volatile unsigned int	CORECFG;				//R/W  0x90000000 Core Bus Configuration Register 
}MISCCOREBUS, *PMISCCOREBUS;

/************************************************************************
*	4-2. Virtual MMU Table  Register Define	(Base Addr = 0xF7000000)
************************************************************************/
//#define	HwVMT_BASE				*(volatile unsigned long *)0x20000000	// VMT Base Regiseter
//#define	HwREGION_BASE			*(volatile unsigned long *)0xF7000000	// R/W, Configuration Register for Region 0

typedef struct _VMTREGION{
	volatile unsigned int	REGION0;				// 0x00 R/W - Configuration Register for Region 0 
	volatile unsigned int	REGION1;				// 0x04 R/W - Configuration Register for Region 1 
	volatile unsigned int	REGION2;				// 0x08 R/W - Configuration Register for Region 2 
	volatile unsigned int	REGION3;				// 0x0C R/W - Configuration Register for Region 3 
	volatile unsigned int	REGION4;				// 0x10 R/W - Configuration Register for Region 4 
	volatile unsigned int	REGION5;				// 0x14 R/W - Configuration Register for Region 5 
	volatile unsigned int	REGION6;				// 0x18 R/W - Configuration Register for Region 6 
	volatile unsigned int	REGION7;				// 0x1C R/W - Configuration Register for Region 7 
}VMTREGION, *PVMTREGION;


/*******************************************************************************
*	 TCC8900_DataSheet_PART 5 IO BUS_V0.00 Dec.11 2008
********************************************************************************/
/*******************************************************************************
*	 4. Memory Stick Host Controller Register Define   (Base Addr = 0xF0590000)
********************************************************************************/
//#define HwSMSHC_BASE                            *(volatile unsigned long*)0xF0590000
//#define HwSMSHCPORTCFG_BASE                     *(volatile unsigned long*)0xF05F1000
typedef struct _SMSHC{
	volatile unsigned int	MS_CONTROL_PROGRAMCOUNTREG;         //  0x00    // R/W 0x0070_1000
	volatile unsigned int	MS_SYSTEMREG;                       //  0x04    // R/W 0x0800_XXXX
	volatile unsigned int	MS_FLAGREG;                         //  0x08    // R 0x4000_XXXX
	volatile unsigned int	MS_MEMORY_CONTROLREG;               //  0x0C    // R/W 0x0001_7000
	volatile unsigned int	MS_GENERALREG01;                    //  0x10    // R/W 0x8000_9000
	volatile unsigned int	MS_GENERALREG23;                    //  0x14    // R/W 0xA000_B000
	volatile unsigned int	MS_GENERALREG45;                    //  0x18    // R/W 0xC000_D000
	volatile unsigned int	MS_TIMERREG;                        //  0x1C    // R 0xE000XXXX
	volatile unsigned int	MS_INSTRUCTIONREG;                  //  0x20    // R/W 0xXXXX_XXXX
	volatile unsigned int	MS_GENERALDATAFIFO;                 //  0x24    // R/W 0x0000_0000
	volatile unsigned int	MS_PAGEBUFFER;                      //  0x28    // R/W 0xXXXX_XXXX
	volatile unsigned int	MS_VERSIONREG;                      //  0x2C    // R 0xXXXX_XXXX
	volatile unsigned int	MS_MSHC_COMMANDREG;                 //  0x30    // R/W 0x0000_XXXX
	volatile unsigned int	MS_MSHC_DATAREG;                    //  0x34    // R/W 0x0000_0000
	volatile unsigned int	MS_MSHC_STATUSREG;                  //  0x38    // R 0x1000_XXXX
	volatile unsigned int	MS_MSHC_SYSTEMREG;                  //  0x3C    // R/W 0x20A5_XXXX
	volatile unsigned int	MS_MSHC_USERCUSTOMREG;              //  0x40    // R 0x0220_XXXX
	volatile unsigned int	MS_MSHC_FIFOCTRLREG;                //  0x44    // R/W 0x0001_XXXX
	volatile unsigned int	NOTDEFINE0;                         //
	volatile unsigned int	MS_MSHC_DMACTRLREG;                 //  0x4C    // R/W 0x0000_XXXX
}SMSHC, *PSMSHC;

typedef struct _SMSHCPORTCFG{
    volatile unsigned int	MS_PORTCFG;                         //   0xF05F1000    // R/W 0x00000000
    volatile unsigned int	MS_PORTDLY;                         //   0xF05F1004    // R/W 0x00000000
}SMSHCPORTCFG, *PSMSHCPORTCFG;

/*******************************************************************************
*	 5. SD/SDIO/MMC/CE-ATA Host Controller Register Define   (Base Addr = 0xF0590000)
********************************************************************************/
/*
#define HwSDCORE0SLOT1_BASE                     *(volatile unsigned long*)0xF05A0000 // Core 0 Slot 0
#define HwSDCORE0SLOT2_BASE                     *(volatile unsigned long*)0xF05A0100 // Core 0 Slot 1
#define HwSDCORE1SLOT2_BASE                     *(volatile unsigned long*)0xF05A0200 // Core 1 Slot 2
#define HwSDCORE1SLOT3_BASE                     *(volatile unsigned long*)0xF05A0300 // Core 1 Slot 3
#define HwSDCHCTRL_BASE                         *(volatile unsigned long*)0xF05A0800 // Channel Control Register
*/
typedef struct _SDHOST{
	volatile unsigned short 	SDMA;                       // 0x000 R/W 0x0000 SDMA System Address
	volatile unsigned short     NOTDEFINE0;                 // 0x002
	volatile unsigned short 	BSIZE;                      // 0x004 R/W 0x0000 Block Size
	volatile unsigned short 	BCNT;                       // 0x006 R/W 0x0000 Block Count
	volatile unsigned short 	ARG;                        // 0x008 R/W 0x0000 Argument
	volatile unsigned short     NOTDEFINE1;                 // 0x00A
	volatile unsigned short 	TMODE;                      // 0x00C R/W 0x0000 Transfer Mode
	volatile unsigned short 	CMD;                        // 0x00E R/W 0x0000 Command
	volatile unsigned short 	RESP0;                      // 0x010 R 0x0000 Response0
	volatile unsigned short 	RESP1;                      // 0x012 R 0x0000 Response1
	volatile unsigned short 	RESP2;                      // 0x014 R 0x0000 Response2
	volatile unsigned short 	RESP3;                      // 0x016 R 0x0000 Response3
	volatile unsigned short 	RESP4;                      // 0x018 R 0x0000 Response4
	volatile unsigned short 	RESP5;                      // 0x01A R 0x0000 Response5
	volatile unsigned short 	RESP6;                      // 0x01C R 0x0000 Response6
	volatile unsigned short 	RESP7;                      // 0x01E R 0x0000 Response7
	volatile unsigned short 	DATAL;                      // 0x020 R/W - Buffer Data Port(Low)
	volatile unsigned short 	DATAH;                      // 0x022 R/W - Buffer Data Port(High)
	volatile unsigned short 	STATEL;                     // 0x024 R 0x0000 Present State(Low)
	volatile unsigned short 	STATEH;                     // 0x026 R 0x0000 Present State(High)
	volatile unsigned short 	CONTL;                      // 0x028 R/W 0x0000 Power Control / Host Control
	volatile unsigned short 	CONTH;                      // 0x02A R/W 0x0000 Wakeup Control / Block Gap Control
	volatile unsigned short 	CLK;                        // 0x02C R/W 0x0000 Clock Control
	volatile unsigned short 	TIME;                       // 0x02E R/W 0x0000 Software Reset / Timeout Control
	volatile unsigned short 	STSL;                       // 0x030 R 0x0000 Normal Interrupt Status(Low)
	volatile unsigned short 	STSH;                       // 0x032 R 0x0000 Normal Interrupt Status(High)
	volatile unsigned short 	STSENL;                     // 0x034 R/W 0x0000 Normal Interrupt Status Enable(Low)
	volatile unsigned short 	STSENH;                     // 0x036 R/W 0x0000 Normal Interrupt Status Enable(High)
	volatile unsigned short 	INTENL;                     // 0x038 R/W 0x0000 Normal Interrupt Signal Enable(Low)
	volatile unsigned short 	INTENH;                     // 0x03A R/W 0x0000 Normal Interrupt Signal Enable(High)
	volatile unsigned short 	CMD12ERR;                   // 0x03C R 0x0000 Auto CMD12 Error Status
	volatile unsigned short     NOTDEFINE2;                 // 0x03E
	volatile unsigned short 	CAPL;                       // 0x040 R 0x30B0 Capabilities(Low)
	volatile unsigned short 	CAPH;                       // 0x042 R 0x69EF Capabilities(High)
	volatile unsigned short     NOTDEFINE3[2];              // 0x044, 0x046
	volatile unsigned short 	CURL;                       // 0x048 R 0x0001 Maximum Current Capabilities(Low)
	volatile unsigned short 	CURH;                       // 0x04A R 0x0000 Maximum Current Capabilities(High)
	volatile unsigned short     NOTDEFINE4[2];              // 0x04C, 0x04E
	volatile unsigned short 	FORCEL;                     // 0x050 W 0x0000 Force event for AutoCmd12 Error
	volatile unsigned short 	FORCEH;                     // 0x052 W 0x0000 Force event for Error Interrupt Status
	volatile unsigned short 	AUDIO_DMAERR;               // 0x054 R/W 0x0000 AUDIO DMA Error Status
	volatile unsigned short     NOTDEFINE5;                 // 0x056
	volatile unsigned short 	ADDR0;                      // 0x058 R/W 0x0000 AUDIO DMA Address[15:0]
	volatile unsigned short 	ADDR1;                      // 0x05A R/W 0x0000 AUDIO DMA Address[31:16]
	volatile unsigned short 	ADDR2;                      // 0x05C R/W 0x0000 AUDIO DMA Address[47:32]
	volatile unsigned short 	ADDR3;                      // 0x05E R/W 0x0000 AUDIO DMA Address[63:48]
	volatile unsigned short     NOTDEFINE6[78];             // 0x060~0x0FA
	volatile unsigned short 	SLOT;                       // 0x0FC R 0x0000 Slot Interrupt Status
	volatile unsigned short 	VERSION;                    // 0x0FE R 0x0002 Host Controller Version    
}SDHOST, *PSDHOST;

typedef struct _SDCHCTRL{
	volatile unsigned int	SDPORTCTRL;                          //  0x00 R/W 0x0000 SD/MMC port control register
	volatile unsigned int	SDPORTDLY0;                          //  0x04 R/W 0x0000 SD/MMC output delay control register
	volatile unsigned int	SDPORTDLY1;                          //  0x08 R/W 0x0000 SD/MMC output delay control register
	volatile unsigned int	SDPORTDLY2;                          //  0x0C R/W 0x0000 SD/MMC output delay control register
	volatile unsigned int	SDPORTDLY3;                          //  0x10 R/W 0x0000 SD/MMC output delay control register    
}SDCHCTRL, *PSDCHCTRL;


/*******************************************************************************
*	 6. NAND Flash Controller(NFC) Register Define   (Base Addr = 0xF05B0000)
********************************************************************************/
//#define HwNFC_BASE                              *(volatile unsigned long*)0xF05B0000

typedef struct _NFC{
	volatile unsigned int	NFC_CMD;				// 0x000  W  	NAND Flash Command Register
	volatile unsigned int	NFC_LADDR;				// 0x004  W  	NAND Flash Linear Address Register
	volatile unsigned int	NFC_BADDR;				// 0x008  W  	NAND Flash Block Address Register
	volatile unsigned int	NFC_SADDR;				// 0x00C  W  	NAND Flash Signal Address Register
	volatile unsigned int	NFC_WDATA;				// 0x01x  R/W	NAND Flash Word Data Register
	volatile unsigned int	NOTDEFINE0[3];				// 0x01x  R/W	NAND Flash Word Data Register
	volatile unsigned int	NFC_LDATA;			// 0x02x/3x R/W	NAND Flash Linear Data Register
	volatile unsigned int	NOTDEFINE1[7];				// 0x01x  R/W	NAND Flash Word Data Register
	volatile unsigned int	NFC_SDATA;				// 0x040  R/W	NAND Flash Single Data Register
	volatile unsigned int	NOTDEFINE2[3];			// 0x044     	Not Used
	volatile unsigned int	NFC_CTRL;				// 0x050  R/W	NAND Flash Control Register
	volatile unsigned int	NFC_PSTART; 			// 0x054  W  	NAND Flash Program Start Register
	volatile unsigned int	NFC_RSTART; 			// 0x058  W  	NAND Flash Read Start Register
	volatile unsigned int	NFC_DSIZE;				// 0x05C  R/W	NAND Flash Data Size Register
	volatile unsigned int	NFC_IREQ;				// 0x060  R/W	NAND Flash Interrupt Request Register
	volatile unsigned int	NFC_RST;				// 0x064  W  	NAND Flash Controller Reset Register
	volatile unsigned int	NFC_CTRL1;				// 0x068  R/W	NAND Flash Control Register 1
	volatile unsigned int	NOTDEFINE3;				// 0x06C     	Not Used
	volatile unsigned int	NFC_MDATA[4];			// 0x07x  R/W	NAND Flash Multiple Data Register	
}NFC, *PNFC;

/*******************************************************************************
*	 7. Static Memory Controller(SMC) Register Define   (Base Addr = 0xF05F0000)
********************************************************************************/
//#define HwSMC_BASE                            *(volatile unsigned long*)0xF05F0000
typedef struct _SMC{
	volatile unsigned int	STATUS;                        //  0x00 R/W Unknown Status Register
	volatile unsigned int   NOTDEFINE0[7];
	volatile unsigned int	CSNCFG0;                       //  0x20 R 0x4b40_3183 External Chip Select0 Config Register
	volatile unsigned int	CSNCFG1;                       //  0x24 R/W 0x4b40_1104 External Chip Select1 Config Register
	volatile unsigned int	CSNCFG2;                       //  0x28 W 0x4b40_4082 External Chip Select2 Config Register
	volatile unsigned int	CSNCFG3;                       //  0x2C R/W 0x4b40_20C5 External Chip Select3 Config. Register
	volatile unsigned int	CSNOFFSET;                     //  0x30 R/W 0x0 Wapping Address Mode OFFSET Register
	volatile unsigned int	INDIRADDR;                     //  0x34 R/W 0x0 Indirect Address    
}SMC, *PSMC;

/*******************************************************************************
*	 8. External Device Interface (EDI) Register Define   (Base Addr = 0xF05F6000)
********************************************************************************/
//#define HwEDI_BASE                            *(volatile unsigned long*)0xF05F6000

typedef struct _EDI{
	volatile unsigned int	EDI_CTRL;                       //  0x00 R/W 0x00000000 EDI Control Register.
	volatile unsigned int	EDI_CSNCFG0;                    //  0x04 R/W 0x00543210 EDI CSN Configuration Register 0.
	volatile unsigned int	EDI_CSNCFG1;                    //  0x08 R/W 0x00BA9876 EDI CSN Configuration Register 1.
	volatile unsigned int	NOTDEFINE0[2];                  //  Reserved 0x0C R/W - -
	volatile unsigned int	EDI_RDYCFG;                     //  0x14 R/W 0x76543210 EDI Ready Configuration Register
	volatile unsigned int	NOTDEFINE1[2];                  //  Reserved 0x18 R/W 0x00000000 EDI Time-Out Configuration Register 0
	volatile unsigned int	EDI_REQOFF;                     //  0x20 R/W 0x00000000 EDI Request OFF Flag register    
}EDI, *PEDI;

/*******************************************************************************
*	 9. IDE Controller Register Define   (Base Addr = 0xF0520000)
********************************************************************************/
//#define HwIDE_BASE                            *(volatile unsigned long*)0xF05F6000

typedef struct _IDE{
	volatile unsigned int	CS00;                           //  0x00 R/W - PIO CS0n Access Register
	volatile unsigned int	CS01;                           //  0x04
	volatile unsigned int	CS02;                           //  0x08
	volatile unsigned int	CS03;                           //  0x0C
	volatile unsigned int	CS04;                           //  0x10
	volatile unsigned int	CS05;                           //  0x14
	volatile unsigned int	CS06;                           //  0x18
	volatile unsigned int	CS07;                           //  0x1C
	volatile unsigned int	CS10;                           //  0x20 R/W - PIO CS1n Access Register
	volatile unsigned int	CS11;                           //  0x24
	volatile unsigned int	CS12;                           //  0x28
	volatile unsigned int	CS13;                           //  0x2C
	volatile unsigned int	CS14;                           //  0x30
	volatile unsigned int	CS15;                           //  0x34
	volatile unsigned int	CS16;                           //  0x38
	volatile unsigned int	CS17;                           //  0x3C
	volatile unsigned int	PIOCTRL;                        //  0x40 R/W 0x00600000 PIO Mode Control Register
	volatile unsigned int	UDMACTRL;                       //  0x44 R/W 0x00000000 UDMA Mode Control Register
	volatile unsigned int	IDMACTRL;                       //  0x48 R/W 0x00000000 IDMA Control Register
	volatile unsigned int	IDMASA;                         //  0x4C R/W 0x00000000 IDMA Source Address Register
	volatile unsigned int	IDMASP;                         //  0x50 R/W 0x00000000 IDMA Source Parameter Register
	volatile unsigned int	IDMACSA;                        //  0x54 R 0x00000000 IDMA Current Source Address Register
	volatile unsigned int	IDMADA;                         //  0x58 R/W 0x00000000 IDMA Destination Address Register
	volatile unsigned int	IDMADP;                         //  0x5C R/W 0x00000000 IDMA Destination Parameter Register
	volatile unsigned int	IDMACDA;                        //  0x60 R 0x00000000 IDMA Current Destination Address Register
	volatile unsigned int	IDEINT;                         //  0x64 R/W 0x0000_0000 IDE Interrupt Register
	volatile unsigned int	UDMATCNT;                       //  0x68 R/W 0x00FF_FFFF UDMA Transfer Counter Register
	volatile unsigned int	UDMAIN;                         //  0x6C R - UDMA-IN Access Register
	volatile unsigned int	UDMAOUT;                        //  0x70 W - UDMA-OUT Access register
	volatile unsigned int	UDMACRC;                        //  0x74 R 0x0000_4ABA UDMA CRC Register
	volatile unsigned int	UDMACTCNT;                      //  0x78 R 0x00FF_FFFF UDMA Current Transfer Counter Register    
}IDE, *PIDE;


/*******************************************************************************
*	 10. SATA Interface Register Define   (Base Addr = 0xF0560000)
********************************************************************************/
//#define HwSATA_BASE                             *(volatile unsigned long*)0xF0560000

typedef struct _SATA{
	volatile unsigned int	CDR0;                           //  0x00 16 RO/WO - Data register in PIO mode Dependencies: Read-only for PIO read/receive operation, write-only for PIO write/transmit operation
	volatile unsigned int	CDR1;                           //  0x04 8/8/8 RO/WO/WO 0xFF/0x00/0x00 Error register Feature Register(currnet value) Feature Expanded Register(previouis value)
	volatile unsigned int	CDR2;                           //  0x08 8/8 R/W 0xFF Sector count register (current value) Sector count expanded register (previous value)
	volatile unsigned int	CDR3;                           //  0x0C 8/8 R/W 0xFF Sector number register (current value) Sector number expanded register (previous value)
	volatile unsigned int	CDR4;                           //  0x10 8/8 R/W 0xFF Cylinder low register (current value) Cylinder low expanded register (previous value)
	volatile unsigned int	CDR5;                           //  0x14 8/8 R/W 0xFF Cylinder high register (current value) Cylinder high expanded register (previous value)
	volatile unsigned int	CDR6;                           //  0x18 8 R/W 0xEF Device/ Head register 
	volatile unsigned int	CDR7;                           //  0x1C 8/8 RO/WO 0x7F/0x00 Status register Dependencies: Value is 0x7F on power-up, then 0x80 when device presence is detected via PHY READY condition. Command register 
	volatile unsigned int	CLR0;                           //  0x20 8/8 RO/WO 0x7F/0x00 Alternative status register Dependencies: Value is 0x7F on power-up, then 0x80 when device presence is detected via PHY READY condition. Device control register
	volatile unsigned int	SCR0;                           //  0x24 32 RO 0x0 SStatus Register
	volatile unsigned int	SCR1;                           //  0x28 32 R/W 0x0 SError Register
	volatile unsigned int	SCR2;                           //  0x2C 32 R/W 0x0 SControl Register
	volatile unsigned int	SCR3;                           //  0x30 32 R/W 0x0 SActive Register
	volatile unsigned int	SCR4;                           //  0x34 32 R/W 0x0 Snotification Register
	volatile unsigned int	SCR5;                           //  0x38
	volatile unsigned int	SCR6;                           //  0x3C
	volatile unsigned int	SCR7;                           //  0x40
	volatile unsigned int	SCR8;                           //  0x44
	volatile unsigned int	SCR9;                           //  0x48
	volatile unsigned int	SCR10;                          //  0x4C
	volatile unsigned int	SCR11;                          //  0x50
	volatile unsigned int	SCR12;                          //  0x54
	volatile unsigned int	SCR13;                          //  0x58
	volatile unsigned int	SCR14;                          //  0x5C
	volatile unsigned int	SCR15;                          //  0x60  See description 0x0 Reserved for SATA Dependencies: Reads to these locations return zeros; writes have no effect
	volatile unsigned int	FPTAGR;                         //  0x64 32 RO 0x0 First Party DMA tag Register
	volatile unsigned int	FPBOR;                          //  0x68 32 RO 0x0 First Party DMA buffer offset Register
	volatile unsigned int	FPTCR;                          //  0x6C 32 RO 0x0 First Party DMA transfer count Register
	volatile unsigned int	DMACR;                          //  0x70 32 R/W 0x0 DMA Control Register
	volatile unsigned int	DBTSR;                          //  0x74 32 R/W 0x0014 _0010 DMA Burst Transaction Size register
	volatile unsigned int	INTPR;                          //  0x78 32 R/W 0x0 Interrupt Pending Register
	volatile unsigned int	INTMR;                          //  0x7C 32 RO 0x0 Interrupt Mask Register
	volatile unsigned int	ERRMR;                          //  0x80 32 RO 0x0 Error Mask Register
	volatile unsigned int	LLCR;                           //  0x84 32 R/W 0x0000 _0007 Link Layer Control Register    
	volatile unsigned int	NOTDEFINE0[222]; 				//0x88 ~ 0x3FC
	volatile unsigned int	DMADR[256];                     //  0x400-0x7FC FIFO Location in DMA mode
}SATA, *PSATA;


/*******************************************************************************
*	 11-1. Audio DMA Controller Register Define   (Base Addr = 0xF0533000)
********************************************************************************/
//#define HwADMA_BASE                         *(volatile unsigned long*)0xF0533000

typedef struct _ADMA{
	volatile unsigned int	RxDaDar;                           //  0x00 R/W 0x00000000 DAI Rx (Right) Data Destination Address
	volatile unsigned int	RxDaParam;                         //  0x04 R/W 0x00000000 DAI Rx Parameters
	volatile unsigned int	RxDaTCnt;                          //  0x08 R/W 0x00000000 DAI Rx Transmission Counter Register
	volatile unsigned int	RxDaCdar;                          //  0x0C R 0x00000000 DAI Rx (Right) Data Current Destination Address
	volatile unsigned int	RxCdDar;                           //  0x10 R/W 0x00000000 CDIF(SPDIF) Rx (Right) Data Destination Address
	volatile unsigned int	RxCdParam;                         //  0x14 R/W 0x00000000 CDIF(SPDIF) Rx Parameters
	volatile unsigned int	RxCdTCnt;                          //  0x18 R/W 0x00000000 CDIF(SPDIF) Rx Transmission Counter Register
	volatile unsigned int	RxCdCdar;                          //  0x1C R 0x00000000 CDIF(SPDIF) Rx (Right) Data Current Destination Address
	volatile unsigned int   NOTDEFINE0[2];
	volatile unsigned int	RxDaDarL;                          //  0x28 R/W 0x00000000 DAI Rx Left Data Destination Address
	volatile unsigned int	RxDaCdarL;                         //  0x2C R 0x00000000 DAI Rx Left Data Current Destination Address
	volatile unsigned int	RxCdDarL;                          //  0x30 R/W 0x00000000 CDIF(SPDIF) Rx Left Data Destination Address
	volatile unsigned int	RxCdCdarL;                         //  0x34 R 0x00000000 CDIF(SPDIF) Rx Left Data Current Destination Address
	volatile unsigned int	TransCtrl;                         //  0x38 R/W 0x0000AA00 DMA Transfer Control Register
	volatile unsigned int	RptCtrl;                           //  0x3C R/W 0x00000000 DMA Repeat Control Register
	volatile unsigned int	TxDaSar;                           //  0x40 R/W 0x00000000 DAI Tx (Right) Data Source Address
	volatile unsigned int	TxDaParam;                         //  0x44 R/W 0x00000000 DAI Tx Parameters
	volatile unsigned int	TxDaTCnt;                          //  0x48 R/W 0x00000000 DAI Tx Transmission Counter Register
	volatile unsigned int	TxDaCsar;                          //  0x4C R 0x00000000 DAI Tx (Right) Data Current Source Address
	volatile unsigned int	TxSpSar;                           //  0x50 R/W 0x00000000 SPDIF Tx (Right) Data Source Address
	volatile unsigned int	TxSpParam;                         //  0x54 R/W 0x00000000 SPDIF Tx Parameters
	volatile unsigned int	TxSpTCnt;                          //  0x58 R/W 0x00000000 SPDIF Tx Transmission Counter Register
	volatile unsigned int	TxSpCsar;                          //  0x5C R 0x00000000 SPDIF Tx (Right) Data Current Source Address
	volatile unsigned int   NOTDEFINE1[2];
	volatile unsigned int	TxDaSarL;                          //  0x68 R/W 0x00000000 DAI Tx Left Data Source Address
	volatile unsigned int	TxDaCsarL;                         //  0x6C R 0x00000000 DAI Tx Left Data Current Source Address
	volatile unsigned int	TxSpSarL;                          //  0x70 R/W 0x00000000 SPDIF Tx Left Data Source Address
	volatile unsigned int	TxSpCsarL;                         //  0x74 R 0x00000000 SPDIF Tx Left Data Current Source address
	volatile unsigned int	ChCtrl;                            //  0x78 R/W 0x00008000 DMA Channel Control Register
	volatile unsigned int	IntStatus;                         //  0x7C R/W 0x00000000 DMA Interrupt Status Register
	volatile unsigned int	GIntReq;                           //  0x80 R/W 0x00000000 General Interrupt Request
	volatile unsigned int	GIntStatus;                        //  0x84 R 0x00000000 General Interrupt Status
	volatile unsigned int   NOTDEFINE2[6];	
	volatile unsigned int	RxDaDar1;                          //  0x100 R/W 0x00000000 DAI1 Rx (Right) Data Destination Address
	volatile unsigned int	RxDaDar2;                          //  0x104 R/W 0x00000000 DAI2 Rx (Right) Data Destination Address
	volatile unsigned int	RxDaDar3;                          //  0x108 R/W 0x00000000 DAI3 Rx (Right) Data Destination Address
	volatile unsigned int	RxDaCar1;                          //  0x10C R 0x00000000 DAI1 Rx (Right) Data Current Destination Address
	volatile unsigned int	RxDaCar2;                          //  0x110 R 0x00000000 DAI2 Rx (Right) Data Current Destination Address
	volatile unsigned int	RxDaCar3;                          //  0x114 R 0x00000000 DAI3 Rx (Right) Data Current Destination Address
	volatile unsigned int	RxDaDarL1;                         //  0x118 R/W 0x00000000 DAI1 Rx Left Data Destination Address
	volatile unsigned int	RxDaDarL2;                         //  0x11C R/W 0x00000000 DAI2 Rx Left Data Destination Address
	volatile unsigned int	RxDaDarL3;                         //  0x120 R/W 0x00000000 DAI3 Rx Left Data Destination Address
	volatile unsigned int	RxDaCarL1;                         //  0x124 R 0x00000000 DAI1 Rx Left Data Current Destination Address
	volatile unsigned int	RxDaCarL2;                         //  0x128 R 0x00000000 DAI2 Rx Left Data Current Destination Address
	volatile unsigned int	RxDaCarL3;                         //  0x12C R 0x00000000 DAI3 Rx Left Data Current Destination Address
	volatile unsigned int	TxDaSar1;                          //  0x130 R/W 0x00000000 DAI1 Tx (Right) Data Source Address
	volatile unsigned int	TxDaSar2;                          //  0x134 R/W 0x00000000 DAI2 Tx (Right) Data Source Address
	volatile unsigned int	TxDaSar3;                          //  0x138 R/W 0x00000000 DAI3 Tx (Right) Data Source Address
	volatile unsigned int	TxDaCsar1;                         //  0x13C R 0x00000000 DAI1 Tx (Right) Data Current Source Address
	volatile unsigned int	TxDaCsar2;                         //  0x140 R 0x00000000 DAI2 Tx (Right) Data Current Source Address
	volatile unsigned int	TxDaCsar3;                         //  0x144 R 0x00000000 DAI3 Tx (Right) Data Current Source Address
	volatile unsigned int	TxDaDarL1;                         //  0x148 R/W 0x00000000 DAI1 Tx Left Data Source Address
	volatile unsigned int	TxDaDarL2;                         //  0x14C R/W 0x00000000 DAI2 Tx Left Data Source Address
	volatile unsigned int	TxDaDarL3;                         //  0x150 R/W 0x00000000 DAI3 Tx Left Data Source Address
	volatile unsigned int	TxDaCarL1;                         //  0x154 R 0x00000000 DAI1 Tx Left Data Current Source Address
	volatile unsigned int	TxDaCarL2;                         //  0x158 R 0x00000000 DAI2 Tx Left Data Current Source Address
	volatile unsigned int	TxDaCarL3;                         //  0x15C R 0x00000000 DAI3 Tx Left Data Current Source Address
}ADMA, *PADMA;


/*******************************************************************************
*	 11-2. DAI Register Define   (Base Addr = 0xF0534000)
********************************************************************************/
//#define HwADMA_DAIBASE                         *(volatile unsigned long*)0xF0534000

typedef struct _ADMADAI{
	volatile unsigned int	DADIR0;                         //  0x00 R - Digital Audio Input Register 0
	volatile unsigned int	DADIR1;                         //  0x04 R - Digital Audio Input Register 1
	volatile unsigned int	DADIR2;                         //  0x08 R - Digital Audio Input Register 2
	volatile unsigned int	DADIR3;                         //  0x0C R - Digital Audio Input Register 3
	volatile unsigned int	DADIR4;                         //  0x10 R - Digital Audio Input Register 4
	volatile unsigned int	DADIR5;                         //  0x14 R - Digital Audio Input Register 5
	volatile unsigned int	DADIR6;                         //  0x18 R - Digital Audio Input Register 6
	volatile unsigned int	DADIR7;                         //  0x1C R - Digital Audio Input Register 7
	volatile unsigned int	DADOR0;                         //  0x20 R/W - Digital Audio Output Register 0
	volatile unsigned int	DADOR1;                         //  0x24 R/W - Digital Audio Output Register 1
	volatile unsigned int	DADOR2;                         //  0x28 R/W - Digital Audio Output Register 2
	volatile unsigned int	DADOR3;                         //  0x2C R/W - Digital Audio Output Register 3
	volatile unsigned int	DADOR4;                         //  0x30 R/W - Digital Audio Output Register 4
	volatile unsigned int	DADOR5;                         //  0x34 R/W - Digital Audio Output Register 5
	volatile unsigned int	DADOR6;                         //  0x38 R/W - Digital Audio Output Register 6
	volatile unsigned int	DADOR7;                         //  0x3C R/W - Digital Audio Output Register 7
	volatile unsigned int	DAMR;                           //  0x40 R/W 0x00000000 Digital Audio Mode Register
	volatile unsigned int	DAVC;                           //  0x44 R/W 0x0000 Digital Audio Volume Control Register
	volatile unsigned int	MCCR0;                          //  0x48 R/W 0x00000000 Multi Channel Control Register 0
	volatile unsigned int	MCCR1;                          //  0x4C R/W 0x00000000 Multi Channel Control Register 1    
}ADMADAI, *PADMADAI; 



/*******************************************************************************
*	 11-3. CDIF Register Define   (Base Addr = 0xF0534000)
********************************************************************************/
//#define HwADMA_CDIFBASE                         *(volatile unsigned long*)0xF0534080

typedef struct _ADMACDIF{
	volatile unsigned int	CDDI_0;                         //  0x80 R CD Digital Audio Input Register 0
	volatile unsigned int	CDDI_1;                         //  0x84 R CD Digital Audio Input Register 1
	volatile unsigned int	CDDI_2;                         //  0x88 R CD Digital Audio Input Register 2
	volatile unsigned int	CDDI_3;                         //  0x8C R CD Digital Audio Input Register 3
	volatile unsigned int	CDDI_4;                         //  0x90 R CD Digital Audio Input Register 4
	volatile unsigned int	CDDI_5;                         //  0x94 R CD Digital Audio Input Register 5
	volatile unsigned int	CDDI_6;                         //  0x98 R CD Digital Audio Input Register 6
	volatile unsigned int	CDDI_7;                         //  0x9C R CD Digital Audio Input Register 7
	volatile unsigned int	CICR;                           //  0xA0 R/W 0x0000 CD Interface Control Register    
}ADMACDIF, *PADMACDIF;


/*******************************************************************************
*	 11-4. ADMA_SPDIF Register Define   (Base Addr = 0xF0535000/0xF0535800)
********************************************************************************/
//#define HwADMA_SPDIFTXBASE                     *(volatile unsigned long*)0xF0535000
//#define HwADMA_SPDIFRXBASE                     *(volatile unsigned long*)0xF0535800

typedef struct _ADMASPDIFTX{
	volatile unsigned int	TxVersion;                         //  0x00 R 0x00003111 Version Register
	volatile unsigned int	TxConfig;                          //  0x04 R/W 0x00000000 Configuration Register
	volatile unsigned int	TxChStat;                          //  0x08 R/W 0x00000000 Channel Status Control Register
	volatile unsigned int	TxIntMask;                         //  0x0C R/W 0x00000000 Interrupt Mask Register
	volatile unsigned int	TxIntStat;                         //  0x10 R/W 0x00000000 Interrupt Status Register
	volatile unsigned int	NOTDEFINE0[24];
	volatile unsigned int	UserData;                          //  0x80~0xDC W - User Data Buffer
	volatile unsigned int	NOTDEFINE1[8];
	volatile unsigned int	ChStatus;                          //  0x100~0x15C W - Channel Status Buffer
	volatile unsigned int	NOTDEFINE2[40];
	volatile unsigned int	TxBuffer;                          //  0x200~0x23C W - Transmit Data Buffer
	volatile unsigned int	NOTDEFILE3[112];
	volatile unsigned int	DMACFG;                            //  0x400 R/W 0x00000007 Additional Configuration for DMA
	volatile unsigned int	NOTDEFINE4[159];
	volatile unsigned int	CSBUDB[24];                        //  0x680~0x6DC W - Merged Window for CSB/UDB   
}ADMASPDIFTX, *PADMASPDIFTX;

typedef union _RXCAP{
    volatile unsigned int	RxCapCtln[16];                //  0x840~0x87C(even) W 0x00000000 Channel Status Capture Control Register
    volatile unsigned int	RxCapn[16];                   //  0x840~0x87C(odd) W 0x00000000 Captured Channel Status / user bit
}RXCAP;
    
typedef struct _ADMASPDIFRX{
	volatile unsigned int	RxVersion;                        //  0x800 R 0x00080111 Version Register
	volatile unsigned int	RxConfig;                         //  0x804 R/W 0x00000000 Configuration Register
	volatile unsigned int	RxStatus;                         //  0x808 R 0x00000000 Signal Status Buffer
	volatile unsigned int	RxIntMask;                        //  0x80C R/W 0x00000000 Interrupt Mask Register
	volatile unsigned int	RxIntStat;                        //  0x810 R/W 0x00000000 Interrupt Status register
	volatile unsigned int   NOTDEFINE0[11];
	RXCAP   RxCap;
	//volatile unsigned int	RxCapCtln[16];                //  0x840~0x87C(even) W 0x00000000 Channel Status Capture Control Register
    //volatile unsigned int	RxCapn[16];                   //  0x840~0x87C(odd) W 0x00000000 Captured Channel Status / user bit
	volatile unsigned int	RxBuffer[8];                      //  0xA00~0xA1C W - Receive Data Buffer    
}ADMASPDIFRX, *PADMASPDIFRX;


/*******************************************************************************
*	 12-1. DAI Register Define   (Base Addr = 0xF0537000
********************************************************************************/
//#define HwDAI_BASE                          *(volatile unsigned long*)0xF0537000
typedef struct _DAI{
	volatile unsigned int	DADI_L0;                         //  0x00 R - Digital Audio Left Input Register 0
	volatile unsigned int	DADI_R0;                         //  0x04 R - Digital Audio Right Input Register 0
	volatile unsigned int	DADI_L1;                         //  0x08 R - Digital Audio Left Input Register 1
	volatile unsigned int	DADI_R1;                         //  0x0C R - Digital Audio Right Input Register 1
	volatile unsigned int	DADI_L2;                         //  0x10 R - Digital Audio Left Input Register 2
	volatile unsigned int	DADI_R2;                         //  0x14 R - Digital Audio Right Input Register 2
	volatile unsigned int	DADI_L3;                         //  0x18 R - Digital Audio Left Input Register 3
	volatile unsigned int	DADI_R3;                         //  0x1C R - Digital Audio Right Input Register 3
	volatile unsigned int	DADO_L0;                         //  0x20 R/W - Digital Audio Left Output Register 0
	volatile unsigned int	DADO_R0;                         //  0x24 R/W - Digital Audio Right Output Register 0
	volatile unsigned int	DADO_L1;                         //  0x28 R/W - Digital Audio Left Output Register 1
	volatile unsigned int	DADO_R1;                         //  0x2C R/W - Digital Audio Right Output Register 1
	volatile unsigned int	DADO_L2;                         //  0x30 R/W - Digital Audio Left Output Register 2
	volatile unsigned int	DADO_R2;                         //  0x34 R/W - Digital Audio Right Output Register 2
	volatile unsigned int	DADO_L3;                         //  0x38 R/W - Digital Audio Left Output Register 3
	volatile unsigned int	DADO_R3;                         //  0x3C R/W - Digital Audio Right Output Register 3
	volatile unsigned int	DAMR;                            //  0x40 R/W 0x00000000 Digital Audio Mode Register
	volatile unsigned int	DAVC;                            //  0x44 R/W 0x0000 Digital Audio Volume Control Register
}DAI, *PDAI;

/*******************************************************************************
*	 12-2. CDIF Register Define   (Base Addr = 0xF0537000
********************************************************************************/
//#define HwCDIF_BASE                             *(volatile unsigned long*)0xF0537080
typedef struct _CDIF{
	volatile unsigned int	CDDI_0;                         // 0x80 R CD Digital Audio Input Register 0
	volatile unsigned int	CDDI_1;                         // 0x84 R CD Digital Audio Input Register 1
	volatile unsigned int	CDDI_2;                         // 0x88 R CD Digital Audio Input Register 2
	volatile unsigned int	CDDI_3;                         // 0x8C R CD Digital Audio Input Register 3
	volatile unsigned int	CICR;                           // 0x90 R/W 0x0000 CD Interface Control Register    
}CDIF, *PCDIF;


/*******************************************************************************
*	 13. SPDIF Register Define   (Base Addr = 0xF0538000)
********************************************************************************/
//#define HwSPDIF_BASE                             *(volatile unsigned long*)0xF0538000
typedef struct _SPDIF{
	volatile unsigned int	TxVersion;                          // 0x00 R Version Register
	volatile unsigned int	TxConfig;                           // 0x04 R/W Configuration Register
	volatile unsigned int	TxChStat;                           // 0x08 R/W Channel Status Control Register
	volatile unsigned int	TxIntMask;                          // 0x0C R/W Interrupt Mask Register
	volatile unsigned int	TxIntStat;                          // 0x10 R/W Interrupt Status Register
	volatile unsigned int	UserData[24];                       // 0x80~0xDC W - User Data Buffer
	volatile unsigned int	ChStatus[24];                       // 0x100~0x15C W - Channel Status Buffer
	volatile unsigned int	TxBuffer[128];                      // 0x200~0x3FC W - Transmit Data Buffer
	volatile unsigned int	DMACFG;                             // 0x400 R/W - Additional Configuration for DMA    
}SPDIF, *PSPDIF;


/*******************************************************************************
*	 14-1. USB1.1 HOST Controller & Transceiver       (Base Addr = 0xF0500000)
********************************************************************************/
//#define HwUSBHOST_BASE *(volatile unsigned long*)0xF0500000
typedef struct _USBHOST11{
	volatile unsigned int	HcRevision;                         // 0x00 R 0x00000010
	volatile unsigned int	HcControl;                          // 0x04 R/W 0x00000000
	volatile unsigned int	HcCommandStatus;                    // 0x08 R 0x00000000
	volatile unsigned int	HcInterruptStatus;                  // 0x0C R 0x00000000
	volatile unsigned int	HcInterruptEnable;                  // 0x10 R/W 0x00000000
	volatile unsigned int	HcInterruptDisable;                 // 0x14 W 0x00000000
	volatile unsigned int	HcHCCA;                             // 0x18 R/W 0x00000000
	volatile unsigned int	HcPeriodCurrentED;                  // 0x1C R 0x00000000
	volatile unsigned int	HcControlHeadED;                    // 0x20 R/W 0x00000000
	volatile unsigned int	HcControlCurrentED;                 // 0x24 R/W 0x00000000
	volatile unsigned int	HcBulkHeadED;                       // 0x28 R/W 0x00000000
	volatile unsigned int	HcBulkCurrentED;                    // 0x2C R/W 0x00000000
	volatile unsigned int	HcDoneHead;                         // 0x30 R 0x00000000
	volatile unsigned int	HcRmInterval;                       // 0x34 R/W 0x00002EDF
	volatile unsigned int	HcFmRemaining;                      // 0x38 R/W 0x00000000
	volatile unsigned int	HcFmNumber;                         // 0x3C R/W 0x00000000
	volatile unsigned int	HcPeriodStart;                      // 0x40 R/W 0x00000000
	volatile unsigned int	HcLSThreshold;                      // 0x44 R/W 0x00000628
	volatile unsigned int	HcRhDescriptorA;                    // 0x48 R/W 0x02001202
	volatile unsigned int	HcRhDescriptorB;                    // 0x4C R/W 0x00000000
	volatile unsigned int	HcRhStatus;                         // 0x50 R/W 0x00000000
	volatile unsigned int	HcRhPortStatus1;                    // 0x54 R/W 0x00000100
	volatile unsigned int	HcRhPortStatus2;                    // 0x58 R/W 0x00000100
}USBHOST11, *PUSBHOST11;


/*******************************************************************************
*	 14-2. USB1.1 HOST Configuration Register        (Base Addr = 0xF05F5000)
********************************************************************************/
//#define HwUSBHOSTCFG_BASE                       *(volatile unsigned long*)0xF05F5000
typedef struct _USBHOST11CFG{
    volatile unsigned int   NOTDEFINE0;
    volatile unsigned int   USB11H;                             // 0x4 R/W 0x00000010 USB1.1 Host Configuration register
}USBHOST11CFG, *PUSBHOST11CFG;


/*******************************************************************************
*	 15-1. USB2.0 OTG Controller Define   (Base Addr = 0xF0550000)
********************************************************************************/
//#define HwUSB20OTG_BASE                         *(volatile unsigned long*)0xF0550000
typedef struct _USB20OTG{
	// Core Global CSR Map
	volatile unsigned int	GOTGCTL;				// 0x000  R/W	OTG Control and Status Register
	volatile unsigned int	GOTGINT;				// 0x004     	OTG Interrupt Register
	volatile unsigned int	GAHBCFG;				// 0x008     	Core AHB Configuration Register 
	volatile unsigned int	GUSBCFG;				// 0x00C     	Core USB Configuration register
	volatile unsigned int	GRSTCTL;				// 0x010     	Core Reset Register 
	volatile unsigned int	GINTSTS;				// 0x014     	Core Interrupt Register 
	volatile unsigned int	GINTMSK;				// 0x018     	Core Interrupt Mask Register 
	volatile unsigned int	GRXSTSR;				// 0x01C     	Receive Status Debug Read register(Read Only)
	volatile unsigned int	GRXSTSP;				// 0x020     	Receive Status Read /Pop register(Read Only)
	volatile unsigned int	GRXFSIZ;				// 0x024     	Receive FIFO Size Register 
	volatile unsigned int	GNPTXFSIZ;				// 0x028     	Non-periodic Transmit FIFO Size register
	volatile unsigned int	GNPTXSTS;				// 0x02C     	Non-periodic Transmit FIFO/Queue Status register (Read Only)
	volatile unsigned int	NOTDEFINE0[3];			// 0x030~    	Reserved
	volatile unsigned int	GUID;					// 0x03C     	User ID Register 
	volatile unsigned int	NOTDEFINE1; 			// 0x040     	Reserved
	volatile unsigned int	GHWCFG1;				// 0x044     	User HW Config1 Register(Read Only) 
	volatile unsigned int	GHWCFG2;				// 0x048     	User HW Config2 Register(Read Only) 
	volatile unsigned int	GHWCFG3;				// 0x04C     	User HW Config3 Register(Read Only) 
	volatile unsigned int	GHWCFG4;				// 0x050     	User HW Config4 Register(Read Only) 
	volatile unsigned int	NOTDEFINE2[43];			// 0x054~    	Reserved
	volatile unsigned int	HPTXFSIZ;				// 0x100     	Host Periodic Transmit FIFO Size Register
	volatile unsigned int	DIEPTXFn[15];			// 0x104~    	Device IN Endpoint Transmit FIFO Size register
	volatile unsigned int	NOTDEFINE3[176];		// 0x140~    	Reserved
	// Host Mode CSR Map
	volatile unsigned int	HCFG;					// 0x400     	Host Configuration Register 
	volatile unsigned int	HFIR;					// 0x404     	Host Frame Interval Register 
	volatile unsigned int	HFNUM;					// 0x408     	Host Frame Number/Frame Time Remaining register
	volatile unsigned int	NOTDEFINE4; 			// 0x40C     	Reserved
	volatile unsigned int	HPTXSTS;				// 0x410     	Host Periodic Transmit FIFO/Queue Status Register
	volatile unsigned int	HAINT;					// 0x414     	Host All Channels Interrupt Register
	volatile unsigned int	HAINTMSK;				// 0x418     	Host All Channels Interrupt Mask register
	volatile unsigned int	NOTDEFINE5[9];			// 0x41C~    	Not Used
	volatile unsigned int	HPRT;					// 0x440     	Host Port Control and Status register
	volatile unsigned int	NOTDEFINE6[47]; 		// 0x444~    	Reserved
	volatile unsigned int	HCCHARn;				// 0x500     	Host Channel 0 Characteristics Register
	volatile unsigned int	HCSPLTn;				// 0x504     	Host Channel 0 Split Control Register 
	volatile unsigned int	HCINTn; 				// 0x508     	Host Channel 0 Interrupt Register 
	volatile unsigned int	HCINTMSKn;				// 0x50C     	Host Channel 0 Interrupt Mask Register
	volatile unsigned int	HCTSIZn;				// 0x510     	Host Channel 0 Transfer Size Register 
	volatile unsigned int	HCDMAn; 				// 0x514     	Host Channel 0 DMA Address Register 
	volatile unsigned int	NOTDEFINE7[2];			// 0x518~    	Reserved
	volatile unsigned int	HCH1[8];				// 0x520~    	Host Channel 1 Registers
	volatile unsigned int	HCH2[8];				// 0x540~    	Host Channel 2 Registers
	volatile unsigned int	HCH3[8];				// 0x560~    	Host Channel 3 Registers
	volatile unsigned int	HCH4[8];				// 0x580~    	Host Channel 4 Registers
	volatile unsigned int	HCH5[8];				// 0x5A0~    	Host Channel 5 Registers
	volatile unsigned int	HCH6[8];				// 0x5C0~    	Host Channel 6 Registers
	volatile unsigned int	HCH7[8];				// 0x5E0~    	Host Channel 7 Registers
	volatile unsigned int	HCH8[8];				// 0x600~    	Host Channel 8 Registers
	volatile unsigned int	HCH9[8];				// 0x620~    	Host Channel 9 Registers
	volatile unsigned int	HCH10[8];				// 0x640~    	Host Channel 10 Registers
	volatile unsigned int	HCH11[8];				// 0x660~    	Host Channel 11 Registers
	volatile unsigned int	HCH12[8];				// 0x680~    	Host Channel 12 Registers
	volatile unsigned int	HCH13[8];				// 0x6A0~    	Host Channel 13 Registers
	volatile unsigned int	HCH14[8];				// 0x6C0~    	Host Channel 14 Registers
	volatile unsigned int	HCH15[8];				// 0x6E0~    	Host Channel 15 Registers
	volatile unsigned int	NOTDEFINE8[64]; 		// 0x700~    	Reserved
	// Device Mode CSR Map
	volatile unsigned int	DCFG;					// 0x800     	Device Configuration Register 
	volatile unsigned int	DCTL;					// 0x804     	Device Control Register
	volatile unsigned int	DSTS;					// 0x808     	Device Status Register (Read Only)
	volatile unsigned int	NOTDEFINE9; 			// 0x80C     	Reserved 
	volatile unsigned int	DIEPMSK;				// 0x810     	Device IN Endpoint Common Interrupt Mask Register
	volatile unsigned int	DOEPMSK;				// 0x814     	Device OUT Endpoint Common Interrupt Mask register
	volatile unsigned int	DAINT;					// 0x818     	Device All Endpoints Interrupt Register 
	volatile unsigned int	DAINTMSK;				// 0x81C     	Device All Endpoints Interrupt Mask Register
	volatile unsigned int	NOTDEFINE10[2];			// 0x820~    	Reserved
	volatile unsigned int	DVBUSDIS;				// 0x828    	Device VBUS Discharge Time Register
	volatile unsigned int	DVBUSPULSE; 			// 0x82C     	Device VBUS Pulsing Time register
	volatile unsigned int	DTHRCTL;				// 0x830     	Device Threshold Control register
	volatile unsigned int	DIEPEMPMSK; 			// 0x834     	Device IN Endpoint FIFO Empty Interrupt Mask register
	volatile unsigned int	NOTDEFINE11[50];		// 0x838~    	Reserved
	volatile unsigned int	DIEPCTL0;				// 0x900     	Device Control IN Endpoint 0 Control Register
	volatile unsigned int	NOTDEFINE12;			// 0x904     	Reserved 
	volatile unsigned int	DIEPINT0;				// 0x908     	Device IN Endpoint 0 Interrupt Register 
	volatile unsigned int	NOTDEFINE13;			// 0x90C     	Reserved
	volatile unsigned int	DIEPTSIZ0;				// 0x910     	Device IN Endpoint 0 Transfer Size register
	volatile unsigned int	DIEPDMA0;				// 0x914     	Device IN Endpoint 0 DMA Address Register
	volatile unsigned int	DTXFSTS0;				// 0x918     	Device IN Endpoint Transmit FIFO Status Register
	volatile unsigned int	NOTDEFINE14;			// 0x91C     	Reserved 
	volatile unsigned int	DEVINENDPT[15][8];		// 0x920~    	Device IN Endpoint 1~15 Registers
	volatile unsigned int	DOEPCTL0;				// 0xB00     	Device Control OUT Endpoint 0 Control register 
	volatile unsigned int	NOTDEFINE15;			// 0xB04     	Reserved 
	volatile unsigned int	DOEPINT0;				// 0xB08     	Device OUT Endpoint 0 Interrupt Register
	volatile unsigned int	NOTDEFINE16;			// 0xB0C     	Reserved 
	volatile unsigned int	DOEPTSIZ0;				// 0xB10     	Device OUT Endpoint 0 Transfer Size Register
	volatile unsigned int	DOEPDMA0;				// 0xB14     	Device OUT Endpoint 0 DMA Address register
	volatile unsigned int	NOTDEFINE17[2]; 		// 0xB18~    	Reserved
	volatile unsigned int	DEVOUTENDPT[15][8];		// 0xB20~    	Device OUT Endpoint 1~15 Registers
	volatile unsigned int	NOTDEFINE18[64];		// 0xD00~    	Reserved
	// Power and Clock Gating CSR Map
	volatile unsigned int	PCGCR;					// 0xE00     	Power and Clock Gating Control Register  
	volatile unsigned int	NOTDEFINE19[127];		// 0xE04~    	Reserved
	// Data FIFO(DFIFO) Access Register Map
	volatile unsigned int	DFIFOENDPT[16][1024];	// 0x1000~   	Device IN Endpoint 0~16/Host Out Channel 0~16: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT0[1024];		// 0x1000~   	Device IN Endpoint 0/Host Out Channel 0: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT1[1024];		// 0x2000~   	Device IN Endpoint 1/Host Out Channel 1: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT2[1024];		// 0x3000~   	Device IN Endpoint 2/Host Out Channel 2: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT3[1024];		// 0x4000~   	Device IN Endpoint 3/Host Out Channel 3: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT4[1024];		// 0x5000~   	Device IN Endpoint 4/Host Out Channel 4: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT5[1024];		// 0x6000~   	Device IN Endpoint 5/Host Out Channel 5: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT6[1024];		// 0x7000~   	Device IN Endpoint 6/Host Out Channel 6: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT7[1024];		// 0x8000~   	Device IN Endpoint 7/Host Out Channel 7: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT8[1024];		// 0x9000~   	Device IN Endpoint 8/Host Out Channel 8: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT9[1024];		// 0xA000~   	Device IN Endpoint 9/Host Out Channel 9: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT10[1024];		// 0xB000~   	Device IN Endpoint 10/Host Out Channel 10: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT11[1024];		// 0xC000~   	Device IN Endpoint 11/Host Out Channel 11: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT12[1024];		// 0xD000~   	Device IN Endpoint 12/Host Out Channel 12: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT13[1024];		// 0xE000~   	Device IN Endpoint 13/Host Out Channel 13: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT14[1024];		// 0xF000~   	Device IN Endpoint 14/Host Out Channel 14: DFIFO Write/Read Access
	//volatile unsigned int	DFIFOENDPT15[1024];		// 0x10000~   	Device IN Endpoint 15/Host Out Channel 15: DFIFO Write/Read Access
}USB20OTG, *PUSB20OTG;

/*******************************************************************************
*	15-2. USB OTG Configuration Register Define   (Base Addr = 0xF05F5000)
********************************************************************************/
//#define HwUSBOTGCFG_BASE                        *(volatile unsigned long*)0xF05F5000
typedef struct _USBOTGCFG{
	volatile unsigned int	OTGCR;					// 0x000  R/W	USBOTG Configuration Register
	volatile unsigned int	OTGID;					// 0x004  R/W	USBOTG ID Register
	volatile unsigned int	NOTUSED[8];     // 0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C, 0x20, 0x24		
	volatile unsigned int	UPCR0;					// 0x028  R/W	USB PHY Configuration Register0
	volatile unsigned int	UPCR1;					// 0x02C  R/W	USB PHY Configuration Register1
	volatile unsigned int	UPCR2;					// 0x030  R/W	USB PHY Configuration Register2
	volatile unsigned int	UPCR3;					// 0x034  R/W	USB PHY Configuration Register3			
}USBOTGCFG, *PUSBOTGCFG;

/*******************************************************************************
*	 15-3. USB PHY Configuration Register Define   (Base Addr = 0xF05F5028)
********************************************************************************/
//#define HwUSBPHYCFG_BASE                        *(volatile unsigned long*)0xF05F5028
typedef struct _USBPHYCFG
{
	volatile unsigned int	UPCR0;					// 0x028  R/W	USB PHY Configuration Register0
	volatile unsigned int	UPCR1;					// 0x02C  R/W	USB PHY Configuration Register1
	volatile unsigned int	UPCR2;					// 0x030  R/W	USB PHY Configuration Register2
	volatile unsigned int	UPCR3;					// 0x034  R/W	USB PHY Configuration Register3
}USBPHYCFG, *PUSBPHYCFG;


/*******************************************************************************
*	 16. External Host Interface Register Define   (Base Addr = 0xF0570000/0xF0580000)
********************************************************************************/
//#define HwEHICS0_BASE                           *(volatile unsigned long*)0xF0570000
//#define HwEHICS1_BASE                           *(volatile unsigned long*)0xF0580000

typedef struct _EHI{
    volatile unsigned int   EHST;               //0x00 R/W R/W 0x00000080 Status register
    volatile unsigned int   EHIINT;             //0x04 R/W R/W 0x00000000 Internal interrupt control register
    volatile unsigned int   EHEINT;             //0x08 R/W R/W 0x00000000 External interrupt control register
    volatile unsigned int   EHA;                //0x0C R R/W 0x00000000 Address register
    volatile unsigned int   EHAM;               //0x10 R/W R 0x00000000 Address masking register
    volatile unsigned int   EHD;                //0x14 R/W R/W 0x00000000 Data register
    volatile unsigned int   EHSEM;              //0x18 R/W R/W 0x00000000 Semaphore register
    volatile unsigned int   EHCFG;              //0x1C R/W R/W 0x00000000 Configuration registers
    volatile unsigned int   EHIND;              //0x20 R W 0x00000000 Index register
    volatile unsigned int   EHRWCS;             //0x24 R R/W 0x00000000 Read/Write Control/Status register    
}EHI, *PEHI;

/*******************************************************************************
*	 17. General Purpose Serial Bus (GPSB) Register Define   (Base Addr = 0xF0057000)
********************************************************************************/
/*
#define HwGPSBCH0_BASE                          *(volatile unsigned long*)0xF0057000
#define HwGPSBCH1_BASE                          *(volatile unsigned long*)0xF0057100
#define HwGPSBCH2_BASE                          *(volatile unsigned long*)0xF0057200
#define HwGPSBCH3_BASE                          *(volatile unsigned long*)0xF0057300
#define HwGPSBCH4_BASE                          *(volatile unsigned long*)0xF0057400
#define HwGPSBCH5_BASE                          *(volatile unsigned long*)0xF0057500
#define HwGPSBPORTCFG_BASE                      *(volatile unsigned long*)0xF0057800
#define HwGPSBPIDTABLE_BASE                     *(volatile unsigned long*)0xF0057F00
*/

typedef struct _GPSB{
    volatile unsigned int   PORT;               // 0x000 R/W 0x0000 Data port
    volatile unsigned int   STAT;               // 0x004 R/W 0x0000 Status register
    volatile unsigned int   INTEN;              // 0x008 R/W 0x0000 Interrupt enable
    volatile unsigned int   MODE;               // 0x00C R/W 0x0004 Mode register
    volatile unsigned int   CTRL;               // 0x010 R/W 0x0000 Control register
    volatile unsigned int   EVTCTRL;            // 0x014 R/W 0x0000 Counter & Ext. Event Control
    volatile unsigned int   CCV;                // 0x018 R 0x0000 Counter Current Value
    volatile unsigned int   NOTDEFINE0;
    volatile unsigned int   TXBASE;             // 0x020 R/W 0x0000 TX base address register
    volatile unsigned int   RXBASE;             // 0x024 R/W 0x0000 RX base address register
    volatile unsigned int   PACKET;             // 0x028 R/W 0x0000 Packet register
    volatile unsigned int   DMACTR;             // 0x02C R/W 0x0000 DMA control register
    volatile unsigned int   DMASTR;             // 0x030 R/W 0x0000 DMA status register
    volatile unsigned int   DMAICR;             // 0x034 R/W 0x0000 DMA interrupt control register
}GPSB, *PGPSB;


typedef struct _GPSBPORTCFG{
    volatile unsigned int   PCFG0;              // 0x800 R/W 0x03020100 Port Configuration Register 0
    volatile unsigned int   PCFG1;              // 0x804 R/W 0x00000504 Port Configuration Port Config Register 1
    volatile unsigned int   CIRQST;             // 0x808 R 0x0000 Channel IRQ Status Register   
}GPSBPORTCFG, *PGPSBPORTCFG;


typedef struct _GPSBPIDTABLE{
    volatile unsigned int   PIDT; // 0xF00 R/W   PID Table 
}GPSBPIDTABLE, *PGPSBPIDTABLE;


/*******************************************************************************
*	 18. The Transport Stream Interface (TSIF) Register Define   (Base Addr = 0xF0538000)
********************************************************************************/
//#define HwTSIF_BASE                             *(volatile unsigned long*)0xF053B000
//#define HwTSIFPORTSEL_BASE                      *(volatile unsigned long*)0xF053B800

typedef struct _TSIF{
    volatile unsigned int  TSDI; //0x00 R 0x0000 TSIF Input Data Register
    volatile unsigned int  TSCR; //0x04 R/W 0x0000 TSIF Control Register
    volatile unsigned int  TSPID; //0x08 R/W 0x0000 TSIF PID Register
    volatile unsigned int  TSCTRL; //0x10 R/W 0x0000 TSIF Interrupt Control Register
    volatile unsigned int  TSSTS; //0x10(14) R 0x0000 TSIF Interrupt Status Register(Test)
}TSIF, *PTSIF;

typedef struct _TSIFPORTSEL{
    volatile unsigned int  TSCHS; //0x800 R/W 0x0000 TSIF Channel(Port) Select Register    
}TSIFPORTSEL, *PTSIFPORTSEL;

/*******************************************************************************
*	 19. GPS Interface Register Define   (Base Addr = )
********************************************************************************/


/*******************************************************************************
*	 20. Remote Control Interface Register Define   (Base Addr = 0xF05F3000)
********************************************************************************/
//#define HwREMOCON_BASE                          *(volatile unsigned long*)0xF05F3000

typedef struct _REMOTECON{
    volatile unsigned int  TXADDR;              // 0x00 R/W 0x0000 IR Data Transfer Address
    volatile unsigned int  CMD;                 // 0x04 R/W 0x0000 Command Register
    volatile unsigned int  CTRL;                // 0x08 R/W 0x0000 Control Register
    volatile unsigned int  STA;                 // 0x0C W 0x0000 Status register
    volatile unsigned int  NOTDEFINE0[13];
    volatile unsigned int  CLKDIV;              // 0x40 R 0x0000 Clock Divide Register   
}REMOTECON, *PREMOTECON;


/*******************************************************************************
*	 21. I2C Controller Register Define   (Base Addr = 0xF0530000)
********************************************************************************/
/*
#define HwI2CMASTER0_BASE                          *(volatile unsigned long*)0xF0530000
#define HwI2CMASTER1_BASE                          *(volatile unsigned long*)0xF0530040
#define HwI2CSLAVE_BASE                            *(volatile unsigned long*)0xF0530080
#define HwI2CSTATUS_BASE                           *(volatile unsigned long*)0xF05300C0
*/

typedef struct _I2CMASTER{
    volatile unsigned int  PRES;                // 0x00 R/W 0xFFFF Clock Prescale register
    volatile unsigned int  CTRL;                // 0x04 R/W 0x0000 Control Register
    volatile unsigned int  TXR;                 // 0x08 W 0x0000 Transmit Register
    volatile unsigned int  CMD;                 // 0x0C W 0x0000 Command Register
    volatile unsigned int  RXR;                 // 0x10 R 0x0000 Receive Register
    volatile unsigned int  SR;                  // 0x14 R 0x0000 Status register
    volatile unsigned int  TIME;                // 0x18 R/W 0x0000 Timing Control Register    
}I2CMASTER, *PI2CMASTER;

typedef struct _I2C{
	volatile unsigned int  PRES0;                // 0x00 R/W 0xFFFF Clock Prescale register
	volatile unsigned int  CTRL0;                // 0x04 R/W 0x0000 Control Register
	volatile unsigned int  TXR0;                 // 0x08 W 0x0000 Transmit Register
	volatile unsigned int  CMD0;                 // 0x0C W 0x0000 Command Register
	volatile unsigned int  RXR0;                 // 0x10 R 0x0000 Receive Register
	volatile unsigned int  SR0;                  // 0x14 R 0x0000 Status register
	volatile unsigned int  TIME0;                // 0x18 R/W 0x0000 Timing Control Register    
	volatile unsigned int  NOTUSING[9];			 // 1c, 20, 24,28,2c,30,34,38,3c,
	volatile unsigned int  PRES1;                // 0x40 R/W 0xFFFF Clock Prescale register
	volatile unsigned int  CTRL1;                // 0x44 R/W 0x0000 Control Register
	volatile unsigned int  TXR1;                 // 0x48 W 0x0000 Transmit Register
	volatile unsigned int  CMD1;                 // 0x4C W 0x0000 Command Register
	volatile unsigned int  RXR1;                 // 0x50 R 0x0000 Receive Register
	volatile unsigned int  SR1;                  // 0x54 R 0x0000 Status register
	volatile unsigned int  TIME1;                // 0x58 R/W 0x0000 Timing Control Register    
	volatile unsigned int  NOTUSING1[26];	
	volatile unsigned int  IRQSTR;              // 0xC0 R 0x00000000 IRQ Status Register    
}I2C, *PI2C;

typedef struct _I2CSLAVE{
    volatile unsigned int  PORT;                // 0x80 R/W - Data Access port (TX/RX FIFO)
    volatile unsigned int  CTL;                 // 0x84 R/W 0x00000000 Control register
    volatile unsigned int  ADDR;                // 0x88 W 0x00000000 Address register
    volatile unsigned int  INT;                 // 0x8C W 0x00000000 Interrupt Enable Register
    volatile unsigned int  STAT;                // 0x90 R 0x00000000 Status Register
    volatile unsigned int  NOTDEFINE0[2];
    volatile unsigned int  MBF;                 // 0x9C R/W 0x00000000 Buffer Valid Flag
    volatile unsigned int  MB0;                 // 0xA0 R/W 0x00000000 Data Buffer 0 (Byte 3 ~ 0)
    volatile unsigned int  MB1;                 // 0xA4 R/W 0x00000000 Data Buffer 1 (Byte 7 ~ 4)    
}I2CSLAVE, *PI2CSLAVE;

typedef struct _I2CSTATUS{
    volatile unsigned int  IRQSTR;              // 0xC0 R 0x00000000 IRQ Status Register    
}I2CSTATUS, *PI2CSTATUS;


/*******************************************************************************
*	 22. UART Controller Register Define   (Base Addr = 0xF0538000)
********************************************************************************/
/*
#define HwUARTCH0_BASE                              *(volatile unsigned long*)0xF0532000
#define HwUARTCH1_BASE                              *(volatile unsigned long*)0xF0532100
#define HwUARTCH2_BASE                              *(volatile unsigned long*)0xF0532200
#define HwUARTCH3_BASE                              *(volatile unsigned long*)0xF0532300
#define HwUARTCH4_BASE                              *(volatile unsigned long*)0xF0532400
#define HwUARTCH5_BASE                              *(volatile unsigned long*)0xF0532500
#define HwUARTPORTMUX_BASE                          *(volatile unsigned long*)0xF0532600
*/
typedef union _UARTREG1{
    volatile unsigned int  RBR;                 // 0x00 R Unknown Receiver Buffer Register(DLAB = 0)
    volatile unsigned int  THR;                 // 0x00 W 0x00 Transmitter Holding Register (DLAB=0)
    volatile unsigned int  DLL;                 // 0x00 R/W 0x00 Divisor Latch (LSB) (DLAB=1)
}UARTREG1;

typedef union _UARTREG2{
    volatile unsigned int  IER;                 // 0x04 R/W 0x00 Interrupt Enable Register (DLAB=0)
    volatile unsigned int  DLM;                 // 0x04 R/W 0x00 Divisor Latch (MSB) (DLAB=1)
}UARTREG2;

typedef union _UARTREG3{
    volatile unsigned int  IIR;                 // 0x08 R Unknown Interrupt Ident. Register (DLAB=0)
    volatile unsigned int  FCR;                 // 0x08 W 0xC0 FIFO Control Register (DLAB=1)
}UARTREG3;

typedef struct _UART{
/*
    volatile unsigned int  RBR;                 // 0x00 R Unknown Receiver Buffer Register(DLAB = 0)
    volatile unsigned int  THR;                 // 0x00 W 0x00 Transmitter Holding Register (DLAB=0)
    volatile unsigned int  DLL;                 // 0x00 R/W 0x00 Divisor Latch (LSB) (DLAB=1)
    volatile unsigned int  IER;                 // 0x04 R/W 0x00 Interrupt Enable Register (DLAB=0)
    volatile unsigned int  DLM;                 // 0x04 R/W 0x00 Divisor Latch (MSB) (DLAB=1)
    volatile unsigned int  IIR;                 // 0x08 R Unknown Interrupt Ident. Register (DLAB=0)
    volatile unsigned int  FCR;                 // 0x08 W 0xC0 FIFO Control Register (DLAB=1)
*/
    UARTREG1    REG1;
    UARTREG2    REG2;
    UARTREG3    REG3;
    volatile unsigned int  LCR;                     // 0x0C R/W 0x03 Line Control Register
    volatile unsigned int  MCR;                     // 0x10 R/W 0x00 MODEM Control Register
    volatile unsigned int  LSR;                     // 0x14 R Unknown Line Status Register
    volatile unsigned int  MSR;                     // 0x18 R Unknown MODEM Status Register
    volatile unsigned int  SCR;                     // 0x1C R/W 0x00 Scratch Register
    volatile unsigned int  AFT;                     // 0x20 R/W 0x00 AFC Trigger Level Register
    volatile unsigned int  UCR;                     // 0x24 R/W 0x00 UART Control register
    volatile unsigned int  NOTDEFINE0[6];
    volatile unsigned int  SRBR;                    // 0x40 R Unknown Rx Buffer Register
    volatile unsigned int  STHR;                    // 0x44 W 0x00 Transmitter Holding Register
    volatile unsigned int  SDLL;                    // 0x48 R/W 0x00 Divisor Latch (LSB)
    volatile unsigned int  SDLM;                    // 0x4C R/W 0x00 Divisor Latch (MSB)
    volatile unsigned int  SIER;                    // 0x50 R/W 0x00 Interrupt Enable register
    volatile unsigned int  NOTDEFINE1[3];
    volatile unsigned int  SCCR;                    // 0x60 R/W 0x00 Smart Card Control Register
    volatile unsigned int  STC;                     // 0x64 R/W 0x00 Smart Card TX Count register
    volatile unsigned int  NOTDEFINE2[6];
    volatile unsigned int  IRCFG;                   // 0x80 R/W 0x00 IRDA Configuration Register
}UART, *PUART;


typedef struct _UARTPORTMUX{
    volatile unsigned int  CHSEL;                   // 0x00 R/W 0x3210 Channel Selection Register
    volatile unsigned int  CHST;                    // 0x00 R 0x0000 Channel Status Register    
}UARTPORTMUX, *PUARTPORTMUX;


/*******************************************************************************
*	 23. CAN Controller Register Define   (Base Addr = 0xF0531000)
********************************************************************************/
//#define HwCAN_BASE                                  0xF0531000

typedef struct _CANCTRL{
    volatile unsigned int  CANCTRLREG;              //  CAN Control Register 0x00 0x0001
    volatile unsigned int  CANSTATUS;               //   Status Register 0x04 0x0000
    volatile unsigned int  CANERRORCNT;             // Error Counter 0x08 0x0000 Read only 
    volatile unsigned int  CANBITTIMING;            //    Bit Timing Register 0x0C 0x2301 Write enabled by CCE
    volatile unsigned int  CANINTREG;               //   Interrupt Register 0x10 0x0000 Read only
    volatile unsigned int  CANTESTREG;              //  Test Register 0x14 0x00 & 0br0000000 1) Write enabled by Test
    volatile unsigned int  CANBRPEXTREG;            //     Extenstion Register 0x18 0x0000 Write enabled by CCE
    volatile unsigned int  NOTDEFINE0;              //  __reserved 0x1C 3)
    volatile unsigned int  CANIF1CMDREQ;            // Command Request 0x20 0x0001
    volatile unsigned int  CANIF1CMDMSK;            // Command Mask 0x24 0x0000
    volatile unsigned int  CANIF1MSK1;              // IF1 Mask1 0x28 0xFFFF
    volatile unsigned int  CANIF1MSK2;              // IF1 Mask2 0x2C 0xFFFF
    volatile unsigned int  CANIF1ARBIT1;            // IF1 Arbitration 1 0x30 0x0000
    volatile unsigned int  CANIF1ARBIT2;            // IF1 Arbitration 2 0x34 0x0000
    volatile unsigned int  CANIF1MSGCTL;            // IF1 Message Control 0x38 0x0000
    volatile unsigned int  CANIF1DTA1;              // IF1 Data A 1 0x3C 0x0000
    volatile unsigned int  CANIF1DTA2;              // IF1 Data A 2 0x40 0x0000
    volatile unsigned int  CANIF1DTB1;              // IF1 Data B 1 0x44 0x0000
    volatile unsigned int  CANIF1DTB2;              // IF1 Data B 2 0x48 0x0000
    volatile unsigned int  NOTDEFINE1[13];          // __reserved 0x50-0x7C 3)
    volatile unsigned int  CANIF2CMDREQ;            // Command Request 0x80 0x0001
    volatile unsigned int  CANIF2CMDMSK;            // Command Mask 0x84 0x0000
    volatile unsigned int  CANIF2MSK1;              // IF1 Mask1 0x88 0xFFFF
    volatile unsigned int  CANIF2MSK2;              // IF1 Mask2 0x8C 0xFFFF
    volatile unsigned int  CANIF2ARBIT1;            // IF1 Arbitration 1 0x90 0x0000
    volatile unsigned int  CANIF2ARBIT2;            // IF1 Arbitration 2 0x94 0x0000
    volatile unsigned int  CANIF2MSGCTL;            // IF1 Message Control 0x98 0x0000
    volatile unsigned int  CANIF2DTA1;              // IF1 Data A 1 0x9C 0x0000
    volatile unsigned int  CANIF2DTA2;              // IF1 Data A 2 0xA0 0x0000
    volatile unsigned int  CANIF2DTB1;              // IF1 Data B 1 0xA4 0x0000
    volatile unsigned int  CANIF2DTB2;              // IF1 Data B 2 0xA8 0x0000
    volatile unsigned int  NOTDEFINE2[20];          // __reserved 0xAC-0xFC 3)
    volatile unsigned int  CANTRSREQ1;              // Transmission Requrest 1 0x100 0x0000 Read only
    volatile unsigned int  CANTRSREQ2;              // Transmission Request 2 0x104 0x0000 Read only
    volatile unsigned int  NOTDEFINE3[5];           // __reserved 0x108-0x11C 3)
    volatile unsigned int  CANNEWDT1;               // New Data 1 0x120 0x0000 Read only
    volatile unsigned int  CANNEWDT2;               // New Data 2 0x124 0x0000 Read only
    volatile unsigned int  NOTDEFINE4[5];           // __reserved 0x128-0x13C 3)
    volatile unsigned int  CANINTPEND1;             // Interrupt Pending 1 0x140 0x0000 Read only
    volatile unsigned int  CANINTPEND2;             // Interrupt Pending 2 0x144 0x0000 Read only
    volatile unsigned int  NOTDEFINE5[5];           // __reserved 0x148-0x15C 3)
    volatile unsigned int  CANMSGVALID1;            // Message Valid 1 0x160 0x0000 Read only
    volatile unsigned int  CANMSGVALID3;            // Message Valid 2 0x164 0x0000 Read only
    volatile unsigned int  NOTDEFINE6[5];           // __reserved 0x168-0x17C        
    
}CANCTRL, *PCANCTRL;




/*******************************************************************************
*	 24. DMA Controller Register Define   (Base Addr = 0xF0540000)
********************************************************************************/
/*
#define HwGDMA0_BASE               *(volatile unsigned long *)0xF0540000
#define HwGDMA1_BASE               *(volatile unsigned long *)0xF0540100
#define HwGDMA2_BASE               *(volatile unsigned long *)0xF0540200
#define HwGDMA3_BASE               *(volatile unsigned long *)0xF0540300
*/
typedef struct _GDMACTRL{
    volatile unsigned int  ST_SADR0;                // 0x00 R/W 0x00000000 Start Address of Source Block
    volatile unsigned int  SPARAM0;                 // 0x04 R/W 0x00000000 Parameter of Source Block
    volatile unsigned int  NOTDEFINE0;           	 //  0x08      
    volatile unsigned int  C_SADR0;                 // 0x0C R 0x00000000 Current Address of Source Block
    volatile unsigned int  ST_DADR0;                // 0x10 R/W 0x00000000 Start Address of Destination Block
    volatile unsigned int  DPARAM0;              	// 0x14 R/W 0x00000000 Parameter of Destination Block
    volatile unsigned int  NOTDEFINE1;           	//  0x18      
    volatile unsigned int  C_DADR0;                 // 0x1C R 0x00000000 Current Address of Destination Block
    volatile unsigned int  HCOUNT0;                 // 0x20 R/W 0x00000000 Initial and Current Hop count
    volatile unsigned int  CHCTRL0;                 // 0x24 R/W 0x00000000 Channel Control Register
    volatile unsigned int  RPTCTRL0;                // 0x28 R/W 0x00000000 Repeat Control Register
    volatile unsigned int  EXTREQ0;                 // 0x2C R/W 0x00000000 External DMA Request Register
    volatile unsigned int  ST_SADR1;                // 0x30 R/W 0x00000000 Start Address of Source Block
    volatile unsigned int  SPARAM1;                 // 0x34 R/W 0x00000000 Parameter of Source Block
    volatile unsigned int  NOTDEFINE2;           	//  0x38      
    volatile unsigned int  C_SADR1;                 // 0x3C R 0x00000000 Current Address of Source Block
    volatile unsigned int  ST_DADR1;                // 0x40 R/W 0x00000000 Start Address of Destination Block
    volatile unsigned int  DPARAM1;              	// 0x44 R/W 0x00000000 Parameter of Destination Block
    volatile unsigned int  NOTDEFINE3;           	//  0x48      
    volatile unsigned int  C_DADR1;                 // 0x4C R 0x00000000 Current Address of Destination Block
    volatile unsigned int  HCOUNT1;                 // 0x50 R/W 0x00000000 Initial and Current Hop count
    volatile unsigned int  CHCTRL1;                 // 0x54 R/W 0x00000000 Channel Control Register
    volatile unsigned int  RPTCTRL1;                // 0x58 R/W 0x00000000 Repeat Control Register
    volatile unsigned int  EXTREQ1;                 // 0x5C R/W 0x00000000 External DMA Request Register
    volatile unsigned int  ST_SADR2;                // 0x60 R/W 0x00000000 Start Address of Source Block
    volatile unsigned int  SPARAM2;                 // 0x64/ R/W 0x00000000 Parameter of Source Block
    volatile unsigned int  NOTDEFINE4;           	//  0x68      
    volatile unsigned int  C_SADR2;                 // 0x6C R 0x00000000 Current Address of Source Block
    volatile unsigned int  ST_DADR2;                // 0x70 R/W 0x00000000 Start Address of Destination Block
    volatile unsigned int  DPARAM2;              	// 0x74/ R/W 0x00000000 Parameter of Destination Block
    volatile unsigned int  NOTDEFINE5;           	//  0x78      
    volatile unsigned int  C_DADR2;                 // 0x7C R 0x00000000 Current Address of Destination Block
    volatile unsigned int  HCOUNT2;                 // 0x80 R/W 0x00000000 Initial and Current Hop count
    volatile unsigned int  CHCTRL2;                 // 0x84 R/W 0x00000000 Channel Control Register
    volatile unsigned int  RPTCTRL2;                // 0x88 R/W 0x00000000 Repeat Control Register
    volatile unsigned int  EXTREQ2;                 // 0x8C R/W 0x00000000 External DMA Request Register
    volatile unsigned int  CHCONFIG;                // 0x90 R/W 0x00000000 Channel Configuration Register    
}GDMACTRL, *PGDMACTRL;


typedef struct _GDMANCTRL{
	volatile unsigned int	ST_SADR;				// 0x000  R/W	Start Address of Source Block
	volatile unsigned int	SPARAM[2]; 				// 0x004~ R/W	Parameter of Source Block
	volatile unsigned int	C_SADR; 				// 0x00C  R  	Current Address of Source Block
	volatile unsigned int	ST_DADR;				// 0x010  R/W	Start Address of Destination Block
	volatile unsigned int	DPARAM[2];				// 0x014~ R/W	Parameter of Destination Block
	volatile unsigned int	C_DADR; 				// 0x01C  R  	Current Address of Destination Block
	volatile unsigned int	HCOUNT; 				// 0x020  R/W	Initial and Current Hop count
	volatile unsigned int	CHCTRL; 				// 0x024  R/W	Channel Control Register
	volatile unsigned int	RPTCTRL;				// 0x028  R/W	Repeat Control Register
	volatile unsigned int	EXTREQ; 				// 0x02C  R/W	External DMA Request Register
} GDMANCTRL, *PGDMANCTRL;

/*******************************************************************************
*	 25. Real Time Clock(RTC) Register Define   (Base Addr = 0xF05F2000)
********************************************************************************/
//#define HwRTC_BASE                                *(volatile unsigned long*)0xF05F2000

typedef struct _RTC{
    volatile unsigned int  RTCCON;                  // 0x00 R/W 0x00 RTC Control Register
    volatile unsigned int  INTCON;                  // 0x04 R/W - RTC Interrupt Control Register
    volatile unsigned int  RTCALM;                  // 0x08 R/W - RTC Alarm Control Register
    volatile unsigned int  ALMSEC;                  // 0x0C R/W - Alarm Second Data Register
    volatile unsigned int  ALMMIN;                  // 0x10 R/W - Alarm Minute Data Register
    volatile unsigned int  ALMHOUR;                 // 0x14 R/W - Alarm Hour Data Register
    volatile unsigned int  ALMDATE;                 // 0x18 R/W - Alarm Date Data Register
    volatile unsigned int  ALMDAY;                  // 0x1C R/W - Alarm Day of Week Data Register
    volatile unsigned int  ALMMON;                  // 0x20 R/W - Alarm Month Data Register
    volatile unsigned int  ALMYEAR;                 // 0x24 R/W - Alarm Year Data Register
    volatile unsigned int  BCDSEC;                  // 0x28 R/W - BCD Second Register
    volatile unsigned int  BCDMIN;                  // 0x2C R/W - BCD Minute Register
    volatile unsigned int  BCDHOUR;                 // 0x30 R/W - BCD Hour Register
    volatile unsigned int  BCDDATE;                 // 0x34 R/W - BCD Date Register
    volatile unsigned int  BCDDAY;                  // 0x38 R/W - BCD Day of Week Register
    volatile unsigned int  BCDMON;                  // 0x3C R/W - BCD Month Register
    volatile unsigned int  BCDYEAR;                 // 0x40 R/W - BCD Year Register
    volatile unsigned int  RTCIM;                   // 0x44 R/W - RTC Interrupt Mode Register
    volatile unsigned int  RTCPEND;                 // 0x48 R/W - RTC Interrupt Pending Register
}RTC, *PRTC;


/*******************************************************************************
*	 26. TouchScreen ADC (TSADC) Register Define   (Base Addr = 0xF05F4000)
********************************************************************************/
//#define HwTSADC_BASE                                *(volatile unsigned long*)0xF05F4000
typedef struct _TSADC{
    volatile unsigned int  ADCCON;                  // 0x00 R/W 0x00003FC4 ADC Control Register
    volatile unsigned int  ADCTSC;                  // 0x04 R/W 0x00000058 ADC Touch Screen Control Register
    volatile unsigned int  ADCDLY;                  // 0x08 R/W 0x000000FF ADC Start or Interval Delay Register
    volatile unsigned int  ADCDAT0;                 // 0x0C R - ADC Conversion Data Register
    volatile unsigned int  ADCDAT1;                 // 0x10 R - ADC Conversion Data Register
    volatile unsigned int  ADCUPDN;                 // 0x14 R/W 0x00000000 Stylus Up or Down Interrupt Register
    volatile unsigned int  ADCCLRINT;               // 0x18 W - Clear ADC Interrrupt
    volatile unsigned int  NOTDEFINE0;              // 0x1C - - Reserved
    volatile unsigned int  ADCCLRUPDN;              // 0x20 W - Clear Pen UP/DOWN Interrupt    
}TSADC, *PTSADC;


/*******************************************************************************
*	 27. Error Correction Code Register Define   (Base Addr = 0xF0539000)
********************************************************************************/
//#define HwECC_BASE                                  *(volatile unsigned long*)0xF0539000
//#define HwECCERRADDR_BASE                           *(volatile unsigned long*)0xF0539050

//=================================================================================
// ECC Code Register
//=================================================================================
typedef struct _ECC{
	volatile unsigned int	ECC_CTRL;				// 0x000  R/W	ECC Control Register
	volatile unsigned int	ECC_BASE;				// 0x004  R/W	Base Address for ECC Calculation
	volatile unsigned int	ECC_MASK;				// 0x008  R/W	Address mask for ECC area.
	volatile unsigned int	ECC_CLEAR;				// 0x00C  R/W	ECC Clear		
	volatile unsigned int	ECC_CODE0;				// 0x010  R/W	1st  ECC Code Register
	volatile unsigned int	ECC_CODE1;				// 0x014  R/W	2nd  ECC Code Register
	volatile unsigned int	ECC_CODE2;				// 0x018  R/W	3rd  ECC Code Register
	volatile unsigned int	ECC_CODE3;				// 0x01C  R/W	4th  ECC Code Register
	volatile unsigned int	ECC_CODE4;				// 0x020  R/W	5th  ECC Code Register
	volatile unsigned int	ECC_CODE5;				// 0x024  R/W	6th  ECC Code Register
	volatile unsigned int	ECC_CODE6;				// 0x028  R/W	7th  ECC Code Register
	volatile unsigned int	ECC_CODE7;				// 0x02C  R/W	8th  ECC Code Register
	volatile unsigned int	ECC_CODE8;				// 0x030  R/W	9th  ECC Code Register
	volatile unsigned int	ECC_CODE9;				// 0x034  R/W	10th  ECC Code Register
	volatile unsigned int	ECC_CODE10; 			// 0x038  R/W	11th  ECC Code Register
	volatile unsigned int	ECC_CODE11; 			// 0x03C  R/W	12th  ECC Code Register
	volatile unsigned int	ECC_CODE12; 			// 0x040  R/W	13th  ECC Code Register
	volatile unsigned int	ECC_CODE13; 			// 0x044  R/W	14th  ECC Code Register
	volatile unsigned int	ECC_CODE14; 			// 0x048  R/W	15th  ECC Code Register
	volatile unsigned int	ECC_CODE15; 			// 0x04C  R/W	16th  ECC Code register
	volatile unsigned int	ECC_EADDR0; 			// 0x050  R  	ECC Error Address Register0
	volatile unsigned int	ECC_EADDR1; 			// 0x054  R  	ECC Error Address Register1
	volatile unsigned int	ECC_EADDR2; 			// 0x058  R  	ECC Error Address Register2
	volatile unsigned int	ECC_EADDR3; 			// 0x05C  R  	ECC Error Address Register3
	volatile unsigned int	ECC_EADDR4; 			// 0x060  R  	ECC Error Address Register4
	volatile unsigned int	ECC_EADDR5; 			// 0x064  R  	ECC Error Address Register5
	volatile unsigned int	ECC_EADDR6; 			// 0x068  R  	ECC Error Address Register6
	volatile unsigned int	ECC_EADDR7; 			// 0x06C  R  	ECC Error Address Register7
	volatile unsigned int	ECC_EADDR8; 			// 0x070  R  	ECC Error Address Register8
	volatile unsigned int	ECC_EADDR9; 			// 0x074  R  	ECC Error Address Register9
	volatile unsigned int	ECC_EADDR10;			// 0x078  R  	ECC Error Address Register10
	volatile unsigned int	ECC_EADDR11;			// 0x07C  R  	ECC Error Address Register11
	volatile unsigned int	ECC_EADDR12;			// 0x080  R  	ECC Error Address Register12
	volatile unsigned int	ECC_EADDR13;			// 0x084  R  	ECC Error Address Register13
	volatile unsigned int	ECC_EADDR14;			// 0x088  R  	ECC Error Address Register14
	volatile unsigned int	ECC_EADDR15;			// 0x08C  R  	ECC Error Address Register15
	volatile unsigned int	ERRNUM;					// 0x090  R  	ECC Error Number
	volatile unsigned int	ECC_IREQ;				// 0x094  R/W	ECC Interrupt Control Register	
}ECC, *PECC;

typedef struct _SLCECC{
    volatile unsigned int  ECC_CTRL;                // 0x00 R/W 0x00000000 ECC Control Register
    volatile unsigned int  ECC_BASE;                //  0x04 R/W 0x00000000 Base Address for ECC Calculation
    volatile unsigned int  ECC_MASK;                //  0x08 R/W 0x00000000 Address mask for ECC area.
    volatile unsigned int  ECC_CLEAR;               //  0x0C R/W ECC Clear    
    volatile unsigned int  SECC_0;                  //  0x10 R/W 0x00000000 1st SLC ECC Code Register
    volatile unsigned int  SECC_1;                  //  0x14 R/W 0x00000000 2nd SLC ECC Code Register
    volatile unsigned int  SECC_2;                  //  0x18 R/W 0x00000000 3rd SLC ECC Code Register
    volatile unsigned int  SECC_3;                  //  0x1C R/W 0x00000000 4th SLC ECC Code Register
    volatile unsigned int  SECC_4;                  //  0x20 R/W 0x00000000 5th SLC ECC Code Register
    volatile unsigned int  SECC_5;                  //  0x24 R/W 0x00000000 6th SLC ECC Code Register
    volatile unsigned int  SECC_6;                  //  0x28 R/W 0x00000000 7th SLC ECC Code Register
    volatile unsigned int  SECC_7;                  //  0x2C R/W 0x00000000 8th SLC ECC Code Register
    volatile unsigned int  SECC_8;                  //  0x30 R/W 0x00000000 9th SLC ECC Code Register
    volatile unsigned int  SECC_9;                  //  0x34 R/W 0x00000000 10th SLC ECC Code Register
    volatile unsigned int  SECC_10;                 //  0x38 R/W 0x00000000 11th SLC ECC Code Register
    volatile unsigned int  SECC_11;                 //  0x3C R/W 0x00000000 12th SLC ECC Code Register
    volatile unsigned int  SECC_12;                 //  0x40 R/W 0x00000000 13th SLC ECC Code Register
    volatile unsigned int  SECC_13;                 //  0x44 R/W 0x00000000 14th SLC ECC Code Register
    volatile unsigned int  SECC_14;                 //  0x48 R/W 0x00000000 15th SLC ECC Code Register
    volatile unsigned int  SECC_15;                 //  0x4C R/W 0x00000000 16th SLC ECC Code register
    
}SLCECC, *PSLCECC;

typedef struct _MLCECC4{
    volatile unsigned int  ECC_CTRL;                //  0x00 R/W 0x00000000 ECC Control Register
    volatile unsigned int  ECC_BASE;                //  0x04 R/W 0x00000000 Base Address for ECC Calculation
    volatile unsigned int  ECC_MASK;                //  0x08 R/W 0x00000000 Address mask for ECC area.
    volatile unsigned int  ECC_CLEAR;               //  0x0C R/W ECC Clear
    volatile unsigned int  MECC4_0;                 //  0x10 R/W 0x00000000 1st MLC ECC4 Code Register
    volatile unsigned int  MECC4_1;                 //  0x14 R/W 0x00000000 2nd MLC ECC4 Code register
    
}MLCECC4, *PMLCECC4;

typedef struct _MLCECC8{
    volatile unsigned int  ECC_CTRL;                //  0x00 R/W 0x00000000 ECC Control Register
    volatile unsigned int  ECC_BASE;                //  0x04 R/W 0x00000000 Base Address for ECC Calculation
    volatile unsigned int  ECC_MASK;                //  0x08 R/W 0x00000000 Address mask for ECC area.
    volatile unsigned int  ECC_CLEAR;               //  0x0C R/W ECC Clear
    volatile unsigned int  MECC8_0;                 //  0x10 R/W 0x00000000 1st MLC ECC8 Code Register
    volatile unsigned int  MECC8_1;                 //  0x14 R/W 0x00000000 2nd MLC ECC8 Code Register
    volatile unsigned int  MECC8_2;                 //  0x18 R/W 0x00000000 3rd MLC ECC8 Code Register
    volatile unsigned int  MECC8_3;                 //  0x1C R/W 0x00000000 4th MLC ECC8 Code Register   
}MLCECC8, *PMLCECC8;


typedef struct _MLCECC12{
    volatile unsigned int  ECC_CTRL;                //  0x00 R/W 0x00000000 ECC Control Register
    volatile unsigned int  ECC_BASE;                //  0x04 R/W 0x00000000 Base Address for ECC Calculation
    volatile unsigned int  ECC_MASK;                //  0x08 R/W 0x00000000 Address mask for ECC area.
    volatile unsigned int  ECC_CLEAR;               //  0x0C R/W ECC Clear    
    volatile unsigned int  MECC12_0;                //  0x10 R/W 0x00000000 1st MLC ECC12 Code Register
    volatile unsigned int  MECC12_1;                //  0x14 R/W 0x00000000 2nd MLC ECC12 Code Register
    volatile unsigned int  MECC12_2;                //  0x18 R/W 0x00000000 3rd MLC ECC12 Code Register
    volatile unsigned int  MECC12_3;                //  0x1C R/W 0x00000000 4th MLC ECC12 Code Register
    volatile unsigned int  MECC12_4;                //  0x20 R/W 0x00000000 5th MLC ECC12 Code Register
}MLCECC12, *PMLCECC12;


typedef struct _MLCECC14{
    volatile unsigned int  ECC_CTRL;                //  0x00 R/W 0x00000000 ECC Control Register
    volatile unsigned int  ECC_BASE;                //  0x04 R/W 0x00000000 Base Address for ECC Calculation
    volatile unsigned int  ECC_MASK;                //  0x08 R/W 0x00000000 Address mask for ECC area.
    volatile unsigned int  ECC_CLEAR;               //  0x0C R/W ECC Clear    
    volatile unsigned int  MECC14_0;                //  0x10 R/W 0x00000000 1st MLC ECC14 Code Register
    volatile unsigned int  MECC14_1;                //  0x14 R/W 0x00000000 2nd MLC ECC14 Code Register
    volatile unsigned int  MECC14_2;                //  0x18 R/W 0x00000000 3rd MLC ECC14 Code Register
    volatile unsigned int  MECC14_3;                //  0x1C R/W 0x00000000 4th MLC ECC14 Code Register
    volatile unsigned int  MECC14_4;                //  0x20 R/W 0x00000000 5th MLC ECC14 Code Register
    volatile unsigned int  MECC14_5;                //  0x24 R/W 0x00000000 6th MLC ECC14 Code Register
}MLCECC14, *PMLCECC14;


typedef struct _MLCECC16{
    volatile unsigned int  ECC_CTRL;                //  0x00 R/W 0x00000000 ECC Control Register
    volatile unsigned int  ECC_BASE;                //  0x04 R/W 0x00000000 Base Address for ECC Calculation
    volatile unsigned int  ECC_MASK;                //  0x08 R/W 0x00000000 Address mask for ECC area.
    volatile unsigned int  ECC_CLEAR;               //  0x0C R/W ECC Clear    
    volatile unsigned int  MECC16_0;                //  0x10 R/W 0x00000000 1st MLC ECC16 Code Register
    volatile unsigned int  MECC16_1;                //  0x14 R/W 0x00000000 2nd MLC ECC16 Code Register
    volatile unsigned int  MECC16_2;                //  0x18 R/W 0x00000000 3rd MLC ECC16 Code Register
    volatile unsigned int  MECC16_3;                //  0x1C R/W 0x00000000 4th MLC ECC16 Code Register
    volatile unsigned int  MECC16_4;                //  0x20 R/W 0x00000000 5th MLC ECC16 Code Register
    volatile unsigned int  MECC16_5;                //  0x24 R/W 0x00000000 6th MLC ECC16 Code Register
    volatile unsigned int  MECC16_6;                //  0x28 R/W 0x00000000 7th MLC ECC16 Code Register
}MLCECC16, *PMLCECC16;

//=================================================================================
// ECC Error Address Register
//=================================================================================
typedef struct _SLCECCERRADDR{
    volatile unsigned int  SECC_EADDR0;             //  0x50 R 0x00000000 SLC ECC Error Address Register0
    volatile unsigned int  SECC_EADDR1;             //  0x54 R 0x00000000 SLC ECC Error Address Register1
    volatile unsigned int  SECC_EADDR2;             //  0x58 R 0x00000000 SLC ECC Error Address Register2
    volatile unsigned int  SECC_EADDR3;             //  0x5C R 0x00000000 SLC ECC Error Address Register3
    volatile unsigned int  SECC_EADDR4;             //  0x60 R 0x00000000 SLC ECC Error Address Register4
    volatile unsigned int  SECC_EADDR5;             //  0x64 R 0x00000000 SLC ECC Error Address Register5
    volatile unsigned int  SECC_EADDR6;             //  0x68 R 0x00000000 SLC ECC Error Address Register6
    volatile unsigned int  SECC_EADDR7;             //  0x6C R 0x00000000 SLC ECC Error Address Register7
    volatile unsigned int  SECC_EADDR8;             //  0x70 R 0x00000000 SLC ECC Error Address Register8
    volatile unsigned int  SECC_EADDR9;             //  0x74 R 0x00000000 SLC ECC Error Address Register9
    volatile unsigned int  SECC_EADDR10;            //  0x78 R 0x00000000 SLC ECC Error Address Register10
    volatile unsigned int  SECC_EADDR11;            //  0x7C R 0x00000000 SLC ECC Error Address Register11
    volatile unsigned int  SECC_EADDR12;            //  0x80 R 0x00000000 SLC ECC Error Address Register12
    volatile unsigned int  SECC_EADDR13;            //  0x84 R 0x00000000 SLC ECC Error Address Register13
    volatile unsigned int  SECC_EADDR14;            //  0x88 R 0x00000000 SLC ECC Error Address Register14
    volatile unsigned int  SECC_EADDR15;            //  0x8C R 0x00000000 SLC ECC Error Address Register15
    volatile unsigned int  ERRNUM;                  //  0x90 R 0x00000000 ECC Error Number
    volatile unsigned int  ECC_IREQ;                //  0x94 R/W 0x00000000 ECC Interrupt Control Register
}SLCECCERRADDR, *PSLCECCERRADDR;


typedef struct _MLCECC4ERRADDR{
    volatile unsigned int  MECC4_EADDR0;            //  0x50 R 0x00000000 MLC ECC Error Address Register0
    volatile unsigned int  MECC4_EADDR1;            //  0x54 R 0x00000000 MLC ECC Error Address Register1
    volatile unsigned int  MECC4_EADDR2;            //  0x58 R 0x00000000 MLC ECC Error Address Register2
    volatile unsigned int  MECC4_EADDR3;            //  0x5C R 0x00000000 MLC ECC Error Address Register3
    volatile unsigned int  NOTDEFINE0[12];
    volatile unsigned int  ERRNUM;                  //  0x90 R 0x00000000 ECC Error Number
    volatile unsigned int  ECC_IREQ;                //  0x94 R/W 0x00000000 ECC Interrupt Control Register    
}MLCECC4ERRADDR, *PMLCECC4ERRADDR;


typedef struct _MLCECC8ERRADDR{
    volatile unsigned int  MECC8_EADDR0;            //  0x50 R 0x00000000 MLC ECC8 Error Address Register0
    volatile unsigned int  MECC8_EADDR1;            //  0x54 R 0x00000000 MLC ECC8 Error Address Register1
    volatile unsigned int  MECC8_EADDR2;            //  0x58 R 0x00000000 MLC ECC8 Error Address Register2
    volatile unsigned int  MECC8_EADDR3;            //  0x5C R 0x00000000 MLC ECC8 Error Address Register3
    volatile unsigned int  MECC8_EADDR4;            //  0x60 R 0x00000000 MLC ECC8 Error Address Register4
    volatile unsigned int  MECC8_EADDR5;            //  0x64 R 0x00000000 MLC ECC8 Error Address Register5
    volatile unsigned int  MECC8_EADDR6;            //  0x68 R 0x00000000 MLC ECC8 Error Address Register6
    volatile unsigned int  MECC8_EADDR7;            //  0x6C R 0x00000000 MLC ECC8 Error Address Register7
    volatile unsigned int  NOTDEFINE0[8];
    volatile unsigned int  ERRNUM;                  //  0x90 R 0x00000000 ECC Error Number
    volatile unsigned int  ECC_IREQ;                //  0x94 R/W 0x00000000 ECC Interrupt Control Register    
}MLCECC8ERRADDR, *PMLCECC8ERRADDR;


typedef struct _MLCECC12ERRADDR{
    volatile unsigned int  MECC12_EADDR0;           //  0x50 R 0x00000000 MLC ECC12 Error Address Register0
    volatile unsigned int  MECC12_EADDR1;           //  0x54 R 0x00000000 MLC ECC12 Error Address Register1
    volatile unsigned int  MECC12_EADDR2;           //  0x58 R 0x00000000 MLC ECC12 Error Address Register2
    volatile unsigned int  MECC12_EADDR3;           //  0x5C R 0x00000000 MLC ECC12 Error Address Register3
    volatile unsigned int  MECC12_EADDR4;           //  0x60 R 0x00000000 MLC ECC12 Error Address Register4
    volatile unsigned int  MECC12_EADDR5;           //  0x64 R 0x00000000 MLC ECC12 Error Address Register5
    volatile unsigned int  MECC12_EADDR6;           //  0x68 R 0x00000000 MLC ECC12 Error Address Register6
    volatile unsigned int  MECC12_EADDR7;           //  0x6C R 0x00000000 MLC ECC12 Error Address Register7
    volatile unsigned int  MECC12_EADDR8;           //  0x70 R 0x00000000 MLC ECC12 Error Address Register8
    volatile unsigned int  MECC12_EADDR9;           //  0x74 R 0x00000000 MLC ECC12 Error Address Register9
    volatile unsigned int  MECC12_EADDR10;          //  0x78 R 0x00000000 MLC ECC12 Error Address Register10
    volatile unsigned int  MECC12_EADDR11;          //  0x7C R 0x00000000 MLC ECC12 Error Address Register11
    volatile unsigned int  NOTDEFINE0[4];
    volatile unsigned int  ERRNUM;                  //  0x90 R 0x00000000 ECC Error Number
    volatile unsigned int  ECC_IREQ;                //  0x94 R/W 0x00000000 ECC Interrupt Control Register    
}MLCECC12ERRADDR, *PMLCECC12ERRADDR;

typedef struct _MLCECC14ERRADDR{
    volatile unsigned int  MECC14_EADDR0;           //  0x50 R 0x00000000 MLC ECC14 Error Address Register0
    volatile unsigned int  MECC14_EADDR1;           //  0x54 R 0x00000000 MLC ECC14 Error Address Register1
    volatile unsigned int  MECC14_EADDR2;           //  0x58 R 0x00000000 MLC ECC14 Error Address Register2
    volatile unsigned int  MECC14_EADDR3;           //  0x5C R 0x00000000 MLC ECC14 Error Address Register3
    volatile unsigned int  MECC14_EADDR4;           //  0x60 R 0x00000000 MLC ECC14 Error Address Register4
    volatile unsigned int  MECC14_EADDR5;           //  0x64 R 0x00000000 MLC ECC14 Error Address Register5
    volatile unsigned int  MECC14_EADDR6;           //  0x68 R 0x00000000 MLC ECC14 Error Address Register6
    volatile unsigned int  MECC14_EADDR7;           //  0x6C R 0x00000000 MLC ECC14 Error Address Register7
    volatile unsigned int  MECC14_EADDR8;           //  0x70 R 0x00000000 MLC ECC14 Error Address Register8
    volatile unsigned int  MECC14_EADDR9;           //  0x74 R 0x00000000 MLC ECC14 Error Address Register9
    volatile unsigned int  MECC14_EADDR10;          //  0x78 R 0x00000000 MLC ECC14 Error Address Register10
    volatile unsigned int  MECC14_EADDR11;          //  0x7C R 0x00000000 MLC ECC14 Error Address Register11
    volatile unsigned int  MECC14_EADDR12;          //  0x80 R 0x00000000 MLC ECC14 Error Address Register12
    volatile unsigned int  MECC14_EADDR13;          //  0x84 R 0x00000000 MLC ECC14 Error Address Register13
    volatile unsigned int  NOTDEFINE0[2];
    volatile unsigned int  ERRNUM;                  //  0x90 R 0x00000000 ECC Error Number
    volatile unsigned int  ECC_IREQ;                //  0x94 R/W 0x00000000 ECC Interrupt Control Register    
}MLCECC14ERRADDR, *PMLCECC14ERRADDR;

typedef struct _MLCECC16ERRADDR{
    volatile unsigned int  MECC16_EADDR0;           //  0x50 R 0x00000000 MLC ECC16 Error Address Register0
    volatile unsigned int  MECC16_EADDR1;           //  0x54 R 0x00000000 MLC ECC16 Error Address Register1
    volatile unsigned int  MECC16_EADDR2;           //  0x58 R 0x00000000 MLC ECC16 Error Address Register2
    volatile unsigned int  MECC16_EADDR3;           //  0x5C R 0x00000000 MLC ECC16 Error Address Register3
    volatile unsigned int  MECC16_EADDR4;           //  0x60 R 0x00000000 MLC ECC16 Error Address Register4
    volatile unsigned int  MECC16_EADDR5;           //  0x64 R 0x00000000 MLC ECC16 Error Address Register5
    volatile unsigned int  MECC16_EADDR6;           //  0x68 R 0x00000000 MLC ECC16 Error Address Register6
    volatile unsigned int  MECC16_EADDR7;           //  0x6C R 0x00000000 MLC ECC16 Error Address Register7
    volatile unsigned int  MECC16_EADDR8;           //  0x70 R 0x00000000 MLC ECC16 Error Address Register8
    volatile unsigned int  MECC16_EADDR9;           //  0x74 R 0x00000000 MLC ECC16 Error Address Register9
    volatile unsigned int  MECC16_EADDR10;          //  0x78 R 0x00000000 MLC ECC16 Error Address Register10
    volatile unsigned int  MECC16_EADDR11;          //  0x7C R 0x00000000 MLC ECC16 Error Address Register11
    volatile unsigned int  MECC16_EADDR12;          //  0x80 R 0x00000000 MLC ECC16 Error Address Register12
    volatile unsigned int  MECC16_EADDR13;          //  0x84 R 0x00000000 MLC ECC16 Error Address Register13
    volatile unsigned int  MECC16_EADDR14;          //  0x88 R 0x00000000 MLC ECC16 Error Address Register14
    volatile unsigned int  MECC16_EADDR15;          //  0x8C R 0x00000000 MLC ECC16 Error Address Register15
    volatile unsigned int  ERRNUM;                  //  0x90 R 0x00000000 ECC Error Number
    volatile unsigned int  ECC_IREQ;                //  0x94 R/W 0x00000000 ECC Interrupt Control Register    
}MLCECC16ERRADDR, *PMLCECC16ERRADDR;


/*******************************************************************************
*	 28. Multi-Protocol Encapsulation Forward Error Correction (MPEFEC)
*    Register Define   (Base Addr = 0xF0510000)
********************************************************************************/
//#define HwMPEFEC_BASE                               *(volatile unsigned long*)0xF0510000

typedef struct _MPEFEC{
    volatile unsigned int  MERR;                    // 0x00 R/W 0x00000000 MPE_FEC Enable/ RESET [1] -> MPE_FEC Enable [0] -> MPE_FEC RESET
    volatile unsigned int  MSR;                     // 0x04 W 0x00000000 MPE_FEC Start (Auto clear)
    volatile unsigned int  MFRNR;                   // 0x08 R/W 0x03FF03FF [25:16] active row number -1 : MPE Frame 중 몇개의 row단위로 MEPFEC를 동작시키는지 설정 함 [9:0] MPE Frame 의 row -1 개수
    volatile unsigned int  MDSAR;                   // 0x0C R/W 0x00000000 Datagram 이 저장되어 있는 memory의 start address Memory Data Source Address
    volatile unsigned int  EFR;                     // 0x10 R/W 0x00000000 Erasure flag 이 저장되어있는 memory의 start address 
    volatile unsigned int  MCR;                     // 0x14 R/W 0x00014000 MPE_FEC Control [25] ->항상 0 이어야 함 [24] -> erasure_on(erasure사용 1’b1) [22:16] -> eras_thres(erasure가eras_thres보다 크면 erasure 를 사용하지않음) [15:8] -> stuffing_len [6:0] -> puncturing_len 
    volatile unsigned int  MSTR;                    // 0x18 R 0x00000000 MPE_FEC Status [1] -> mpe_done [0] -> mpe_over 
    volatile unsigned int  MIER;                    // 0x1C R 0x00000000 MPE_FEC IRQ Enable [1] -> mpe_done IRQ Enable [0] -> mpe_over IRQ Enable
    volatile unsigned int  MICR;                    // 0x20 W 0x00000000 MPE_FEC IRQ_clear [1] -> mpe_done IRQ clear [0] -> mpe_over IRQ clear
    volatile unsigned int  MARR;                    // 0x24 R/W 0x03FF0000 MPE Frame 중 몇번째 row 부터 MPEFEC 를 할 것인지 설정함 [9:0] start row number [25:16] end row number
    volatile unsigned int  ECNT;                    // 0x28 R/W 0x00000000 MPEFECs 복구된 Error 개수 
    volatile unsigned int  NOTDEFINE0[3];           // 0x34-0x3C R/W Reserved    
}MPEFEC, *PMPEFEC;


/*******************************************************************************
*	 29. IOBUS Configuration Register Define   (Base Addr = 0xF05F5000)
********************************************************************************/
//#define HwIOBUSCFG_BASE                             *(volatile unsigned long*)0xF05F5000

typedef struct _IOBUSCFG{
    volatile unsigned int  USBOTG;                  // 0x00 Refer to USB OTG Configuration Register (OTGCR) in 15.2 register Description for USB 2.0 OTG Controller USB OTG Configuration register (OTGCR)USB OTG Configuration Register (OTGCR)USB OTG Configuration register (OTGCR)USB OTG Configuration Register (OTGCR)USB OTG Configuration Register (OTGCR).
    volatile unsigned int  USB11H;                  // 0x04 Refer to USB 1.1 Host Configuration Register (USB11H) in 14.2 register Description for USB 1.1 Host Controller & Transceiver”
    volatile unsigned int  IOBAPB;                  // 0x08 IOBUS APB wait counter register
    volatile unsigned int  STORAGE;                 // 0x0C Storage Device Configuration Register
    volatile unsigned int  HCLKEN0;                 // 0x10 IOBUS AHB clock enable Register 0
    volatile unsigned int  HCLKEN1;                 // 0x14 IOBUS AHB clock enable Register 1
    volatile unsigned int  HCLKMEN;                 // 0x18 DMA AHB clock mask enable Register
    volatile unsigned int  NOTDEFINE0;              // 0x1C Reserved
    volatile unsigned int  HRSTEN0;                 // 0x20 IOBUS AHB Hreset Control register 0
    volatile unsigned int  HRSTEN1;                 // 0x24 IOBUS AHB Hreset Control register 1
    volatile unsigned int  USBOTG0;                 // 0x28 Refer to USB PHY Configuration Register0 (UPCR0) in 15.2 register Description for USB 2.0 OTG Controller
    volatile unsigned int  USBOTG1;                 // 0x2C Refer to USB PHY Configuration Register1 (UPCR1) in 15.2 register Description for USB 2.0 OTG Controller
    volatile unsigned int  USBOTG2;                 // 0x30 Refer to USB PHY Configuration Register2 (UPCR2) in 15.2 register Description for USB 2.0 OTG Controller
    volatile unsigned int  USBOTG3;                 // 0x34 Refer to USB PHY Configuration Register3 (UPCR3) in 15.2 register Description for USB 2.0 OTG Controller
    volatile unsigned int  IO_A2X;                  // 0x38 IOBUS AHB2AXI Control Register    
}IOBUSCFG, *PIOBUSCFG;



/************************************************************************
*	Channel 0 Memory Controller Register Define	(Base Addr = 0xF1000000)
************************************************************************/
//#define	HwEMC_BASE					*(volatile unsigned long *)0xF1000000	// External Memory Controller Base Register
typedef struct _EMC{
	volatile unsigned int	SDCFG;				//           0x00  R/W  0x6A484C00  SDRAM Configuration Register 
	volatile unsigned int	SDFSM;				//           0x04  R  0x00000000  SDRAM FSM Status Register 
	volatile unsigned int	MCFG;				//           0x08  R/W  0x01000042  Miscellaneous Configuration Register 
	volatile unsigned int	TST;				//           0x0C  W  0x00000000  Should not write to this ? it’s for TEST 
	volatile unsigned int	CSCFG0;				//           0x10  R/W  0x468AC809 External Chip Select 0 Config. Register   (CSN_CS0) 
	volatile unsigned int	NOTDEFINE0[2];		//           0x14  R/W  0x508AD01A  Reserved -  0x18  R/W  0x608AD03A  Reserved 
	volatile unsigned int	CSCFG3;				//           0x1C  R/W  0x728AD01A External Chip Select 3 Config. Register (CSN_NOR) 
	volatile unsigned int	CLKCFG;				//           0x20  R/W  0x00000A05 Memory Controller  Version  &  Periodic Clock  Enable Count Register 
	volatile unsigned int	SDCMD;				//           0x24  R/W  -  SDRAM Command Write Register 
	volatile unsigned int	SDCFG1;				//           0x28  R/W  0xFFFFFFFF  Extra SDRAM Configuration Register 
}EMC, *PEMC;


/*******************************************************************************
*	 TCC8900_DataSheet_PART 6 DDI_BUS_V0.00 Dec.11 2008
********************************************************************************/
/************************************************************************
*	4. LCD INTERFACE Register Define				(Base Addr = 0xF0200000)
************************************************************************/
//#define	HwLCDC0_BASE							*(volatile unsigned long *)0xF0200000	// LCDC0 Control Base Register
//#define	HwLCDC1_BASE							*(volatile unsigned long *)0xF0204000	// LCDC1 Control Base Register

typedef struct _LCDC{
	volatile unsigned int	LCTRL;				//   0x00  R/W  0x00000000  LCD Control Register 
	volatile unsigned int	LBC;				//   0x04  R/W  0x00000000  LCD Background Color Register 
	volatile unsigned int	LCLKDIV;			//   0x08  R/W  0x00000000  LCD Clock Divider Register 
	volatile unsigned int	LHTIME1;			//   0x0C  R/W  0x00000000  LCD Horizontal Timing Register 1 
	volatile unsigned int	LHTIME2;			//   0x10  R/W  0x00000000  LCD Horizontal Timing Register 2 
	volatile unsigned int	LVTIME1;			//   0x14  R/W  0x00000000  LCD Vertical Timing Register 1 
	volatile unsigned int	LVTIME2;			//   0x18  R/W  0x00000000  LCD Vertical Timing Register 2 
	volatile unsigned int	LVTIME3;			//   0x1C  R/W  0x00000000  LCD Vertical Timing Register 3 
	volatile unsigned int	LVTIME4;			//   0x20  R/W  0x00000000  LCD Vertical Timing Register 4 
	volatile unsigned int	LLUTR;				//   0x24  R/W  0x00000000  LCD Lookup Register for Red 
	volatile unsigned int	LLUTG;				//   0x28  R/W  0x00000000  LCD Lookup Register for Green 
	volatile unsigned int	LLUTB;				//   0x2C  R/W  0x00000000  LCD Lookup Register for Blue 
	volatile unsigned int	LDP7L;				//   0x30  R/W  0x4d2b3401  LCD Modulo 7 Dithering Pattern (low) 
	volatile unsigned int	LDP7H;				//   0x34  R/W  0x0000003f  LCD Modulo 7 Dithering Pattern (high) 
	volatile unsigned int	LDP5;				//   0x38  R/W  0x1d0b0610  LCD Modulo 5 Dithering Pattern Register 
	volatile unsigned int	LDP4;				//   0x3C  R/W  0x00000768  LCD Modulo 4 Dithering Pattern Register 
	volatile unsigned int	LDP3;				//   0x40  R/W  0x00000034  LCD 3-bit Dithering Pattern Register 
	volatile unsigned int	LCP1;				//   0x44  R/W  0x000000ff  LCD Clipping Register1 
	volatile unsigned int	LCP2;				//   0x48  R/W  0x000000ff  LCD Clipping Register2 
	volatile unsigned int	LDS;				//   0x4C  R/W  0x00000000  LCD Display Size Register 
	volatile unsigned int	LSTATUS;			//   0x50  R/CLR  0x00000000  LCD Status Register 
	volatile unsigned int	LIM;				//   0x54  R/W  0x0000001f  LCD Interrupt Register. 
	volatile unsigned int	LGR0;				//   0x58  R/W  0x00000000  LCD Gamma Correction Register 0 for Red Color 
	volatile unsigned int	LGR1;				//   0x5C  R/W  0x00000000  LCD Gamma Correction Register 1 for Red Color 
	volatile unsigned int	LGG0;				//   0x60  R/W  0x00000000  LCD Gamma Correction Register 0 for Green Color 
	volatile unsigned int	LGG1;				//   0x64  R/W  0x00000000  LCD Gamma Correction Register 1 for Green Color 
	volatile unsigned int	LGB0;				//   0x68  R/W  0x00000000  LCD Gamma Correction Register 0 for Blue Color 
	volatile unsigned int	LGB1;				//   0x6C  R/W  0x00000000  LCD Gamma Correction Register 1 for Blue Color 
	volatile unsigned int	LENH;				//   0x70  R/W  0x00000020  LCD Color Enhancement Register 
	volatile unsigned int	NOTDEFINE0;		    // 	 0x74
	volatile unsigned int	LI0C;				//   0x78  R/W  0x00000000  LCD Image 0 Control Register 
	volatile unsigned int	LI0P;				//   0x7C  R/W  0x00000000  LCD Image 0 Position Register 
	volatile unsigned int	LI0S;				//   0x80  R/W  0x00000000  LCD Image 0 Size Register 
	volatile unsigned int	LI0BA0;				//   0x84  R/W  0x00000000  LCD Image 0 Base Address 0 Register. 
	volatile unsigned int	LI0CA;				//   0x88  R/W  0x00000000  LCD Image 0 Current Address Register. 
	volatile unsigned int	LI0BA1;				//   0x8C  R/W  0x00000000  LCD Image 0 Base Address 1 Register 
	volatile unsigned int	LI0BA2;				//   0x90  R/W  0x00000000  LCD Image 0 Base Address 2 Register 
	volatile unsigned int	LI0O;				//   0x94  R/W  0x00000000  LCD Image 0 Offset Register 
	volatile unsigned int	LI0SR;				//   0x98  R/W  0x00000000  LCD Image 0 Scale ratio 
	volatile unsigned int	LI0A;				//   0x9C  R/W  0x00000000  LCD Image 0 Alpha Configuration Register 
	volatile unsigned int	LI0KR;				//   0xA0  R/W  0x00000000  LCD Image 0 Keying Register for RED or LUMA(Y) 
	volatile unsigned int	LI0KG;				//   0xA4  R/W  0x00000000  LCD Image 0 Keying Register for BLUE or CHROMA(Cb) 
	volatile unsigned int	LI0KB;				//   0xA8  R/W  0x00000000  LCD Image 0 Keying Register for GREEN or CHROMA(Cr) 
	volatile unsigned int	LI0EN;				//   0xAC  R/W  0x00000000  LCD Image 0 Enhancement Register 
	volatile unsigned int	LI1C;				//   0xB0  R/W  0x00000000  LCD Image 1 Control Register 
	volatile unsigned int	LI1P;				//   0xB4  R/W  0x00000000  LCD Image 1 Position Register 
	volatile unsigned int	LI1S;				//   0xB8  R/W  0x00000000  LCD Image 1 Size Register 
	volatile unsigned int	LI1BA0;				//   0xBC  R/W  0x00000000  LCD Image 1 Base Address 0 Register. 
	volatile unsigned int	LI1CA;				//   0xC0  R/W  0x00000000  LCD Image 1 Current Address Register. 
	volatile unsigned int	LI1BA1;				//   0xC4  R/W  0x00000000  Not Used 
	volatile unsigned int	LI1BA2;				//   0xC8  R/W  0x00000000  Not Used 
	volatile unsigned int	LI1O;				//   0xCC  R/W  0x00000000  LCD Image 1 Offset Register 
	volatile unsigned int	LI1SR;				//   0xD0  R/W  0x00000000  LCD Image 1 Scale ratio- 
	volatile unsigned int	LI1A;				//   0xD4  R/W  0x00000000  LCD Image 1 Alpha Configuration Register 
	volatile unsigned int	LI1KR;				//   0xD8  R/W  0x00000000  LCD Image 1 Keying Register for RED or LUMA(Y) 
	volatile unsigned int	LI1KG;				//   0xDC  R/W  0x00000000  LCD Image 1 Keying Register for BLUE or CHROMA(Cb) 
	volatile unsigned int	LI1KB;				//   0xE0  R/W  0x00000000  LCD Image 1 Keying Register for GREEN or CHROMA(Cr) 
	volatile unsigned int	LI1EN;				//   0xE4  R/W  0x00000000  LCD Image 1 Enhancement Register 
	volatile unsigned int	LI2C;				//   0xE8  R/W  0x00000000  LCD Image 2 Control Register 
	volatile unsigned int	LI2P;				//   0xEC  R/W  0x00000000  LCD Image 2 Position Register 
	volatile unsigned int	LI2S;				//   0xF0  R/W  0x00000000  LCD Image 2 Size Register 
	volatile unsigned int	LI2BA0;				//   0xF4  R/W  0x00000000  LCD Image 2 Base Address 0 Register. 
	volatile unsigned int	LI2CA;				//   0xF8  R/W  0x00000000  LCD Image 2 Current Address Register. 
	volatile unsigned int	LI2BA1;				//   0xFC  R/W  0x00000000  Not Used 
	volatile unsigned int	LI2BA2;				//   0x100  R/W  0x00000000  Not Used 
	volatile unsigned int	LI2O;				//   0x104  R/W  0x00000000  LCD Image 2 Offset Register 
	volatile unsigned int	LI2SR;				//   0x108  R/W  0x00000000  LCD Image 2 Scale ratio 
	volatile unsigned int	LI2A;				//   0x10C  R/W  0x00000000  LCD Image 2 Alpha Register 
	volatile unsigned int	LI2KR;				//   0x110  R/W  0x00000000  LCD Image 2 Keying Register for RED or LUMA(Y) 
	volatile unsigned int	LI2KG;				//   0x114  R/W  0x00000000  LCD Image 2 Keying Register for BLUE or CHROMA(Cb) 
	volatile unsigned int	LI2KB;				//   0x118  R/W  0x00000000  LCD Image 2 Keying Register for GREEN or CHROMA(Cr) 
	volatile unsigned int	LI2EN;				//   0x11C  R/W  0x00000000  LCD Image 2 Enhancement Register 
	volatile unsigned int	LUTIDX;				//   0x120      Lookup Table index Register 

}sHwLCD,LCDC, *PLCDC;


//#define HwLCDLUT0_BASE							*(volatile unsigned long *)0xF0200400	// LCD LUT 0 Base Register
//#define HwLCDLUT1_BASE							*(volatile unsigned long *)0xF0204400	// LCD LUT 1 Base Register

typedef struct _LUT_TYPE{
	volatile unsigned char	BCr;	// [7:0]
	volatile unsigned char	GCb;	// [15:8]
	volatile unsigned char	RY;		// [23:16]
	volatile unsigned char	dummy;	// [31:24]]
}LUT_TYPE, *PLUT_TYPE;

typedef struct _LCDLUT{
	volatile LUT_TYPE	LUTDAT[256];			// 0x400 ~ 0x7FF
}LCDLUT, *PLCDLUT;


/************************************************************************
*	5. LCD System Interface Register Define		(Base Addr = 0xF020C400)
************************************************************************/
//#define	HwLCDSI_BASE							*(volatile unsigned long *)0xF020C400	// LCDSI Base Register

typedef struct _LCDSI0{
	volatile unsigned int		CTRL0;				// 0x400 R/W 0x00000000 Control register for LCDSI
}LCDSI0, *PLCDSI0;

typedef struct _LCDSI1{
	volatile unsigned int		CTRL1;				// 0x800 R/W 0xA0229011 Control register for nCS0 when RS=0(for core access)
	volatile unsigned int		CTRL2;				// 0x804 R/W 0xA0429021 Control register for nCS0 when RS=1(for core access)
	volatile unsigned int		CTRL3;				// 0x808 R/W 0xA0129009 Control register for nCS1 when RS=0(for core access)
	volatile unsigned int		CTRL4;				// 0x80C R/W 0xA0229011 Control register for nCS1 when RS=1(for core access)
	volatile unsigned int		CS0RS0;				// 0x810 R/W -if this register is read or written, reading or writing operations are generated on nCS0 while RS = 0.
	volatile unsigned int		NOTDEFINE0;			// 0x814
	volatile unsigned int		CS0RS1;				// 0x818 R/W -if this register is read or written, reading or writing operations are generated on nCS0 while RS = 1.
	volatile unsigned int		NOTDEFINE1;			// 0x81C
	volatile unsigned int		CS1RS0;				// 0x820 R/W -if this register is read or written, reading or writing operations are generated on nCS1 while RS = 0.
	volatile unsigned int		NOTDEFINE2;			// 0x824
	volatile unsigned int		CS1RS1;				// 0x828 R/W -if this register is read or written, reading or writing operations are generated on nCS1 while RS = 1.
	volatile unsigned int		NOTDEFINE3;			// 0x82C
	volatile unsigned int		CTRL5;				// 0x830 R/W 0xA0229011 Control register for nCS0 when RS=0(for lcd access)
	volatile unsigned int		CTRL6;				// 0x834 R/W 0xA0429021 Control register for nCS0 when RS=1(for lcd access)
	volatile unsigned int		CTRL7;				// 0x838 R/W 0xA0129009 Control register for nCS1 when RS=0(for lcd access)
	volatile unsigned int		CTRL8;				// 0x83C R/W 0xA0229011 Control register for nCS1 when RS=1(for lcd access)
}LCDSI1, *PLCDSI1;


/***********************************************************************
*	6. Memory to Memory Scaler Register Define	(Base Addr = 0xF0210000/0xF0220000)
************************************************************************/
//#define HwM2MSCALER1_BASE                       *(volatile unsigned long *)0xF0251000
//#define HwM2MSCALER1_BASE                       *(volatile unsigned long *)0xF0252000
typedef struct _M2MSCALER{
	volatile unsigned int	SRCBASEY;				// 0x000 R/W 0x00000000 Scaler source base address for Y
	volatile unsigned int	SRCBASEU;				// 0x004 R/W 0x00000000 Scaler source base address for U (Cb)
	volatile unsigned int	SRCBASEV;				// 0x008 R/W 0x00000000 Scaler source base address for V (Cr)
	volatile unsigned int	SRCSIZE;				// 0x00c R/W 0x00000000 Source image size register
	volatile unsigned int	SRCOFF; 				// 0x010 R/W 0x00000000 Source image line offset register
	volatile unsigned int	SRCCFG;	    			// 0x014 R/W 0x00000000 Source image configuration register
	volatile unsigned int	NOTDEFINE0[2];
	volatile unsigned int	DSTBASEY;				// 0x020 R/W 0x00000000 Scaler destination base address for Y
	volatile unsigned int	DSTBASEU;				// 0x024 R/W 0x00000000 Scaler destination base address for U (Cb)
	volatile unsigned int	DSTBASEV;				// 0x028 R/W 0x00000000 Scaler destination base address for V (Cr)
	volatile unsigned int	DST_SIZE;				// 0x02c R/W 0x00000000 Destination image size register
	volatile unsigned int	DSTOFF;		    		// 0x030 R/W 0x00000000 Destination image line offset register
	volatile unsigned int	DSTCFG;			    	// 0x034 R/W 0x00000000 Destination image configuration register
	volatile unsigned int	NOTDEFINE1[2];
	volatile unsigned int	MSCINF;		    		// 0x040 R/W 0x00000000 Scaling information register
	volatile unsigned int	MSCCTR;		    		// 0x044 R/W 0x00000000 Scaler control register
	volatile unsigned int	MSCSTR;		    		// 0x048 R/W 0x00000000 Scaler status register
	volatile unsigned int	NOTDEFINE2;
	volatile unsigned int	DSTSLINE;				// 0x050 R/W 0x00000000 Destination Set Line
	volatile unsigned int	DSTCLINE;				// 0x054 R 0x00000000 Destination Current Line
}M2MSCALER, *PM2MSCALER;


/************************************************************************
*	7. NTSC/PAL ENCODER Composite Output Register Define (Base Addr = 0xF9000000)
************************************************************************/
//#define	HwTVE_BASE								*(volatile unsigned long *)0xF9000000	// TV Encoder Base Register
typedef struct _NTSCPAL{
	volatile unsigned int 	STATA;					//0x00
	volatile unsigned int	ECMDA;					//0x04
	volatile unsigned int	ECMDB;					//0x08
	volatile unsigned int	GLK;					//0x0C
	volatile unsigned int	SCH;					//0x10
	volatile unsigned int	HUE;					//0x14
	volatile unsigned int	SAT;					//0x18
	volatile unsigned int	CONT;					//0x1C
	volatile unsigned int	BRIGHT;					//0x20
	volatile unsigned int	FSC_ADJM;				//0x24
	volatile unsigned int	FSC_ADJL;				//0x28
	volatile unsigned int	ECMDC;					//0x2C
	volatile unsigned int	NOTDEFINE0[4];			//0x30, 34, 38, 3C
	volatile unsigned int	DACSEL;					//0x40
	volatile unsigned int	NOTDEFINE1[3];			//0x44, 48, 4C
	volatile unsigned int	DACPD;					//0x50
	volatile unsigned int	NOTDEFINE2[11];			//0x54, 58, 5C, 60, 64, 68, 6C, 70, 74, 78, 7C
	volatile unsigned int	ICNTL;					//0x80
	volatile unsigned int	HVOFFST;				//0x84
	volatile unsigned int	HOFFST;					//0x88
	volatile unsigned int	VOFFST;					//0x8C
	volatile unsigned int	HSVSO;					//0x90
	volatile unsigned int	HSOE;					//0x94
	volatile unsigned int	HSOB;					//0x98
	volatile unsigned int	VSOB;					//0x9C
	volatile unsigned int	VSOE;					//0xA0
}NTSCPAL, *PNTSCPAL;

typedef struct _NTSCPALOP{
	volatile unsigned int	VENCON;					//0xF9080000
	volatile unsigned int	VENCIF;					//0xF9080004
}NTSCPALOP, *PNTSCPALOP;

/************************************************************************
*	8. HDMI Register Define				(Base Addr = 0xF0254000)
************************************************************************/
//#define HwHDMICTRL_BASE							*(volatile unsigned long *)0xF0254000	//Controller register base address 
typedef struct _HDMICTRL{
	volatile unsigned int	INTC_CON;				//  0x0000  R/W  Interrupt Control Register  0x00 
	volatile unsigned int	INTC_FLAG;				//  0x0004  R/W  Interrupt Flag Register  0x00 
	volatile unsigned int	AESKEY_VALID;			//  0x0008  R  aeskey_valid Register  0x00 
	volatile unsigned int	HPD;					//  0x000C  R  HPD signal  0x00 
}HDMICTRL, *PHDMICTRL;

//#define HwHDMICORE_BASE  							*(volatile unsigned long *)0xF0255000  
typedef struct _HDMICORE{
	volatile unsigned int	HDMI_CON_0;			//  0x0000  R/W  HDMI system control register 0  0x00 
	volatile unsigned int	HDMI_CON_1;			//  0x0004  R/W  HDMI system control register 1  0x00 
	volatile unsigned int	HDMI_CON_2;			//  0x0008  R/W  HDMI system control register 2  0x00 
	volatile unsigned int	STATUS;					//  0x0010  R/W  HDMI system status register  0x00 
	volatile unsigned int	PHY_STATUS;			//  0x0014  R  PHY status register  0x00 
	volatile unsigned int	NOTDEFINE0[2];			//	0x18, 0x1C
	volatile unsigned int	STATUS_EN;				//  0x0020  R/W  HDMI system status enable register  0x00 
	volatile unsigned int	NOTDEFINE1[3];			//	0x24, 0x28, 0x2C
	volatile unsigned int	HPD;					//  0x0030  R/W  HPD control register  0x00 
	volatile unsigned int	NOTDEFINE2[3];			//	0x34, 0x38, 0x3C
	volatile unsigned int	MODE_SEL;				//  0x0040  R/W  HDMI/DVI mode selection  0x00 
	volatile unsigned int	ENC_EN;					//  0x0044  R/W  HDCP encryption enable register  0x00 
	volatile unsigned int	NOTDEFINE3[2];			//	0x48, 0x4C
//Video Related Registers 
	volatile unsigned int	BLUE_SCREEN_0;			//  0x0050  R/W  Pixel values for blue screen  0x00 
	volatile unsigned int	BLUE_SCREEN_1;			//  0x0054  R/W  Pixel values for blue screen 0x00   
	volatile unsigned int	BLUE_SCREEN_2;			//  0x0058  R/W  Pixel values for blue screen 0x00   
	volatile unsigned int	NOTDEFINE4;			//	0x5C
	volatile unsigned int	HDMI_YMAX;				//  0x0060  R/W  Maximum Y (or "R,G,B)" pixel value 0x00   
	volatile unsigned int	HDMI_YMIN;				//  0x0064  R/W  Minimum Y (or "R,G,B)" pixel value 0x00   
	volatile unsigned int	HDMI_CMAX;				//  0x0068  R/W  Maximum Cb/Cr pixel value  0x00 
	volatile unsigned int	HDMI_CMIN;				//  0x006C  R/W  Minimum Cb/Cr pixel value  0x00 
	volatile unsigned int	NOTDEFINE5[12];		//	0x70~0x9C
	volatile unsigned int	H_BLANK[2];				//  0x00A0 ,0x00A4 R/W  Horizontal blanking setting  0x00 
	volatile unsigned int	NOTDEFINE6[2];		//	0xA8 0xAC
	volatile unsigned int	V_BLANK[3];				//  0x00B0,0x00B4, 0x00B8 R/W  Vertical blanking setting  0x00 
	volatile unsigned int	NOTDEFINE7[2];		//	0xB8 0xBC
	volatile unsigned int	H_V_LINE[3];			//  0x00C0,0x00C4, 0x00C8 R/W  Horizontal line & vertical line setting  0x00 
	volatile unsigned int	NOTDEFINE8[6];		//	0xCC, 0xD0, 0xD4, 0xD8, 0xDC, 0xE0
	volatile unsigned int	VSYNC_POL;				//  0x00E4  R/W  Vertical sync polarity control register  0x00 
	volatile unsigned int	INT_PRO_MODE ;			// 0x00E8  R/W  Interlace/Progressive control register  0x00 
	volatile unsigned int	NOTDEFINE9[9];		//	0xEC, 0xF0, 0xF4, 0xF8, 0xFC, 0x100, 0x104, 0x108, 0x10C
	volatile unsigned int	V_BLANK_F[3];			//  0x0110,0x0114, 0x0118 R/W  Vertical blanking setting for bottom field  0x00 
	volatile unsigned int	NOTDEFINE10;			//	0x011C
	volatile unsigned int	H_SYNC_GEN[3] ;		// 0x0120,0x0124 ,0x0128 R/W  Horizontal sync generation setting  0x00 
	volatile unsigned int	NOTDEFINE11;			//	0x012C
	volatile unsigned int	V_SYNC_GEN1[3];		//  0x0130,0x0134,0x0138  R/W  Vertical sync generation for top field or frame  0x01 
	volatile unsigned int	NOTDEFINE12;			//	0x013C
	volatile unsigned int	V_SYNC_GEN2[3];		//  0x0140,0x0144,0x0148  R/W Vertical sync generation for bottom field   - vertical position 0x01 
	volatile unsigned int	NOTDEFINE13;			//	0x014C
	volatile unsigned int	V_SYNC_GEN3[3];		//  0x0150,0x0154,0x0158  R/W Vertical sync generation for bottom field   - horizontal position 0x01 
	volatile unsigned int	NOTDEFINE14;			//	0x015C
//Audio Related Registers 
	volatile unsigned int	ASP_CON;				//  0x0160  R/W  ASP packet control register  0x00 
	volatile unsigned int	ASP_SP_FLAT;			//  0x0164  R/W  ASP packet sp_flat bit control  0x00 
	volatile unsigned int	NOTDEFINE15[2];		//	0x0168,0x016C
	volatile unsigned int	ASP_CHCFG[4];			//  0x0170,0x0174,0x0178,0x017C  R/W  ASP audio channel configuration  0x04 
	volatile unsigned int	ACR_CON;				//  0x0180  R/W  ACR packet control register  0x00 
	volatile unsigned int	ACR_MCTS[3];			//  0x0184,0x0188,0x018C  R/W  Measured CTS value  0x01 
	volatile unsigned int	ACR_CTS[3];				//  0x0190,0x0194,0x0198  R/W  CTS value for fixed CTS transmission mode.  0xe8 
	volatile unsigned int	NOTDEFINE16;		//	0x019C
	volatile unsigned int	ACR_N[3];				//  0x01A0,0x01A4,0x01A8  R/W  N value for ACR packet  0xe8 
	volatile unsigned int	NOTDEFINE17;		//	0x01AC
	volatile unsigned int	ACR_LSB2;				//  0x01B0  R/W  Alternate LSB for fixed CTS transmission mode  0x00 
	volatile unsigned int	ACR_TXCNT;				//  0x01B4  R/W  Number of ACR packet transmission per frame  0x1f 
	volatile unsigned int	ACR_TXINTERVAL;		//  0x01B8  R/W  Interval for ACR packet transmission  0x63 
	volatile unsigned int	ACR_CTS_OFFSET;		//  0x01BC  R/W  CTS offset for measured CTS mode  0x00 
//Packet Related Registers 
	volatile unsigned int	GCP_CON ;				// 0x01C0  R/W  ACR packet control register  0x00 
	volatile unsigned int	NOTDEFINE18[3];	//	0x01C4,0x01C8,0x01CC,
	volatile unsigned int	GCP_BYTE[3];			//  0x01D0,0x01D4,0x01D8  R/W  GCP packet body  0x00 
	volatile unsigned int	NOTDEFINE19;		//	0x01DC,
	volatile unsigned int	ACP_CON;				//  0x01E0  R/W  ACP packet control register  0x00 
	volatile unsigned int	NOTDEFINE20[3];		//	0x01E4,0x01E8,0x01EC,
	volatile unsigned int	ACP_TYPE;				//  0x01F0  R/W  ACP packet header  0x00 
	volatile unsigned int	NOTDEFINE21[3];		//	0x01F4,0x01F8,0x01FC,
	volatile unsigned int	ACP_DATA[17];			// 0x0200~0x0240  R/W ACP packet body 0x00 
	volatile unsigned int	NOTDEFINE22[3];		//	0x0244,0x0248,0x024C,
	volatile unsigned int	ISRC_CON;				//  0x0250  R/W  ACR packet control register  0x00 
	volatile unsigned int	NOTDEFINE23[4];		//	0x0254,0x0258,0x025C,0x0260
	volatile unsigned int	ISRC1_HEADER1;			//  0x0264  R/W  ISCR1 packet header  0x00 
	volatile unsigned int	NOTDEFINE24[2];		//	0x0268,0x026C,
	volatile unsigned int	ISRC1_DATA[16];		//	0x0270~0x02AC R/W ISRC1 packet body 0x00 
	volatile unsigned int	ISRC2_DATA[16];		//	0x02B0~0x02EC R/W ISRC2 packet body 0x00 
	volatile unsigned int	NOTDEFINE25[4];		//	0x02F0,0x02F4,0x02F8,0x2FC

	volatile unsigned int	AVI_CON;				//  0x0300  R/W  AVI packet control register  0x00 
	volatile unsigned int	NOTDEFINE26[3];		//	0x0304,0x0308,0x030c
	volatile unsigned int	AVI_CHECK_SUM;			//  0x0310  R/W  AVI packet checksum  0x00 
	volatile unsigned int	NOTDEFINE27[3];		//	0x0314,0x0318,0x031c
	volatile unsigned int	AVI_BYTE[13];			//	0x0320~0x0350 R/W AVI packet body 0x00 
	volatile unsigned int	NOTDEFINE28[3];		//	0x0354,0x0358,0x035c
	volatile unsigned int	AUI_CON;				//  0x0360  R/W  AUI packet control register  0x00 
	volatile unsigned int	NOTDEFINE29[3];		//	0x0364,0x0368,0x036c
	volatile unsigned int	AUI_CHECK_SUM;			//  0x0370  R/W  AUI packet checksum  0x00 
	volatile unsigned int	NOTDEFINE30[3];		//	0x0374,0x0378,0x037c
	volatile unsigned int	AUI_BYTE[5];			// 0x0380~0x0390  R/W AUI packet body 0x00 
	volatile unsigned int	NOTDEFINE31[3];		//	0x0394,0x0398,0x039c

	volatile unsigned int	MPG_CON;				//  0x03A0  R/W  ACR packet control register  0x00 
	volatile unsigned int	NOTDEFINE32[3];		//	0x03A4,0x03A8,0x03Ac
	volatile unsigned int	MPG_CHECK_SUM;			//  0x03B0  R/W  MPG packet checksum  0x00 
	volatile unsigned int	NOTDEFINE33[3];		//	0x03B4,0x03B8,0x03Bc

	volatile unsigned int	MPG_BYTE[5];			// 0x03C0~0x03D0  R/W MPG packet body 0x00 
	volatile unsigned int	NOTDEFINE34[11];		//	0x03D4,0x03D8,0x03Dc
													//	0x03E0,
													//  0x03F
	volatile unsigned int	SPD_CON;				//  0x0400  R/W  SPD packet control register  0x00 
	volatile unsigned int	NOTDEFINE35[3];		//	0x0344,0x0348,0x034c
	volatile unsigned int	SPD_HEADER0;			//  0x0410~  R/W  SPD packet header  0x00 
	volatile unsigned int	SPD_HEADER1;			//  0x0414  R/W  SPD packet header  0x00 
	volatile unsigned int	SPD_HEADER2;			//  0x0418  R/W  SPD packet header  0x00 
	volatile unsigned int	NOTDEFINE36;		//	0x041C
	volatile unsigned int	SPD_DATA[28];			//	 0x0420~0x048C R/W SPD packet body 0x00 

//HDCP Related Registes 
	volatile unsigned int	GAMUT_CON;				//  0x0500  R/W  GAMUT packet control register  0x00 
	volatile unsigned int	GAMUT_HEADER0;			//  0x0504  R/W  GAMUT packet header  0x00 
	volatile unsigned int	GAMUT_HEADER1;			//  0x0508  R/W  GAMUT packet header  0x00 
	volatile unsigned int	GAMUT_HEADER2;			//  0x050C  R/W  GAMUT packet header  0x00 
	volatile unsigned int	GAMUT_DATA[28];		// 0x0510~0x057C R/W  GAMUT packet body  0x00 
	volatile unsigned int	NOTDEFINE37[16];		//	0x0580~
													// 	0x0590,~
													//	0x05A0~
													//	0x05B0~
	volatile unsigned int	DC_CONTROL;			//  0x05C0  R/W  Deep Color Control Register  0x00 
	volatile unsigned int	VIDEO_PATTERN_GEN;		//  0x05C4  R/W  Video Pattern Generation Register  0x00 
	volatile unsigned int	HPD_GEN ;				// 0x05C8  R/W  HPD Duration value register  0x01 
	volatile unsigned int	NOTDEFINE38[113];		//	0x05CC
													// 	0x05D0,
													//	0x05E0
													//	0x05F0

	volatile unsigned int	HDCP_SHA1[20];			//	0x0600~0x064C R/W  SHA-1 value from repeater  0x00 
	volatile unsigned int	HDCP_KSV_LIST[5];		// 0x0650~0x0660 R/W  KSV list from repeater  0x00 

	volatile unsigned int	HDCP_KSV_LIST_CON;		//  0x0664  R/W  KSV list control  0x00 
	volatile unsigned int	NOTDEFINE39[2];		//	0x0668,0x066C
	volatile unsigned int	HDCP_SHA_RESULT;		//  0x0670  R/W  SHA-1 checking result register  0x00 
	volatile unsigned int	NOTDEFINE40[3];		//	0x0674,0x0678,0x067c
	volatile unsigned int	HDCP_CTRL1;			//  0x0680  R/W  HDCP control register1  0x00 
	volatile unsigned int	HDCP_CTRL2;			//  0x0684  R/W  HDCP control register2  0x00 
	volatile unsigned int	NOTDEFINE41[2];		//	0x0688,0x068c
	volatile unsigned int	HDCP_CHECK_RESULT;		//  0x0690  R/W  Ri and Pj value checking result  0x00 
	volatile unsigned int	NOTDEFINE42[3];		//	0x0394,0x0398,0x039c
	volatile unsigned int	HDCP_BKSV[5];			// 0x06A0~0x06B0 R/W  KSV of Rx  0x00
	volatile unsigned int	NOTDEFINE43[3];		//	0x06B4,0x06B8,0x06Bc
 
	volatile unsigned int	HDCP_AKSV[5];			//0x06C0~ 0x06D0 R/W  KSV of Tx  0x00 
	volatile unsigned int	NOTDEFINE44[3];		//	0x06D4,0x06D8,0x06Dc
	volatile unsigned int	HDCP_An[8];				// 0x06E0~ 0x06FC R/W  An value  0x00 
	volatile unsigned int	HDCP_BCAPS;			//  0x0700  R/W  BCAPS from Rx  0x00 
	volatile unsigned int	NOTDEFINE45[3];		//	0x0704,0x0708,0x070c
	volatile unsigned int	HDCP_BSTATUS_0;		//  0x0710  R/W  BSTATUS from Rx  0x00 
	volatile unsigned int	HDCP_BSTATUS_1;		//  0x0714  R/W  BSTATUS from Rx  0x00 
	volatile unsigned int	NOTDEFINE46[10];		//	0x0718,0x071c
													//	0x0720,
													//	0x0730
	volatile unsigned int	HDCP_Ri_0;				//  0x0740  R/W  Ri value of Tx  0x00 
	volatile unsigned int	HDCP_Ri_1;				//  0x0744  R/W  Ri value of Tx  0x00 
	volatile unsigned int	NOTDEFINE47[13];		//	0x0748
													//	0x0750,
													//	0x0760,
													//	0x0770
	volatile unsigned int	HDCP_I2C_INT;			//  0x0780  R/W  I2C interrupt flag  0x00 
	volatile unsigned int	NOTDEFINE48[3];		//	0x0784,0x0788,0x078c
	
	volatile unsigned int	HDCP_AN_INT;			//  0x0790  R/W  An value ready interrupt flag  0x00 
	volatile unsigned int	NOTDEFINE49[3];		//	0x0794,0x0798,0x079c
	
	volatile unsigned int	HDCP_WATCGDOG_INT;		//  0x07A0  R/W  Wachdog interrupt flag  0x00 
	volatile unsigned int	NOTDEFINE50[3];		//	0x07A4,0x07A8,0x07Ac
	
	volatile unsigned int	HDCP_Ri_INT;			//  0x07B0  R/W  Ri value update interrupt flag  0x00 
	volatile unsigned int	NOTDEFINE51[7];		//	0x07B4,0x07B8,0x07Bc
													//	0x07C0,
	volatile unsigned int	HDCP_Ri_Compare_0;		//  0x07D0  R/W  HDCP Ri Interrupt Frame number index register 0  0x80 
	volatile unsigned int	HDCP_Ri_Compare_1;		//  0x07D4  R/W  HDCP Ri Interrupt Frame number index register 1  0x7f 
	volatile unsigned int	NOTDEFINE52[2];		//	0x07D8,0x07Dc
	
	volatile unsigned int	HDCP_Frame_Count;		// 0x07E0  R  Current value of the frame count index in the hardware 0x00 
}HDMICORE, *PHDMICORE;

//AES register base address 
//fHDMIAES_BASE  							*(volatile unsigned long *)0xF0256000  //AES register base address 
typedef struct _HDMIAES{
	volatile unsigned int	AES_START;				//   0x0000  R/W  AES_START  0x00 
	volatile unsigned int	NOTDEFINE0[7];			//	0x0004,0x0008,0x000c
													//	0x0010,
	volatile unsigned int	AES_DATA_SIZE_L;		//   0x0020  R/W  AES_DATA_SIZE_L  0x20 
	volatile unsigned int	AES_DATA_SIZE_H;		//   0x0024  R/W  AES_DATA_SIZE_H  0x01 
	volatile unsigned int	NOTDEFINE1[6];			//	0x0028,0x002c
													//	0x0030
	volatile unsigned int	AES_DATA;				//   0x0040  W  AES_DATA  - 
}HDMIAES, *PHDMIAES;

//SPDIF Receiver register base address 
//#define HwHDMISPDIF_BASE  							*(volatile unsigned long *)0xF0257000  
typedef struct _HDMISPDIF{
	volatile unsigned int	SPDIFIN_CLK_CTRL;				//  0x0000  R/W  SPDIFIN Clock Control Register  0x02 
	volatile unsigned int	SPDIFIN_OP_CTRL;				//  0x0004  R/W  SPDIFIN Operation Control Register 1  0x00 
	volatile unsigned int	SPDIFIN_IRQ_MASK;				//  0x0008  R/W  SPDIFIN Interrupt Request Mask Register  0x00 
	volatile unsigned int	SPDIFIN_IRQ_STATUS;			//  0x000C  R/W  SPDIFIN Interrupt Request Status Register  0x00 
	volatile unsigned int	SPDIFIN_CONFIG_1;				//  0x0010  R/W  SPDIFIN Configuration Register 1  0x02 
	volatile unsigned int	SPDIFIN_CONFIG_2;				//  0x0014  R/W  SPDIFIN Configuration Register 2  0x00 
	volatile unsigned int	NOTDEFINE0[2];					//	0x0018 0x001C -  Reserved  - 
	volatile unsigned int	SPDIFIN_USER_VALUE_1;			//  0x0020  R  SPDIFIN User Value Register 1  0x00 
	volatile unsigned int	SPDIFIN_USER_VALUE_2;			//  0x0024  R  SPDIFIN User Value Register 2  0x00 
	volatile unsigned int	SPDIFIN_USER_VALUE_3;			//  0x0028  R  SPDIFIN User Value Register 3  0x00 
	volatile unsigned int	SPDIFIN_USER_VALUE_4;			//  0x002C  R  SPDIFIN User Value Register 4  0x00 
	volatile unsigned int	SPDIFIN_CH_STATUS_0_1;		//  0x0030  R  SPDIFIN Channel Status Register 0-1  0x00 
	volatile unsigned int	SPDIFIN_CH_STATUS_0_2;		//  0x0034  R  SPDIFIN Channel Status Register 0-2  0x00 
	volatile unsigned int	SPDIFIN_CH_STATUS_0_3;		//  0x0038  R  SPDIFIN Channel Status Register 0-3  0x00 
	volatile unsigned int	SPDIFIN_CH_STATUS_0_4;		//  0x003C  R  SPDIFIN Channel Status Register 0-4  0x00 
	volatile unsigned int	SPDIFIN_CH_STATUS_1;			//  0x0040  R  SPDIFIN Channel Status Register 1  0x00 
	volatile unsigned int	NOTDEFINE1;					//  0x0044 -  Reserved  - 
	volatile unsigned int	SPDIFIN_FRAME_PERIOD_1;		//  0x0048  R  SPDIFIN Frame Period Register 1  0x00 
	volatile unsigned int	SPDIFIN_FRAME_PERIOD_2;		//  0x004C  R  SPDIFIN Frame Period Register 2  0x00 
	volatile unsigned int	SPDIFIN_Pc_INFO_1;				//  0x0050  R  SPDIFIN Pc Info Register 1  0x00 
	volatile unsigned int	SPDIFIN_Pc_INFO_2;				//  0x0054  R  SPDIFIN Pc Info Register 2  0x00 
	volatile unsigned int	SPDIFIN_Pd_INFO_1;				//  0x0058  R  SPDIFIN Pd Info Register 1  0x00 
	volatile unsigned int	SPDIFIN_Pd_INFO_2;				//  0x005C  R  SPDIFIN Pd Info Register 2  0x00 
	volatile unsigned int	SPDIFIN_DATA_BUF_0_1;			//  0x0060  R  SPDIFIN Data Buffer Register 0_1  0x00 
	volatile unsigned int	SPDIFIN_DATA_BUF_0_2;			//  0x0064  R  SPDIFIN Data Buffer Register 0_2  0x00 
	volatile unsigned int	SPDIFIN_DATA_BUF_0_3;			//  0x0068  R  SPDIFIN Data Buffer Register 0_3  0x00 
	volatile unsigned int	SPDIFIN_USER_BUF_0;			//  0x006C  R  SPDIFIN User Buffer Register 0  0x00 
	volatile unsigned int	SPDIFIN_DATA_BUF_1_1;			//  0x0070  R  SPDIFIN Data Buffer Register 1_1  0x00 
	volatile unsigned int	SPDIFIN_DATA_BUF_1_2;			//  0x0074  R  SPDIFIN Data Buffer Register 1_2  0x00 
	volatile unsigned int	SPDIFIN_DATA_BUF_1_3;			//  0x0078  R  SPDIFIN Data Buffer Register 1_3  0x00 
	volatile unsigned int	SPDIFIN_USER_BUF_1;			//  0x007C  R  SPDIFIN User Buffer Register 1  0x00 
}HDMISPDIF, *PHDMISPDIF;

//I2S Receiver register base address 
//#define HwHDMII2S_BASE  							*(volatile unsigned long *)0xF0258000  
typedef struct _HDMII2S{
	volatile unsigned int	I2S_CLK_CON;					//  0x0000  R/W  I2S Clock Enable Register  0x00 
	volatile unsigned int	I2S_CON_1;						//  0x0004  R/W  I2S Control Register 1  0x00 
	volatile unsigned int	I2S_CON_2;						//  0x0008  R/W  I2S Control Register 2  0x00 
	volatile unsigned int	I2S_PIN_SEL_0;					//  0x000C  R/W  I2S Input Pin Selection Register 0  0x77 
	volatile unsigned int	I2S_PIN_SEL_1;					//  0x0010  R/W  I2S Input Pin Selection Register 1  0x77 
	volatile unsigned int	I2S_PIN_SEL_2;					//  0x0014  R/W  I2S Input Pin Selection Register 2  0x77 
	volatile unsigned int	I2S_PIN_SEL_3;					//  0x0018  R/W  I2S Input Pin Selection Register 3  0x07 
	volatile unsigned int	I2S_DSD_CON;					//  0x001C  R/W  I2S DSD Control Register  0x02 
	volatile unsigned int	I2S_MUX_CON;					//  0x0020  R/W  I2S In/Mux Control Register  0x60 
	volatile unsigned int	I2S_CH_ST_CON;					//  0x0024  R/W  I2S Channel Status Control Register  0x00 
	volatile unsigned int	I2S_CH_ST_0;					//  0x0028  R/W  I2S Channel Status Block 0  0x00 
	volatile unsigned int	I2S_CH_ST_1;					//  0x002C  R/W  I2S Channel Status Block 1  0x00 
	volatile unsigned int	I2S_CH_ST_2;					//  0x0030  R/W  I2S Channel Status Block 2  0x00 
	volatile unsigned int	I2S_CH_ST_3;					//  0x0034  R/W  I2S Channel Status Block 3  0x00 
	volatile unsigned int	I2S_CH_ST_4;					//  0x0038  R/W  I2S Channel Status Block 4  0x00 
	volatile unsigned int	I2S_CH_ST_SH_0;				//  0x003C  R  I2S Channel Status Block Shadow Register 0  0x00 
	volatile unsigned int	I2S_CH_ST_SH_1;				//  0x0040  R  I2S Channel Status Block Shadow Register 1  0x00 
	volatile unsigned int	I2S_CH_ST_SH_2;				//  0x0044  R  I2S Channel Status Block Shadow Register 2  0x00 
	volatile unsigned int	I2S_CH_ST_SH_3;				// 	0x0048  R  I2S Channel Status Block Shadow Register 3  0x00 
	volatile unsigned int	I2S_CH_ST_SH_4;				//  0x004C  R  I2S Channel Status Block Shadow Register 4  0x00 
	volatile unsigned int	I2S_VD_DATA;					//  0x0050  R/W  I2S Audio Sample Validity Register  0x00 
	volatile unsigned int	I2S_MUX_CH;					//  0x0054  R/W  I2S Channel Enable Register  0x03 
	volatile unsigned int	I2S_MUX_CUV;					//  0x0058  R/W  I2S CUV Enable Register  0x03 
	volatile unsigned int	I2S_IRQ_MASK;					//  0x005C  R/W  I2S Interrupt Request Mask Register  0x03 
	volatile unsigned int	I2S_IRQ_STATUS;				//  0x0060  R/W  I2S Interrupt Request Status Register  0x00 
	volatile unsigned int	I2S_CH0_L_0;					//  0x0064  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH0_L_1;					//  0x0068  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH0_L_2;					//  0x006C  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH0_L_3;					//  0x0070  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH0_R_0;					//  0x0074  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH0_R_1;					//  0x0078  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH0_R_2;					//  0x007C  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH0_R_3;					//  0x0080  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH1_L_0;					//  0x0084  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH1_L_1;					//  0x0088  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH1_L_2;					//  0x008C  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH1_L_3;					//  0x0090  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH1_R_0;					//  0x0094  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH1_R_1;					//  0x0098  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH1_R_2;					//  0x009C  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH1_R_3;					//  0x00A0  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH2_L_0;					//  0x00A4  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH2_L_1;					//  0x00A8  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH2_L_2;					//  0x00AC  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH2_L_3;					//  0x00B0  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH2_R_0;					//  0x00B4  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH2_R_1;					//  0x00B8  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH2_R_2;					//  0x00BC  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_Ch2_R_3;					//  0x00C0  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH3_L_0;					//  0x00C4  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH3_L_1;					//  0x00C8  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH3_L_2;					//  0x00CC  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH3_R_0;					//  0x00D0  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH3_R_1;					//  0x00D4  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CH3_R_2;					//  0x00D8  R  I2S PCM Output Data Register  0x00 
	volatile unsigned int	I2S_CUV_L_R;					//  0x00DC  R  I2S CUV Output Data Register  0x00 
}HDMII2S, *PHDMII2S;

 //CEC register base address 					
//#define HwHDMICEC_BASE  							*(volatile unsigned long *)0xF0259000 
typedef struct _HDMICEC{
	volatile unsigned int	CEC_TX_STATUS_0;					//  0x0000  R  CEC Tx status register 0.  0x00 
	volatile unsigned int	CEC_TX_STATUS_1;					//  0x0004  R  CEC Tx status register 1. Number of blocks transferred. 0x00 
	volatile unsigned int	CEC_RX_STATUS_0;					//  0x0008  R  CEC Rx status register 0.  0x00 
	volatile unsigned int	CEC_RX_STATUS_1;					//  0x000C  R  CEC Rx status register 1. Number of blocks received. 0x00 
	volatile unsigned int	CEC_INTR_MASK;						//  0x0010  R/W  CEC interrupt mask register  0x00 
	volatile unsigned int	CEC_INTR_CLEAR;					//  0x0014  R/W  CEC interrupt clear register  0x00 
	volatile unsigned int	NOTDEFINE0[2];						//	0x0018 0x001C -  Reserved  - 
	volatile unsigned int	CEC_LOGIC_ADDR;					//  0x0020  R/W  HDMI Tx logical address register  0x0F 
	volatile unsigned int	NOTDEFINE1[3];					//	0x0024 0x0028, 0x002C -  Reserved  - 
	volatile unsigned int	CEC_DIVISOR_0;						//  0x0030  R/W  Clock divisor for 0.05ms period count ([7:0] of 32-bit) 0x00 
	volatile unsigned int	CEC_DIVISOR_1;						//  0x0034  R/W  Clock divisor for 0.05ms period count ([15:8] of 32-bit) 0x00 
	volatile unsigned int	CEC_DIVISOR_2;						//  0x0038  R/W  Clock divisor for 0.05ms period count ([23:16] of 32-bit) 0x00 
	volatile unsigned int	CEC_DIVISOR_3;						//  0x003C  R/W  Clock divisor for 0.05ms period count ([31:24] of 32-bit) 0x00 
//CEC Tx related Registers 
	volatile unsigned int	CEC_TX_CTRL;						//  0x0040  R/W  CEC Tx control register  0x10 
	volatile unsigned int	CEC_TX_BYTE_NUM;					//  0x0044  R/W  Number of blocks in a message to be transferred  0x00 
	volatile unsigned int	NOTDEFINE2[6];						//	0x0048 0x004C -  Reserved  - 
																//  0x0050
	volatile unsigned int	CEC_TX_STATUS_2;					//  0x0060  R  CEC Tx status register 2  0x00 
	volatile unsigned int	CEC_TX_STATUS_3;					//  0x0064  R  CEC Tx status register 3  0x00 
	volatile unsigned int	NOTDEFINE3[6];						//	0x0068 0x006C -  Reserved  - 
																//  0x0070
	volatile unsigned int	NOTDEFINE4[3];					//	0x0074 0x0078, 0x007C -  Reserved  - 
	volatile unsigned int	CEC_TX_BUFFER[16];					// 0x0080 ~ 0x00BC R/W  Byte #0 ~ #15 of CEC message to be transferred. (#0 is transferred 1st ) 0x00 
//CEC Rx related Registers 
	volatile unsigned int	CEC_RX_CTRL;						//  0x00C0  R/W  CEC Rx control register  0x00 
	volatile unsigned int	NOTDEFINE5[7];						//	0x00C4 0x00C8, 0x00CC -  Reserved  - 
																//  0x00D0
	volatile unsigned int	CEC_RX_STATUS_2;					//  0x00E0  R  CEC Rx status register 2  0x00 
	volatile unsigned int	CEC_RX_STATUS_3;					//  0x00E4  R  CEC Rx status register 3eived 1st )  0x00 
	volatile unsigned int	NOTDEFINE6[2];						//	0x00E8, 0x00EC -  Reserved  - 
	volatile unsigned int	CEC_RX_BUFFER[16];					// 0x0100 ~ 0x013C R  Byte #0 ~ #15 of CEC message received (#0 is received 1st ) 0x00 
}HDMICEC, *PHDMICEC;

/***********************************************************************
*	 9-1. Camera Interface Register Define			(Base Addr = 0xF0230000)
************************************************************************/
//#define	HwCIF_BASE								*(volatile unsigned long *)0xF0230000	// CIF Base Register
typedef struct _CIF{
	volatile unsigned int	ICPCR1;					// 0x00 W/R 0x00000000 Input Image Color/Pattern Configuration Register 1
	volatile unsigned int	CCIR656FCR1;			// 0x04 W/R 0x06ff0000 CCIR656 Format Configuration Register 1
	volatile unsigned int	CCIR656FCR2;			// 0x08 W/R 0x010b CCIR656 Format Configuration Register 2
	volatile unsigned int	IIS;					// 0x0C W/R 0x00000000 Input Image Size
	volatile unsigned int	IIW1;					// 0x10 W/R 0x00000000 Input Image Windowing 1
	volatile unsigned int	IIW2;					// 0x14 W/R 0x00000000 Input Image Windowing 2
	volatile unsigned int	CDCR1;					// 0x18 W/R 0x0003 DMA Configuration Register 1
	volatile unsigned int	CDCR2;					// 0x1C W/R 0x00000000 DMA Configuration Register 2
	volatile unsigned int	CDCR3;					// 0x20 W/R 0x00000000 DMA Configuration Register 3
	volatile unsigned int	CDCR4;					// 0x24 W/R 0x00000000 DMA Configuration Register 4
	volatile unsigned int	CDCR5;					// 0x28 W/R 0x00000000 DMA Configuration Register 5
	volatile unsigned int	CDCR6;					// 0x2C W/R 0x00000000 DMA Configuration Register 6
	volatile unsigned int	CDCR7;					// 0x30 W/R 0x00000000 DMA Configuration Register 7
	volatile unsigned int	CDCR8;					// 0x34 W/R 0x00000000 DMA Configuration Register 7
	volatile unsigned int	FIFOSTATE;				// 0x38 R 0x00000000 FIFO Status Register
	volatile unsigned int	CIRQ;					// 0x3C W/R 0x00000000 Interrupt & Status register
	volatile unsigned int	OCTRL1;					// 0x40 W/R 0x37000000 Overlay Control 1
	volatile unsigned int	OCTRL2;					// 0x44 W/R 0x00000000 Overlay Control 2
	volatile unsigned int	OCTRL3;					// 0x48 W/R 0x00000000 Overlay Control 3
	volatile unsigned int	OCTRL4;					// 0x4C W/R 0x00000000 Overlay Control 4
	volatile unsigned int	OIS;					// 0x50 W/R 0x00000000 Overlay Image Size
	volatile unsigned int	OIW1;					// 0x54 W/R 0x00000000 Overlay Image Windowing 1
	volatile unsigned int	OIW2;					// 0x58 W/R 0x00000000 Overlay Image Windowing 2
	volatile unsigned int	COBA;					// 0x5C W/R 0x00000000 Overlay Base Address
	volatile unsigned int	COBO;					// 0x60 W/R 0x00000000 Overlay Base Address Offset 
	volatile unsigned int	CDS;					// 0x64 W/R 0x00000000 Camera Down Scaler
	volatile unsigned int	CCM1;					// 0x68 W/R 0x00000000 Capture Mode Configuration 1
	volatile unsigned int	CCM2;					// 0x6C W/R 0x00000000 Capture Mode Configuration 2
	volatile unsigned int	CESA;					// 0x70 W/R 0x00000000 Point Encoding Start Address
	volatile unsigned int	CR2Y;					// 0x74  W/R 0x00000000 RGB2YUV Format converter Configuration
	volatile unsigned int	CCYA;					// 0x78 R - Current Y Address
	volatile unsigned int	CCYU;					// 0x7C R - Current U Address
	volatile unsigned int	CCYV;					// 0x80 R - Current V Address
	volatile unsigned int	CCLC;					// 0x84 R Current Line count
}CIF, *PCIF;

/***********************************************************************
*	 9-2. Effect Register Define			(Base Addr = 0xF0230100)
************************************************************************/
//#define	HwCEM_BASE    								*(volatile unsigned long *)0xF0230100  //W/R  0x00000000  Effect mode register 

typedef struct _EFFECT{
	volatile unsigned int	CEM;					// 0x00 W/R 0x00000000 Effect mode register
	volatile unsigned int	CSUV;				// 0x04 W/R 0x00000000 Sepia UV setting
	volatile unsigned int	CCS;					// 0x08 W/R 0x00000000 Color selection register
	volatile unsigned int	CHFC;				// 0x0C W/R 0x00000000 H-filter coefficent0
	volatile unsigned int	CST;				// 0x10 W/R 0x00000000 Sketch threshold register
	volatile unsigned int	CCT;				// 0x14 W/R 0x00000000 Clamp threshold register
	volatile unsigned int	CBR;					// 0x18 W/R 0x00000000 BIAS register
	volatile unsigned int	CEIS;					// 0x1C W/R 0x00000000 Image size register
	volatile unsigned int	NOTDEFINE0[8];			// 0x20, 24, 28, 2C, 30, 34, 38, 3C,
	volatile unsigned int	INPATH_CTRL;			// 0x40 W/R 0x00000000 Inpath configuration
	volatile unsigned int	CISA1;				// 0x44 W/R 0x00000000 Source address in Y channel
	volatile unsigned int	CISA2;				// 0x48 W/R 0x00000000 Source address in U channel
	volatile unsigned int	CISA3;				// 0x4C W/R 0x00000000 Source address in V channel
	volatile unsigned int	CISS;				// 0x50 W/R 0x00000000 Source image size
	volatile unsigned int	CISO;				// 0x54 W/R 0x00000000 Source image offset
	volatile unsigned int	CIDS;				// 0x58 W/R 0x00000000 Destination image size
	volatile unsigned int	CIS;				// 0x5C W/R 0x00000000 Target scale	
}EFFECT, *PEFFECT;

/***********************************************************************
*	 9-3. Scaler Register Define			(Base Addr = 0xF0230200)
************************************************************************/
//#define	HwCSC_BASE    								*(volatile unsigned long *)0xF0230200  //W/R  0x00000000  Scaler configuration 
typedef struct _CIFSACLER{
	volatile unsigned int	CSC;				//  0x00  W/R  0x00000000  Scaler configuration 
	volatile unsigned int	CSSF;				//  0x04  W/R  0x00000000  Scale factor 
	volatile unsigned int	CSSO;				//  0x08  W/R  0x00000000  Image offset 
	volatile unsigned int	CSSS;				//  0x0C  W/R  0x00000000  Source image size 
	volatile unsigned int	CSDS;				//  0x10  W/R  0x00000000  Destination image size 
}CIFSACLER, *PCIFSACLER;



/***********************************************************************
*	10. Video and Image  Quality Enhancer Register Define	(Base Addr = 0xF0230200)
************************************************************************/
//#define HwVIQE_BASE                     *(volatile unsigned long *)0xF0252000
typedef struct _VIQE{
	volatile unsigned int	CTRL;               // 0x000 R/W 0x00000000 VIQE General Control Register
	volatile unsigned int	SIZE;               // 0x004 R/W 0x00000000 VIQE SIZE Register
	volatile unsigned int	TIMEGEN;            // 0x008 R/W 0x00000000 VIQE Time Generator Register
	volatile unsigned int	LUMADLY;            // 0x00C R/W 0x00000000 VIQE Luma Delay Register
	volatile unsigned int	IMGCONF;            // 0x010 R/W 0x00000000 VIQE Image Configuration Register
	volatile unsigned int	IMGFMT;             // 0x014 R/W 0x00000000 VIQE Image Format Register
	volatile unsigned int	MISCC;              // 0x018 R/W 0x00000000 VIQE Misc, Control Register
	volatile unsigned int	FRMC;               // 0x01C R/W 0x00000000 VIQE Frame Control Register
	volatile unsigned int	INT;                // 0x020 R/W 0x00000000 VIQE Interrupt Register
	volatile unsigned int	INTMASK;            // 0x024 R/W 0x00000000 VIQE Interrupt Mask register
	volatile unsigned int   NOTDEFINE0[5];
	volatile unsigned int	VERSION;            // 0x03c R 0x4d2b3401 VIQE Version register
    volatile unsigned int   NOTDEFINE1[16];
	volatile unsigned int	DI_CTRL;            // 0x080 R/W 0x00000000 De-interlacer Control Register
	volatile unsigned int	DI_ENGINE0;         // 0x084 R/W 0x00000000 De-interlacer Engine 0 Register
	volatile unsigned int	DI_ENGINE1;         // 0x088 R/W 0x00000000 De-interlacer Engine 1 Register
	volatile unsigned int	PD_THRES0;          // 0x08C R/W 0x00000000 De-interlacer Pulldown Threshold 0 Register
	volatile unsigned int	PD_THRES1;          // 0x090 R/W 0x00000000 De-interlacer Pulldown Threshold 1 Register
	volatile unsigned int	PD_JUDDER;          // 0x094 R/W 0x00000000 De-interlacer Pulldown Judder Register
	volatile unsigned int	DI_MISCC;           // 0x098 R/W 0x00000000 De-interlacer Misc. Control register
	volatile unsigned int   NOTDEFINE2;
	volatile unsigned int	DI_STATUS;          // 0x0A0 R De-interlacer Status Register
	volatile unsigned int	PD_STATUS;          // 0x0A4 R De-interlacer Pulldown Status Register
	volatile unsigned int	DI_REGION0;         // 0x0A8 R/W 0x00000000 De-interlacer Region 0 Register
	volatile unsigned int	DI_REGION1;         // 0x0AC R/W 0x00000000 De-interlacer Region 1 register
	volatile unsigned int   NOTDEFINE3[3];
	volatile unsigned int	DI_INT;             // 0x0BC R/W 0x00000000 De-interlacer Interrupt Register
    volatile unsigned int   NOTDEFINE4[16];
    
	volatile unsigned int	DN_C_H_Y0;          // 0x100 R/W 0xbfffffa4 Temporal De-noiser horizontal coefficient #0 in luminance
	volatile unsigned int	DN_C_H_Y1;          // 0x104 R/W 0x15556aaa Temporal De-noiser horizontal coefficient #1 in luminance
	volatile unsigned int	DN_C_V_Y0;          // 0x108 R/W 0xaaaaaaa4 Temporal De-noiser vertical coefficient #0 in luminance
	volatile unsigned int	DN_C_V_Y1;          // 0x10C R/W 0x15556aaa Temporal De-noiser vertical coefficient #1 in luminance
	volatile unsigned int	DN_C_T_Y0;          // 0x110 R/W 0xaaaaaaa4 Temporal De-noiser temporal coefficient #0 in luminance
	volatile unsigned int	DN_C_T_Y1;          // 0x114 R/W 0x15556aaa Temporal De-noiser temporal coefficient #1 in luminance
	volatile unsigned int	DN_C_H_C0;          // 0x118 R/W 0xbfffffa4 Temporal De-noiser horizontal coefficient #0 in chrominance 
	volatile unsigned int	DN_C_H_C1;          // 0x11C R/W 0x15556aaa Temporal De-noiser horizontal coefficient #1 in chrominance
	volatile unsigned int	DN_C_V_C0;          // 0x120 R/W 0xaaaaaaa4 Temporal De-noiser vertical coefficient #0 in chrominance
	volatile unsigned int	DN_C_V_C1;          // 0x124 R/W 0x15556aaa Temporal De-noiser vertical coefficient #1 in chrominance
	volatile unsigned int	DN_C_T_C0;          // 0x128 R/W 0xaaaaaaa4 Temporal De-noiser temporal coefficient #0 in chrominance
	volatile unsigned int	DN_C_T_C1;          // 0x12C R/W 0x15556aaa Temporal De-noiser temporal coefficient #1 in chrominance
	volatile unsigned int	DN_STATE0_TEM;      // 0x130 R/W 0x00000000 Temporal De-noiser count states and int. mask
	volatile unsigned int	DN_STATE1_TEM;      // 0x134 R - Temporal De-noiser count states
	volatile unsigned int	DN_DIV_IMG_TEM;     // 0x138 R/W 0x00000168 Temporal De-noiser image divide
	volatile unsigned int   NOTDEFINE5;
	volatile unsigned int	DN_C_SPA_Y0;        // 0x140 R/W 0x12320e0a Spatial De-noiser coefficient #0 in luminance
	volatile unsigned int	DN_C_SPA_Y1;        // 0x144 R/W 0x373c051d Spatial De-noiser coefficient #1 in luminance
	volatile unsigned int	DN_C_SPA_Y2;        // 0x148 R/W 0x4a0640ff Spatial De-noiser coefficient #2 in luminance
	volatile unsigned int	DN_C_SPA_Y3;        // 0x14C R/W 0x003100fb Spatial De-noiser coefficient #3 in luminance
	volatile unsigned int	DN_C_SPA_C0;        // 0x150 R/W 0x12190805 Spatial De-noiser coefficient #0 in chrominance
	volatile unsigned int	DN_C_SPA_C1;        // 0x154 R/W 0x373c0507 Spatial De-noiser coefficient #1 in chrominance
	volatile unsigned int	DN_C_SPA_C2;        // 0x158 R/W 0x4a0640ff Spatial De-noiser coefficient #2 in chrominance
	volatile unsigned int	DN_C_SPA_C3;        // 0x15C R/W 0x003100fb Spatial De-noiser coefficient #3 in chrominance
	volatile unsigned int	DN_FIFOSTATE;       // 0x160 R/W 0x00000000 De-noiser FIFO states
	volatile unsigned int	DN_STATE0_SPA;      // 0x164 R/W 0x00000000 Spatial De-noiser count states and int.mask
	volatile unsigned int	DN_STATE1_SPA;      // 0x168 R - Spatial De-noiser count states
	volatile unsigned int	DN_CTRL;            // 0x16C R/W 0x00000000 De-noiser FIFO and coefficient ctrl
	volatile unsigned int	DN_DIV_IMG_SPA;     // 0x170 R/W 0x00000168 Spatial De-noiser image divide
	volatile unsigned int   NOTDEFINE6[3];

	volatile unsigned int	RD_IMG0_BASE0;      // 0x180 R/W 0x00000000 RDMA image #0 base address in Y channel
	volatile unsigned int	RD_IMG0_BASE1;      // 0x184 R/W 0x00000000 RDMA image #0 base address in U channel
	volatile unsigned int	RD_IMG0_BASE2;      // 0x188 R/W 0x00000000 RDMA image #0 base address in V channel
	volatile unsigned int	RD_IMG0_OFS;        // 0x18C R/W 0x00000000 RDMA image #0 address offset
	volatile unsigned int	RD_IMG1_BASE0;      // 0x190 R/W 0x00000000 RDMA image #1 base address in Y channel
	volatile unsigned int	RD_IMG1_BASE1;      // 0x194 R/W 0x00000000 RDMA image #1 base address in U channel
	volatile unsigned int	RD_IMG1_BASE2;      // 0x198 R/W 0x00000000 RDMA image #1 base address in V channel
	volatile unsigned int	RD_IMG1_OFS;        // 0x19C R/W 0x00000000 RDMA image #1 address offset
	volatile unsigned int	RD_IMG2_BASE0_0;    // 0x1A0 R/W 0x00000000 RDMA decomp. data #0 base address in Y channel
	volatile unsigned int	RD_IMG2_BASE1_0;    // 0x1A4 R/W 0x00000000 RDMA decomp. data #0 base address in U channel
	volatile unsigned int	RD_IMG2_BASE2_0;    // 0x1A8 R/W 0x00000000 RDMA decomp. data #0 base address in V channel
	volatile unsigned int	RD_IMG2_BASE0_1;    // 0x1AC R/W 0x00000000 RDMA decomp. data #1 base address in Y channel
	volatile unsigned int	RD_IMG2_BASE1_1;    // 0x1B0 R/W 0x00000000 RDMA decomp. data #1 base address in U channel
	volatile unsigned int	RD_IMG2_BASE2_1;    // 0x1B4 R/W 0x00000000 RDMA decomp. data #1 base address in V channel
	volatile unsigned int	RD_CUR_ADDR0;       // 0x1B8 R - RDMA image #0 current address
	volatile unsigned int	RD_CUR_ADDR1;       // 0x1BC R - RDMA image #1 current address
	volatile unsigned int	RD_CUR_ADDR2;       // 0x1C0 R - RDMA decomp. data current address
	volatile unsigned int	RD_FIFOSTATE;       // 0x1C4 R/W 0x00000000 RDMA FIFO States
	volatile unsigned int	RD_LINE_STATE0;     // 0x1C8 R - RDMA count states #0
	volatile unsigned int	RD_LINE_STATE1;     // 0x1CC R/W 0x00000000 RDMA count states #1
	volatile unsigned int	RD_CTRL;            // 0x1D0 R/W 0x00000000 RDMA control register
	volatile unsigned int	RD_COMP_PL0;        // 0x1D4 R/W 0x00000000 RDMA decomp. data number in Y channel
	volatile unsigned int	RD_COMP_PL1;        // 0x1D8 R/W 0x00000000 RDMA decomp. data number in C channel
	volatile unsigned int   NOTDEFINE7[9];

	volatile unsigned int	CD_BASE0_0;         // 0x200 R/W 0x00000000 Comp. DMA #0 base address in Y channel
	volatile unsigned int	CD_BASE1_0;         // 0x204 R/W 0x00000000 Comp. DMA #0 base address in U channel
	volatile unsigned int	CD_BASE2_0;         // 0x208 R/W 0x00000000 Comp. DMA #0 base address in V channel
	volatile unsigned int	CD_BASE0_1;         // 0x20C R/W 0x00000000 Comp. DMA #1 base address in Y channel
	volatile unsigned int	CD_BASE1_1;         // 0x210 R/W 0x00000000 Comp. DMA #1 base address in U channel
	volatile unsigned int	CD_BASE2_1;         // 0x214 R/W 0x00000000 Comp. DMA #1 base address in V channel
	volatile unsigned int	CD_CUR_ADDR;        // 0x218 R - Comp. DMA current address
	volatile unsigned int	CD_STATE;           // 0x21C R/W 0x00000000 Comp. DMA states
	volatile unsigned int	CD_CTRL;            // 0x220 R/W 0x00000000 Comp. DMA control register
	volatile unsigned int   NOTDEFINE8[3];
	volatile unsigned int	CD_HUFF_CNT0;       // 0x230 R - Comp. DMA compressed data count in Y channel
	volatile unsigned int	CD_HUFF_CNT1;       // 0x234 R - Comp. DMA compressed data count in U channel
	volatile unsigned int	CD_HUFF_CNT2;       // 0x238 R Comp. DMA compressed data count in V channel
    volatile unsigned int   NOTDEFINE9[17];

	volatile unsigned int	OD_BASE0;           // 0x280 R/W 0x00000000 ODMA base address in Y channel
	volatile unsigned int	OD_BASE1;           // 0x284 R/W 0x00000000 ODMA base address in U channel
	volatile unsigned int	OD_BASE2;           // 0x288 R/W 0x00000000 ODMA base address in V channel
	volatile unsigned int	OD_SIZE;            // 0x28C R/W 0x00000000 ODMA image size
	volatile unsigned int	OD_OFS;             // 0x290 R/W 0x00000000 ODMA address offset
	volatile unsigned int	OD_CFG;             // 0x294 R/W 0x00000000 ODMA image type
	volatile unsigned int   NOTDEFINE10[3];
	volatile unsigned int	OD_CTRL;            // 0x2A4 R/W 0x00000000 ODMA control register
	volatile unsigned int	OD_STATE;           // 0x2A8 R/W 0x00000000 ODMA States
	volatile unsigned int   NOTDEFINE11[85];

	volatile unsigned int	GM_CTRL;            // 0x400 R/W 0x00000000 Gamut-mapper Control Register
	volatile unsigned int	GM_STATUS;          // 0x404 R/W 0x00000000 Gamut-mapper Status Register
	volatile unsigned int	GM_REGION0;         // 0x408 R/W 0x00000000 Gamut-mapper Region 0 Register
	volatile unsigned int	GM_REGION1;         // 0x40C R/W 0x00000000 Gamut-mapper Region 1 register
	volatile unsigned int   NOTDEFINE12[3];
	volatile unsigned int	GM_INT;             // 0x41C R/W 0x00000000 Gamut-mapper Interrupt Register
    volatile unsigned int   NOTDEFINE13[120];

	volatile unsigned int	HI_CTRL;            // 0x600 R/W 0x00000000 Histogram Control Register
	volatile unsigned int	HI_STATUS;          // 0x604 R/W 0x00000000 Histogram Status Register
	volatile unsigned int	HI_CONFIG;          // 0x608 R/W 0x00000000 Histogram Configuration Register
	volatile unsigned int	HI_REGION0;         // 0x60C R/W 0x00000000 Histogram Region 0 Register
	volatile unsigned int	HI_REGION1;         // 0x610 R/W 0x00000000 Histogram Region 1 register
	volatile unsigned int   NOTDEFINE14[2];
	volatile unsigned int	HI_INT;             // 0x61C R/W 0x00000000 Histogram Interrupt Register
	volatile unsigned int	HI_SEGS[4];         // 0x620 ~ 0x62C R/W 0x00000000 Histogram Segments Register
	volatile unsigned int	HI_CDFS[4];         // 0x630 ~ 0x63C R 0x00000000 Histogram CDF Register
	volatile unsigned int	HI_CNTS[8];         // 0x640 ~ 0x65C R 0x00000000 Histogram CNT Register
	volatile unsigned int	HI_SCALE[4];        // 0x660 ~ 0x66C R/W 0x00000000 Histogram Scale Register
	volatile unsigned int	HI_LUTS[64];        // 0x700 ~ 0x7FC R/W 0x00000000 Histogram LUT Table register
}VIQE, *PVIQE;



/***********************************************************************
*   11. DDI_CONFIG/DDI_CACHE Register Define   	(Base Addr = 0xF0251000)
************************************************************************/
//#define HwDDI_CONFIG_BASE               *(volatile unsigned long *)0xF0251000
//#define HwDDI_CACHE_BASE                *(volatile unsigned long *)0xF0250000

typedef struct _DDICONFIG{
	volatile unsigned int	NTSCPAL_SEL;        // 0x000 R/W 0x00000001 NTSCPAL_Encoder select
	volatile unsigned int	LVDS_CTRL;          // 0x004 R/W 0x04444443 LVDS Control register
	volatile unsigned int	LVDS_TXO_SEL0;      // 0x008 R/W 0x03020100 LVDS TXOUT select #0
	volatile unsigned int	LVDS_TXO_SEL1;      // 0x00C R/W 0x09080504 LVDS TXOUT select #1
	volatile unsigned int	LVDS_TXO_SEL2;      // 0x010 R/W 0x0D0C0B0A LVDS TXOUT select #2
	volatile unsigned int	LVDS_TXO_SEL3;      // 0x014 R/W 0x13121110 LVDS TXOUT select #3
	volatile unsigned int	LVDS_TXO_SEL4;      // 0x018 R/W 0x1A191514 LVDS TXOUT select #4
	volatile unsigned int	LVDS_TXO_SEL5;      // 0x01C R/W 0x0E070618 LVDS TXOUT select #5
	volatile unsigned int	LVDS_TXO_SEL6;      // 0x020 R/W 0x1B17160F LVDS TXOUT select #6
	volatile unsigned int	LVDS_TXO_SEL7;      // 0x024 R/W 0x1F1E1F1E LVDS TXOUT select #7
	volatile unsigned int	LVDS_TXO_SEL8;      // 0x028 R/W 0x001E1F1E LVDS TXOUT select #8
	volatile unsigned int	HDMI_CTRL;          // 0x02C R/W 0x00000002 HDMI Control register
	volatile unsigned int	PWDN;               // 0x030 R/W 0x00000000 Power Down
	volatile unsigned int	SWRESET;            // 0x034 R/W 0x00000000 Soft Reset
	volatile unsigned int	ON_THE_FLY;         // 0x038 R/W 0x00000000 On-The-Fly mode
	volatile unsigned int   NOTDEFINE0;
	volatile unsigned int	HDMI_AES;           // 0x040 R/W 0x00000000 HDMI AES
	volatile unsigned int	HDMI_AES_DATA0;     // 0x044 RW 0x00000000 HDMI AES DATA #0
	volatile unsigned int	HDMI_AES_DATA1;     // 0x048 R/W 0x00000000 HDMI AES DATA #1
	volatile unsigned int	HDMI_AES_HW0;       // 0x050 R/W 0x00000000 HDMI AES HW #0
	volatile unsigned int	HDMI_AES_HW1;       // 0x054 R/W 0x00000000 HDMI AES HW #1
	volatile unsigned int	HDMI_AES_HW2;       // 0x058 R/W 0x00000000 HDMI AES HW #2
}DDICONFIG, *PDDICONFIG;


// Where is DDI_CONGIF in DataSheet ??

typedef struct _DDICACHE{
	volatile unsigned int	DDIC_CTRL;          //  0x000 R/W 0x00000000 DDI_CACHE Control
	volatile unsigned int	DDIC_CFG0;          //  0x004 R/W 0x00000000 DDI_CACHE Configuration #0
	volatile unsigned int	DDIC_CFG1;          //  0x008 R/W 0x00000000 DDI_CACHE Configuration #1    
}DDICACHE, *PDDICACHE;


/*******************************************************************************
*	 TCC8900_DataSheet_PART 7 VIDEO BUS_V0.00 Dec.11 2008
********************************************************************************/
/***********************************************************************
*   4. VIDEO CODEC Register Define                	(Base Addr = 0x0xF0700000)
************************************************************************/
//#define HwVIDEOCODEC_BASE                           *(volatile unsigned long*)0xF0700000

/*******************************************************************************
*	 TCC8900_DataSheet_PART 7 VIDEO BUS_V0.00 Dec.11 2008
********************************************************************************/
/*******************************************************************************
*	 5-1. JPEG Encoder Register Define   (Base Addr = 0xF0720000)
********************************************************************************/
//#define HwJPEGDECODER_BASE                          *(volatile unsigned long*)0xF0710000
//#define HwJPEGENCODER_BASE                          *(volatile unsigned long*)0xF0720000
//#define HwVIDEOCACHE_BASE                           *(volatile unsigned long*)0xF0701000
typedef struct _JPEGENCODER{
    volatile unsigned int  NOTDEFINE0;              // 0x000
    volatile unsigned int  JP_MOD;                  //  0x004 R/W ALL 0x00000000 JPEG codec mode register
    volatile unsigned int  JP_INT_MASK;             //  0x008 R/W ALL 0x0000001f Interrupt mask register
    volatile unsigned int  JP_INT_LEVEL;            //  0x00c R/W SLV 0x000000ff FIFO interrupt level register
    volatile unsigned int  JP_TRG_MOD;              //  0x010 R/W ALL 0x00000000 Polling or Interrupt mode selection register
    volatile unsigned int  NOTDEFINE1[3];
    volatile unsigned int  R_YBUF_ADDR;             //  0x020 R/W JP 0x00000000 Raw data buffer Y address register
    volatile unsigned int  R_UBUF_ADDR;             //  0x024 R/W JP 0x00000000 Raw data buffer U address register
    volatile unsigned int  R_VBUF_ADDR;             //  0x028 R/W JP 0x00000000 Raw data V address register
    volatile unsigned int  R_BUF_INFO;              //  0x02c R/W JP 0x00000000 Raw data buffer information register
    volatile unsigned int  JP_SIZE;                 //  0x030 R/W JP 0x00000000 Image size information register
    volatile unsigned int  JP_CHROMA;               //  0x034 R/W JP 0x00000000 Image format information register
    volatile unsigned int  JP_CBUF_ADDR;            //  0x38 R/W JP 0x00000000 Coded data buffer address register
    volatile unsigned int  JP_CBUF_SIZE;            //  0x03c R/W JP 0x00000fff Coded data buffer size register
    volatile unsigned int  NOTDEFINE2[12];
    volatile unsigned int  JP_START;                //  0x070 W ALL 0x00000000 Codec start command register
    volatile unsigned int  NOTDEFINE3[3];
    volatile unsigned int  JP_SBUF_WCNT;            //  0x080 R/W MST 0x00000000 Source buffer write count register
    volatile unsigned int  JP_SBUF_RCNT;            //  0x084 R MST 0x00000000 Source buffer read count register
    volatile unsigned int  JP_DBUF_WCNT;            //  0x088 R MST 0x00000000 Destination buffer write count register
    volatile unsigned int  JP_DBUF_RCNT;            //  0x08c R/W MST 0x00000000 Destination buffer read count register
    volatile unsigned int  JP_IFIFO_ST;             //  0x090 R SLV 0x00000000 Input FIFO status register
    volatile unsigned int  JP_OFIFO_ST;             //  0x094 R SLV 0x00000000 Output FIFO status register
    volatile unsigned int  NOTDEFINE4[2];
    volatile unsigned int  JP_INT_FLAG;             //  0x0a0 R ALL 0x00000000 Interrupt flag register
    volatile unsigned int  JP_INT_ACK;              //  0x0a4 R ALL 0x00000000 Interrupt ack register
    volatile unsigned int  NOTDEFINE5[6];
    volatile unsigned int  JP_IFIFO_WD;             //  0x0c0 W SLV 0x00000000 Input FIFO write data register
    volatile unsigned int  NOTDEFINE6[7];
    volatile unsigned int  JP_OFIFO_RD;             //  0x0e0 R SLV 0x00000000 Output FIFO read data register
    volatile unsigned int  NOTDEFINE7[7];
    volatile unsigned int  JPC_QTAB0[64];           //  0x100 - W JPC 0x00000000 Encoder Q table 0 (64 entries)
    volatile unsigned int  JPC_QTAB1[64];           //  0x200 - W JPC 0x00000000 Encoder Q table 1 (64 entries)
}JPEGENCODER, *PJPEGENCODER;

/*******************************************************************************
*	 5-2. JPEG Decoder Register Define   (Base Addr = 0xF0710000)
********************************************************************************/
typedef struct _JPEGDECODER{
    volatile unsigned int  NOTDEFINE0;              // 0x000
    volatile unsigned int  JP_MOD;                  // 0x004 R/W ALL 0x00000000 JPEG codec mode register
    volatile unsigned int  JP_INT_MASK;             // 0x008 R/W ALL 0x0000001f Interrupt mask register
    volatile unsigned int  JP_INT_LEVEL;            // 0x00c R/W SLV 0x000000ff FIFO interrupt level register
    volatile unsigned int  JP_TRG_MOD;              // 0x010 R/W ALL 0x00000000 Polling or Interrupt mode selection register
    volatile unsigned int  R_YBUF_ADDR;             // 0x020 R/W JP 0x00000000 Raw data buffer Y address register
    volatile unsigned int  R_UBUF_ADDR;             // 0x024 R/W JP 0x00000000 Raw data buffer U address register
    volatile unsigned int  R_VBUF_ADDR;             // 0x028 R/W JP 0x00000000 Raw data V address register
    volatile unsigned int  R_BUF_INFO;              // 0x02c R/W JP 0x00000000 Raw data buffer information register
    volatile unsigned int  JP_SIZE;                 // 0x030 R/W JP 0x00000000 Image size information register
    volatile unsigned int  JP_CHROMA;               // 0x034 R/W JP 0x00000000 Image format information register
    volatile unsigned int  JP_CBUF_ADDR;            // 0x38 R/W JP 0x00000000 Coded data buffer address register
    volatile unsigned int  JP_CBUF_SIZE;            // 0x03c R/W JP 0x00000fff Coded data buffer size register
    volatile unsigned int  NOTDEFINE1[4];
    volatile unsigned int  JPD_TBL_ID;              // 0x050 R/W JPD 0x00000000 Decoder table index register
    volatile unsigned int  JPD_RST_INTV;            // 0x054 R/W JPD 0x00000000 Decoder reset interval register
    volatile unsigned int  JPD_OUT_SCL;             // 0x058 R/W JPD 0x00000000 Decoder output scaling register
    volatile unsigned int  NOTDEFINE2[5];
    volatile unsigned int  JP_START;                // 0x070 W ALL 0x00000000 Codec start command register
    volatile unsigned int  NOTDEFINE3[3];
    volatile unsigned int  JP_SBUF_WCNT;            // 0x080 R/W MST 0x00000000 Source buffer write count register
    volatile unsigned int  JP_SBUF_RCNT;            // 0x084 R MST 0x00000000 Source buffer read count register
    volatile unsigned int  JP_DBUF_WCNT;            // 0x088 R MST 0x00000000 Destination buffer write count register
    volatile unsigned int  JP_DBUF_RCNT;            // 0x08c R/W MST 0x00000000 Destination buffer read count register
    volatile unsigned int  JP_IFIFO_ST;             // 0x090 R SLV 0x00000000 Input FIFO status register
    volatile unsigned int  JP_OFIFO_ST;             // 0x094 R SLV 0x00000000 Output FIFO status register
    volatile unsigned int  NOTDEFINE4[2];
    volatile unsigned int  JP_INT_FLAG;             // 0x0a0 R ALL 0x00000000 Interrupt flag register
    volatile unsigned int  JP_INT_ACK;              // 0x0a4 R ALL 0x00000000 Interrupt ack register
    volatile unsigned int  NOTDEFINE5[6];
    volatile unsigned int  JP_IFIFO_WD;             // 0x0c0 W SLV 0x00000000 Input FIFO write data register
    volatile unsigned int  NOTDEFINE6[7];
    volatile unsigned int  JP_OFIFO_RD;             // 0x0e0 R SLV 0x00000000 Output FIFO read data register
    volatile unsigned int  NOTDEFINE7[135];
    volatile unsigned int  JPD_IQTAB0[64];          // 0x300 - W JPD 0x00000000 Decoder IQ table 0 (64 entries)
    volatile unsigned int  JPD_IQTAB1[64];          // 0x400 - W JPD 0x00000000 Decoder IQ table 1 (64 entires)
    volatile unsigned int  JPD_IQTAB2[64];          // 0x500 - W JPD 0x00000000 Decoder IQ table 2 (64 entires)
    volatile unsigned int  JPD_HT_DC0_C[16];        // 0x600 - W JPD 0x00000000 Decoder huffman table (dc0 code, 16 entreis)
    volatile unsigned int  JPD_HT_AC0_C[16];        // 0x640 - W JPD 0x00000000 Decoder huffman table (ac0 code, 16 entreis)
    volatile unsigned int  JPD_HT_DC1_C[16];        // 0x680 - W JPD 0x00000000 Decoder huffman table (dc1 code, 16 entreis)
    volatile unsigned int  JPD_HT_AC1_C[16];        // 0x6c0 - W JPD 0x00000000 Decoder huffman table (ac1 code, 16 entreis)
    volatile unsigned int  JPD_HT_DC0_A[16];        // 0x700 - W JPD 0x00000000 Decoder huffman table (dc0 addr, 16 entreis)
    volatile unsigned int  JPD_HT_AC0_A[16];        // 0x740 - W JPD 0x00000000 Decoder huffman table (ac0 addr, 16 entreis)
    volatile unsigned int  JPD_HT_DC1_A[16];        // 0x780 - W JPD 0x00000000 Decoder huffman table (dc1 addr, 16 entreis)
    volatile unsigned int  JPD_HT_AC1_A[16];        // 0x7c0 - W JPD 0x00000000 Decoder huffman table (ac1 addr, 16 entreis)
    volatile unsigned int  JPD_HT_DC0_V[12];        // 0x800 - W JPD 0x00000000 Decoder huffman table (dc0 var, 12 entreis)
    volatile unsigned int  JPD_HT_AC0_V[162];       // 0x840 - W JPD 0x00000000 Decoder huffman table (ac0 var, 162 entreis)
    volatile unsigned int  NOTDEFINE8[78];
    volatile unsigned int  JPD_HT_DC1_V[12];        // 0xc00 - W JPD 0x00000000 Decoder huffman table (dc1 var, 12 entreis)
    volatile unsigned int  NOTDEFINE9[4];
    volatile unsigned int  JPD_HT_AC1_V[162];       // 0xc40 - W JPD 0x00000000 Decoder huffman table (ac1 var, 162 entreis)
}JPEGDECODER, *PJPEGDECODER;


/*******************************************************************************
*	 TCC8900_DataSheet_PART 8 GRAPHIC BUS_V0.00 Dec.11 2008
********************************************************************************/
/***********************************************************************
*	 4. Overlay Mixer Register Define	(Base Addr = 0xF6000000)
************************************************************************/
//#define HwOVERLAYMIXER_BASE                     *(volatile unsigned long *)0xF6000000
typedef struct _OVERLAYMIXER{
	volatile unsigned int	FCH0_SADDR0;        // 0x00 R/W 0x00000000 Front-End Channel 0 Source Address 0
	volatile unsigned int	FCH0_SADDR1;        // 0x04 R/W 0x00000000 Front-End Channel 0 Source Address 1
	volatile unsigned int	FCH0_SADDR2;        // 0x08 R/W 0x00000000 Front-End Channel 0 Source Address 2
	volatile unsigned int	FCH0_SFSIZE;        // 0x0C R/W 0x00000000 Front-End Channel 0 Source Frame Pixel Size
	volatile unsigned int	FCH0_SOFF;          // 0x10 R/W 0x00000000 Front-End Channel 0 Source Pixel Offset
	volatile unsigned int	FCH0_SISIZE;        // 0x14 R/W 0x00000000 Front-End Channel 0 Source Image Pixel Size
	volatile unsigned int	FCH0_WOFF;          // 0x18 R/W 0x00000000 Front-End Channel 0 Window Pixel Offset
	volatile unsigned int	FCH0_SCTRL;         // 0x1C R/W 0x00000000 Front-End Channel 0 Control
	volatile unsigned int	FCH1_SADDR0;        // 0x20 R/W 0x00000000 Front-End Channel 1 Source Address 0
	volatile unsigned int	FCH1_SADDR1;        // 0x24 R/W 0x00000000 Front-End Channel 1 Source Address 1
	volatile unsigned int	FCH1_SADDR2;        // 0x28 R/W 0x00000000 Front-End Channel 1 Source Address 2
	volatile unsigned int	FCH1_SFSIZE;        // 0x2C R/W 0x00000000 Front-End Channel 1 Source Frame Pixel Size
	volatile unsigned int	FCH1_SOFF;          // 0x30 R/W 0x00000000 Front-End Channel 1 Source Pixel Offset
	volatile unsigned int	FCH1_SISIZE;        // 0x34 R/W 0x00000000 Front-End Channel 1 Source Image Pixel Size
	volatile unsigned int	FCH1_WOFF;          // 0x38 R/W 0x00000000 Front-End Channel 1 Window Pixel Offset
	volatile unsigned int	FCH1_SCTRL;         // 0x3C R/W 0x00000000 Front-End Channel 1 Control
	volatile unsigned int	FCH2_SADDR0;        // 0x40 R/W 0x00000000 Front-End Channel 1 Source Address 0
	volatile unsigned int	FCH2_SADDR1;        // 0x44 R/W 0x00000000 Front-End Channel 1 Source Address 1
	volatile unsigned int	FCH2_SADDR2;        // 0x48 R/W 0x00000000 Front-End Channel 1 Source Address 2
	volatile unsigned int	FCH2_SFSIZE;        // 0x4C R/W 0x00000000 Front-End Channel 1 Source Frame Pixel Size
	volatile unsigned int	FCH2_SOFF;          // 0x50 R/W 0x00000000 Front-End Channel 1 Source Pixel Offset
	volatile unsigned int	FCH2_SISIZE;        // 0x54 R/W 0x00000000 Front-End Channel 1 Source Image Pixel Size
	volatile unsigned int	FCH2_WOFF;          // 0x58 R/W 0x00000000 Front-End Channel 1 Window Pixel Offset
	volatile unsigned int	FCH2_SCTRL;         // 0x5C R/W 0x00000000 Front-End Channel 1 Control
	volatile unsigned int	S0_CHROMA;          // 0x60 R/W 0x00000000 Source 0 Chroma-Key Parameter
	volatile unsigned int	S0_PAR;             // 0x64 R/W 0x00000000 Source 0 Arithmetic Parameter
	volatile unsigned int	S1_CHROMA;          // 0x68 R/W 0x00000000 Source 1 Chroma-Key Parameter
	volatile unsigned int	S1_PAR;             // 0x6C R/W 0x00000000 Source 1 Arithmetic Parameter
	volatile unsigned int	S2_CHROMA;          // 0x70 R/W 0x00000000 Source 2 Chroma-Key Parameter
	volatile unsigned int	S2_PAR;             // 0x74 R/W 0x00000000 Source 2 Arithmetic Parameter
	volatile unsigned int	S_CTRL;             // 0x78 R/W 0x00000000 Source Control Register
	volatile unsigned int	NOTDEFINE0;         // 0x7C - - Reserved
	volatile unsigned int	OP0_PAT;            // 0x80 R/W 0x00000000 Source Operator 0 Pattern
	volatile unsigned int	OP1_PAT;            // 0x84 R/W 0x00000000 Source Operator 1 Pattern
	volatile unsigned int	OP_CTRL;            // 0x88 R/W 0x00000000 Source Operation Control Register
	volatile unsigned int	NOTDEFINE1;         // 0x8C - - Reserved
	volatile unsigned int	BCH_DADDR0;         // 0x90 R/W 0x00000000 Back-End Channel Destination Address 0
	volatile unsigned int	BCH_DADDR1;         // 0x94 R/W 0x00000000 Back -End Channel Destination Address 1
	volatile unsigned int	BCH_DADDR2;         // 0x98 R/W 0x00000000 Back -End Channel Destination Address 2
	volatile unsigned int	BCH_DFSIZE;         // 0x9C R/W 0x00000000 Back -End Channel Destination Frame Pixel Size
	volatile unsigned int	BCH_DOFF;           // 0xA0 R/W 0x00000000 Back -End Channel Destination Pixel Offset
	volatile unsigned int	BCH_DCTRL;          // 0xA4 R/W 0x00000000 Back -End Channel Control

	volatile unsigned int	NOTDEFINE2[2];      // 0xA8 - 0xAF - - Reserved
	volatile unsigned int	BCH_DDMAT0;         // 0xB0 R/W 0x00000000 Back-End Channel Destination Dither Matrix 0
	volatile unsigned int	BCH_DDMAT1;         // 0xB4 R/W 0x00000000 Back-End Channel Destination Dither Matrix 1
	volatile unsigned int	BCH_DDMAT2;         // 0xB8 R/W 0x00000000 Back-End Channel Destination Dither Matrix 2
	volatile unsigned int	BCH_DDMAT3;         // 0xBC R/W 0x00000000 Back-End Channel Destination Dither Matrix 3
	volatile unsigned int	OM_CTRL;            // 0xC0 R/W 0x00000000 Overlay Mixer Control
	volatile unsigned int	OM_IREQ;            // 0xC4 R/W 0x00000000 Overlay Mixer Interrupt Request
	volatile unsigned int	NOTDEFINE3[206];    // 0xC8 - 0x3FF - - Reserved
	
	volatile unsigned int	FCH0_LUT[256];      // 0x400 ? 0x7FF R/W - Front-End Channel 0 Lookup Table
	volatile unsigned int	FCH1_LUT[256];      // 0x800 ? 0xBFF R/W - Front-End Channel 1 Lookup Table
	volatile unsigned int	FCH2_LUT[256];      // 0xC00 ? 0xFFF R/W - Front-End Channel 2 Lookup Table
}OVERLAYMIXER, *POVERLAYMIXER;


/*******************************************************************************
*	 5-1. 2D/3D GPU
*
*	 Pixel Processor Register Map Register Define   (Base Addr = 0xF0000000)
********************************************************************************/
// #define HwPIXELPROCESSOR_BASE                   *(volatile unsigned long *)0xF0000000
typedef struct _GPUPIXELPROCESSOR{
	volatile unsigned int	REND_LIST_ADDR;              // 0x0000 R/W 0x00000000 Renderer List Address
	volatile unsigned int	REND_RSW_BASE;               // 0x0004 R/W 0x00000000 Renderer State Word Base Address
	volatile unsigned int	REND_VERTEX_BASE;            // 0x0008 R/W 0x00000000 Renderer Vertex Base Address
	volatile unsigned int	FEATURE_ENABLE;              // 0x000C R/W 0x00000002 Feature Enable
	volatile unsigned int	Z_CLEAR_VALUE;               // 0x0010 R/W 0x00000009 Z Clear Value
	volatile unsigned int	STENCIL_CLEAR_VALUE;         // 0x0014 R/W 0x00000000 Stencil Clear value
	volatile unsigned int	ABGR_CLEAR_VALUE_0;          // 0x0018 R/W 0x00000000 ABGR Clear Value 0
	volatile unsigned int	ABGR_CLEAR_VALUE_1;          // 0x001C R/W 0x00000000 ABGR Clear Value 1
	volatile unsigned int	ABGR_CLEAR_VALUE_2;          // 0x0020 R/W 0x00000000 ABGR Clear Value 2
	volatile unsigned int	ABGR_CLEAR_VALUE_3;          // 0x0024 R/W 0x00000000 ABGR Clear Value 3
	volatile unsigned int	BOUNDING_BOX_LEFT_RIGHT;     // 0x0028 R/W 0x00000000 Bounding Box left Right
	volatile unsigned int	BOUNDING_BOX_BOTTOM;         // 0x002C R/W 0x00000000 Bounding Box Bottom
	volatile unsigned int	FS_STACK_ADDR;               // 0x0030 R/W 0x00000000 FS Stack Address
	volatile unsigned int	FS_STACK_SIZE_AND_INIT_VAL;  // 0x0034 R/W 0x00000000 FS Stack Size and Initial Value
	volatile unsigned int	ORIGIN_OFFSET_X;             // 0x0040 R/W 0x00000000 Origin Offset X
	volatile unsigned int	ORIGIN_OFFSET_Y;             // 0x0044 R/W 0x00000000 Origin Offset Y
	volatile unsigned int	SUBPIXEL_SPECIFIER;          // 0x0048 R/W 0x00000075 Subpixel Specifier
	volatile unsigned int	TIEBREAK_MODE;               // 0x004C R/W 0x00000000 Tiebreak mode Register
	volatile unsigned int	NOTDEFINE0[44];
	volatile unsigned int	WB0_SOURCE_SELECT;           // 0x0100 R/W 0x00000000 WB0 Source Select
	volatile unsigned int	WB0_TARGET_ADDR;             // 0x0104 R/W 0x00000000 WB0 Target Addres
	volatile unsigned int	WB0_TARGET_PIXEL_FORMAT;     // 0x0108 R/W 0x00000000 WB0 Target Pixel Format
	volatile unsigned int	WB0_TARGET_AA_FORMAT;        // 0x010C R/W 0x00000000 WB0 Target AA Format
	volatile unsigned int	WB0_TARGET_LAYOUT;           // 0x0110 R/W 0x00000000 WB0 Target Layout
	volatile unsigned int	WB0_TARGET_SCANLINE_LENGTH;  // 0x0114 R/W 0x00000000 WB0 Target Scanline length
	volatile unsigned int	WB0_TARGET_FLAGS;            // 0x0118 R/W 0x00000000 WB0 Target Flags
	volatile unsigned int	WB0_MRT_ENABLE;              // 0x011C R/W 0x00000000 WB0 MRT Enagle
	volatile unsigned int	WB0_MRT_OFFSET;              // 0x0120 R/W 0x00000000 WB0 MRT Offset
	volatile unsigned int	WB0_GLOBAL_TEST_ENABLE;      // 0x0124 R/W 0x00000000 WB0 Global Test Enable
	volatile unsigned int	WB0_GLOBAL_TEST_REF_VALUE;   // 0x0128 R/W 0x00000000 WB0 Global Test Reference
	volatile unsigned int	WB0_GLOBAL_TEST_CMP_FUNC;    // 0x012C R/W 0x00000000 WB0 Global Test Compare Function
	volatile unsigned int	NOTDEFINE1[52];
	volatile unsigned int	WB1_SOURCE_SELECT;           // 0x0200 R/W 0x00000000 WB1 Source Select
	volatile unsigned int	WB1_TARGET_ADDR;             // 0x0204 R/W 0x00000000 WB1 Target Addres
	volatile unsigned int	WB1_TARGET_PIXEL_FORMAT;     // 0x0208 R/W 0x00000000 WB1 Target Pixel Format
	volatile unsigned int	WB1_TARGET_AA_FORMAT;        // 0x020C R/W 0x00000000 WB1 Target AA Format
	volatile unsigned int	WB1_TARGET_LAYOUT;           // 0x0210 R/W 0x00000000 WB1 Target Layout
	volatile unsigned int	WB1_TARGET_SCANLINE_LENGTH;  // 0x0214 R/W 0x00000000 WB1 Target Scanline length
	volatile unsigned int	WB1_TARGET_FLAGS;            // 0x0218 R/W 0x00000000 WB1 Target Flags
	volatile unsigned int	WB1_MRT_ENABLE;              // 0x021C R/W 0x00000000 WB1 MRT Enagle
	volatile unsigned int	WB1_MRT_OFFSET;              // 0x0220 R/W 0x00000000 WB1 MRT Offset
	volatile unsigned int	WB1_GLOBAL_TEST_ENABLE;      // 0x0224 R/W 0x00000000 WB1 Global Test Enable
	volatile unsigned int	WB1_GLOBAL_TEST_REF_VALUE;   // 0x0228 R/W 0x00000000 WB1 Global Test Reference
	volatile unsigned int	WB1_GLOBAL_TEST_CMP_FUNC;    // 0x022C R/W 0x00000000 WB1 Global Test Compare Function
	volatile unsigned int	NOTDEFINE2[52];
	volatile unsigned int	WB2_SOURCE_SELECT;           // 0x0300 R/W 0x00000000 WB2 Source Select
	volatile unsigned int	WB2_TARGET_ADDR;             // 0x0304 R/W 0x00000000 WB2 Target Addres
	volatile unsigned int	WB2_TARGET_PIXEL_FORMAT;     // 0x0308 R/W 0x00000000 WB2 Target Pixel Format
	volatile unsigned int	WB2_TARGET_AA_FORMAT;        // 0x030C R/W 0x00000000 WB2 Target AA Format
	volatile unsigned int	WB2_TARGET_LAYOUT;           // 0x0310 R/W 0x00000000 WB2 Target Layout
	volatile unsigned int	WB2_TARGET_SCANLINE_LENGTH;  // 0x0314 R/W 0x00000000 WB2 Target Scanline length
	volatile unsigned int	WB2_TARGET_FLAGS;            // 0x0318 R/W 0x00000000 WB2 Target Flags
	volatile unsigned int	WB2_MRT_ENABLE;              // 0x031C R/W 0x00000000 WB2 MRT Enagle
	volatile unsigned int	WB2_MRT_OFFSET;              // 0x0320 R/W 0x00000000 WB2 MRT Offset
	volatile unsigned int	WB2_GLOBAL_TEST_ENABLE;      // 0x0324 R/W 0x00000000 WB2 Global Test Enable
	volatile unsigned int	WB2_GLOBAL_TEST_REF_VALUE;   // 0x0328 R/W 0x00000000 WB2 Global Test Reference
	volatile unsigned int	WB2_GLOBAL_TEST_CMP_FUNC;    // 0x032C R/W 0x00000000 WB2 Global Test Compare Function
	volatile unsigned int	NOTDEFINE3[820];
	volatile unsigned int	VERSION;                     // 0x1000 R 0xC8070005 Version
	volatile unsigned int	CURRENT_REND_LIST_ADDR;      // 0x1004 R/W 0x00000000 Current Renderer List Address
	volatile unsigned int	STATUS;                      // 0x1008 R/W 0x00000000 Status
	volatile unsigned int	CTRL_MGMT;                   // 0x100C W N/A Control Management
	volatile unsigned int	INT_RAWSTAT;                 // 0x1020 R/W 0x00000000 Interrupt Rawstat
	volatile unsigned int	INT_CLEAR;                   // 0x1024 W N/A Interrupt Clear
	volatile unsigned int	INT_MASK;                    // 0x1028 RW 0x00001FF Interrupt Mask
	volatile unsigned int	INT_STATUS;                  // 0x102c R 0x00000000 Interrupt Status
	volatile unsigned int	WRITE_BOUNDARY_ENABLE;       // 0x1040 R/W 0x00000000 Write Boundary Enable
	volatile unsigned int	WRITE_BOUNDARY_LOW;          // 0x1044 R/W 0x00000000 Write Boundary Low
	volatile unsigned int	WRITE_BOUNDARY_HIGH;         // 0x1048 R/W 0x00000000 Write Boundary High
	volatile unsigned int	WRITE_BOUNDARY_ADDRESS;      // 0x104C R 0x00000000 Write Boundary Address
	volatile unsigned int	BUS_ERROR_STATUS;            // 0x1050 R 0x00000000 Bus Error Status
	volatile unsigned int	WATCHDOG_DISABLE;            // 0x1060 R/W 0x00000000 Watchdog Disable
	volatile unsigned int	WATCHDOG_TIMEOUT;            // 0x1064 R/W 0x000F4240 Watchdog Timeout
	volatile unsigned int	PERF_CNT_0_ENABLE;           // 0x1080 R/W 0x00000000 Performance Counter 0 Enable
	volatile unsigned int	PERF_CNT_0_SRC;              // 0x1084 R/W 0x00000000 Performance Counter 0 SRC
	volatile unsigned int	PERF_CNT_0_LIMIT;            // 0x1088 R/W 0x00000000 Performance Counter 0 Limit
	volatile unsigned int	PERF_CNT_0_VALUE;            // 0x108C R/W 0x00000000 Performance Counter 0 Value
	volatile unsigned int	PERF_CNT_1_ENABLE;           // 0x10A0 R/W 0x00000000 Performance Counter 1 Enable
	volatile unsigned int	PERF_CNT_1_SRC;              // 0x10A4 R/W 0x00000000 Performance Counter 1 SRC
	volatile unsigned int	PERF_CNT_1_LIMIT;            // 0x10A8 R/W 0x00000000 Performance Counter 1 Limit
	volatile unsigned int	PERF_CNT_1_VALUE;            // 0x10AC R/W 0x00000000 Performance Counter 1 Value
}GPUPIXELPROCESSOR,*PGPUPIXELPROCESSOR;

/*******************************************************************************
*	 5-2. Geometry Processor Register Map Register Define   (Base Addr = 0xF0000000)
********************************************************************************/
//#define HwGEOMETRYPROCESSOR_BASE                   *(volatile unsigned long *)0xF0002000
typedef struct _GPUGEOMETRYPROCESSOR{
	volatile unsigned int	CONTR_REG_VSCL_START_ADDR;      // 0x2000 R/W 0x00000000 Control Register VSCL Start Address
	volatile unsigned int	CONTR_REG_VSCL_END_ADDR;        // 0x2004 R/W 0x00000000 Control Register VSCL End Address
	volatile unsigned int	CONTR_REG_PLBCL_START_ADDR;     // 0x2008 R/W 0x00000000 Control Register PLBCL Start Address
	volatile unsigned int	CONTR_REG_PLBCL_END_ADDR;       // 0x200C R/W 0x00000000 Control Register PLBCL End Address
	volatile unsigned int	CONTR_REG_PLB_ALLOC_START_ADDR; // 0x2010 R/W 0x00000000 Control Register PLB Allocate Start Address
	volatile unsigned int	CONTR_REG_PLB_ALLOC_END_ADDR;   // 0x2014 R/W 0x00000000 Control Register PLB Allocate End Address
	volatile unsigned int	CONTR_REG_CMD;                  // 0x2020 W N/A Control Register Command
	volatile unsigned int	CONTR_REG_INT_RAWSTAT;          // 0x2024 R/W 0x00000000 Control Register Interrupt Rawstat
	volatile unsigned int	CONTR_REG_INT_CLEAR;            // 0x2028 W N/A Control Register Interrupt Clear
	volatile unsigned int	CONTR_REG_INT_MASK;             // 0x202C R/W 0x00000000 Control Register Interrupt Mask
	volatile unsigned int	CONTR_REG_INT_STAT;             // 0x2030 R 0x00000000 Control Register Interrupt Status
	volatile unsigned int	CONTR_REG_WRITE_BOUND_LOW;      // 0x2034 R/W 0x00000000 Control Register Write Boundary Low
	volatile unsigned int	CONTR_REG_WRITE_BOUND_HIGH;     // 0x2038 R/W 0xFFFFFF00 Control Register Write Boundary High
	volatile unsigned int	CONTR_REG_PERF_CNT_0_ENABLE;    // 0x203C R/W 0x00000000 Control Register Performance Counter 0 Enable
	volatile unsigned int	CONTR_REG_PERF_CNT_1_ENABLE;    // 0x2040 R/W 0x00000000 Control Register Performance Counter 1 Enable
	volatile unsigned int	CONTR_REG_PERF_CNT_0_SRC;       // 0x2044 R/W 0x00000000 Control Register Performance Counter 0 Source
	volatile unsigned int	CONTR_REG_PERF_CNT_1_SRC;       // 0x2048 R/W 0x00000000 Control Register Performance Counter 1 Source
	volatile unsigned int	CONTR_REG_PERF_CNT_0_VAL;       // 0x204C R 0x00000000 Control Register Performance Counter 0 Value
	volatile unsigned int	CONTR_REG_PERF_CNT_1_VAL;       // 0x2050 R 0x00000000 Control Register Performance Counter 1 Value
	volatile unsigned int	CONTR_REG_PERF_CNT_0_LIMIT;     // 0x2054 R/W 0x00000000 Control Register Performance Counter 0 Limit
	volatile unsigned int	CONTR_REG_PERF_CNT_1_LIMIT;     // 0x2058 R/W 0x00000000 Control Register Performance Counter 1 Limit
	volatile unsigned int	CONTR_REG_STATUS;               // 0x2068 R 0x00000000 Control Register Status
	volatile unsigned int	CONTR_REG_VERSION;              // 0x206C R 0x0A070005 Control Register VERSION
	volatile unsigned int	CONTR_REG_VSCL_INITIAL_ADDR;    // 0x2080 R 0x00000000 Control Register VSCL Initial Address
	volatile unsigned int	CONTR_REG_PLBCL_INITIAL_ADDR;   // 0x2084 R 0x00000000 Control Register PLBCL Initial Address
	volatile unsigned int	CONTR_REG_WRITE_BOUNDARY_ERROR_ADDR;// 0x2088 R 0x00000000 Control Register Write Error Address
	volatile unsigned int	CONTR_REG_AXI_BUS_ERROR_STAT;   // 0x2094 R 0x00000000 Control AXI Bus Error Status
	volatile unsigned int	CONTR_REG_WATCHDOG_DISABLE;     // 0x20A0 R/W 0x00000000 Control Register Watchdog Disable
	volatile unsigned int	CONTR_REG_WATCHDOG_TIMEOUT;     // 0x20A4 R/W 0x000F4240 Control Register Watchdog Timeout
}GPUGEOMETRYPROCESSOR, *PGPUGEOMETRYPROCESSOR;


// MaliGP2 PLB Configuration Register Map
//#define HwPLBCFG_BASE                                       *(volatile unsigned long*)0xFFFFFFFF
typedef struct _GPUPLBCFG{	
	volatile unsigned char	PLB_CONF_REG_VERTEX_ARRAY_ADDR; // 0x0100 W 0x00000000 PLB Configuration Register Vertex Array Address
	volatile unsigned char	PLB_CONF_REG_INDEX_ARRAY_ADDR;  // 0x0101 W 0x00000000 PLB Configuration Register Index Array Address
	volatile unsigned char	PLB_CONF_REG_POINT_SIZE_ADDR;   // 0x0102 W 0x00000000 PLB Configuration Register Point Size Address
	volatile unsigned char	PLB_CONF_REG_HEAP_START_ADDR;   // 0x0103 W 0x00000000 PLB Configuration Register Heap Start Address
	volatile unsigned char	PLB_CONF_REG_HEAP_END_ADDR;     // 0x0104 W 0x00000000 PLB Configuration Register Heap End Address
	volatile unsigned char	PLB_CONF_REG_VIEWPORT_TO;      // 0x0105 W 0x00000000 PLB Configuration Register Viewport Top
	volatile unsigned char	PLB_CONF_REG_VIEWPORT_BOTTOM;   // 0x0106 W 0x00000000 PLB Configuration Register Viewport Bottom
	volatile unsigned char	PLB_CONF_REG_VIEWPORT_LEFT;     // 0x0107 W 0x00000000 PLB Configuration Register Viewport Left
	volatile unsigned char	PLB_CONF_REG_VIEWPORT_RIGHT;    // 0x0108 W 0x00000000 PLB Configuration Register Viewport Right
	volatile unsigned char	PLB_CONF_REG_SCREENSIZE;        // 0x0109 W 0x00000000 PLB Configuration Register Screen Size
	volatile unsigned char	PLB_CONF_REG_OFFSET_VERTEX_ARRAY;// 0x010A W 0x00000000 PLB Configuration Register Offset Vertex Array
	volatile unsigned char	PLB_CONF_REG_PARAMS;            // 0x010B W 0x00000000 PLB Configuration Register Parameters
	volatile unsigned char	PLB_CONF_REG_TILE_SIZE;         // 0x010C W 0x00000000 PLB Configuration Register Tile Size
	volatile unsigned char	PLB_CONF_REG_POchar_SIZE;        // 0x010D W 0x00000000 PLB Configuration Register Pochar Size
	volatile unsigned char	PLB_CONF_REG_Z_NEAR;            // 0x010E W 0x00000000 PLB Configuration Register Z Near
	volatile unsigned char	PLB_CONF_REG_Z_FAR;             // 0x010F W 0x3F800000 PLB Configuration Register Z Far
}GPUPLBCFG, *PGPUPLBCFG;


// MaliGP2 Vertex Shader Register Map
//#define HwGPUVERTEXSHADER_BASE                              *(volatile unsigned long*)0xFFFFFFFF
typedef union _VSCFGREG1{
	//volatile unsigned char	VS_CONF_REG_INP_ADDR[32];           // 0x0000-0x001E W 0x00000000 VS Configuration Register Input Address
	//volatile unsigned char	VS_CONF_REG_INP_SPEC[32];           // 0x0001-0x001F W 0x0000003F VS Configuration Register Input Specifier
    volatile unsigned char	ADDR[32];           // 0x0000-0x001E W 0x00000000 VS Configuration Register Input Address
	volatile unsigned char	SPEC[32];           // 0x0001-0x001F W 0x0000003F VS Configuration Register Input Specifier
}VSCFGREG;	
/*
typedef union _VSCFGREG2{
    volatile unsigned char	ADDR[32];          // 0x0020-0x003E W 0x00000000 VS Configuration Register Output Address
    volatile unsigned char	SPEC[32];          // 0x0021-0x003F W 0x0000003F VS Configuration Register Output Specifier
}VSCFGREG2;
*/
typedef struct _GPUVERTEXSHADER{
    VSCFGREG  VS_CONF_REG_INP;	
    VSCFGREG  VS_CONF_REG_OUTP;
	volatile unsigned char	VS_CONF_REG_PROG_PARAM;         // 0x0040 W 0x00000000 VS Configuration Register Program Parameter Create
	volatile unsigned char	VS_CONF_REG_PREFETCH;           // 0x0041 W 0x00000000 VS Configuration Register Prefetch
	volatile unsigned char	VS_CONF_REG_OPMOD;              // 0x0042 W 0x0F000000 VS Configuration Register OPMOD
	volatile unsigned char	VS_CONF_REG_VERTICES_ALT_STRIDE;    // 0x0043 W 0x00000000 VS Configuration Register Vertices Alternative Stride
	volatile unsigned char	VS_CONF_REG_INPUT_ALT_STRIDE_0; // 0x0044 W 0x00000000 VS Configuration Register Input Alternative Stride 0
	volatile unsigned char	VS_CONF_REG_INPUT_ALT_STRIDE_1; // 0x0045 W 0x00000000 VS Configuration Register Input Alternative Stride 1
	volatile unsigned char	VS_CONF_REG_INPUT_ALT_STRIDE_2; // 0x0046 W 0x00000000 VS Configuration Register Input Alternative Stride 2
	volatile unsigned char	VS_CONF_REG_INPUT_ALT_STRIDE_3; // 0x0047 W 0x00000000 VS Configuration Register Input Alternative Stride 3
	volatile unsigned char	VS_CONF_REG_OUTPUT_ALT_STRIDE_0;    // 0x0048 W 0x00000000 VS Configuration Register Output Alternative Stride 0
	volatile unsigned char	VS_CONF_REG_OUTPUT_ALT_STRIDE_1;    // 0x0049 W 0x00000000 VS Configuration Register Output Alternative Stride 1
	volatile unsigned char	VS_CONF_REG_OUTPUT_ALT_STRIDE_2;    // 0x004A W 0x00000000 VS Configuration Register Output Alternative Stride 2
	volatile unsigned char	VS_CONF_REG_OUTPUT_ALT_STRIDE_3;    // 0x004B W 0x00000000 VS Configuration Register Output Alternative Stride 3
}GPUVERTEXSHADER, *PGPUVERTEXSHADER;


/*******************************************************************************
*	 5-3. MMU Configuration Register Define   (Base Addr = 0xF0003000)
********************************************************************************/
//#define HwMMUCONFIG_BASE                      *(volatile unsigned long *)0xF0003000
typedef struct _GPUMMUCONFIG{
	volatile unsigned int	MMU_DTE_ADDR;                       // 0x3000 R/W 0x00000000 MMU Current Page Table Address
	volatile unsigned int	MMU_STATUS;                         // 0x3004 R 0x00000018 MMU Status
	volatile unsigned int	MMU_COMMAND;                        // 0x3008 W N/A MMU Command
	volatile unsigned int	MMU_PAGE_FAULT_ADDR;                // 0x300C R 0x00000000 MMU Logical Address of Last Page Fault
	volatile unsigned int	MMU_ZAP_ONE_LINE;                   // 0x3010 W N/A MMU Zap Cache Line
	volatile unsigned int	MMU_INT_RAWSTA;                    // 0x3014 R/W 0x00000000 MMU Raw Interrupt Status
	volatile unsigned int	MMU_INT_CLEAR;                      // 0x3018 W N/A MMU Interrupt Clear
	volatile unsigned int	MMU_INT_MASK;                       // 0x301C R/W 0x00000000 MMU Interrupt Mask
	volatile unsigned int	MMU_INT_STATUS;                     // 0x3020 R 0x00000000 MMU Interrup Status
}GPUMMUCONFIG, *PGPUMMUCONFIG;

/*******************************************************************************
*	 5-4. GRPBUS Configuration Register Define   (Base Addr = 0xF0004000)
********************************************************************************/
//#define HwGRPBUS_BASE                         *(volatile unsigned long *)0xF0004000
typedef struct _GPUGRPBUSCONFIG{
	volatile unsigned int	GRPBUS_PWRDOWN;                     // 0x0000 R/W 0x00000000 Graphics bus power down
	volatile unsigned int	GRPBUS_SWRESET;                     // 0x0004 R/W 0x00000000 Graphics bus software reset
	volatile unsigned int	GRPBUS_MALI_IDLE;                   // 0x0008 R/W 0x00000002 Mali idle configration
}GPUGRPBUSCONFIG, *PGPUGRPBUSCONFIG;

/*******************************************************************************
*	 5-5. GRPBUS BWRAP Register Define   (Base Addr = 0xF0005000)
********************************************************************************/
//#define HwGRPBUSBWRAP_BASE                      *(volatile unsigned long *)0xF0005000
typedef struct _GPUGRPBUSBWRAP{
	volatile unsigned int	GRPBUS_BWRAPCTRL;                   // 0x0000 R/W 0x00000000 Graphics bus bwrap control
}GPUGRPBUSBWRAP, *PGPUGRPBUSBWRAP;




#endif
