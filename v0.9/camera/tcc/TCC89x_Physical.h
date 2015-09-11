/****************************************************************************
 *   FileName    : TCC89x_Physical.h
 *   Description : 
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/

/****************************************************************************

  Revision History

 ****************************************************************************

 ****************************************************************************/

/************************************************************************
*	TCC89x Internal Register Definition File
************************************************************************/
#ifndef	__TCC89x_H__
#define	__TCC89x_H__


/************************************************************************
*	Bit Field Definition
************************************************************************/
#define	Hw37									(1LL << 37)
#define	Hw36									(1LL << 36)
#define	Hw35									(1LL << 35)
#define	Hw34									(1LL << 34)
#define	Hw33									(1LL << 33)
#define	Hw32									(1LL << 32)
#define	Hw31									0x80000000
#define	Hw30									0x40000000
#define	Hw29									0x20000000
#define	Hw28									0x10000000
#define	Hw27									0x08000000
#define	Hw26									0x04000000
#define	Hw25									0x02000000
#define	Hw24									0x01000000
#define	Hw23									0x00800000
#define	Hw22									0x00400000
#define	Hw21									0x00200000
#define	Hw20									0x00100000
#define	Hw19									0x00080000
#define	Hw18									0x00040000
#define	Hw17									0x00020000
#define	Hw16									0x00010000
#define	Hw15									0x00008000
#define	Hw14									0x00004000
#define	Hw13									0x00002000
#define	Hw12									0x00001000
#define	Hw11									0x00000800
#define	Hw10									0x00000400
#define	Hw9										0x00000200
#define	Hw8										0x00000100
#define	Hw7										0x00000080
#define	Hw6										0x00000040
#define	Hw5										0x00000020
#define	Hw4										0x00000010
#define	Hw3										0x00000008
#define	Hw2										0x00000004
#define	Hw1										0x00000002
#define	Hw0										0x00000001
#define	HwZERO									0x00000000


#define			HwREG(X)					*(volatile unsigned long *)(X)


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
#define HwCLK_BASE					*(volatile unsigned long *)0xF0400000

/************************************************************************
*	2. Vectored Priority Interrupt Controller Register Map(Base Addr = 0xF0401000)
************************************************************************/
#define	HwPIC_BASE					*(volatile unsigned long *)0xF0401000
#define	HwVIC_BASE					*(volatile unsigned long *)0xF0401200
#define HwPIC						((PPIC)&HwPIC_BASE)
#define HwVIC						((PVIC)&HwVIC_BASE)


// Interrupt Enable 0
#define	HwINT0_EHI0					Hw31										// R/W, External Host Interface0 Interrupt Enable
#define	HwINT0_ECC					Hw30										// R/W, ECC Interrupt Enable 
#define	HwINT0_DMA					Hw29										// R/W, DMA Controller Interrupt Enable
#define	HwINT0_TSADC				Hw28										// R/W, TSADC Interrupt Enable
#define	HwINT0_G2D					Hw27										// R/W, Graphic Engine 2D Hardware Interrupt Enable
#define	HwINT0_3DMMU				Hw26										// R/W, 3D MMU Interrupt Enable
#define	HwINT0_3DGP					Hw25										// R/W, 3D Geometary Interrupt Enable
#define	HwINT0_3DPP					Hw24										// R/W, 3D Pixel Processor Interrupt Enable
#define	HwINT0_VCDC					Hw23										// R/W, Video CODEC Interrupt Enable
#define	HwINT0_JPGD					Hw22										// R/W, JPEG Decoder Interrupt Enable
#define	HwINT0_JPGE					Hw21										// R/W, JPEG Encoder Interrupt Enable
#define	HwINT0_VIPET				Hw20										// R/W, VIPET Controller Interrupt Enable
#define	HwINT0_LCD1					Hw19										// R/W, LCD Controller1 Interrupt Enable
#define	HwINT0_LCD0					Hw18										// R/W, LCD Controller0 Interrupt Enable
#define	HwINT0_CAM					Hw17										// R/W, Camera Interrupt Enable
#define	HwINT0_SC1					Hw16										// R/W, Mem-to-Mem Scaler1 Interrupt Enable
#define	HwINT0_SC0					Hw15										// R/W, Mem-to-Mem Scaler0 Interrupt Enable
#define	HwINT0_EI11					Hw14										// R/W, External Interrupt11 Enable
#define	HwINT0_EI10					Hw13										// R/W, External Interrupt10 Enable
#define	HwINT0_EI9					Hw12										// R/W, External Interrupt9 Enable
#define	HwINT0_EI8					Hw11										// R/W, External Interrupt8 Enable
#define	HwINT0_EI7					Hw10										// R/W, External Interrupt7 Enable
#define	HwINT0_EI6					Hw9											// R/W, External Interrupt6 Enable
#define	HwINT0_EI5					Hw8											// R/W, External Interrupt5 Enable
#define	HwINT0_EI4					Hw7											// R/W, External Interrupt4 Enable
#define	HwINT0_EI3					Hw6											// R/W, External Interrupt3 Enable
#define	HwINT0_EI2					Hw5											// R/W, External Interrupt2 Enable
#define	HwINT0_EI1					Hw4											// R/W, External Interrupt1 Enable
#define	HwINT0_EI0					Hw3											// R/W, External Interrupt0 Enable
#define	HwINT0_SMUI2C				Hw2											// R/W, SMU_I2C Interrupt Enable
#define	HwINT0_TC1					Hw1											// R/W, Timer1 Interrupt Enable
#define	HwINT0_TC0					Hw0											// R/W, Timer0 Interrupt Enable

// Interrupt Enable 1
#define	HwINT1_AEIRQ				Hw31										// R/W, Not maskable error ARM DMA interrupt enable
#define	HwINT1_ASIRQ				Hw30										// R/W, Secure ARM DMA select interrupt enable
#define	HwINT1_AIRQ					Hw29										// R/W, Non secure ARM DMA interrupt enable
#define	HwINT1_APMU					Hw28										// R/W, ARM System Metrics interrupt enable
#define	HwINT1_AUDIO				Hw27										// R/W, AUDIO interrupt enable
#define	HwINT1_ADMA					Hw26										// R/W, AUDIO DMA interrupt enable
#define	HwINT1_DAITX				Hw25										// R/W, DAI transmit interrupt enable
#define	HwINT1_DAIRX				Hw24										// R/W, DAI receive interrupt enable
#define	HwINT1_CDRX					Hw23										// R/W, CDIF receive interrupt enable
#define	HwINT1_TSIF1				Hw22										// R/W, TS interface 1 interrupt enable
#define	HwINT1_TSIF0				Hw21										// R/W, TS interface 0 interrupt enable
#define	HwINT1_GPS2					Hw20										// R/W, GPS AGPS interrupt enable
#define	HwINT1_GPS1					Hw19										// R/W, GPS TCXO expired interrupt enable
#define	HwINT1_GPS0					Hw18										// R/W, GPS RTC expired interrupt enable
#define	HwINT1_NotUsed				Hw17										// R/W, Reserved
#define	HwINT1_UOTG					Hw16										// R/W, USB 2.0 OTG interrupt enable
#define	HwINT1_UART					Hw15										// R/W, UART interrupt enable
#define	HwINT1_SPDTX				Hw14										// R/W, SPDIF transmitter interrupt enable
#define	HwINT1_SD1					Hw13										// R/W, SD/MMC 1 interrupt enable
#define	HwINT1_SD0					Hw12										// R/W, SD/MMC 0 interrupt enable
#define	HwINT1_RTC					Hw11										// R/W, RTC interrupt enable
#define	HwINT1_RMT					Hw10										// R/W, Remote Control interrupt enable
#define	HwINT1_NFC					Hw9											// R/W, Nand flash controller interrupt enable
#define	HwINT1_MS					Hw8											// R/W, Memory Stick interrupt enable
#define	HwINT1_MPEFEC				Hw7											// R/W, MPEFEC interrupt enable
#define	HwINT1_I2C					Hw6											// R/W, I2C interrupt enable
#define	HwINT1_HDD					Hw5											// R/W, HDD controller interrupt enable
#define	HwINT1_GPSB					Hw4											// R/W, GPSB Interrupt Enable
#define	HwINT1_NotUsed1				Hw3											// R/W, Reserved
#define	HwINT1_HDMI					Hw2											// R/W, HDMI interrupt enable
#define	HwINT1_NotUsed2				Hw1											// R/W, Reserved
#define	HwINT1_EHI1					Hw0											// R/W, External Host Interface1 Interrupt Enable

