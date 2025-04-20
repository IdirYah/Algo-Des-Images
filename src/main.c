#include <stdio.h>
#include <stdlib.h>
#include "../include/pgm.h"
#include "../include/dcontours.h"
//----------------
int main(){
    pgm* p = pgm_read_asc("data/eye_s_asc.pgm");
    gaussian_blur(p,15.0,15);
    pgm_write_asc("data/gaussPGM.pgm",p);
    return 0;
}