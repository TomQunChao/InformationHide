#pragma once
#include "Random.h"
#include <iostream>
#include <fstream>
#include "util.h"
#include <cmath>
#include <memory.h>

using namespace std;

#define PI 3.1415926535
// 每一轮的大小

class Roc {
public:
	enum Flag {
		ggd05,
		ggd1,
		ggd2,
		normal
	};
private:
	//static Roc r;
	char* w;
	unsigned N=1000,bins=50;
	double* x,*xv;
	GgdGen *ggX;
	BoxMuller *bmX;
	BoxMuller *bmN;
	Mt19937 *m;
	uint32_t *pfaMat,*pmMat;
	double *range;
	double a, sumx, sumwmx,sumvx,sumwmvx;
	//Roc();
	void gen(Flag flag,double std,double nStd=0); //默认没有噪声
	double merfc(double x);
	double ierfc(double x);
	double Q(double x);
	double iQ(double x);
	void attack(double nStd);
	double sum(double *x);
	void addWm(double a,double *x);
	// 给出误检概率，返回漏检概率
	double theValue(double pfa,double a, double sigmax);
	double theValue(double pfa,double a, double sigmax, double sigmav);
	double getPsi(double pfa, double sigmax);
public:
	Roc();
	// 无攻击 epN是轮数 epS 是轮大小
	void test(string filename,double a,double c,int epN,int epS,Flag flag);
	// 有攻击
	void test(string filename,double a,double nStd,int epN,int epS,bool isNoise);
	//
	void set(unsigned N);
	void setV(int s1, int s2);
	void setX(double beta,int s1, int s2, int s3, int s4, int s5);
	void setX(int s1, int s2);
	void setWRand(int s);
	void setRange(double d1, double d2,unsigned bins);

	void testAll(unsigned epN=1, unsigned epS=100, unsigned bin = 50);
	//static Roc& instance();
	/*
	
	文件写出规则:
	X信息: X长度 (normal 中心 STD | ggd 中心 STD c) 
	水印信息: 嵌入强度 w
	噪声信息: 0/1 中心 STD
	是否有实验数据 0/1
	数据组数
	[数据] 如果无攻击，每行两个数据，否则每行四个数据
	*/
};

/* 无攻击仿真，X~GGD(0,10),N=1000,STD=10
[c=2.0/1.0/0.5,a=1.8] 理论性能/实验性能
[c=2.0,a=1.5/1.8] 理论性能/实验性能 */

/* 有攻击仿真,X~N(0,10),a=1.8,N=1000
未受攻击 理论性能
受攻击 理论性能/实验性能
[Noise~N(0,2.5/5)]*/