#define	HwALLMSK_FIQ				Hw1										// FIQ mask register
#define	HwALLMSK_IRQ				Hw0										// IRQ mask register
	
/***********************************************************************
*	3. Timer/Counter Register Map (Base Address = 0xF0403000) 
************************************************************************/
#define	HwTMR_BASE					*(volatile unsigned long *)0xF0403000	// Timer/Counter Base Register

/***********************************************************************
*	4. PMU(POWER MANAGEMENT UNIT) Register Map (Base Address = 0xF0404000) 
************************************************************************/
#define	HwPMU_BASE  				*(volatile unsigned long *)0xF0404000 	//R/W   PMU Control Register 

/*******************************************************************************
*	5. SMUI2C Controller Register Define   (Base Addr = 0xF0405000)
********************************************************************************/
#define HwSMU_I2CMASTER0_BASE     	*(volatile unsigned long *)0xF0405000
#define HwSMU_I2CMASTER1_BASE     	*(volatile unsigned long *)0xF0405040
#define HwSMU_I2CICLK_BASE     		*(volatile unsigned long *)0xF0405080	//I2C_SCL divider Regist
#define HwI2CSTATUS_BASE            *(volatile unsigned long *)0xF05300C0

/*******************************************************************************
*	 TCC8900_DataSheet_PART 3 GPIO_V0.00 Dec.11 2008
********************************************************************************/
/************************************************************************
*	1. GPIO Register Map (Base Address = 0xF0102000) 
************************************************************************/
#define	HwGPIO_BASE  				*(volatile unsigned long *)0xF0102000  	// 
#define	HwGPIOA_BASE  				*(volatile unsigned long *)0xF0102000  	// 
#define	HwGPIOB_BASE  				*(volatile unsigned long *)0xF0102040  	// 
#define	HwGPIOC_BASE  				*(volatile unsigned long *)0xF0102080  	// 
#define	HwGPIOD_BASE  				*(volatile unsigned long *)0xF01020C0  	// 
#define	HwGPIOE_BASE  				*(volatile unsigned long *)0xF0102100  	// 
#define	HwGPIOF_BASE  				*(volatile unsigned long *)0xF0102140  	// 
#define	HwEINTSEL_BASE  			*(volatile unsigned long *)0xF0102180  	// 
#define HwGPIO						((PGPIO)&HwGPIO_BASE)
#define HwGPIOA						((PGPION)&HwGPIOA_BASE)
#define HwGPIOB						((PGPION)&HwGPIOB_BASE)
#define HwGPIOC						((PGPION)&HwGPIOC_BASE)
#define HwGPIOD						((PGPION)&HwGPIOD_BASE)
#define HwGPIOE						((PGPION)&HwGPIOE_BASE)
#define HwGPIOF						((PGPION)&HwGPIOF_BASE)
#define HwEINTSEL					((PGPIOINT)&HwEINTSEL_BASE)


/*******************************************************************************
*	 TCC8900_DataSheet_PART 4 CORE & MEMORY BUS_V0.00 Dec.11 2008
********************************************************************************/
/************************************************************************
*	3. DRAM CONTROLLER Register Map (Base Address = 0xF0301000) 
************************************************************************/
#define	HwDRAM_BASE  				*(volatile unsigned long *)0xF0301000  	//
#define	HwDRAMM0_BASE  				*(volatile unsigned long *)0xF0301000  	//
#define	HwDRAMM1_BASE  				*(volatile unsigned long *)0xF0302000  	//
#define	HwDRAMMISC_BASE  			*(volatile unsigned long *)0xF0303000  	//
#define	HwDRAMPHY_BASE  			*(volatile unsigned long *)0xF0304400  	//
#define	HwDRAMMEMBUS_BASE  			*(volatile unsigned long *)0xF0305004  	//

/************************************************************************
*	4-1. MISC CORE BUS CONFIGURATION REGISTERS				(Base Addr = 0xF0101000)
************************************************************************/
#define	HwCORECFG_BASE				*(volatile unsigned long *)0xF0101000	
       
/************************************************************************
*	4-2. Virtual MMU Table  Register Define	(Base Addr = 0xF7000000)
************************************************************************/
#define	HwVMT_BASE					*(volatile unsigned long *)0x20000000	// VMT Base Regiseter
#define	HwREGION_BASE				*(volatile unsigned long *)0xF0600000	// R/W, Configuration Register for Region 0


/*******************************************************************************
*	 TCC8900_DataSheet_PART 5 IO BUS_V0.00 Dec.11 2008
********************************************************************************/
/*******************************************************************************
*	 4. Memory Stick Host Controller Register Define   (Base Addr = 0xF0590000)
********************************************************************************/
#define HwSMSHC_BASE                *(volatile unsigned long *)0xF0590000


/********************************************************************************
*	 5. SD/SDIO/MMC/CE-ATA Host Controller Register Define   (Base Addr = 0xF0590000)
********************************************************************************/
//Core 0 Slot 0
#define HwSDCORE0SLOT1_BASE         *(volatile unsigned long *)0xF05A0000
//Core 0 Slot 1
#define HwSDCORE0SLOT2_BASE         *(volatile unsigned long *)0xF05A0100
//Core 1 Slot 2
#define HwSDCORE1SLOT2_BASE         *(volatile unsigned long *)0xF05A0200
//Core 1 Slot 3
#define HwSDCORE1SLOT3_BASE         *(volatile unsigned long *)0xF05A0300
// Channel Control Register
#define HwSDCHCTRL_BASE             *(volatile unsigned long *)0xF05A0800 // R/W 0x0000 SD/MMC port control register

/*******************************************************************************
*	 6. NAND Flash Controller(NFC) Register Define   (Base Addr = 0xF05B0000)
********************************************************************************/
#define HwNFC_BASE                  *(volatile unsigned long *)0xF05B0000
#define HwNFC						((PNFC)&HwNFC_BASE)

