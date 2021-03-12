#include "image.h"

void initImageFac(unsigned size1) {
	size = size1;
	mt = (double*) malloc(sizeof(double) * size * size);
	dctMat = (double*) malloc(sizeof(double) * size * size);
	zzMat = (unsigned*) malloc(sizeof(unsigned) * size * size);
	//init dct matrix
	for (unsigned i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			dctMat[ind(i, j, size)] =
					i ? sqrt(2.0 / size)
								* cos((2 * j + 1) * (i) * PI / 2 / size) :
						1 / sqrt(size);
		}
	}

	// i 行坐标 j 列坐标 cnt ZigZag数组下标
	int i = 0, j = 0, cnt = 0;
	// 是否计算完毕
	while (cnt < size * size) {
		// 判断边界
		while (i >= 0 && j < size) {
			// 一维数组表示二维数组，把二维坐标(i,j)映射到一维为i*4+j
			zzMat[cnt++] = i * size + j;
			// 向右上角移动
			i--;
			j++;
		}
		// 如果是上三角，则上面的函数结束后i<0,j<4
		// 是下三角,j=4
		i = j == size ? i + 2 : 0;
		j = j == size ? size - 1 : j;
		while (j >= 0 && i < size) {
			zzMat[cnt++] = i * size + j;
			// 向左下角移动
			j--;
			i++;
		}
		// 如果是上三角，结束后j<0,i<4
		// 下三角，i=4
		j = i == size ? j + 2 : 0;
		i = i == size ? size - 1 : i;
	}
}
void freeImageFac() {
	free(dctMat);
	free(zzMat);
	free(mt);
}

void dct(char *src, double *res) {

	memset(mt, 0, size * size * sizeof(double));
	memset(res, 0, sizeof(double) * size * size);
	//前两个矩阵相乘
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				mt[ind(i, j, size)] += dctMat[ind(i, k, size)]
						* (src[ind(k, j, size)]);
			}
		}
	}
	//第二个和第三个矩阵相乘
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				res[ind(i, j, size)] += mt[ind(i, k, size)]
						* dctMat[ind(j, k, size)];
			}
		}
	}
}
void dctR(double *src, char *res) {
	double t;
	memset(mt, 0, sizeof(double) * size * size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				mt[ind(i, j, size)] += dctMat[ind(k, i, size)]
						* src[ind(k, j, size)];
			}
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			t = 0;
			for (int k = 0; k < size; k++) {
				t += mt[ind(i, k, size)] * dctMat[ind(k, j, size)];
			}
			if (t > 127)
				t = 127;
			if (t < -128)
				t = -128;
			res[ind(i, j, size)] = (char) (t + 0.5);
		}

	}
}

int readImage(char *filename, Image *res) {
	FILE *f;
	f = fopen(filename, "rb");
	if (!f) {
		return -1;
	}
	fread(&res->bfOffBits, sizeof(res->bfType), 1, f);
	fread(&res->bfSize, sizeof(res->bfSize), 1, f);
	fseek(f, 10, SEEK_SET);
	fread(&res->bfOffBits, sizeof(res->bfOffBits), 1, f);
	fseek(f, 14, SEEK_SET);
	fread(&res->biSize, sizeof(res->biSize), 1, f);
	fseek(f, 18, SEEK_SET);
	fread(&res->w, sizeof(res->w), 1, f);
	fseek(f, 22, SEEK_SET);
	fread(&res->h, sizeof(res->h), 1, f);
	res->data = (char*) malloc(sizeof(char) * res->bfSize);
	fseek(f, 0, SEEK_SET);
	fread(res->data, sizeof(char), res->bfSize, f);
	fclose(f);
	res->realDat = &res->data[res->bfOffBits];
	res->size = res->bfSize - res->bfOffBits;
	return 0;
}
int writeImage(char *filename, Image *src) {
	FILE *f;
	f = fopen(filename, "wb");
	if (!f)
		return -1;
	fwrite(src->data, sizeof(char), src->bfSize, f);
	fclose(f);
	return 0;
}
void freeImage(Image *im) {
	free(im->data);
}
void copyImage(Image *dst,const Image *src) {
	memcpy(dst, src, sizeof(Image));
	dst->data = (char*) malloc(sizeof(char) * dst->bfSize);
	memcpy(dst->data, src->data, sizeof(char) * dst->bfSize);
	dst->realDat = &dst->data[dst->bfOffBits];
}

void changeBright(Image *des,  Image *src, double bright) {
	for (unsigned i = 0; i < src->size; i++) {
		double x = (unsigned char) (src->realDat[i]) + bright * 255;
		if (x < 0)
			x = 0;
		if (x > 255)
			x = 255;
		des->realDat[i] = (unsigned char) (x);
	}
}
void changeContrast(Image *des,  Image *src, double contrast) {
	double k = tan((45 + 44 * contrast) / 180 * PI);
	for (unsigned i = 0; i < src->size; i++) {
		double x = ((unsigned char) (src->realDat[i]) - 127.5) * k + 127.5;
		if (x < 0)
			x = 0;
		if (x > 255)
			x = 255;
		des->realDat[i] = (unsigned char) (x + 0.5);
	}
}
void attackGuss(Image *dst,  Image *src,  double std) {
	for (unsigned i = 0; i < src->size; i++) {
		double x = (unsigned char) (src->realDat[i]) + extractGauss() * std;
		if (x < 0)
			x = 0;
		if (x > 255)
			x = 255;
		dst->realDat[i] = (unsigned char) (x);
	}
}
void jpegCompress(Image *des, Image *src,unsigned level) {
	if (!level)
		return;
	double qua = level > 50 ? (100.0 - level) / 50 : 50.0 / level;
	for (int i = 0; i < size * size; i++) {
		Q[i] = (int) (jpegMat[i] * qua);
		if (Q[i] > 255)
			Q[i] = 255;
		if (Q[i] < 0)
			Q[i] = 0;
	}
	char *srcD = src->realDat;
	char *desD = des->realDat;
	double *dctTemp = (double*) alloca(sizeof(double) * size * size);
	char *t1 = (char*) alloca(sizeof(char) * size * size);
	for (unsigned i = 0; i < src->w; i += 8) {
		for (unsigned j = 0; j < src->h; j += 8) {
			for (unsigned k = 0; k < size * size; k++) {
				t1[k] = srcD[(i + k / size) * src->w + k % size + j] - 128;
			}
			dct(t1, dctTemp);
			// quantization
			for (unsigned k = 0; k < size * size && qua != 0; k++) {
//				double t = dctTemp[k];
				if (Q[k])
					dctTemp[k] = (int) (dctTemp[k] / Q[k] + 0.5) * Q[k];
				//printf("%f\n", dctTemp[k]-t);
			}
			//printf("\n");
			dctR(dctTemp, t1);
			for (unsigned k = 0; k < size * size; k++) {
				desD[(i + k / size) * src->w + k % size + j] = t1[k] + 128;
			}

		}
	}
}
