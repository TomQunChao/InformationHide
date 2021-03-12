#pragma once
#include "Random.h"
#include <iostream>
#include <fstream>
#include "util.h"
#include <cmath>
#include <memory.h>

using namespace std;

#define PI 3.1415926535
// ÿһ�ֵĴ�С

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
	void gen(Flag flag,double std,double nStd=0); //Ĭ��û������
	double merfc(double x);
	double ierfc(double x);
	double Q(double x);
	double iQ(double x);
	void attack(double nStd);
	double sum(double *x);
	void addWm(double a,double *x);
	// ���������ʣ�����©�����
	double theValue(double pfa,double a, double sigmax);
	double theValue(double pfa,double a, double sigmax, double sigmav);
	double getPsi(double pfa, double sigmax);
public:
	Roc();
	// �޹��� epN������ epS ���ִ�С
	void test(string filename,double a,double c,int epN,int epS,Flag flag);
	// �й���
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
	
	�ļ�д������:
	X��Ϣ: X���� (normal ���� STD | ggd ���� STD c) 
	ˮӡ��Ϣ: Ƕ��ǿ�� w
	������Ϣ: 0/1 ���� STD
	�Ƿ���ʵ������ 0/1
	��������
	[����] ����޹�����ÿ���������ݣ�����ÿ���ĸ�����
	*/
};

/* �޹������棬X~GGD(0,10),N=1000,STD=10
[c=2.0/1.0/0.5,a=1.8] ��������/ʵ������
[c=2.0,a=1.5/1.8] ��������/ʵ������ */

/* �й�������,X~N(0,10),a=1.8,N=1000
δ�ܹ��� ��������
�ܹ��� ��������/ʵ������
[Noise~N(0,2.5/5)]*/