// NFC Control Register
#define	HwNFC_CTRL_RDYIEN_EN		Hw31							// Nand Flash Ready Interrupt Enable
#define	HwNFC_CTRL_RDYIEN_DIS		~Hw31							// Nand Flash Ready Interrupt Disable
#define	HwNFC_CTRL_PROGIEN_EN		Hw30							// Nand Flash Program Interrupt Enable 
#define	HwNFC_CTRL_PROGIEN_DIS		~Hw30							// Nand Flash Program Interrupt Disable
#define	HwNFC_CTRL_READIEN_EN		Hw29							// Nand Flash Read Interrupt Enable
#define	HwNFC_CTRL_READIEN_DIS		~Hw29							// Nand Flash Read Interrupt Disable
#define	HwNFC_CTRL_DEN_EN			Hw28							// Nand Flash DMA Request Enable
#define	HwNFC_CTRL_DEN_DIS			~Hw28							// Nand Flash DMA Request Disable
#define	HwNFC_CTRL_FS_RDY			Hw27							// FIFO status is Ready to write and read in FIFO
#define	HwNFC_CTRL_FS_BUSY			~Hw27							// FIFO status is Busy to write and read in FIFO
#define	HwNFC_CTRL_BW_16			Hw26							// Bus width = 8bit
#define	HwNFC_CTRL_BW_8				HwZERO							// Bus width = 16bit
#define	HwNFC_CTRL_CS3SEL_1			Hw25							// Nand Flash nCS3 is High (Disabled)
#define	HwNFC_CTRL_CS3SEL_0			HwZERO							// Nand Flash nCS3 is Low (Enabled)
#define	HwNFC_CTRL_CS2SEL_1			Hw24							// Nand Flash nCS2 is High (Disabled)
#define	HwNFC_CTRL_CS2SEL_0			HwZERO							// Nand Flash nCS2 is Low (Enabled)
#define	HwNFC_CTRL_CS1SEL_1			Hw23							// Nand Flash nCS1 is High (Disabled)
#define	HwNFC_CTRL_CS1SEL_0			HwZERO							// Nand Flash nCS1 is Low (Enabled)
#define	HwNFC_CTRL_CS0SEL_1			Hw22							// Nand Flash nCS0 is High (Disabled)
#define	HwNFC_CTRL_CS0SEL_0			HwZERO							// Nand Flash nCS0 is Low (Enabled)
#define	HwNFC_CTRL_CFG_nCS3			HwNFC_CTRL_CS3SEL_1
#define	HwNFC_CTRL_CFG_nCS2			HwNFC_CTRL_CS2SEL_1
#define	HwNFC_CTRL_CFG_nCS1			HwNFC_CTRL_CS1SEL_1
#define	HwNFC_CTRL_CFG_nCS0			HwNFC_CTRL_CS0SEL_1
#define	HwNFC_CTRL_CSnSEL(X)		((X)*Hw22)						// Nand Flash nCS[3:0] Set
#define	HwNFC_CTRL_CFG_NOACT		HwNFC_CTRL_CSnSEL(15)
#define	HwNFC_CTRL_RDY_RDY			Hw21							// External Nand Flash Controller is Ready
#define	HwNFC_CTRL_RDY_BUSY			~Hw21							// External Nand Flash Controller is Busy
#define	HwNFC_CTRL_BSIZE(X)			((X)*Hw19)
#define	HwNFC_CTRL_BSIZE_1			HwNFC_CTRL_BSIZE(0)				// 1Read/Write
#define	HwNFC_CTRL_BSIZE_2			HwNFC_CTRL_BSIZE(1)				// 2Read/Write
#define	HwNFC_CTRL_BSIZE_4			HwNFC_CTRL_BSIZE(2)				// 4Read/Write
#define	HwNFC_CTRL_BSIZE_8			HwNFC_CTRL_BSIZE(3)				// 8Read/Write
#define	HwNFC_CTRL_BSIZE_MASK		HwNFC_CTRL_BSIZE(3)
#define	HwNFC_CTRL_PSIZE(X)			((X)*Hw16)
#define	HwNFC_CTRL_PSIZE_256		HwNFC_CTRL_PSIZE(0)				// 1 Page = 256 Half-Word
#define	HwNFC_CTRL_PSIZE_512		HwNFC_CTRL_PSIZE(1)				// 1 Page = 512 Byte
#define	HwNFC_CTRL_PSIZE_1024		HwNFC_CTRL_PSIZE(2)				// 1 Page = 1024 Half-Word
#define	HwNFC_CTRL_PSIZE_2048		HwNFC_CTRL_PSIZE(3)				// 1 Page = 2048 Byte
#define	HwNFC_CTRL_PSIZE_4096		HwNFC_CTRL_PSIZE(4)				// 1 Page = 4096 Byte
#define	HwNFC_CTRL_PSIZE_MASK		HwNFC_CTRL_PSIZE(7)
#define	HwNFC_CTRL_MASK_EN			Hw15							// Address/Command Mask Enable
#define	HwNFC_CTRL_CADDR			Hw12							// Number of Address Cycle
#define	HwNFC_CTRL_bSTP(X)			((X)*Hw8)						// Number of Base cycle for Setup Time
#define	HwNFC_CTRL_bSTP_MASK		HwNFC_CTRL_bSTP(15)
#define	HwNFC_CTRL_bPW(X)			((X)*Hw4)						// Number of Base cycle for Pulse Width
#define	HwNFC_CTRL_bPW_MASK			HwNFC_CTRL_bPW(15)
#define	HwNFC_CTRL_bHLD(X)			((X)*Hw0)						// Number of Base cycle for Hold Time
#define	HwNFC_CTRL_bHLD_MASK		HwNFC_CTRL_bHLD(15)

#define	HwNFC_IREQ_FLAG2			Hw6						//
#define	HwNFC_IREQ_FLAG1			Hw5						//
#define	HwNFC_IREQ_FLAG0			Hw4						//
#define	HwNFC_IREQ_IRQ2				Hw2						// Ready Interrupt
#define	HwNFC_IREQ_IRQ1				Hw1						// Program Interrupt
#define	HwNFC_IREQ_IRQ0				Hw0						// Reading Interrupt

/*******************************************************************************
*	 7. Static Memory Controller(SMC) Register Define   (Base Addr = 0xF05F0000)
********************************************************************************/
#define HwSMC_BASE                  *(volatile unsigned long *)0xF05F0000
#define HwSMC_STATUS                *(volatile unsigned long *)0xF05F0000 // R/W Unknown Status Register
#define HwSMC_CSNCFG0               *(volatile unsigned long *)0xF05F0020 // R 0x4b40_3183 External Chip Select0 Config Register
#define HwSMC_CSNCFG1               *(volatile unsigned long *)0xF05F0024 // R/W 0x4b40_1104 External Chip Select1 Config Register
#define HwSMC_CSNCFG2               *(volatile unsigned long *)0xF05F0028 // W 0x4b40_4082 External Chip Select2 Config Register
#define HwSMC_CSNCFG3               *(volatile unsigned long *)0xF05F002C // R/W 0x4b40_20C5 External Chip Select3 Config. Register
#define HwSMC_CSNOFFSET             *(volatile unsigned long *)0xF05F0030 // R/W 0x0 Wapping Address Mode OFFSET Register
#define HwSMC_INDIRADDR             *(volatile unsigned long *)0xF05F0034 // R/W 0x0 Indirect Address

/*******************************************************************************
*	 8. External Device Interface (EDI) Register Define   (Base Addr = 0xF05F6000)
********************************************************************************/
#define HwEDI_BASE                  *(volatile unsigned long *)0xF05F6000
#define HwEDI						((PEDI)&HwEDI_BASE)

/*******************************************************************************
*	 9. IDE Controller Register Define   (Base Addr = 0xF0520000)
********************************************************************************/
#define HwIDE_BASE                  *(volatile unsigned long *)0xF05F6000

/*******************************************************************************
*	 10. SATA Interface Register Define   (Base Addr = 0xF0560000)
********************************************************************************/
#define HwSATA_BASE                 *(volatile unsigned long *)0xF0560000
//SCR5-SCR15 0x38-0x60 32 See description 0x0 Reserved for SATA Dependencies: Reads to these locations return zeros; writes have no effect

/*******************************************************************************
*	 11-1. Audio DMA Controller Register Define   (Base Addr = 0xF0533000)
********************************************************************************/
#define HwADMA_BASE                 *(volatile unsigned long *)0xF0533000

/*******************************************************************************
*	 11-2. DAI Register Define   (Base Addr = 0xF0534000)
********************************************************************************/
#define HwADMA_DAIBASE              *(volatile unsigned long *)0xF0534000

/*******************************************************************************
*	 11-3. CDIF Register Define   (Base Addr = 0xF0534000)
********************************************************************************/
#define HwADMA_CDIFBASE             *(volatile unsigned long *)0xF0534080

/*******************************************************************************
*	 11-4. SPDIF Register Define   (Base Addr = 0xF0535000/0xF0535800)
********************************************************************************/
#define HwADMA_SPDIFTXBASE          *(volatile unsigned long *)0xF0535000

/*******************************************************************************
*	 12-1. DAI Register Define   (Base Addr = 0xF0537000
********************************************************************************/
#define HwDAI_BASE                  *(volatile unsigned long *)0xF0537000

/*******************************************************************************
*	 12-2. CDIF Register Define   (Base Addr = 0xF0537000
********************************************************************************/
#define HwCDIF_BASE                 *(volatile unsigned long *)0xF0537080

/*******************************************************************************
*	 13. SPDIF Register Define   (Base Addr = 0xF0538000)
********************************************************************************/
#define HwSPDIF_BASE                *(volatile unsigned long *)0xF0538000

/*******************************************************************************
*	 14-1. USB1.1 HOST Controller & Transceiver       (Base Addr = 0xF0500000)
********************************************************************************/
#define HwUSBHOST_BASE              *(volatile unsigned long *)0xF0500000

/*******************************************************************************
*	 14-2 USB1.1 HOST Configuration Register        (Base Addr = 0xF05F5000)
********************************************************************************/
#define HwUSBHOSTCFG_BASE           *(volatile unsigned long *)0xF05F5000

/*******************************************************************************
*	 15-1. USB2.0 OTG Controller Define   (Base Addr = 0xF0550000)
********************************************************************************/
#define HwUSB20OTG_BASE             *(volatile unsigned long *)0xF0550000

/*******************************************************************************
*	 15-2. USB OTG Configuration Register Define   (Base Addr = 0xF05F5000)
********************************************************************************/
#define HwUSBOTGCFG_BASE            *(volatile unsigned long *)0xF05F5000

/*******************************************************************************
*	 15-3. USB PHY Configuration Register Define   (Base Addr = 0xF05F5028)
********************************************************************************/
#define HwUSBPHYCFG_BASE            *(volatile unsigned long *)0xF05F5028

/*******************************************************************************
*	 16. External Host Interface Register Define   (Base Addr = 0xF0570000/0xF0580000)
********************************************************************************/
#define HwEHICS0_BASE               *(volatile unsigned long *)0xF0570000
#define HwEHICS1_BASE               *(volatile unsigned long *)0xF0580000

