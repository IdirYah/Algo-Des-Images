#include <stdio.h>
#include <stdlib.h>
#include "../include/pgm.h"
//----------------
int main(){
    pgm* p = pgm_read_asc("data/extractAsc.pgm");
    pgm_write_histogram("data/histogram.txt",p);
    return 0;
}