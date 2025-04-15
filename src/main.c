#include <stdio.h>
#include <stdlib.h>
#include "../include/ppm.h"
#include "../include/pgm.h"
//----------------
int main(){
    ppm* p = ppm_read_asc("data/eye_s_asc.ppm");
    ppm_write_histogram("data/histogramRGB.txt",p);
    pgm* p1 = ppm_to_pgm(p);
    pgm_write_asc("data/pgmToPpm.pgm",p1);
    return 0;
}