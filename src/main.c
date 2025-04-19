#include <stdio.h>
#include <stdlib.h>
#include "../include/pgm.h"
#include "../include/dcontours.h"
//----------------
int main(){
    pgm* p = pgm_read_asc("data/eye_s_asc.pgm");
    pgm* image = filtreSobel(p);
    pgm_write_asc("data/sobelPGM.pgm",image);
    return 0;
}