/*******************************************************************************
*	 17. General Purpose Serial Bus (GPSB) Register Define   (Base Addr = 0xF0538000)
********************************************************************************/
#define HwGPSBCH0_BASE              *(volatile unsigned long *)0xF0057000
#define HwGPSBCH1_BASE              *(volatile unsigned long *)0xF0057100
#define HwGPSBCH2_BASE              *(volatile unsigned long *)0xF0057200
#define HwGPSBCH3_BASE              *(volatile unsigned long *)0xF0057300
#define HwGPSBCH4_BASE              *(volatile unsigned long *)0xF0057400
#define HwGPSBCH5_BASE              *(volatile unsigned long *)0xF0057500
#define HwGPSBPORTCFG_BASE          *(volatile unsigned long *)0xF0057800
#define HwGPSBPIDTABLE_BASE         *(volatile unsigned long *)0xF0057F00

/*******************************************************************************
*	 18. The Transport Stream Interface (TSIF) Register Define   (Base Addr = 0xF0538000)
********************************************************************************/
#define HwTSIF_BASE                 *(volatile unsigned long *)0xF053B000
#define HwTSIFPORTSEL_BASE          *(volatile unsigned long *)0xF053B800

/*******************************************************************************
*	 19. GPS Interface Register Define   (Base Addr = )
********************************************************************************/


/*******************************************************************************
*	 20. Remote Control Interface Register Define   (Base Addr = 0xF05F3000)
********************************************************************************/
#define HwREMOCON_BASE             *(volatile unsigned long *)0xF05F3000


/*******************************************************************************
*	 21. I2C Controller Register Define   (Base Addr = 0xF0530000)
********************************************************************************/
#define HwI2CMASTER0_BASE          *(volatile unsigned long *)0xF0530000
#define HwI2CMASTER1_BASE          *(volatile unsigned long *)0xF0530040
#define HwI2CSLAVE_BASE            *(volatile unsigned long *)0xF0530080
#define HwI2CSTATUS_BASE           *(volatile unsigned long *)0xF05300C0

/*******************************************************************************
*	 22. UART Controller Register Define   (Base Addr = 0xF0538000)
********************************************************************************/
#define HwUARTCH0_BASE             *(volatile unsigned long *)0xF0532000
#define HwUARTCH1_BASE             *(volatile unsigned long *)0xF0532100
#define HwUARTCH2_BASE             *(volatile unsigned long *)0xF0532200
#define HwUARTCH3_BASE             *(volatile unsigned long *)0xF0532300
#define HwUARTCH4_BASE             *(volatile unsigned long *)0xF0532400
#define HwUARTCH5_BASE             *(volatile unsigned long *)0xF0532500
#define HwUARTPORTMUX_BASE         *(volatile unsigned long *)0xF0532600

/*******************************************************************************
*	 23. CAN Controller Register Define   (Base Addr = 0xF0531000)
********************************************************************************/
#define HwCAN_BASE                 *(volatile unsigned long *)0xF0531000

/*******************************************************************************
*	 24. DMA Controller Register Define   (Base Addr = 0xF0540000)
********************************************************************************/
#define HwGDMA0_BASE               *(volatile unsigned long *)0xF0540000
#define HwGDMA1_BASE               *(volatile unsigned long *)0xF0540100
#define HwGDMA2_BASE               *(volatile unsigned long *)0xF0540200
#define HwGDMA3_BASE               *(volatile unsigned long *)0xF0540300

/*******************************************************************************
*	 25. Real Time Clock(RTC) Register Define   (Base Addr = 0xF05F2000)
********************************************************************************/
#define HwRTC_BASE                 *(volatile unsigned long *)0xF05F2000

/*******************************************************************************
*	 26. TouchScreen ADC (TSADC) Register Define   (Base Addr = 0xF05F4000)
********************************************************************************/
#define HwTSADC_BASE               *(volatile unsigned long *)0xF05F4000

/*******************************************************************************
*	 27. Error Correction Code Register Define   (Base Addr = 0xF0539000)
********************************************************************************/
#define HwECC_BASE                 *(volatile unsigned long *)0xF0539000

// ECC Control
#define	HwECC_CTRL_IEN_MECC16_EN	Hw20										// MLC ECC16 Decoding Interrupt Enable
#define	HwECC_CTRL_IEN_MECC16_DIS	~Hw20										// MLC ECC16 Decoding Interrupt Disable
#define	HwECC_CTRL_IEN_MECC14_EN	Hw19										// MLC ECC14 Decoding Interrupt Enable
#define	HwECC_CTRL_IEN_MECC14_DIS	~Hw19										// MLC ECC14 Decoding Interrupt Disable
#define	HwECC_CTRL_IEN_MECC12_EN	Hw18										// MLC ECC12 Decoding Interrupt Enable
#define	HwECC_CTRL_IEN_MECC12_DIS	~Hw18										// MLC ECC12 Decoding Interrupt Disable
#define	HwECC_CTRL_IEN_MECC8_EN		Hw17										// MLC ECC8 Decoding Interrupt Enable
#define	HwECC_CTRL_IEN_MECC8_DIS	~Hw17										// MLC ECC8 Decoding Interrupt Disable
#define	HwECC_CTRL_IEN_MECC4_EN		Hw16										// MLC ECC4 Decoding Interrupt Enable
#define	HwECC_CTRL_IEN_MECC4_DIS	~Hw16										// MLC ECC4 Decoding Interrupt Disable

// ECC Disable
#define	HwECC_CTRL_EN_SLCEN			Hw2											// SLC ECC Encoding Enable
#define	HwECC_CTRL_EN_SLCDE			(Hw2|Hw0)									// SLC ECC Decoding Enable
#define	HwECC_CTRL_EN_MCL4EN		(Hw2|Hw1)									// MLC ECC4 Encoding Enable
#define	HwECC_CTRL_EN_MCL4DE		(Hw2|Hw1|Hw0)								// MLC ECC4 Decoding Enable
#define	HwECC_CTRL_EN_MCL8EN		(Hw3)										// MLC ECC8 Encoding Enable
#define	HwECC_CTRL_EN_MCL8DE		(Hw3|Hw0)									// MLC ECC8 Decoding Enable
#define	HwECC_CTRL_EN_MCL12EN		(Hw3|Hw1)									// MLC ECC12 Encoding Enable
#define	HwECC_CTRL_EN_MCL12DE		(Hw3|Hw1|Hw0)								// MLC ECC12 Decoding Enable
#define	HwECC_CTRL_EN_MCL14EN		(Hw3|Hw2)									// MLC ECC14 Encoding Enable
#define	HwECC_CTRL_EN_MCL14DE		(Hw3|Hw2|Hw0)								// MLC ECC14 Decoding Enable
#define	HwECC_CTRL_EN_MCL16EN		(Hw3|Hw2|Hw1)								// MLC ECC16 Encoding Enable
#define	HwECC_CTRL_EN_MCL16DE		(Hw3|Hw2|Hw1|Hw0)							// MLC ECC16 Decoding Enable
#define	HwECC_CTRL_EN_DIS			~(Hw3|Hw2|Hw1|Hw0)							// ECC Disable

// ECC Error Number
#define	HwERR_NUM_ERR1				Hw0											// Correctable Error(SLC), Error Occurred(MLC3), 1 Error Occurred(MLC4)
#define	HwERR_NUM_ERR2				Hw1											// 2 Error Occurred(MLC4)
#define	HwERR_NUM_ERR3				(Hw1|Hw0)									// 3 Error Occurred(MLC4)
#define	HwERR_NUM_ERR4				Hw2											// 4 Error Occurred(MLC4)
#define	HwERR_NUM_ERR5				(Hw2|Hw0)									// 5 Error Occurred(MLC4)
#define	HwERR_NUM_ERR6				(Hw2|Hw1)									// 5 Error Occurred(MLC4)
#define	HwERR_NUM_ERR7				(Hw2|Hw1|Hw0)								// 5 Error Occurred(MLC4)
#define	HwERR_NUM_ERR8				Hw3											// 5 Error Occurred(MLC4)
#define	HwERR_NUM_ERR9				(Hw3|Hw0)									// 5 Error Occurred(MLC4)
#define	HwERR_NUM_ERR10				(Hw3|Hw1)									// 5 Error Occurred(MLC4)
#define	HwERR_NUM_ERR11				(Hw3|Hw1|Hw0)								// 5 Error Occurred(MLC4)
#define	HwERR_NUM_ERR12				(Hw3|Hw2)									// 5 Error Occurred(MLC4)
#define	HwERR_NUM_ERR13				(Hw3|Hw2|Hw0)								// 5 Error Occurred(MLC4)
#define	HwERR_NUM_ERR14				(Hw3|Hw2|Hw1)								// 5 Error Occurred(MLC4)
#define	HwERR_NUM_ERR15				(Hw3|Hw2|Hw1|Hw0)							// 5 Error Occurred(MLC4)
#define	HwERR_NUM_ERR16				Hw4											// 5 Error Occurred(MLC4)
#define	HwERR_NUM_NOERR				HwZERO										// No Error
#define	HwERR_NUM_CORIMP			(Hw1|Hw0)									// Correction Impossible(SLC, MLC4)

