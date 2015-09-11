#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "viqe_cmd.h"
#define TRUE 1
#define FALSE 0

TCC_CUI_VIQE Tcc_Cui_VIQE;	//	Viqe_cmd.h extern TCC_CUI_VIQE Tcc_Cui_VIQE;

/*
TCC_CUI_CMD_t	TCC_Cui_Cmd [] =
{
	{"show_state", 		10, 		TCC_CUI_VIQE_SHOW_STATE	},
	{"turn", 		4, 		TCC_CUI_VIQE_TURN	},
	{"conf_deintl", 	11, 		TCC_CUI_VIQE_CONF_DEINTL},
	{"conf_dnts", 		9, 		TCC_CUI_VIQE_CONF_DNTS	},
	{"conf_dnsp", 		9, 		TCC_CUI_VIQE_CONF_DNSP	},
	{"conf_hpf", 		8, 		TCC_CUI_VIQE_CONF_HPF	},
	{"conf_hiseq", 		10, 		TCC_CUI_VIQE_CONF_HISEQ	},
	{"conf_gamut", 		10, 		TCC_CUI_VIQE_CONF_GAMUT	},
	{"viqe_help",		9,		TCC_CUI_VIQE_CONF_HELP	},
	{0,			0,		0			}
};
*/

pTCC_CUI_VIQE Get_Tcc_Cui_VIQE ()
{
	return &Tcc_Cui_VIQE;
}

void TCC_CUI_VIQE_Menu_Display ()
{
	printf("\n############# Video Quality Enhancer Command #############\n");
	printf("#Show VIQE state --->: show_state [all | deintl | dnts | dnsp | hpf | hiseq | gamut | viqe | dma | deblock ]\n");
	printf("#Trun (on/off) filter --->: turn [on|off] [deintl | dnts | dnsp | hpf | hiseq | gamut | huff | mon | scale]\n");
	printf("#Config deinterlacer ---> : conf_deintl -spatial:[on|off] -dm_mode:[on|off] -film_mode:[on|off]\n");
	printf("#Config denoiser (temporal/spatial) ---> : conf_dnts -s:[spatial_luma_strength(0-4):spatial_chroma_strength(0-4)] -t:[temporal_luma_strength(0-4):temporal_chroma_strength(0-4)]\n");
	printf("Config denoiser (spatial) ---> : conf_dnsp -s:[strength(0-4)]\n");
	printf("Config high pass filter ---> : conf_hpf -s:[strength(0-4)]\n");
	printf("Config histogram equalizer ---> : conf_hiseq -s:[strength(0-255)] -m:[multiframe_number]\n");
	printf("Config gamut mapper ---> : conf_gamut -a:[algorithm_number] -s:[strength](1.0-inf.) -r:[strength] -g:[strength] -b:[strength]\n");
	printf("Randomly config VIQE ---> : conf_random [on|off]\n");
	printf("Half processing mode ---> : half_mode [on|off]\n");
	printf("step by step mode --> : step [on|off]\n");
	printf("config huffman --> : conf_huff -s:[strength]\n");
	return;
}
/*
int TCC_CUI_Parse_CMD(char* Buffer)
{
	int i;
	for( i = 0 ; i < TCC_CUI_NUM ; i ++ )
	{
		if( strncmp(Buffer,TCC_Cui_Cmd[i].Cmd,TCC_Cui_Cmd[i].Cmd_Size) == 0 )
		{
			return (TCC_Cui_Cmd[i].Index );
		}
	}
	return( TCC_CUI_NULL);

}
*/
void TCC_CUI_VIQE_ShowDeintl (int En)
{
	if (En == FALSE) return;
	printf ("\n********** Deintl **********\n");
	printf ("onoff : %d\n", Tcc_Cui_VIQE.Deintl.OnOff);
	printf ("spatial : %d\n", Tcc_Cui_VIQE.Deintl.SpatialFlag);
	printf ("odd first : %d\n", Tcc_Cui_VIQE.Deintl.OddFirstFlag);
	printf ("manual mode : %d\n", Tcc_Cui_VIQE.Deintl.ManualModeFlag);
	return;
}

