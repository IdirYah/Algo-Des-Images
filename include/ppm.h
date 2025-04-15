#ifndef _PPM_H
#define _PPM_H
#include "./pgm.h"
//------------------
typedef struct rgb{
    unsigned char r,g,b;
}rgb;
//------------------
typedef struct ppm{
    int height,width,max_value;
    rgb** pixels;
}ppm;
//------------------
ppm* ppm_alloc(int,int,int);
void ppm_free(ppm*);
ppm* ppm_read_asc(char*);
int ppm_write_asc(char*,ppm*);
ppm* ppm_read_bin(char*);
int ppm_write_bin(char*,ppm*);
ppm* ppm_negative(ppm*);
void ppm_extract(ppm*,char*,int,int,int,int);
int** ppm_get_histogram(ppm*);
void ppm_write_histogram(char*,ppm*);
pgm* ppm_to_pgm(ppm*);
//------------------
#endif 