// ECC Interrupt Control
#define	HwECC_IREQ_SEF				Hw17										// SLC ECC Encoding Flag Register
#define	HwECC_IREQ_SDF				Hw16										// SLC ECC Decoding Flag Register
#define	HwECC_IREQ_M4EF				Hw19										// MLC ECC4 Encoding Flag Register
#define	HwECC_IREQ_M4DF				Hw18										// MLC ECC4 Decoding Flag Register
#define	HwECC_IREQ_M8EF				Hw21										// MLC ECC8 Encoding Flag Register
#define	HwECC_IREQ_M8DF				Hw20										// MLC ECC8 Decoding Flag Register
#define	HwECC_IREQ_M12EF			Hw23										// MLC ECC12 Encoding Flag Register
#define	HwECC_IREQ_M12DF			Hw22										// MLC ECC12 Decoding Flag Register
#define	HwECC_IREQ_M14EF			Hw25										// MLC ECC14 Encoding Flag Register
#define	HwECC_IREQ_M14DF			Hw24										// MLC ECC14 Decoding Flag Register
#define	HwECC_IREQ_M16EF			Hw27										// MLC ECC16 Encoding Flag Register
#define	HwECC_IREQ_M16DF			Hw26										// MLC ECC16 Decoding Flag Register
#define	HwECC_IREQ_M4DI				Hw2											// MLC ECC4 Decoding Interrupt Request Register
#define	HwECC_IREQ_M8DI				Hw4											// MLC ECC8 Decoding Interrupt Request Register
#define	HwECC_IREQ_M12DI			Hw6											// MLC ECC12 Decoding Interrupt Request Register
#define	HwECC_IREQ_M14DI			Hw8											// MLC ECC14 Decoding Interrupt Request Register
#define	HwECC_IREQ_M16DI			Hw10										// MLC ECC16 Decoding Interrupt Request Register
#define	HwECC_IREQ_CLR				(Hw27|Hw26|Hw25|Hw24|Hw23|Hw22|Hw21|Hw20|Hw19|Hw18|Hw17|Hw16|Hw10|Hw8|Hw6|Hw4|Hw2)

/*******************************************************************************
*	 28. Multi-Protocol Encapsulation Forward Error Correction (MPEFEC)
*    Register Define   (Base Addr = 0xF0510000)
********************************************************************************/
#define HwMPEFEC_BASE              *(volatile unsigned long *)0xF0510000

/*******************************************************************************
*	 29. IOBUS Configuration Register Define   (Base Addr = 0xF05F5000)
********************************************************************************/
#define HwIOBUSCFG_BASE            *(volatile unsigned long *)0xF05F5000
#define HwIOBUSCFG					((PIOBUSCFG)&HwIOBUSCFG_BASE)

// IOBUS AHB 0
#define HwIOBUSCFG_USB				Hw1											// USB2.0 OTG
#define HwIOBUSCFG_IDE				Hw2											// IDE Controller
#define HwIOBUSCFG_DMA				Hw3											// DMA Controller
#define HwIOBUSCFG_SD				Hw4											// SD/MMC Controller
#define HwIOBUSCFG_MS				Hw6											// Memory Stick Controller
#define HwIOBUSCFG_I2C				Hw7											// I2C Controller
#define HwIOBUSCFG_NFC				Hw8											// NFC Controller
#define HwIOBUSCFG_EHI0				Hw9											// External Host Interface 0
#define HwIOBUSCFG_EHI1				Hw10										// External Host Interface 1
#define HwIOBUSCFG_UART0			Hw11										// UART Controller 0
#define HwIOBUSCFG_UART1			Hw12										// UART Controller 1
#define HwIOBUSCFG_UART2			Hw13										// UART Controller 2
#define HwIOBUSCFG_UART3			Hw14										// UART Controller 3
#define HwIOBUSCFG_UART4			Hw15										// UART Controller 4
#define HwIOBUSCFG_UART5			Hw16										// UART Controller 5
#define HwIOBUSCFG_GPSB0			Hw17										// GPSB Controller 0
#define HwIOBUSCFG_GPSB1			Hw18										// GPSB Controller 1
#define HwIOBUSCFG_GPSB2			Hw19										// GPSB Controller 2
#define HwIOBUSCFG_GPSB3			Hw20										// GPSB Controller 3
#define HwIOBUSCFG_GPSB4			Hw21										// GPSB Controller 4
#define HwIOBUSCFG_GPSB5			Hw22										// GPSB Controller 5
#define HwIOBUSCFG_DAI				Hw23										// DAI/CDIF Interface
#define HwIOBUSCFG_ECC				Hw24										// ECC Calculator
#define HwIOBUSCFG_SPDIF			Hw25										// SPDIF Tx Controller
#define HwIOBUSCFG_RTC				Hw26										// RTC
#define HwIOBUSCFG_TSADC			Hw27										// TSADC Controller
#define HwIOBUSCFG_GPS				Hw28										// GPS Interface
#define HwIOBUSCFG_ADMA				Hw31										// Audio DMA Controller

// IOBUS AHB 1
#define HwIOBUSCFG_MPE				Hw0											// MPE_FEC
#define HwIOBUSCFG_TSIF				Hw1											// TSIF
#define HwIOBUSCFG_SRAM				Hw2											// SRAM Controller

#define	HwIOBUSCFG_STORAGE_ECC		~(Hw17|Hw16)							// Storage Bus
#define	HwIOBUSCFG_STORAGE_AHB_BUS1	Hw16							// I/O bus
#define	HwIOBUSCFG_STORAGE_AHB_BUS2	Hw17							// General purpose SRAM or DTCM
#define	HwIOBUSCFG_STORAGE_NFC		(Hw17|Hw16)						// Main processor data bus

/************************************************************************
*	Channel 0 Memory Controller Register Define	(Base Addr = 0xF1000000)
************************************************************************/
#define	HwEMC_BASE				   *(volatile unsigned long *)0xF1000000	// External Memory Controller Base Register

/*******************************************************************************
*	 TCC8900_DataSheet_PART 6 DDI_BUS_V0.00 Dec.11 2008
********************************************************************************/
/************************************************************************
*	4. LCD INTERFACE Register Define				(Base Addr = 0xF0200000)
************************************************************************/
#define	HwLCDC0_BASE				*(volatile unsigned long *)0xF0200000	// LCDC0 Control Base Register
#define HwLCDLUT0_BASE				*(volatile unsigned long *)0xF0200400	// LCD LUT 0 Base Register
#define	HwLCDC1_BASE				*(volatile unsigned long *)0xF0204000	// LCDC1 Control Base Register
#define HwLCDLUT1_BASE				*(volatile unsigned long *)0xF0204400	// LCD LUT 1 Base Register

/************************************************************************
*	5. LCD System Interface Register Define		(Base Addr = 0xF0200400)
************************************************************************/
#define	HwLCDSI_BASE				*(volatile unsigned long *)0xF020C400	// LCDSI Base Register

/***********************************************************************
*	6. Memory to Memory Scaler Register Define	(Base Addr = 0xF0210000/0xF0220000)
************************************************************************/
#define HwM2MSCALER0_BASE           *(volatile unsigned long *)0xF0210000
#define HwM2MSCALER1_BASE           *(volatile unsigned long *)0xF0220000

/************************************************************************
*	7. NTSC/PAL ENCODER Composite Output Register Define (Base Addr = 0xF9000000)
************************************************************************/
#define	HwTVE_BASE					*(volatile unsigned long *)0xF9000000	// TV Encoder Base Register

/************************************************************************
*	8. HDMI Register Define				(Base Addr = 0xF0254000)
************************************************************************/
//Controller register base address 
#define HwHDMICTRL_BASE				*(volatile unsigned long *)0xF0254000	//Controller register base address 
	
