#ifndef TCC_VIQE_H
#define TCC_VIQE_H


void TCC_DeinitalizeVIQE();

void TCC_TurnOnVIQE();

void TCC_InitalizeVIQE( unsigned int FrmWidth, unsigned int FrmHeight, int input_color_format, int offset);

void TCC_RunVIQE( unsigned int CurYBase, unsigned int CurUBase, unsigned int CurVBase, 
		unsigned int PrevYBase, unsigned int PrevUBase, unsigned int PrevVBase, unsigned int M2MMode, int OutputFormat, int offset);

void TCC_ExecuteVIQE( unsigned int src_width, unsigned int src_height, unsigned int dst_width, unsigned int dst_height, int *src_addr, unsigned int dest_y , unsigned int h_offset, unsigned int v_offset, int need_offset, int w_offset);




#endif