void TCC_CUI_VIQE_ShowDnts (int En)
{
	if (En == FALSE) return;
	printf ("\n********** Dnts **********\n");
	printf ("onoff : %d\n", Tcc_Cui_VIQE.Dnts.OnOff);
	printf ("spatial luma strength : %d\n", Tcc_Cui_VIQE.Dnts.SpatialLumaStrength);
	printf ("spatial chroma strength : %d\n", Tcc_Cui_VIQE.Dnts.SpatialChromaStrength);
	printf ("tmp luma strength : %d\n", Tcc_Cui_VIQE.Dnts.TmpLumaStrength);
	printf ("tmp chroma strength : %d\n", Tcc_Cui_VIQE.Dnts.TmpChromaStrength);
	return;

}

void TCC_CUI_VIQE_ShowDnsp (int En)
{
	if (En == FALSE) return;
	printf ("\n********** Dnsp **********\n");
	printf ("onoff : %d\n", Tcc_Cui_VIQE.Dnsp.OnOff);
	printf ("strength : %d\n", Tcc_Cui_VIQE.Dnsp.Strength);
}

void TCC_CUI_VIQE_ShowHpf (int En)
{
	if (En == FALSE) return;
	printf ("\n********** Hpf **********\n");
	printf ("onoff : %d\n", Tcc_Cui_VIQE.Hpf.OnOff);
	printf ("luma_strength : %d\n", Tcc_Cui_VIQE.Hpf.LumaStrength);
	printf ("chroma_strength : %d\n", Tcc_Cui_VIQE.Hpf.ChromaStrength);
	return;
}

void TCC_CUI_VIQE_ShowHiseq (int En)
{
	if (En == FALSE) return;
	printf ("\n********** Hiseq **********\n");
	printf ("onoff : %d\n", Tcc_Cui_VIQE.Hiseq.OnOff);
	printf ("strength : %d\n", Tcc_Cui_VIQE.Hiseq.Strength);
	printf ("multi frame : %d\n", Tcc_Cui_VIQE.Hiseq.MultiFrame);
	return;
}

void TCC_CUI_VIQE_ShowGamut (int En)
{
	#if 0
	if (En == FALSE) return;
	printf ("\n********** Gamut **********\n");
	printf ("onoff : %d\n", Tcc_Cui_VIQE.Gamut.OnOff);
	printf ("algorithm number : %d\n", Tcc_Cui_VIQE.Gamut.AlgorithmNumber);
	printf ("luv mode : %d\n", Tcc_Cui_VIQE.Gamut.LUVMode);
	printf ("default strength : %lf\n", Tcc_Cui_VIQE.Gamut.DefaultStrength);
	printf ("StrYishG : %lf\n", Tcc_Cui_VIQE.Gamut.StrYishG);
	printf ("StrYG : %lf\n", Tcc_Cui_VIQE.Gamut.StrYG);
	printf ("StrGishY : %lf\n", Tcc_Cui_VIQE.Gamut.StrGishY);
	printf ("StrY : %lf\n", Tcc_Cui_VIQE.Gamut.StrY);
	printf ("StrYishO : %lf\n", Tcc_Cui_VIQE.Gamut.StrYishO);
	printf ("StrO : %lf\n", Tcc_Cui_VIQE.Gamut.StrO);
	printf ("StrOPin : %lf\n", Tcc_Cui_VIQE.Gamut.StrOPin);
	printf ("StrPin : %lf\n", Tcc_Cui_VIQE.Gamut.StrPin);
	printf ("StrPishPin : %lf\n", Tcc_Cui_VIQE.Gamut.StrPishPin);
	printf ("StrRishO : %lf\n", Tcc_Cui_VIQE.Gamut.StrRishO);
	printf ("StrR : %lf\n", Tcc_Cui_VIQE.Gamut.StrR);
	printf ("StrPishR : %lf\n", Tcc_Cui_VIQE.Gamut.StrPishR);
	printf ("StrRP : %lf\n", Tcc_Cui_VIQE.Gamut.StrRP);
	printf ("StrRishP : %lf\n", Tcc_Cui_VIQE.Gamut.StrRishP);
	printf ("StrP : %lf\n", Tcc_Cui_VIQE.Gamut.StrP);
	printf ("StrBishP : %lf\n", Tcc_Cui_VIQE.Gamut.StrBishP);
	printf ("StrPishB : %lf\n", Tcc_Cui_VIQE.Gamut.StrPishB);
	printf ("StrB : %lf\n", Tcc_Cui_VIQE.Gamut.StrB);
	printf ("StrGishB : %lf\n", Tcc_Cui_VIQE.Gamut.StrGishB);
	printf ("StrBG : %lf\n", Tcc_Cui_VIQE.Gamut.StrBG);
	printf ("StrBishG : %lf\n", Tcc_Cui_VIQE.Gamut.StrBishG);
	printf ("StrG : %lf\n", Tcc_Cui_VIQE.Gamut.StrG);
	return;
	#endif
}