//HDMI register base address 
#define HwHDMICORE_BASE  			*(volatile unsigned long *)0xF0255000  

//AES register base address 
#define HwHDMIAES_BASE  			*(volatile unsigned long *)0xF0256000  //AES register base address 

//SPDIF Receiver register base address 
#define HwHDMISPDIF_BASE  			*(volatile unsigned long *)0xF0257000  

//I2S Receiver register base address 
#define HwHDMII2S_BASE  			*(volatile unsigned long *)0xF0258000  
				
 //CEC register base address 					
#define HwHDMICEC_BASE  			*(volatile unsigned long *)0xF0259000 

/***********************************************************************
*	 9-1. Camera Interface Register Define			(Base Addr = 0xF0230000)
************************************************************************/
#define	HwCIF_BASE					*(volatile unsigned long *)0xF0230000	// CIF Base Register


/***********************************************************************
*	 9-2. Effect Register Define			(Base Addr = 0xF0230100)
************************************************************************/
#define	HwCEM_BASE    				*(volatile unsigned long *)0xF0230100  //W/R  0x00000000  Effect mode register 

/***********************************************************************
*	 9-3. Scaler Register Define			(Base Addr = 0xF0230200)
************************************************************************/
#define	HwCSC_BASE    				*(volatile unsigned long *)0xF0230200  //W/R  0x00000000  Scaler configuration 

#define	HwMSC_BASE								*(volatile unsigned long *)0xF0210000	// MSC Base Register
#define	HwMSC_BASE_0							*(volatile unsigned long *)0xF0210000	// MSC #0 Base Register
#define	HwMSC_BASE_1							*(volatile unsigned long *)0xF0220000	// MSC #1 Base Register

#define	HwSRCBASEY								*(volatile unsigned long *)0xF0230200	// R/W, Source Base Address for Y

#define	HwSRCBASEU								*(volatile unsigned long *)0xF0230204	// R/W, Source Base Address for U(Cb)

#define	HwSRCBASEV								*(volatile unsigned long *)0xF0230208	// R/W, Source Base Address for V(Cr)

#define	HwSRCSIZE								*(volatile unsigned long *)0xF023020C	// R/W, Source Size Information Register
	#define	HwSRCSIZE_C_OFF(X)						((X)*Hw16)
	#define	HwSRCSIZE_C_OFF_MASK					HwSRCSIZE_C_OFF(4095)
	#define	HwSRCSIZE_Y_OFF(X)						((X)*Hw0)
	#define	HwSRCSIZE_Y_OFF_MASK					HwSRCSIZE_Y_OFF(4095)

#define	HwSRCOFF								*(volatile unsigned long *)0xF0230210	// R/W, Source Offset Information Register
	#define	HwSRCOFF_C_OFF(X)						((X)*Hw16)
	#define	HwSRCOFF_C_OFF_MASK					HwSRCOFF_C_OFF(4095)
	#define	HwSRCOFF_Y_OFF(X)						((X)*Hw0)
	#define	HwSRCOFF_Y_OFF_MASK					HwSRCOFF_Y_OFF(4095)

#define	HwSRCCFG								*(volatile unsigned long *)0xF0230214	// R/W, Source Configuration Register
	#define	HwSRCCFG_TYPE(X)						((X)*Hw0)
	#define	HwSRCCFG_422SEQ0						HwSRCCFG_TYPE(0)				// YUV 4:2:2 Sequential Mode 0
	#define	HwSRCCFG_422SEQ1						HwSRCCFG_TYPE(1)				// YUV 4:2:2 Sequential Mode 1
	#define	HwSRCCFG_422SEP						HwSRCCFG_TYPE(2)				// YUV 4:2:2 Separate Mode
	#define	HwSRCCFG_420SEP						HwSRCCFG_TYPE(3)				// YUV 4:2:0 Separate Mode
	#define	HwSRCCFG_565							HwSRCCFG_TYPE(4)				// RGB 565
	#define	HwSRCCFG_555							HwSRCCFG_TYPE(5)				// RGB 555
	#define	HwSRCCFG_444							HwSRCCFG_TYPE(6)				// RGB 444
	#define	HwSRCCFG_454							HwSRCCFG_TYPE(7)				// RGB 454
	#define	HwSRCCFG_TYPE_MASK					HwSRCCFG_TYPE(7)

#define	HwDSTBASEY								*(volatile unsigned long *)0xF0230220	// R/W, Destination Base Address for Y

#define	HwDSTBASEU								*(volatile unsigned long *)0xF0230224	// R/W, Destination Base Address for U(Cb)

#define	HwDSTBASEV								*(volatile unsigned long *)0xF0230228	// R/W, Destination Base Address for V(Cr)

#define	HwDSTSIZE								*(volatile unsigned long *)0xF023022C	// R/W, Destination Size Information Register
	#define	HwDSTSIZE_VSIZE(X)						((X)*Hw16)
	#define	HwDSTSIZE_VSIZE_MASK					HwDSTSIZE_VSIZE(4095)
	#define	HwDSTSIZE_HSIZE(X)						((X)*Hw0)
	#define	HwDSTSIZE_HSIZE_MASK					HwDSTSIZE_HSIZE(4095)

#define	HwDSTOFF								*(volatile unsigned long *)0xF0230230	// R/W, Destination Offset Information Register
	#define	HwDSTOFF_C_OFF(X)						((X)*Hw16)
	#define	HwDSTOFF_C_OFF_MASK					HwDSTOFF_C_OFF(4095)
	#define	HwDSTOFF_Y_OFF(X)						((X)*Hw0)
	#define	HwDSTOFF_Y_OFF_MASK					HwDSTOFF_Y_OFF(4095)

#define	HwDSTCFG								*(volatile unsigned long *)0xF0230234	// R/W, Destination Configuration Register
	#define	HwDSTCFG_COP							Hw11							// Chrominance Writing Mode Register Defined for 4:2:0 Seperate Mode
																					// 0 : Y0->U0->V0->Y1->Y2->U1...
																					// 1 : Y0->U0->Y1->V0->Y2->U1...
	#define	HwDSTCFG_WAITCNT(X)					((X)*Hw8)
	#define	HwDSTCFG_WAITCNT_MASK					HwDSTCFG_WAITCNT(7)
	#define	HwDSTCFG_RDY							Hw6								// Wait until Output FIFO is not empty
	#define	HwDSTCFG_RDY_WAITCNT					HwZERO							// Wait for [WAITCNT+1] cycle
	#define	HwDSTCFG_PATH_LCD						Hw4								// Destination Type Register (to LCD)
	#define	HwDSTCFG_PATH_MEM						HwZERO							// Destination Type Register (to Memory)
	#define	HwDSTCFG_TYPE(X)						((X)*Hw0)
	#define	HwDSTCFG_422SEQ0						HwDSTCFG_TYPE(0)				// 4:2:2 Sequential Mode 0
	#define	HwDSTCFG_422SEQ1						HwDSTCFG_TYPE(1)				// 4:2:2 Sequential Mode 1
	#define	HwDSTCFG_422SEP						HwDSTCFG_TYPE(2)				// 4:2:2 Separate Mode
	#define	HwDSTCFG_420SEP						HwDSTCFG_TYPE(3)				// 4:2:0 Separate Mode
	#define	HwDSTCFG_565							HwDSTCFG_TYPE(4)				// RGB 565
	#define	HwDSTCFG_555							HwDSTCFG_TYPE(5)				// RGB 555
	#define	HwDSTCFG_444							HwDSTCFG_TYPE(6)				// RGB 444
	#define	HwDSTCFG_454							HwDSTCFG_TYPE(7)				// RGB 454
	#define	HwDSTCFG_TYPE_MASK					HwDSTCFG_TYPE(7)

#define	HwSC_RATIO								*(volatile unsigned long *)0xF0230240	// R/W, Scaling Ratio Register
#define	HwMSCINF								*(volatile unsigned long *)0xF0230240	// R/W, Scaling Ratio Register
	#define	HwSC_RATIO_VRATIO(X)					((X)*Hw16)						// Vertical scale ratio (= 256*SRC_VSIZE/DST_VSIZE)
	#define	HwMSCINF_VRATIO						((X)*Hw16)
	#define	HwSC_RATIO_VRATIO_MASK				HwSC_RATIO_VRATIO(0x3FFF)
	#define	HwMSCINF_VRATIO_MASK					HwSC_RATIO_VRATIO(0x3FFF)
	#define	HwSC_RATIO_HRATIO(X)					((X)*Hw0)						// Horizontal scale ratio (= 256*SRC_HSIZE/DST_HSIZE)
	#define	HwMSCINF_HRATIO						((X)*Hw0)
	#define	HwSC_RATIO_HRATIO_MASK				HwSC_RATIO_HRATIO(0x3FFF)
	#define	HwMSCINF_HRATIO_MASK					HwSC_RATIO_HRATIO(0x3FFF)

