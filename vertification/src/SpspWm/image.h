#pragma once
#include <string>
#include "Random.h"
#include <fstream>
#include <time.h>
#include <memory.h>
#include <cmath>
#include "util.h"

using namespace std;

struct Image {
	//bitmap file header
	uint16_t bfType=0;	// 0
	uint32_t bfSize=0;	// 2
	uint32_t bfOffBits=0;	// 10
	//bitmap infomation header
	uint32_t biSize=0;	// 14
	uint32_t w=0;	// 18
	uint32_t h=0;	// 22

	char* data=NULL;
	char* realDat=NULL;
	uint32_t size=0;
	~Image();
};
// 带水印图像
struct WmImage:Image {
	uint32_t start=0;	// 开始嵌入位
	uint32_t K, N=0;	// 取参数数量，每一位嵌入多少次
	uint32_t blkS=0;	// 分块宽(高)
	double* dctMat=NULL;	// dct矩阵
	char* wMat=NULL;	// +1 -1矩阵
};
class ImgFac {
private:
	Random r;
	Image imgModel;
	Image wmModel;
	unsigned jpegN;
	int jpegMat[64] = {
		16,11,10,16,24,40,51,61,
		12,12,14,19,26,58,60,55,
		14,13,16,24,40,57,69,56,
		14,17,22,29,51,87,80,62,
		18,22,37,56,68,109,103,77,
		24,35,55,64,81,104,113,92,
		49,64,78,87,103,121,120,101,
		72,92,95,98,112,100,103,99
	};
	int Q[64];
	double* dctMat;
	int* zzMat;
	static ImgFac* imf;
	ImgFac();
public:
	void setRandom(const Random &rm);
	void setImgModel(const Image& im);
	void setWmModel(const Image& im);
	void setJpegN(const unsigned N);
	Image readImage(string filename);
	WmImage genWmImage(uint32_t size=0);
	//void freeImg(Image &img);
	//void freeWmImg(WmImage& wi);
	Image genImage(int size=0);
	Image genWm(int size=0);
	void changeBright(Image& des,const Image &src,const double bright);
	void changeContrast(Image& des,const Image &src,const double contrast);
	void attackGuss(Image& des,const Image &src,const double std);
	void jpegCompress(Image &des,const Image& src,const unsigned level);

	bool saveImage(const Image& im, const string filename="");
	static ImgFac &instance();
	//ImgFac();
	~ImgFac();
//private:
	//static ImgFac imf;
};
