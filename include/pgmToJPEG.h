#ifndef _PGMTOJPEG_H
#define _PGMTOJPEG_H
#include "./pgm.h"
//-----------------
void pgm_extract_blk(pgm*,double [8][8],int,int);
void pgm_dct(double [8][8]);
void pgm_quantify(double [8][8],int [8][8]);
void pgm_zigzag_extract(double [8][8],int [64]);
void pgm_rle(FILE* ,int [64]);
void pgm_to_jpeg(char*,pgm*);
//-----------------
#endif 
