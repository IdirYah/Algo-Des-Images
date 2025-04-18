#ifndef _TRANSIMAGES_H
#define _TRANSIMAGES_H
#include "./pgm.h"
//--------------
#define N 2
//--------------
double absolu(double);
double B0(double);
double B1(double);
double B2(double);
double B3(double);
unsigned char interpolation_pgm(pgm*,double,double);
pgm* rotation_pgm(pgm*,double,int,int);
pgm* pgm_zoom(pgm*,double,int,int,int,int);
pgm* pgm_shear(pgm* image,double,double,int,int);
//--------------
#endif 
