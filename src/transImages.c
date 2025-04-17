#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "../include/transImages.h"
#include "../include/pgm.h"
//----------------------
double absolu(double x){
    if(x<0){
        return (double)-1*x;
    }
    return (double)x;
}
//-----------------
double B0(double x){
    if(absolu(x)>0.5){
        return 0;
    }
    if(absolu(x)<0.5){
        return 1;
    }
    return 0.5;
}
//-----------------
double B1(double x){
    if(absolu(x)>1){
        return (double)x+1;
    }
    return (double)1-x;
}
//-----------------
double B2(double x){
    if(absolu(x)>1.5){
        return 0;
    }
    if(x>=-1.5 && x<=-0.5){
        return (double)0.5*(x+1.4)*(x+1.4);
    }
    if(absolu(x)<=0.5){
        return (double)0.75-(x*x);
    }
    return (double)0.5*(x-1.5)*(x-1.5);
}
//-----------------
double B3(double x){
    if(absolu(x)>2){
        return 0;
    }
    if(absolu(x)>=0 && absolu(x)<=1){
        return (double)0.5*absolu(x)*absolu(x)*absolu(x)-x*x+(2.0/3.0);
    }
    double tmp = (2-absolu(x))*(2-absolu(x))*(2-absolu(x));
    return (double)tmp/6;
}
//----------------------
double (*B[4])(double) = {B0,B1,B2,B3};
//----------------------
unsigned char interpolation_pgm(pgm* p,double x,double y){
    int x0 = (int)x;
    int y0 = (int)y;
    double dx = x - x0;
    double dy = y - y0;
    if(dx>0.5){
        x0++;
    }
    if(dy>0.5){
        y0++;
    }
    double sum = 0.0;
    int radius = (N+1)/2;
    for (int m = -1*radius; m <= radius + (N % 2); m++) {
        for (int n = -1*radius; n <= radius + (N % 2); n++) {
            int xi = x0 + m;
            int yi = y0 + n;
            if(xi>=0 && xi<p->height && yi>=0 && yi<p->width){
                double wx = B[N](x - xi);
                double wy = B[N](y - yi);
                double weight = wx * wy;
                sum += p->pixels[xi][yi] * weight;
            }
        }
    }
    if (sum < 0) sum = 0;
    if (sum > p->max_value) sum = p->max_value;
    return (unsigned char)sum;
}
//---------------------
pgm* rotation_pgm(pgm* p,double theta,int x0,int y0){
    pgm* img = pgm_alloc(p->height,p->width,p->max_value);
    for(int i=0;i<p->height;i++){
        for(int j=0;j<p->width;j++){
            double x = x0 + cos(theta)*(i-x0) + sin(theta)*(j-y0);
            double y = y0 - sin(theta)*(i-x0) + cos(theta)*(j-y0);
            img->pixels[i][j] = interpolation_pgm(p,x,y);
        }
    }
    return img;
}