#define	HwSC_CTRL								*(volatile unsigned long *)0xF0230244	// R/W, Scale Control Register
#define	HwMSCCTR								*(volatile unsigned long *)0xF0230244	// R/W, Scale Control Register
	#define	HwMSCCTR_FLS							Hw31							// Flush the internal FIFOs
	#define	HwMSCCTR_TST							Hw30							// Should be zero for test purpose
	#define	HwMSCCTR_REN_EN						Hw23							// Enable Rolling Operation Mode (can use rolling interrupt, rolling go-stop)
	#define	HwSC_CTRL_REN_EN						Hw23							// Enable Rolling Operation Mode (can use rolling interrupt, rolling go-stop)
	#define	HwMSCCTR_MEN_EN						Hw22							// Enable Middle Operation Mode (can use middle interrupt, middle go-stop)
	#define	HwSC_CTRL_MEN_EN						Hw22							// Enable Middle Operation Mode (can use middle interrupt, middle go-stop)
	#define	HwMSCCTR_RLS_RELEASE					Hw19							// Release stop mode (operation continue). This bit is cleared automatically
	#define	HwSC_CTRL_RLS_RELEASE					Hw19							// Release stop mode (operation continue). This bit is cleared automatically
	#define	HwMSCCTR_RGSM_EN						Hw17							// Enable Rolling Go-Stop Mode (scaler stop operation after DST_ROLL_CNT lines)
	#define	HwSC_CTRL_RGSM_EN						Hw17							// Enable Rolling Go-Stop Mode (scaler stop operation after DST_ROLL_CNT lines)
	#define	HwMSCCTR_MGSM_EN						Hw16							// Enable Middle Go-Stop Mode (scaler stop operation after DST_MID_CNT lines)
	#define	HwSC_CTRL_MGSM_EN						Hw16							// Enable Middle Go-Stop Mode (scaler stop operation after DST_MID_CNT lines)
	#define	HwMSCCTR_RIRQ_EN						Hw7								// Enable Rolling Interrupt
	#define	HwSC_CTRL_RIRQ_EN						Hw7								// Enable Rolling Interrupt
	#define	HwMSCCTR_MIRQ_EN						Hw6								// Enable Middle Interrupt
	#define	HwSC_CTRL_MIRQ_EN						Hw6								// Enable Middle Interrupt
	#define	HwMSCCTR_IEN_BUSY						Hw2								// Interrupt Enable Register
	#define	HwSC_CTRL_IEN_BUSY						Hw2								// Interrupt Enable Register
	#define	HwMSCCTR_IEN_RDY						Hw1								// Interrupt Enable Register
	#define	HwSC_CTRL_IEN_RDY						Hw1								// Interrupt Enable Register
	#define	HwMSCCTR_EN							Hw0								// Enable Register
	#define	HwSC_CTRL_EN							Hw0								// Enable Register

#define	HwSC_STATUS							*(volatile unsigned long *)0xF0230248	// R/W, Status Register
#define	HwMSCSTR								*(volatile unsigned long *)0xF0230248	// R/W, Status Register
	#define	HwMSCSTR_IR							Hw7								// Rolling Interrupt Flag (Cleared by writing this bit to 1)
	#define	HwSC_STATUS_IR							Hw7								// Rolling Interrupt Flag (Cleared by writing this bit to 1)
	#define	HwMSCSTR_IM							Hw6								// Middle Interrupt Flag (Cleared by writing this bit to 1)
	#define	HwSC_STATUS_IM							Hw6								// Middle Interrupt Flag (Cleared by writing this bit to 1)
	#define	HwMSCSTR_IBUSY							Hw5								// Busy Interrupt Flag
	#define	HwSC_STATUS_IBUSY						Hw5								// Busy Interrupt Flag
	#define	HwMSCSTR_IRDY							Hw4								// Ready Interrupt Flag
	#define	HwSC_STATUS_IRDY						Hw4								// Ready Interrupt Flag
	#define	HwMSCSTR_BUSY							Hw1								// Busy Status Register
	#define	HwSC_STATUS_BUSY						Hw1								// Busy Status Register
	#define	HwMSCSTR_RDY							Hw0								// Ready Status Register
	#define	HwSC_STATUS_RDY						Hw0								// Ready Status Register

#define	HwDST_RM_CNT							*(volatile unsigned long *)0xF0230250	// R/W, Rolling & Middle Counter
	#define	HwDST_RM_CNT_ROLL(X)					((X)*Hw16)
	#define	HwDST_RM_CNT_ROLL_MASK				HwDST_RM_CNT_ROLL(2047)
	#define	HwDST_RM_CNT_MID(X)					((X)*Hw0)
	#define	HwDST_RM_CNT_MID_MASK					HwDST_RM_CNT_MID(2047)

#define	HwC_R_CNT								*(volatile unsigned long *)0xF0230254	// R, Current Rolling Counter
	#define	HwC_R_CNT_ROLL							(HwC_R_CNT >> 16)



/***********************************************************************
*   10. Video and Image  Quality Enhancer Register Define	(Base Addr = 0xF0230200)
************************************************************************/
#define HwVIQE_BASE                 *(volatile unsigned long *)0xF0252000
#define HwVIQE_BASE1                 *(volatile unsigned long *)0xF0253000

/***********************************************************************
*   11. LVDS Register Define                	(Base Addr = 0xF0230200)
************************************************************************/
#define HwDDI_CONFIG_BASE           *(volatile unsigned long *)0xF0251000
	#define HwSWRESET_VIQE 		Hw1
	#define HwBCLKCTR_VIQE		Hw1
	#define HwSWRESET_M2MS 		Hw5
	#define HwBCLKCTR_M2MS		Hw5
	#define HwSWRESET_M2MS_0	Hw5
	#define HwBCLKCTR_M2MS_0	Hw5
	#define HwSWRESET_M2MS_1	Hw6
	#define HwBCLKCTR_M2MS_1	Hw6
#define HwDDI_CACHE_BASE            *(volatile unsigned long *)0xF0250000

/*******************************************************************************
*	 TCC8900_DataSheet_PART 7 VIDEO BUS_V0.00 Dec.11 2008
********************************************************************************/
/***********************************************************************
*   4. VIDEO CODEC Register Define                	(Base Addr = 0x0xF0700000)
************************************************************************/
#define HwVIDEOCODEC_BASE           *(volatile unsigned long *)0xF0700000

/***********************************************************************
*   5. JPEG CODEC Register Define                	(Base Addr = 0x0xF0710000/0xF0720000)
************************************************************************/
#define HwJPEGDECODER_BASE          *(volatile unsigned long *)0xF0710000
#define HwJPEGENCODER_BASE          *(volatile unsigned long *)0xF0720000
#define HwVIDEOCACHE_BASE           *(volatile unsigned long *)0xF0701000

/*******************************************************************************
*	 TCC8900_DataSheet_PART 8 GRAPHIC BUS_V0.00 Dec.11 2008
********************************************************************************/
/***********************************************************************
*	 4. Overlay Mixer Register Define	(Base Addr = 0xF6000000)
************************************************************************/
//#define HwOVERLAYMIXER_BASE         *(volatile unsigned long *)0xF6000000
#define HwOVERLAYMIXER_BASE         *(volatile unsigned long *)0xF0010000
#define Hw2D_DMA_BASE_ADDR	    *(volatile unsigned long *)0xF0010000					


#define	Hw2D_FCHO_SADDR0 	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR 	 ) 		// Front-End Channel 0 Source Address 0
#define	Hw2D_FCHO_SADDR1  	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x04)	// Front-End Channel 0 Source Address 1
#define	Hw2D_FCHO_SADDR2    	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x08) 	// Front-End Channel 0 Source Address 2
#define	Hw2D_FCHO_SFSIZE  	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x0C) 	// Front-End Channel 0 Source Frame Pixel Size
#define	Hw2D_FCHO_SOFF    		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x10) 	// Front-End Channel 0 Source Pixel Offset
#define	Hw2D_FCHO_SISIZE    	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x14) 	// Front-End Channel 0 Source Image Pixel Size
#define	Hw2D_FCHO_WOFF  		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x18) 	// Front-End Channel 0 Window Pixel Offset
#define	Hw2D_FCHO_SCTRL    	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x1C) 	// Front-End Channel 0 Control

