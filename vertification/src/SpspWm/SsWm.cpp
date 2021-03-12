#include "SsWm.h"

SsWm::SsWm(int dctSize) {
	this->dctSize = dctSize;
	t1 = new char[dctSize * dctSize];
	dctMat = new double[dctSize * dctSize];
	dctTemp = new double[dctSize * dctSize];
	mt = new double[dctSize * dctSize];
	zzMat = new int[dctSize * dctSize];
	m = new Mt19937(1000);
	//init dct matrix
	for (int i = 0; i < dctSize; i++) {
		for (int j = 0; j < dctSize; j++) {
			dctMat[ind(i,j,dctSize)] = i ? sqrt(double(2) / dctSize) 
				* cos((2 * j + 1) * (i) * PI / 2 / dctSize)
				: 1 / sqrt(dctSize);
		}
	}
	// i 行坐标 j 列坐标 cnt ZigZag数组下标
	int i = 0, j = 0, cnt = 0;
	// 是否计算完毕
	while (cnt < dctSize*dctSize) {
		// 判断边界
		while (i >= 0&&j<dctSize) {
			// 一维数组表示二维数组，把二维坐标(i,j)映射到一维为i*4+j
			zzMat[cnt++] = i * dctSize + j;
			// 向右上角移动
			i--;
			j++;
		}
		// 如果是上三角，则上面的函数结束后i<0,j<4
		// 是下三角,j=4
		i = j == dctSize ? i + 2 : 0;
		j = j == dctSize ? dctSize-1 : j;
		while (j >= 0&&i<dctSize) {
			zzMat[cnt++] = i * dctSize + j;
			// 向左下角移动
			j--;
			i++;
		}
		// 如果是上三角，结束后j<0,i<4
		// 下三角，i=4
		j = i == dctSize ? j + 2 : 0;
		i = i == dctSize ? dctSize-1 : i;
	}
	//prMat(zzMat, dctSize*dctSize);
}
SsWm::~SsWm() {
	delete[] dctMat;
	delete[] mt;
	delete[] zzMat;
}
void SsWm::dctI(char *data,double *res) {
	memset(mt, 0, dctSize * dctSize * sizeof(double));
	memset(res, 0, sizeof(double) * dctSize * dctSize);
	//前两个矩阵相乘
	for (int i = 0; i < dctSize; i++) {
		for (int j = 0; j < dctSize; j++) {
			for (int k = 0; k < dctSize; k++) {
				mt[ind(i,j,dctSize)] += dctMat[ind(i,k,dctSize)] * (unsigned char)(data[ind(k,j,dctSize)]);
			}
		}
	}
	//第二个和第三个矩阵相乘
	for (int i = 0; i < dctSize; i++) {
		for (int j = 0; j < dctSize; j++) {
			for (int k = 0; k < dctSize; k++) {
				res[ind(i, j, dctSize)] += mt[ind(i, k, dctSize)] * dctMat[ind(j, k, dctSize)];
			}
		}
	}
}
void SsWm::dctIR(double* data,char *res) {
	double t;
	memset(mt, 0, sizeof(double) * dctSize * dctSize);
	for (int i = 0; i < dctSize; i++) {
		for (int j = 0; j < dctSize; j++) {
			for (int k = 0; k < dctSize; k++) {
				mt[ind(i,j,dctSize)] += dctMat[ind(k,i,dctSize)] * data[ind(k,j,dctSize)];
			}
		}
	}
	for (int i = 0; i < dctSize; i++) {
		for (int j = 0; j < dctSize; j++) {
			t = 0;
			for (int k = 0; k < dctSize; k++) {
				t += mt[ind(i,k,dctSize)] * dctMat[ind(k,j,dctSize)];
			}
			if (t > 255)t = 255;
			if (t < 0)t = 0;
			res[ind(i, j, dctSize)] = char(t+0.5);
			
		}
	}
	
}

