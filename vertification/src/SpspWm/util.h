#pragma once

#define PI 3.1415926535

#define ind(i,j,size) ((i)*(size)+j)

#define prMatI(x,size) \
	for(unsigned i=0;i<size;i++){\
		cout<<" "<<int(x[i]);\
	}\
	cout<<endl;

#define prMatD(x,size)\
	for(unsigned i=0;i<size;i++){\
		cout<<" "<<double(x[i]);\
	}\
	cout<<endl;
// 交换x和y
#define swap(x,y) x=x+y;\
	y=x-y;\
	x=x-y;

// 水印不同的比例
#include <stdio.h>
#define wmDiff(x,y,n,res)\
	{unsigned cnt=0;\
	for(unsigned i=0;i<((n)<<3);i++){\
		if(((x)[(i)>>3]&(1<<(7-((i)&0x7))))^((y)[(i)>>3]&(1<<(7-((i)&0x7)))))cnt++;\
	}\
	res=double(cnt)/((n)<<3)/2;}

#define mse(s,x,n,res)\
	res=0;\
	for(unsigned i=0;i<n;i++){\
		res+=(s[i]-x[i])*(s[i]-x[i])/double(n);\
	}

#define nouse(f,bins,size)\
	for(unsigned i=0;i<bins;i++){\
		for(unsigned j=0;j<size;j++){\
			f<<0<<" ";\
		}\
		f<<"\n";\
	}
#include <memory.h>
#include <malloc.h>
void dct(char* data, double* res, double* dctMat, unsigned dctSize);
void dctR(double* data, char* res, double* dctMat, unsigned dctSize);