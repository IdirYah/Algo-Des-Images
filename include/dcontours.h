#ifndef _DCONTOURS_H
#define _DCONTOURS_H
#include "./pgm.h"
//----------------------
unsigned char max_pgm(pgm*);
int absolute(int);
pgm* naive(pgm*,int [3][3],double);
pgm* norm(pgm*,pgm*);
pgm* naive_edge_detector(pgm*);
pgm* filtreNaive(pgm*);
pgm* filtreSobel(pgm*);
double gaussian(double,double);
double** calculerKernel(double,int);
void gaussian_blur(pgm*,double,int);
//----------------------
#endif 
