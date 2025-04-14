#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/pgm.h"
//--------------
pgm* pgm_alloc(int h,int w,int max){
    pgm* p = (pgm*)malloc(sizeof(pgm));
    p->height = h;
    p->width = w;
    p->max_value = max;
    p->pixels = (unsigned char**)malloc(h*sizeof(unsigned char*));
    for(int i=0;i<h;i++){
        p->pixels[i] = (unsigned char*)malloc(w*sizeof(unsigned char));
    }
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            p->pixels[i][j] = max;
        }
    }
    return p;
}
//-----------------
void pgm_free(pgm* p){
    for(int i=0;i<p->height;i++){
        free(p->pixels[i]);
    }
    free(p->pixels);
    free(p);
}
//-------------------
pgm* pgm_read_asc(char* fname){
    FILE *f = fopen(fname,"r");
    if(f == NULL) return NULL;
    char format[3];
    fscanf(f,"%s",format);
    if(strcmp(format,"P2")){
        return NULL;
    }
    int c = fgetc(f);
    while(c == '#'){
        while(c != '\n' && c != EOF){
            c = fgetc(f);
        }
        c = fgetc(f);
    }
    ungetc(c,f);
    int h,w,max;
    fscanf(f,"%d%d%d",&w,&h,&max);
    pgm* p = pgm_alloc(h,w,max);
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            int tmp;
            fscanf(f,"%d",&tmp);
            p->pixels[i][j] = tmp;
        }
    }
    fclose(f);
    return p;
}
//-------------------
int pgm_write_asc(char* fname,pgm* p){
    FILE *f = fopen(fname,"w");
    if(f == NULL) return 0;
    fprintf(f,"P2\n%d %d\n%d\n",p->width,p->height,p->max_value);
    for(int i=0;i<p->height;i++){
        for(int j=0;j<p->width;j++){
            fprintf(f,"%d\n",p->pixels[i][j]);
        }
    }
    fclose(f);
    pgm_free(p);
    return 1;
}
//-----------------------
pgm* pgm_read_bin(char* fname){
    FILE *f = fopen(fname,"rb");
    if(f == NULL) return NULL;
    char format[3];
    fscanf(f,"%s",format);
    if(strcmp(format,"P5")){
        return NULL;
    }
    int h,w,max;
    fscanf(f,"%d%d%d",&w,&h,&max);
    pgm* p = pgm_alloc(h,w,max);
    for(int i=0;i<p->height;i++){
        fread(p->pixels[i],sizeof(unsigned char),w,f);
    }
    fclose(f);
    return p;
}
//-----------------------
int pgm_write_bin(char* fname,pgm* p){
    FILE *f = fopen(fname,"wb");
    if(f == NULL) return 0;
    fprintf(f,"P5\n%d %d\n%d\n",p->width,p->height,p->max_value);
    for(int i=0;i<p->height;i++){
        fwrite(p->pixels[i],sizeof(unsigned char),p->width,f);
    }
    fclose(f);
    free(p);
    return 1;
}
//---------------------
pgm* pgm_negative(pgm* src){
    pgm* dst = pgm_alloc(src->height,src->width,src->max_value);
    for(int i=0;i<src->height;i++){
        for(int j=0;j<src->width;j++){
            dst->pixels[i][j] = src->max_value - src->pixels[i][j];
        }
    }
    return dst;
}
//-------------------
void pgm_extract(pgm* p,char* fname,int dx,int dy,int h,int w){
    if(dx+h>=p->height || dy+w>=p->width){
        printf("Impossible d'extraire avec ces coordonnees.\n");
        return;
    }
    pgm* img = pgm_alloc(h,w,p->max_value);
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            img->pixels[i][j] = p->pixels[i+dx][j+dy];
        }
    }
    pgm_write_asc(fname,img);
}
//--------------------
int* pgm_get_histogram(pgm* p){
    int* tab = (int*)malloc(sizeof(int)*(p->max_value+1));
    for(int i=0;i<=p->max_value;i++){
        tab[i] = 0;
    }
    for(int i=0;i<p->height;i++){
        for(int j=0;j<p->width;j++){
            tab[p->pixels[i][j]]++;
        }
    }
    return tab;
}
//------------------
void pgm_write_histogram(char* fname,pgm* p){
    int* t = pgm_get_histogram(p);
    FILE *f = fopen(fname,"w");
    for(int i=0;i<=p->max_value;i++){
        fprintf(f,"%d : %d\n",i,t[i]);
    }
    fclose(f);
    free(t);
}