void TCC_CUI_VIQE_ShowRandomMode (int En)
{
	if (En == FALSE) return;
	printf ("\n********** RandomMode **********\n");
	printf ("onoff : %d\n", Tcc_Cui_VIQE.RandomMode);
	return;
}

void TCC_CUI_VIQE_ShowHalfProcessing (int En)
{
	if (En == FALSE) return;
	printf ("\n********** HalfProcessing **********\n");
	printf ("onoff : %d\n", Tcc_Cui_VIQE.HalfProcessing);
	return;
}

void TCC_CUI_VIQE_ShowScaleOnOff (int En)
{
	if (En == FALSE) return;
	printf ("\n********** ScaleOnOff **********\n");
	printf ("onoff : %d\n", !(Tcc_Cui_VIQE.ScaleOnOff));
	return;
}

void TCC_CUI_VIQE_ShowDeblockOnOff (int En)
{
	if (En == FALSE) return;
	printf ("\n********** Deblocking **********\n");
	printf ("onoff : %d\n", !(Tcc_Cui_VIQE.DeblockOnOff));
	return;
}

void TCC_CUI_VIQE_ShowState (char *Buffer)
{
	int ShowDeintl = FALSE;
	int ShowDnts = FALSE;
	int ShowDnsp = FALSE;
	int ShowHpf = FALSE;
	int ShowHiseq = FALSE;
	int ShowGamut = FALSE;
	if (strstr (Buffer, "all"))
	{
		ShowDeintl = TRUE;
		ShowDnts = TRUE;
		ShowDnsp = TRUE;
		ShowHpf = TRUE;
		ShowHiseq = TRUE;
		ShowGamut = TRUE;
	}
	if (strstr (Buffer, "deintl")) ShowDeintl = TRUE;
	if (strstr (Buffer, "dnts")) ShowDnts = TRUE;
	if (strstr (Buffer, "dnsp")) ShowDnsp = TRUE;
	if (strstr (Buffer, "hpf")) ShowHpf = TRUE;
	if (strstr (Buffer, "hiseq")) ShowHiseq = TRUE;
	if (strstr (Buffer, "gamut")) ShowGamut = TRUE;
	if (strstr (Buffer, "viqe")) ShowVIQE ();
	TCC_CUI_VIQE_ShowRandomMode (1);
	TCC_CUI_VIQE_ShowHalfProcessing (1);
	TCC_CUI_VIQE_ShowScaleOnOff (1);
	TCC_CUI_VIQE_ShowDeintl (ShowDeintl);
	TCC_CUI_VIQE_ShowDnts (ShowDnts);
	TCC_CUI_VIQE_ShowDnsp (ShowDnsp);
	TCC_CUI_VIQE_ShowHpf (ShowHpf);
	TCC_CUI_VIQE_ShowHiseq (ShowHiseq);
	TCC_CUI_VIQE_ShowGamut (ShowGamut);
	return;
}

void TCC_CUI_VIQE_StepOn (char *Buffer)
{

	if (strstr (Buffer, "on")) 
	{
		Tcc_Cui_VIQE.StepOnOff = 1;
		Tcc_Cui_VIQE.StepEvent = 0;
	}
	else if (strstr (Buffer, "off")) 
	{
		Tcc_Cui_VIQE.StepOnOff = 0;
		Tcc_Cui_VIQE.StepEvent = 0;
	}
	else
	{
		Tcc_Cui_VIQE.StepEvent = 1;
	}
	return;
}

