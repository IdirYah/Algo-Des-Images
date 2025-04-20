#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "../include/pgm.h"
#include "../include/dcontours.h"
//----------------
int main(){
    pgm* px = pgm_read_asc("data/eye_s_asc.pgm");
    pgm* py = pgm_read_asc("data/sobelPGM.pgm");
    double** g = gradient_angle(py,px);
    non_maxima_suppression(px,g);
    pgm_write_asc("data/test.pgm",px);
    return 0;
}