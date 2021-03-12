#ifndef STDMWM_H
#define STDMWM_H
#include "image.h"

#define START 28

double *dctTemp;
char *t1,*w;
unsigned dctSize;
unsigned lastN;

void initWm(unsigned size);

void addWm(const Image *im,const Image *wm,WmImage *res,unsigned K,double delta);

void getWm(const WmImage *src,Image *res,const double delta);

void freeWm();
#endif
