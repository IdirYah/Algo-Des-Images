#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ppm.h"
//------------------
ppm* ppm_alloc(int h,int w,int max){
    ppm* p = (ppm*)malloc(sizeof(ppm));
    p->height = h;
    p->width = w;
    p->max_value = max;
    p->pixels = (rgb**)malloc(sizeof(rgb*)*h);
    for(int i=0;i<h;i++){
        p->pixels[i] = (rgb*)malloc(sizeof(rgb)*w);
    }
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            p->pixels[i][j].r = max;
            p->pixels[i][j].g = max;
            p->pixels[i][j].b = max;
        }
    }
    return p;
}
//-------------------
void ppm_free(ppm* p){
    for(int i=0;i<p->height;i++){
        free(p->pixels[i]);
    }
    free(p->pixels);
    free(p);
}
//--------------------
ppm* ppm_read_asc(char* fname){
    FILE *f = fopen(fname,"r");
    if(f == NULL){return NULL;}
    char format[3];
    fscanf(f,"%s",format);
    if(strcmp(format,"P3")){
        return NULL;
    }
    int c = fgetc(f);
    c = fgetc(f);
    while(c == '#'){
        while(c != '\n' && c != EOF){
            c = fgetc(f);
        }
        c = fgetc(f);
    }
    ungetc(c,f);
    int h,w,max;
    fscanf(f,"%d%d%d",&w,&h,&max);
    ppm* p = ppm_alloc(h,w,max);
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            int r1,g1,b1;
            if(fscanf(f,"%d%d%d",&r1,&g1,&b1) != 3) return NULL;
            p->pixels[i][j].r = r1;
            p->pixels[i][j].g = g1;
            p->pixels[i][j].b = b1;
        }
    }
    fclose(f);
    return p;
}
//----------------------
int ppm_write_asc(char* fname,ppm* p){
    FILE *f = fopen(fname,"w");
    if(f == NULL) return 0;
    fprintf(f,"P3\n%d %d\n%d\n",p->width,p->height,p->max_value);
    for(int i=0;i<p->height;i++){
        for(int j=0;j<p->width;j++){
            fprintf(f,"%d\n%d\n%d\n",p->pixels[i][j].r,p->pixels[i][j].g,p->pixels[i][j].b);
        }
    }
    fclose(f);
    return 1;
}
//----------------------------
ppm* ppm_read_bin(char* fname){
    FILE *f = fopen(fname,"rb");
    if(f == NULL) return NULL;
    char format[3];
    fscanf(f,"%s",format);
    if(strcmp(format,"P6")){
        return NULL;
    }
    int c = fgetc(f); 
    c = fgetc(f);
    c = fgetc(f);
    while(c == '#'){
        while(c != '\n' && c != EOF){
            c = fgetc(f);
        }
        c = fgetc(f); 
    }
    ungetc(c,f);
    int h,w,max;
    fscanf(f,"%d%d%d",&w,&h,&max);
    ppm* p = ppm_alloc(h,w,max);
    for(int i=0;i<p->height;i++){
        if(fread(p->pixels[i],sizeof(rgb),w,f) != w) {
            fclose(f);
            return NULL;
        }
    }
    fclose(f);
    return p;
}
//-----------------------
int ppm_write_bin(char* fname,ppm* p){
    FILE *f = fopen(fname,"wb");
    if(f == NULL) return 0;
    fprintf(f,"P6\n%d %d\n%d\n",p->width,p->height,p->max_value);
    for(int i=0;i<p->height;i++){
        fwrite(p->pixels[i],sizeof(rgb),p->width,f);
    }
    fclose(f);
    return 1;
}
//---------------------
ppm* ppm_negative(ppm* src){
    ppm* dst = ppm_alloc(src->height,src->width,src->max_value);
    for(int i=0;i<src->height;i++){
        for(int j=0;j<src->width;j++){
            dst->pixels[i][j].r = src->max_value - src->pixels[i][j].r;
            dst->pixels[i][j].g = src->max_value - src->pixels[i][j].g;
            dst->pixels[i][j].b = src->max_value - src->pixels[i][j].b;
        }
    }
    return dst;
}
//-----------------------
void ppm_extract(ppm* p,char* fname,int dx,int dy,int h,int w){
    if(dx+h>=p->height || dy+w>=p->width){
        printf("Impossible d'extraire avec ces coordonnees.\n");
        return;
    }
    ppm* img = ppm_alloc(h,w,p->max_value);
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            img->pixels[i][j].r = p->pixels[i+dx][j+dy].r;
            img->pixels[i][j].g = p->pixels[i+dx][j+dy].g;
            img->pixels[i][j].b = p->pixels[i+dx][j+dy].b;
        }
    }
    ppm_write_asc(fname,img);
}
//-----------------------
int** ppm_get_histogram(ppm* p){
    int** tab = (int**)malloc((p->max_value+1)*sizeof(int*));
    for(int i=0;i<=p->max_value;i++){
        tab[i] = (int*)malloc(3*sizeof(int));
    }
    for(int i=0;i<=p->max_value;i++){
        for(int j=0;j<3;j++){
            tab[i][j] = 0;
        }
    }
    for(int i=0;i<p->height;i++){
        for(int j=0;j<p->width;j++){
            tab[p->pixels[i][j].r][0]++;
            tab[p->pixels[i][j].g][1]++;
            tab[p->pixels[i][j].b][2]++;
        }
    }
    return tab;
}
//---------------------
void ppm_write_histogram(char* fname,ppm* p){
    int** t = ppm_get_histogram(p);
    FILE *f = fopen(fname,"w");
    for(int i=0;i<=p->max_value;i++){
        fprintf(f,"%d : %d %d %d\n",i,t[i][0],t[i][1],t[i][2]);
    }
    fclose(f);
}
//--------------------
pgm* ppm_to_pgm(ppm* img){
    pgm* p = pgm_alloc(img->height,img->width,img->max_value);
    for(int i=0;i<p->height;i++){
        for(int j=0;j<p->width;j++){
            p->pixels[i][j] = (unsigned char)(0.3*img->pixels[i][j].r + 0.59*img->pixels[i][j].g + 0.11*img->pixels[i][j].b);
        }
    }
    return p;
}