void TCC_CUI_VIQE_Turn (char *Buffer)
{
	int OnOff;
	if (strstr (Buffer, "on")) OnOff = TRUE;
	else if (strstr (Buffer, "off")) OnOff = FALSE;
	if (strstr (Buffer, "deintl")) Tcc_Cui_VIQE.Deintl.OnOff = OnOff;
	if (strstr (Buffer, "dnts")) Tcc_Cui_VIQE.Dnts.OnOff = OnOff;
	if (strstr (Buffer, "dnsp")) Tcc_Cui_VIQE.Dnsp.OnOff = OnOff;
	if (strstr (Buffer, "hpf")) Tcc_Cui_VIQE.Hpf.OnOff = OnOff;
	if (strstr (Buffer, "hiseq")) Tcc_Cui_VIQE.Hiseq.OnOff = OnOff;
	if (strstr (Buffer, "gamut")) 
	{
		Tcc_Cui_VIQE.Gamut.OnOff = OnOff;
		if (OnOff == 1)
		{
			GamutInterfaceMain (&Tcc_Cui_VIQE);
/*			GPLLUTGen (Tcc_Cui_VIQE.Gamut.Bright,
			Tcc_Cui_VIQE.Gamut.Cont + 100,
			Tcc_Cui_VIQE.Gamut.Hue,
			Tcc_Cui_VIQE.Gamut.Sat + 100);*/
/*			CLUTGen_Interface_Example (
				Tcc_Cui_VIQE.Gamut.LUVMode,
				Tcc_Cui_VIQE.Gamut.StrYishG, 
				Tcc_Cui_VIQE.Gamut.StrYG, 
				Tcc_Cui_VIQE.Gamut.StrGishY,
				Tcc_Cui_VIQE.Gamut.StrY, 
				Tcc_Cui_VIQE.Gamut.StrYishO, 
				Tcc_Cui_VIQE.Gamut.StrO,
				Tcc_Cui_VIQE.Gamut.StrOPin, 
				Tcc_Cui_VIQE.Gamut.StrPin, 
				Tcc_Cui_VIQE.Gamut.StrPishPin,
				Tcc_Cui_VIQE.Gamut.StrRishO, 
				Tcc_Cui_VIQE.Gamut.StrR, 
				Tcc_Cui_VIQE.Gamut.StrPishR,
				Tcc_Cui_VIQE.Gamut.StrRP, 
				Tcc_Cui_VIQE.Gamut.StrRishP, 
				Tcc_Cui_VIQE.Gamut.StrP,
				Tcc_Cui_VIQE.Gamut.StrBishP, 
				Tcc_Cui_VIQE.Gamut.StrPishB, 
				Tcc_Cui_VIQE.Gamut.StrB,
				Tcc_Cui_VIQE.Gamut.StrGishB, 
				Tcc_Cui_VIQE.Gamut.StrBG, 
				Tcc_Cui_VIQE.Gamut.StrBishG,
				Tcc_Cui_VIQE.Gamut.StrG
			);*/
			Tcc_Cui_VIQE.Gamut.LUTUpdated = 1;
		}
	}
	if (strstr (Buffer, "huff")) Tcc_Cui_VIQE.Huff.OnOff = OnOff;
	if (strstr (Buffer, "mon")) Tcc_Cui_VIQE.Monitor = OnOff;
	if (strstr (Buffer, "scale")) Tcc_Cui_VIQE.ScaleOnOff = !OnOff;
	if (strstr (Buffer, "deblock")) Tcc_Cui_VIQE.DeblockOnOff = OnOff;
	return;
}

void TCC_CUI_VIQE_ConfHuff (char *Buffer)
{
	char *StartPoint;
	int Mode;
	StartPoint = strstr (Buffer, "-s:");
	if (StartPoint)
	{
		sscanf (StartPoint + 3, "%d", &Mode);
		Tcc_Cui_VIQE.Huff.Mode = Mode;
	}
	return;
}

void TCC_CUI_VIQE_ConfDeintl (char *Buffer)
{
	char *StartPoint;
	int OnOff;
	StartPoint = strstr (Buffer, "-spatial:");
	if (StartPoint)
	{
		StartPoint += 9;
		if (!strncmp (StartPoint, "on", 2)) OnOff = TRUE;
		else if (!strncmp (StartPoint, "off", 3)) OnOff = FALSE;
		Tcc_Cui_VIQE.Deintl.SpatialFlag = OnOff;
	}
	StartPoint = strstr (Buffer, "-odd_first:");
	if (StartPoint)
	{
		StartPoint += 11;
		if (!strncmp (StartPoint, "on", 2)) OnOff = TRUE;
		else if (!strncmp (StartPoint, "off", 3)) OnOff = FALSE;
		Tcc_Cui_VIQE.Deintl.OddFirstFlag = OnOff;
	}
	StartPoint = strstr (Buffer, "-manual_mode:");
	if (StartPoint)
	{
		StartPoint += 13;
		if (!strncmp (StartPoint, "on", 2)) OnOff = TRUE;
		else if (!strncmp (StartPoint, "off", 3)) OnOff = FALSE;
		Tcc_Cui_VIQE.Deintl.ManualModeFlag = OnOff;
	}
	return;
}

