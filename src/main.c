#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "../include/pgm.h"
#include "../include/transImages.h"
//----------------
int main(){
    pgm* p = pgm_read_asc("data/eye_s_asc.pgm");
    pgm* image = pgm_zoom(p,3,0,0,p->height,p->width);
    pgm_write_bin("data/zoomPGM.pgm",image);
    return 0;
}