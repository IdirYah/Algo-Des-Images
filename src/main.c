#include <stdio.h>
#include <stdlib.h>
#include "../include/pgm.h"
#include "../include/pgmToJPEG.h"
//----------------
int main(){
    pgm* p = pgm_read_asc("data/eye_s_asc.pgm");
    pgm_to_jpeg("data/pgm_to_jpeg.txt",p);
    return 0;
}