void TCC_CUI_VIQE_ConfDnts (char *Buffer)
{
	char *StartPoint;
	int SpatialLumaStrength;
	int SpatialChromaStrength;
	int TmpLumaStrength;
	int TmpChromaStrength;
	
	StartPoint = strstr (Buffer, "-s:");
	if (StartPoint)
	{
		sscanf (StartPoint + 3, "%d:%d", &SpatialLumaStrength, &SpatialChromaStrength);
		Tcc_Cui_VIQE.Dnts.SpatialLumaStrength = SpatialLumaStrength;
		Tcc_Cui_VIQE.Dnts.SpatialChromaStrength = SpatialChromaStrength;
	}
	StartPoint = strstr (Buffer, "-t:");
	if (StartPoint)
	{
		sscanf (StartPoint + 3, "%d:%d", &TmpLumaStrength, &TmpChromaStrength);
		Tcc_Cui_VIQE.Dnts.TmpLumaStrength = TmpLumaStrength;
		Tcc_Cui_VIQE.Dnts.TmpChromaStrength = TmpChromaStrength;
	}
	return;
}

void TCC_CUI_VIQE_ConfDnsp (char *Buffer)
{
	char *StartPoint;
	int Strength;
	StartPoint = strstr (Buffer, "-s:");
	if (StartPoint)
	{
		sscanf (StartPoint + 3, "%d", &Strength);
		Tcc_Cui_VIQE.Dnsp.Strength = Strength;
	}
	return;
}

void TCC_CUI_VIQE_ConfHpf (char *Buffer)
{
	char *StartPoint;
	int LumaStrength;
	int ChromaStrength;
	StartPoint = strstr (Buffer, "-s:");
	if (StartPoint)
	{
		sscanf (StartPoint + 3, "%d:%d", &LumaStrength, &ChromaStrength);
		Tcc_Cui_VIQE.Hpf.LumaStrength = LumaStrength;
		Tcc_Cui_VIQE.Hpf.ChromaStrength = ChromaStrength;
	}
	return;
}

void TCC_CUI_VIQE_ConfHiseq (char *Buffer)
{
	char *StartPoint;
	int Strength;
	int MultiFrame;
	int i;
	StartPoint = strstr (Buffer, "-s:");
	if (StartPoint)
	{
		sscanf (StartPoint + 3, "%d", &Strength);
		for (i = 0; i < 16; i++)
		{
			Tcc_Cui_VIQE.Hiseq.Strength[i] = Strength;
		}
	}
	StartPoint = strstr (Buffer, "-d");
	if (StartPoint)
	{
		sscanf (StartPoint + 3, "%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d", 
			&Tcc_Cui_VIQE.Hiseq.Strength[0],
			&Tcc_Cui_VIQE.Hiseq.Strength[1],
			&Tcc_Cui_VIQE.Hiseq.Strength[2],
			&Tcc_Cui_VIQE.Hiseq.Strength[3],
			&Tcc_Cui_VIQE.Hiseq.Strength[4],
			&Tcc_Cui_VIQE.Hiseq.Strength[5],
			&Tcc_Cui_VIQE.Hiseq.Strength[6],
			&Tcc_Cui_VIQE.Hiseq.Strength[7],
			&Tcc_Cui_VIQE.Hiseq.Strength[8],
			&Tcc_Cui_VIQE.Hiseq.Strength[9],
			&Tcc_Cui_VIQE.Hiseq.Strength[10],
			&Tcc_Cui_VIQE.Hiseq.Strength[11],
			&Tcc_Cui_VIQE.Hiseq.Strength[12],
			&Tcc_Cui_VIQE.Hiseq.Strength[13],
			&Tcc_Cui_VIQE.Hiseq.Strength[14],
			&Tcc_Cui_VIQE.Hiseq.Strength[15]
			);
	}
	StartPoint = strstr (Buffer, "-m");
	if (StartPoint)
	{
		sscanf (StartPoint + 3, "%d", &MultiFrame);
		Tcc_Cui_VIQE.Hiseq.MultiFrame = MultiFrame;
	}
	return;
}

