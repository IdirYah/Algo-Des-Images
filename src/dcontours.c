#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
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
//-----------------
double gaussian(double x,double sigma){
    return exp(-(x*x)/(2*sigma*sigma))/(sigma*sqrt(2*M_PI));
}
//-----------------
double** calculerKernel(double sigma,int n){
    double** kernel;
    double sum = 0.0;
    kernel = (double**)malloc(n*sizeof(double*));
    for(int i=0;i<n;i++){
        kernel[i] = (double*)malloc(n*sizeof(double));
    }
    int p = n/2;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            double x = i - p;
            double y = j - p;
            kernel[i][j] = gaussian(x,sigma)*gaussian(y,sigma);
            sum = sum + kernel[i][j];
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            kernel[i][j] /= sum;
        }
    }
    return kernel;
}
//-------------
void gaussian_blur(pgm* image,double sigma,int n){
    int p = n/2;
    double ** kernel = calculerKernel(sigma,n);
    pgm* copy = pgm_alloc(image->height,image->width,image->max_value);
    for(int i=0;i<image->height;i++){
        for(int j=0;j<image->width;j++){
            copy->pixels[i][j] = image->pixels[i][j];
        }
    }
    for(int i=p;i<image->height-p;i++){
        for(int j=p;j<image->width-p;j++){
            double sum = 0.0;
            for(int k=-p;k<=p;k++){
                for(int l=-p;l<=p;l++){
                    sum = sum + kernel[k+p][l+p]*copy->pixels[i+k][j+l];
                }
            }
            int pixel_value = (int)round(sum+0.5);
            pixel_value = (pixel_value > 255) ? 255 : ((pixel_value < 0) ? 0 : pixel_value);
            image->pixels[i][j] = (unsigned char)pixel_value;
        }
    }
}
//----------------------
double **gradient_angle(pgm* grad_x,pgm* grad_y){
    if(grad_x->height != grad_y->height || grad_x->width != grad_y->width) return NULL;
    double** gradient = (double**)malloc(grad_x->height*sizeof(double*));
    for(int i=0;i<grad_x->height;i++){
        gradient[i] = (double*)malloc(grad_x->width*sizeof(double));
        for(int j=0;j<grad_x->width;j++){
            gradient[i][j] = atan2(grad_y->pixels[i][j],grad_x->pixels[i][j]);
        }
    }
    return gradient;
}
//------------------------
void non_maxima_suppression(pgm* norm,double** angle){
    pgm *copy = pgm_alloc(norm->height,norm->width,norm->max_value);
    for(int i = 0;i<norm->height;i++){
        for(int j = 0;j<norm->width;j++){
            copy->pixels[i][j] = norm->pixels[i][j];
        }
    }
    for(int i=1;i<norm->height-1;i++) {
        for (int j=1;j<norm->width-1;j++) {
            double ang = angle[i][j];
            unsigned char current = copy->pixels[i][j];
            unsigned char neighbor1, neighbor2;
            if((ang >= -M_PI/8 && ang < M_PI/8) || (ang >= 7*M_PI/8 || ang < -7*M_PI/8)){
                neighbor1 = copy->pixels[i][j+1];
                neighbor2 = copy->pixels[i][j-1];
            } 
            else if((ang >= M_PI/8 && ang < 3*M_PI/8) || (ang >= -7*M_PI/8 && ang < -5*M_PI/8)){
                neighbor1 = copy->pixels[i-1][j+1];
                neighbor2 = copy->pixels[i+1][j-1];
            } 
            else if((ang >= 3*M_PI/8 && ang < 5*M_PI/8) || (ang >= -5*M_PI/8 && ang < -3*M_PI/8)){
                neighbor1 = copy->pixels[i-1][j];
                neighbor2 = copy->pixels[i+1][j];
            } 
            else{
                neighbor1 = copy->pixels[i-1][j-1];
                neighbor2 = copy->pixels[i+1][j+1];
            }
            if(current < neighbor1 || current < neighbor2){
                norm->pixels[i][j] = 0;
            }
        }
    }
}