#ifndef _PGM_H
#define _PGM_H
//----------------
typedef struct pgm{
    int height,width,max_value;
    unsigned char **pixels;
}pgm;
//----------------
pgm* pgm_alloc(int,int,int);
void pgm_free(pgm*);
pgm* pgm_read_asc(char*);
int pgm_write_asc(char*,pgm*);
pgm* pgm_read_bin(char*);
int pgm_write_bin(char*,pgm*);
pgm* pgm_negative(pgm*);
void pgm_extract(pgm*,char*,int,int,int,int);
int* pgm_get_histogram(pgm*);
void pgm_write_histogram(char*,pgm*);
//----------------
#endif 