void TCC_CUI_VIQE_ConfGamut (char *Buffer)
{
	char *StartPoint;
	double Strength;
	int BrightStr, HueStr, ContStr, SatStr;
	float DefaultStrength;
	int AlgorithmNumber;
	int iStrength;
	char GMFileName [100];
	int CurAlgMode = -1;
	StartPoint = strstr (Buffer, "-a:");
	if (StartPoint)
	{
		sscanf (StartPoint + 3, "%d", &AlgorithmNumber);
		Tcc_Cui_VIQE.Gamut.AlgorithmNumber = AlgorithmNumber;
	}

	if (Tcc_Cui_VIQE.Gamut.AlgorithmNumber == 0 || Tcc_Cui_VIQE.Gamut.AlgorithmNumber == 1)
		CurAlgMode = 0;
	else if (Tcc_Cui_VIQE.Gamut.AlgorithmNumber == 2 || Tcc_Cui_VIQE.Gamut.AlgorithmNumber == 3)
		CurAlgMode = 1;
	
	StartPoint = strstr (Buffer, "-Default:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 9, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrDefault = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 9, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrDefault = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrDefault = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrDefault = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrDefault = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-YishG:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 7, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrYishG = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 7, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrYishG = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrYishG = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrYishG = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrYishG = SatStr;	
		}
	}
	StartPoint = strstr (Buffer, "-YG:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 4, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrYG = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 4, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrYG = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrYG = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrYG = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrYG = SatStr;	
		}
	}
	StartPoint = strstr (Buffer, "-GishY:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 7, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrGishY = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 7, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrGishY = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrGishY = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrGishY = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrGishY = SatStr;	
		}
	}
	StartPoint = strstr (Buffer, "-Y:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 3, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrY = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 3, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrY = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrY = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrY = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrY = SatStr;	
		}
	}
	StartPoint = strstr (Buffer, "-YishO:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 7, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrYishO = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 7, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrYishO = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrYishO = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrYishO = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrYishO = SatStr;	
		}
	}
	StartPoint = strstr (Buffer, "-O:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 3, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrO = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 3, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrO = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrO = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrO = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrO = SatStr;	
		}
	}
	StartPoint = strstr (Buffer, "-OPin:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 6, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrOPin = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 6, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrOPin = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrOPin = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrOPin = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrOPin = SatStr;	
		}
	}
	StartPoint = strstr (Buffer, "-Pin:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 5, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrPin = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 5, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrPin = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrPin = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrPin = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrPin = SatStr;	
		}
	}
	StartPoint = strstr (Buffer, "-PishPin:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 9, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrPishPin = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 9, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrPishPin = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrPishPin = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrPishPin = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrPishPin = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-RishO:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 7, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrRishO = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 7, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrRishO = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrRishO = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrRishO = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrRishO = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-R:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 3, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrR = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 3, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrR = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrR = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrR = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrR = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-PishR:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 7, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrPishR = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 7, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrPishR = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrPishR = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrPishR = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrPishR = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-RP:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 4, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrRP = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 4, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrRP = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrRP = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrRP = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrRP = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-RishP:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 7, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrRishP = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 7, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrRishP = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrRishP = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrRishP = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrRishP = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-P:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 3, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrP = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 3, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrP = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrP = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrP = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrP = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-BishP:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 7, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrBishP = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 7, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrBishP = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrBishP = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrBishP = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrBishP = SatStr;
		}
	}

	StartPoint = strstr (Buffer, "-PishB:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 7, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrPishB = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 7, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrPishB = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrPishB = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrPishB = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrPishB = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-B:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 3, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrB = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 3, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrB = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrB = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrB = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrB = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-GishB:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 7, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrGishB = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 7, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrGishB = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrGishB = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrGishB = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrGishB = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-BG:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 4, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrBG = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 4, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrBG = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrBG = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrBG = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrBG = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-BishG:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 7, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrBishG = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 7, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrBishG = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrBishG = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrBishG = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrBishG = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-G:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 3, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrG = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 3, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrG = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrG = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrG = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrG = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-luv");
	if (StartPoint)
	{
		Tcc_Cui_VIQE.Gamut.LUVMode = 1;
	}
	StartPoint = strstr (Buffer, "-xy");
	if (StartPoint)
	{
		Tcc_Cui_VIQE.Gamut.LUVMode = 0;
	}
	StartPoint = strstr (Buffer, "-XR:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 4, "%lf", &Strength);
//			Tcc_Cui_VIQE.Gamut.Alg01.StrOPin = Strength;
//			Tcc_Cui_VIQE.Gamut.Alg01.StrRishO = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrPin = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrR = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrPishPin = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrPishR = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 4, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
//			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrOPin = BrightStr;
//			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrRishO = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrPin = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrR = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrPishPin = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrPishR = BrightStr;

//			Tcc_Cui_VIQE.Gamut.Alg23.HueStrOPin = HueStr;
//			Tcc_Cui_VIQE.Gamut.Alg23.HueStrRishO = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrPin = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrR = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrPishPin = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrPishR = HueStr;

//			Tcc_Cui_VIQE.Gamut.Alg23.ContStrOPin = ContStr;
//			Tcc_Cui_VIQE.Gamut.Alg23.ContStrRishO = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrPin = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrR = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrPishPin = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrPishR = ContStr;

//			Tcc_Cui_VIQE.Gamut.Alg23.SatStrOPin = SatStr;
//			Tcc_Cui_VIQE.Gamut.Alg23.SatStrRishO = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrPin = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrR = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrPishPin = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrPishR = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-XG:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 4, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrBishG = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrG = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrYishG = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 4, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrBishG = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrG = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrYishG = BrightStr;

			Tcc_Cui_VIQE.Gamut.Alg23.HueStrBishG = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrG = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrYishG = HueStr;

			Tcc_Cui_VIQE.Gamut.Alg23.ContStrBishG = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrG = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrYishG = ContStr;

			Tcc_Cui_VIQE.Gamut.Alg23.SatStrBishG = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrG = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrYishG = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-XB:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 4, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrBG = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrGishB = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrB = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrPishB = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrBishP = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrP = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrRishP = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrRP = Strength;
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 4, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrBG = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrGishB = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrB = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrPishB = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrBishP = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrP = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrRishP = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrRP = BrightStr;

			Tcc_Cui_VIQE.Gamut.Alg23.HueStrBG = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrGishB = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrB = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrPishB = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrBishP = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrP = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrRishP = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrRP = HueStr;

			Tcc_Cui_VIQE.Gamut.Alg23.ContStrBG = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrGishB = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrB = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrPishB = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrBishP = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrP = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrRishP = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrRP = ContStr;

			Tcc_Cui_VIQE.Gamut.Alg23.SatStrBG = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrGishB = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrB = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrPishB = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrBishP = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrP = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrRishP = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrRP = SatStr;
		}
	}
	StartPoint = strstr (Buffer, "-XY:");
	if (StartPoint)
	{
		if (CurAlgMode == 0)
		{
			sscanf (StartPoint + 4, "%lf", &Strength);
			Tcc_Cui_VIQE.Gamut.Alg01.StrYG = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrGishY = Strength;
			Tcc_Cui_VIQE.Gamut.Alg01.StrY = Strength;	
		}
		else if (CurAlgMode == 1)
		{
			sscanf (StartPoint + 4, "%d:%d:%d:%d", &BrightStr, &HueStr, &ContStr, &SatStr);

			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrYG = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrGishY = BrightStr;
			Tcc_Cui_VIQE.Gamut.Alg23.BrightStrY = BrightStr;	

			Tcc_Cui_VIQE.Gamut.Alg23.HueStrYG = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrGishY = HueStr;
			Tcc_Cui_VIQE.Gamut.Alg23.HueStrY = HueStr;	

			Tcc_Cui_VIQE.Gamut.Alg23.ContStrYG = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrGishY = ContStr;
			Tcc_Cui_VIQE.Gamut.Alg23.ContStrY = ContStr;	

			Tcc_Cui_VIQE.Gamut.Alg23.SatStrYG = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrGishY = SatStr;
			Tcc_Cui_VIQE.Gamut.Alg23.SatStrY = SatStr;	
		}
	}
	if (Tcc_Cui_VIQE.Gamut.OnOff == 1)
	{
		GamutInterfaceMain (&Tcc_Cui_VIQE);
		/*GPLLUTGen (Tcc_Cui_VIQE.Gamut.Bright,
			Tcc_Cui_VIQE.Gamut.Cont + 100,
			Tcc_Cui_VIQE.Gamut.Hue,
			Tcc_Cui_VIQE.Gamut.Sat + 100);*/
/*		CLUTGen_Interface_Example (
			Tcc_Cui_VIQE.Gamut.LUVMode,
			Tcc_Cui_VIQE.Gamut.StrYishG, 
			Tcc_Cui_VIQE.Gamut.StrYG, 
			Tcc_Cui_VIQE.Gamut.StrGishY,
			Tcc_Cui_VIQE.Gamut.StrY, 
			Tcc_Cui_VIQE.Gamut.StrYishO, 
			Tcc_Cui_VIQE.Gamut.StrO,
			Tcc_Cui_VIQE.Gamut.StrOPin, 
			Tcc_Cui_VIQE.Gamut.StrPin, 
			Tcc_Cui_VIQE.Gamut.StrPishPin,
			Tcc_Cui_VIQE.Gamut.StrRishO, 
			Tcc_Cui_VIQE.Gamut.StrR, 
			Tcc_Cui_VIQE.Gamut.StrPishR,
			Tcc_Cui_VIQE.Gamut.StrRP, 
			Tcc_Cui_VIQE.Gamut.StrRishP, 
			Tcc_Cui_VIQE.Gamut.StrP,
			Tcc_Cui_VIQE.Gamut.StrBishP, 
			Tcc_Cui_VIQE.Gamut.StrPishB, 
			Tcc_Cui_VIQE.Gamut.StrB,
			Tcc_Cui_VIQE.Gamut.StrGishB, 
			Tcc_Cui_VIQE.Gamut.StrBG, 
			Tcc_Cui_VIQE.Gamut.StrBishG,
			Tcc_Cui_VIQE.Gamut.StrG
		);*/
		Tcc_Cui_VIQE.Gamut.LUTUpdated = 1;
	}
	return;
}

