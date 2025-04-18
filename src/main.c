#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "../include/pgm.h"
#include "../include/transImages.h"
//----------------
int main(){
    pgm* p = pgm_read_asc("data/eye_s_asc.pgm");
    pgm* image = pgm_shear(p,0.4,0.2,p->height*2-100,p->width*2-100);
    pgm_write_asc("data/shearPGM.pgm",image);
    return 0;
}