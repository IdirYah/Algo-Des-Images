#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "../include/pgm.h"
#include "../include/transImages.h"
//----------------
int main(){
    pgm* p = pgm_read_asc("data/eye_s_asc.pgm");
    p = rotation_pgm(p,M_PI/12,p->height/2,p->width/2);
    pgm_write_asc("data/rotationPGM.pgm",p);
    return 0;
}