#define	Hw2D_FCH1_SADDR0  	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x20) 	// Front-End Channel 1 Source Address 0
#define	Hw2D_FCH1_SADDR1    	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x24) 	// Front-End Channel 1 Source Address 1
#define	Hw2D_FCH1_SADDR2   	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x28) 	// Front-End Channel 1 Source Address 2
#define	Hw2D_FCH1_SFSIZE 		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x2C) 	// Front-End Channel 1 Source Frame Pixel Size
#define	Hw2D_FCH1_SOFF   		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x30) 	// Front-End Channel 1 Source Pixel Offset
#define	Hw2D_FCH1_SISIZE 		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x34) 	// Front-End Channel 1 Source Image Pixel Size
#define	Hw2D_FCH1_WOFF 		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x38) 	// Front-End Channel 1 Window Pixel Offset
#define	Hw2D_FCH1_SCTRL 		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x3C) 	// Front-End Channel 1 Control

#define	Hw2D_FCH2_SADDR0    	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x40) 	// Front-End Channel 2 Source Address 0
#define	Hw2D_FCH2_SADDR1  	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x44) 	// Front-End Channel 2 Source Address 1
#define	Hw2D_FCH2_SADDR2  	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x48) 	// Front-End Channel 2 Source Address 2
#define	Hw2D_FCH2_SFSIZE  		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x4C) 	// Front-End Channel 2 Source Frame Pixel Size
#define	Hw2D_FCH2_SOFF   		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x50) 	// Front-End Channel 2 Source Pixel Offset
#define	Hw2D_FCH2_SISIZE   	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x54) 	// Front-End Channel 2 Source Image Pixel Size
#define	Hw2D_FCH2_WOFF     	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x58) 	// Front-End Channel 2 Window Pixel Offset
#define	Hw2D_FCH2_SCTRL   		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x5C) 	// Front-End Channel 2 Control

#define	Hw2D_FCH_SCTRL_OPMODE  	(Hw8+Hw9+Hw10)		//Operation Mode
#define	Hw2D_FCH_SCTRL_ZF  		(Hw5)				//Zero Fill
#define	Hw2D_FCH_SCTRL_SDFRM  	(Hw0+Hw1+Hw2+Hw3+Hw4)			//Source Data Format	




#define	Hw2D_S0_CHROMA   		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x60) 	// Source 0 Chroma-Key Parameter
#define	Hw2D_S0_PAR 			*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x64) 	// Source 0 Arithmetic Parameter
#define	Hw2D_S1_CHROMA  		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x68) 	// Source 1 Chroma-Key Parameter
#define	Hw2D_S1_PAR  			*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x6C) 	// Source 1 Arithmetic Parameter
#define	Hw2D_S2_CHROMA   		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x70) 	// Source 2 Chroma-Key Parameter
#define	Hw2D_S2_PAR   			*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x74) 	// Source 2 Arithmetic Parameter
#define	Hw2D_SCTRL   			*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x78) 	// Source Control Register

#define   Hw2D_SCTRL_S2_ARITHMODE 	(Hw27+Hw26+Hw25)
#define   Hw2D_SCTRL_S1_ARITHMODE 	(Hw24+Hw23+Hw22)
#define   Hw2D_SCTRL_S0_ARITHMODE 	(Hw21+Hw20+Hw19)
#define   Hw2D_SCTRL_S2_Y2REN 		(Hw18)
#define   Hw2D_SCTRL_S1_Y2REN 		(Hw17)
#define   Hw2D_SCTRL_S0_Y2REN 		(Hw16)
#define   Hw2D_SCTRL_S2_Y2RMODE 	(Hw14+Hw13)
#define   Hw2D_SCTRL_S1_Y2RMODE 	(Hw12+Hw11)
#define   Hw2D_SCTRL_S0_Y2RMODE 	(Hw10+Hw9)
#define   Hw2D_SCTRL_S2_CHROMAEN	(Hw8)
#define   Hw2D_SCTRL_S1_CHROMAEN	(Hw7)
#define   Hw2D_SCTRL_S0_CHROMAEN	(Hw6)
#define   Hw2D_SCTRL_S2_SEL			(Hw5+Hw4)
#define   Hw2D_SCTRL_S1_SEL			(Hw3+Hw2)
#define   Hw2D_SCTRL_S0_SEL			(Hw1+Hw0)



#define	Hw2D_OP0_PAT   		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x80) 	// Source Operator 0 Pattern
#define	Hw2D_OP1_PAT   		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x84) 	// Source Operator 1 Pattern
#define	Hw2D_OP_CTRL   		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x88) 	// Source Operation Control Register

#define	Hw2D_OP_CTRL_CSEL1  		(Hw22+Hw21)
#define	Hw2D_OP_CTRL_OP1_MODE  	(Hw20+Hw19+Hw18+Hw17+Hw16)
#define	Hw2D_OP_CTRL_CSEL0  		(Hw6+Hw5)
#define	Hw2D_OP_CTRL_OP0_MODE  	(Hw4+Hw3+Hw2+Hw1+Hw0)



#define	Hw2D_BCH_DADDR0   	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x90) 	// Back-End Channel Destination Address 0
#define	Hw2D_BCH_DADDR1   	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x94) 	// Back -End Channel Destination Address 1
#define	Hw2D_BCH_DADDR2   	*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x98) 	// Back -End Channel Destination Address 2
#define	Hw2D_BCH_DFSIZE   		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0x9C) 	// Back -End Channel Destination Frame Pixel Size
#define	Hw2D_BCH_DOFF   		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0xA0) 	// Back -End Channel Destination Pixel Offset
#define	Hw2D_BCH_DCTRL   		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0xA4) 	// Back -End Channel Control

#define   Hw2D_BCH_DCTRL_YSEL		(Hw21)
#define   Hw2D_BCH_DCTRL_YSEL		(Hw18)
#define   Hw2D_BCH_DCTRL_XSEL		(Hw17+Hw16)
#define   Hw2D_BCH_DCTRL_Y2R		(Hw15)
#define   Hw2D_BCH_DCTRL_Y2RMODE	(Hw14+Hw13)
#define   Hw2D_BCH_DCTRL_DSUV		(Hw11)
#define   Hw2D_BCH_DCTRL_OPMODE	(Hw10+Hw9+Hw8)
#define   Hw2D_BCH_DCTRL_DOP		(Hw6)
#define   Hw2D_BCH_DCTRL_DEN		(Hw5)
#define   Hw2D_BCH_DCTRL_DDFRM	(Hw4+Hw3+Hw2+Hw1+Hw0)



#define	Hw2D_GE_CTRL   		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0xC0) 	// Graphic Engine Control

#define	Hw2D_GE_CTRL_IEN			(Hw16)
#define	Hw2D_GE_CTRL_EN			(Hw0+Hw1+Hw2)



#define	Hw2D_GE_IREQ   		*(volatile unsigned long *)(Hw2D_DMA_BASE_ADDR +0xC4) 	// Graphic Engine Interrupt Request

#define	Hw2D_GE_IREQ_FLG			(Hw16)   
#define	Hw2D_GE_IREQ_IRQ			(Hw0)


/*******************************************************************************
*	 5-1. 2D/3D GPU
*
*	 Pixel Processor Register Map Register Define   (Base Addr = 0xF0000000)
********************************************************************************/
#define HwPIXELPROCESSOR_BASE       *(volatile unsigned long *)0xF0000000

/*******************************************************************************
*	 5-2. Geometry Processor Register Map Register Define   (Base Addr = 0xF0000000)
********************************************************************************/
#define HwGEOMETRYPROCESSOR_BASE    *(volatile unsigned long *)0xF0002000

/*******************************************************************************
*	 5-3. MMU Configuration Register Define   (Base Addr = 0xF0003000)
********************************************************************************/
#define HwMMUCONFIG_BASE            *(volatile unsigned long *)0xF0003000

/*******************************************************************************
*	 5-4. GRPBUS Configuration Register Define   (Base Addr = 0xF0004000)
********************************************************************************/
#define HwGRPBUS_BASE               *(volatile unsigned long *)0xF0004000

/*******************************************************************************
*	 5-5. GRPBUS BWRAP Register Define   (Base Addr = 0xF0005000)
********************************************************************************/
#define HwGRPBUSBWRAP_BASE          *(volatile unsigned long *)0xF0005000

#endif						
