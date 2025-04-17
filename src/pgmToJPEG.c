#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "../include/pgmToJPEG.h"
//-----------------
void pgm_extract_blk(pgm* p,double blk[8][8],int dx,int dy){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            blk[i][j] = (double)p->pixels[i+dx][j+dy];
        }
    }
}
//-----------------------
void pgm_dct(double blk[8][8]){
    double dct[8][8];
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            double ci = (i == 0)? 1/sqrt(2):1;
            double cj = (j == 0)? 1/sqrt(2):1;
            double somme = 0;
            for(int x=0;x<8;x++){
                for(int y=0;y<8;y++){
                    somme = somme + blk[x][y]*cos(((2*x+1)*i*M_PI)/16)*cos(((2*y+1)*j*M_PI)/16);
                }
            }
            dct[i][j] = 0.25*ci*cj*somme;
        }
    }
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            blk[i][j] = dct[i][j];
        }
    }
}
//-------------------------
void pgm_quantify(double blk[8][8],int Q[8][8]){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            blk[i][j] = round(blk[i][j]/Q[i][j]);
        }
    }
}
//------------------------
void pgm_zigzag_extract(double blk[8][8],int zgzg[64]){
    int index = 0;
    int row = 0,col = 0;
    for(int k=0;k<64;k++){
        zgzg[k] = (int)blk[row][col];
        if((col+row)%2 == 0){
            if(col == 7){
                row++;
            }else if(row == 0){
                col++;
            }else{
                row--;col++;
            }
        }else{
            if(row == 7){
                col++;
            }else if(col == 0){
                row++;
            }else{
                col--;row++;
            }
        }
    }
}
//------------------------
void pgm_rle(FILE* fd,int zgzg[64]){
    int k = 0;
    while(k<64){
        if(zgzg[k] == 0){
            int count = 1;
            while(k+count<64 && zgzg[k+count] == 0) count++;
            if(count >=2){
                fprintf(fd,"@%d\n",count);
            }else{
                fprintf(fd,"%d\n",zgzg[k]);
            }
            k = k+count;
        }else{
            fprintf(fd,"%d\n",zgzg[k]);
            k++;
        }
    }
}
//---------------------------
void pgm_to_jpeg(char* fname,pgm* p){
    int Q[8][8] = {
        {16, 11, 10, 16, 24, 40, 51, 61},
        {12, 12, 14, 19, 26, 58, 60, 55},
        {14, 13, 16, 24, 40, 57, 69, 56},
        {14, 17, 22, 29, 51, 87, 80, 62},
        {18, 22, 37, 56, 68, 109, 103, 77},
        {24, 35, 55, 64, 81, 104, 113, 92},
        {49, 64, 78, 87, 103, 121, 120, 101},
        {72, 92, 95, 98, 112, 100, 103, 99}
    };
    FILE* f = fopen(fname,"w");
    fprintf(f,"JPEG\n%d %d\n",p->height,p->width);
    for(int i=0;i+8<p->height;i=i+8){
        for(int j=0;j+8<p->width;j=j+8){
            double blk[8][8];
            int zgzg[64];
            pgm_extract_blk(p,blk,i,j);
            pgm_dct(blk);
            pgm_quantify(blk,Q);
            pgm_zigzag_extract(blk,zgzg);
            pgm_rle(f,zgzg);
        }
    }
    fclose(f);
}