void TCC_CUI_VIQE_ConfRandomMode (char *Buffer)
{
	if (strstr (Buffer, "on")) Tcc_Cui_VIQE.RandomMode = TRUE;
	else if (strstr (Buffer, "off")) Tcc_Cui_VIQE.RandomMode = FALSE;
	return;
}

void TCC_CUI_VIQE_ConfHalfProcessing (char *Buffer)
{
	if (strstr (Buffer, "on")) Tcc_Cui_VIQE.HalfProcessing = TRUE;
	else if (strstr (Buffer, "off")) Tcc_Cui_VIQE.HalfProcessing = FALSE;
	return;
}

/*
void ParseCmd (char *Buffer, int CurCmd)
{
	switch (CurCmd)
	{
		case TCC_CUI_VIQE_SHOW_STATE :
			TCC_CUI_VIQE_ShowState (Buffer);
			break;
		case TCC_CUI_VIQE_TURN :
			TCC_CUI_VIQE_Turn (Buffer);
			break;
		case TCC_CUI_VIQE_CONF_DEINTL :
			TCC_CUI_VIQE_ConfDeintl (Buffer);
			break;
		case TCC_CUI_VIQE_CONF_DNTS :
			TCC_CUI_VIQE_ConfDnts (Buffer);
			break;
		case TCC_CUI_VIQE_CONF_DNSP :
			TCC_CUI_VIQE_ConfDnsp (Buffer);
			break;
		case TCC_CUI_VIQE_CONF_HPF :
			TCC_CUI_VIQE_ConfHpf (Buffer);
			break;
		case TCC_CUI_VIQE_CONF_HISEQ :
			TCC_CUI_VIQE_ConfHiseq (Buffer);
			break;
		case TCC_CUI_VIQE_CONF_GAMUT :
			TCC_CUI_VIQE_ConfGamut (Buffer);
			break;
		case TCC_CUI_VIQE_CONF_HELP :
			TCC_CUI_VIQE_Menu_Display ();
			break;
		default : break;
	}
	return;
}

int main ()
{
	char Buffer [100];
	int CurCmd;
	while (1)
	{
		gets (Buffer);
		CurCmd = TCC_CUI_Parse_CMD(Buffer);
		ParseCmd (Buffer, CurCmd);
	}
}
*/

