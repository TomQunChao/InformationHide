#pragma once
#ifndef IMAGE_H
#define IMAGE_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "random.h"
#include "util.h"

typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef struct Image {
	//bitmap file header
	uint16_t bfType;	// 0
	uint32_t bfSize;	// 2
	uint32_t bfOffBits;	// 10
	//bitmap infomation header
	uint32_t biSize;	// 14
	uint32_t w;	// 18
	uint32_t h;	// 22

	char *data;
	char *realDat;
	uint32_t size;
} Image;

typedef struct WmImage {
	Image im;
	char *w;
	unsigned start;
	unsigned dctSize;
	unsigned K, N;
} WmImage;

static int jpegMat[64] = { 16, 11, 10, 16, 24, 40, 51, 61, 12, 12, 14, 19, 26, 58, 60,
		55, 14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62, 18,
		22, 37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64,
		78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99 };

int Q[64];

unsigned *zzMat;
double *dctMat;
unsigned size;
double *mt;
// store/read

void initImageFac(unsigned size1);
void freeImageFac();
int readImage(char *filename, Image *res);
int writeImage(char *filename, Image *res);
void copyImage(Image *dst,const Image *src);
void freeImage(Image *im);

void dct(char *src, double *res);
void dctR(double *src, char *res);

void changeBright(Image *des, Image *src, double bright);
void changeContrast(Image *des, Image *src, double contrast);
void attackGuss(Image *des,Image *src,double std);
void jpegCompress(Image *des, Image *src, unsigned level);

#endif
