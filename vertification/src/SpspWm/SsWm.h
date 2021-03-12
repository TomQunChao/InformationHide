#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <memory.h>
#include <iomanip>
#include <cmath>
#include "Random.h"
#include "image.h"


using namespace std;

class SsWm {
private:
	double* dctMat, *dctTemp;
	double* mt;
	int dctSize;
	int* zzMat;
	char* w,*t1;
	int K, N,start=28;
	Mt19937 *m;
	
	void dctI(char* data,double *res);
	void dctIR(double* data,char *res);
public:
	SsWm(int dctSize);
	
	
	WmImage addWm(const Image &data,const Image &wm,const double a=4.3,const int K=8);
	void addWm(WmImage &dst, const Image& data, const Image& wm, const double a = 4.3, const int K = 8);
	void getWm(Image &dst, const WmImage& wi);
	Image getWm(const WmImage &wi);
	~SsWm();
};