WmImage SsWm::addWm(const Image& data,const Image& wm,const double a,const int K) {
	//ImgFac imf = ImgFac::instance();
	ImgFac imf=ImgFac::instance();
	imf.setImgModel(data);
	WmImage wi = imf.genWmImage();
	char* ndat = wi.realDat;
	char* dat = data.realDat;	// gray bitmap
	char* wmd = wm.realDat;	// only black and white bitmap
	int subcnt = 0,bitcnt=0;	//embed count
	N = K * data.size / (wm.size*8*dctSize*dctSize);
	this->K = K;
	// get w
	delete[] w;
	w = new char[N];
	memset(w, 1, sizeof(char) *N/ 2);
	memset(&w[N/2], -1, sizeof(char) * N / 2);
	for (int i = 1; i < N; i++) {
		/*if (m->extractDouble() > 0.5) {
			w[i] = -1;
			w[i + N / 2] = 1;
		}*/
		w[i] = w[i - 1] == 1 ? -1 : 1;
	}
	wi.wMat=new char[N];
	memcpy(wi.wMat,w,sizeof(char)*N);
	wi.start = start;
	wi.blkS = dctSize;
	wi.K = K;
	wi.dctMat = dctMat;
	wi.N = N;
	for (unsigned i = 0; i < data.w ; i+=dctSize) {
		for (unsigned j = 0; j < data.h ; j+=dctSize) {
			// read out, coordinate of up left cornor is (i,j) 
			for (int k = 0; k < dctSize*dctSize; k++) {
				t1[k] = dat[(i+k/dctSize) * data.w + j+k%dctSize];
			}
			// dct
			dctI(t1, dctTemp);
			//cout << dctTemp[0] << endl;
			// Add water mark to this block
			int pos = m->extractLong() % K + dctSize * dctSize / 2 - K;
			for (int k = 0; k < K; k++) {
				dctTemp[zzMat[start + k]] += wmd[bitcnt>>3]&(1<<7-(bitcnt&0x7))?a*w[subcnt]:-a*w[subcnt];
				subcnt++;	// 
				// 如果嵌入总数到达了N位，就嵌入水印图像的下一位
				if (subcnt >= N) {
					bitcnt++;
					subcnt = 0;
				}
			}
			// Inverse dct
			dctIR(dctTemp, t1);
			// write back
			for (int k = 0; k < dctSize * dctSize; k++) {
				ndat[(i + k / dctSize) * data.w + j + k % dctSize] = t1[k];
			}
		}
	}

	return wi;
}
void SsWm::addWm(WmImage &wi, const Image& data, const Image& wm, const double a, const int K){
	char* ndat = wi.realDat;
	char* dat = data.realDat;	// gray bitmap
	char* wmd = wm.realDat;	// only black and white bitmap
	int subcnt = 0, bitcnt = 0;	//embed count
	N = K * data.size / (wm.size * 8 * dctSize * dctSize);
	this->K = K;
	// get w
	w = new char[N];
	memset(w, 1, sizeof(char) * N / 2);
	memset(&w[N / 2], -1, sizeof(char) * N / 2);
	w[0] = 1;
	for (int i = 1; i < N; i++) {
		/*if (m->extractDouble() > 0.5) {

			w[i] = -1;
			w[i + N / 2] = 1;
		}*/
		w[i] = w[i - 1] == 1 ? -1 : 1;
	}
	memcpy(wi.wMat, w, sizeof(char) * N);
	memcpy(wi.dctMat, dctMat, sizeof(double) * dctSize * dctSize);
	wi.start = start;
	wi.blkS = dctSize;
	wi.K = K;
	wi.N = N;
	for (unsigned i = 0; i < data.w; i += dctSize) {
		for (unsigned j = 0; j < data.h; j += dctSize) {
			// read out, coordinate of up left cornor is (i,j) 
			for (int k = 0; k < dctSize * dctSize; k++) {
				t1[k] = dat[(i + k / dctSize) * data.w + j + k % dctSize];
			}
			// dct
			dctI(t1, dctTemp);
			//cout << dctTemp[0] << endl;
			// Add water mark to this block
			for (int k = 0; k < K; k++) {
				dctTemp[zzMat[start + k]] += wmd[bitcnt>>3]&(1<<7-(bitcnt&0x7))?a*w[subcnt]:-a*w[subcnt];
				subcnt++;	// 
				// 如果嵌入总数到达了N位，就嵌入水印图像的下一位
				if (subcnt >= N) {
					bitcnt++;
					subcnt = 0;
				}
			}
			// Inverse dct
			dctIR(dctTemp, t1);
			// write back
			for (int k = 0; k < dctSize * dctSize; k++) {
				ndat[(i + k / dctSize) * data.w + j + k % dctSize] = t1[k];
			}
		}
	}
}
void SsWm::getWm(Image &res, const WmImage& wi){
	int wmSize = wi.K * wi.size / (8 * N * dctSize * dctSize);
	char* wmdat = res.realDat;
	char* dat = wi.realDat;
	double sum = 0;
	unsigned bitcnt = 0, subcnt = 0;
	memset(wmdat, 0, sizeof(char) * wmSize);
	for (unsigned i = 0; i < wi.w; i += dctSize) {
		for (unsigned j = 0; j < wi.h; j += dctSize) {
			// read out
			for (int k = 0; k < dctSize * dctSize; k++) {
				t1[k] = dat[(i + k / dctSize) * wi.w + j + k % dctSize];
			}
			dctI(t1, dctTemp);
			for (unsigned k = 0; k < wi.K; k++) {
				subcnt++;
				sum += dctTemp[zzMat[wi.start + k]] * wi.wMat[subcnt];
				if (subcnt == wi.N) {
					if (sum <= 0)wmdat[bitcnt >> 3] |= (1 << (7 - (bitcnt & 0x7)));
					sum = 0;
					subcnt = 0;
					bitcnt++;
				}
			}
		}
	}
}
Image SsWm::getWm(const WmImage &wi) {
	
	int wmSize = wi.K * wi.size / (8*N*dctSize*dctSize);
	//ImgFac imf;
	ImgFac imf = ImgFac::instance();
	//imf.setWmModel(wm);
	Image res = imf.genWm(wmSize);
	char* wmdat = res.realDat;
	char* dat = wi.realDat;
	memset(wmdat, 0, sizeof(char)*wmSize);
	double sum = 0;
	int bitcnt = 0, subcnt = 0;
	for (unsigned i = 0; i < wi.w; i += dctSize) {
		for (unsigned j = 0; j < wi.h; j+=dctSize) {
			// read out
			for (int k = 0; k < dctSize * dctSize; k++) {
				t1[k] = dat[(i + k / dctSize) * wi.w + j + k % dctSize];
			}
			dctI(t1, dctTemp);
			for (int k = 0; k < wi.K; k++) {
				subcnt++;
				sum += dctTemp[zzMat[wi.start+k]]*wi.wMat[subcnt];
				if (subcnt == wi.N) {
					if (sum <= 0)wmdat[bitcnt >> 3] |= (1 << (7 - (bitcnt & 0x7)));
					sum = 0;
					subcnt = 0;
					bitcnt++;
				}
			}
		}
	}
	//printf("%d %d %d %d\n", bitcnt, subcnt, N,wmSize);

	return res;
}
