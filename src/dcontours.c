#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/dcontours.h"
#include "../include/pgm.h"
//-------------------------
unsigned char max_pgm(pgm* image){
    unsigned char max = image->pixels[0][0];
    for(int i=0;i<image->height;i++){
        for(int j=0;j<image->width;j++){
            if(image->pixels[i][j]>max){
                max = image->pixels[i][j];
            }
        }
    }
    return max;
}
//--------------
int absolute(int a){
    if(a<0) return -1*a;
    return a;
}
//--------------
pgm* naive(pgm* img,int K[3][3],double cte){
    pgm* p = pgm_alloc(img->height,img->width,img->max_value);
    for(int i=0;i<p->height;i++){
        for(int j=0;j<p->width;j++){
            double somme = 0.0;
            for(int k=0;k<3;k++){
                for(int l=0;l<3;l++){
                    if(i+k-1>=0 && i+k-1<p->height && j+l-1>=0 && j+l-1<p->width){
                        somme = somme + cte*K[k][l]*img->pixels[i+k-1][j+l-1];
                    }
                }
            }
            int v = (int)somme;
            p->pixels[i][j] = absolute(v);
        }
    }
    return p;
}
//--------------
pgm* norme(pgm* img1,pgm* img2){
    pgm* img = pgm_alloc(img1->height,img1->width,img1->max_value);
    for(int i=0;i<img->height;i++){
        for(int j=0;j<img->width;j++){
            img->pixels[i][j] = sqrt(img1->pixels[i][j]*img1->pixels[i][j]+img2->pixels[i][j]*img2->pixels[i][j]);
        }
    }
    return img;
}
//--------------
pgm* naive_edge_detector(pgm* img){
    pgm* p = pgm_alloc(img->height,img->width,img->max_value);
    unsigned char m = max_pgm(img);
    for(int i=0;i<p->height;i++){
        for(int j=0;j<p->width;j++){
            p->pixels[i][j] = round((img->pixels[i][j]*255)/m);
        }
    }
    return p;
}
//--------------
pgm* filtreNaive(pgm* p){
    int k1[3][3] = {{0,0,0},{-1,0,1},{0,0,0}};
    int k2[3][3] = {{0,-1,0},{0,0,0},{0,1,0}};
    pgm* p1 = naive(p,k1,1);
    pgm* p2 = naive(p,k2,1);
    pgm* img = norme(p1,p2);
    img = naive_edge_detector(img);
    return img;
}
//--------------
pgm* filtreSobel(pgm* p){
    int k1[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
    int k2[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    pgm* p1 = naive(p,k1,0.25);
    pgm* p2 = naive(p,k2,0.25);
    pgm* img = norme(p1,p2);
    img = naive_edge_detector(img);
    return img;
}