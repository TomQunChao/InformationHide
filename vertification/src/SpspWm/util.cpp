#include "util.h"

void dct(char* data, double* res, double* dctMat, unsigned dctSize) {
	double* mt = (double*)alloca(dctSize * dctSize * sizeof(double));
	memset(mt, 0, sizeof(double) * dctSize * dctSize);
	memset(res, 0, sizeof(double) * dctSize * dctSize);
	//前两个矩阵相乘
	for (int i = 0; i < dctSize; i++) {
		for (int j = 0; j < dctSize; j++) {
			for (int k = 0; k < dctSize; k++) {
				int t = data[ind(k, j, dctSize)];
				mt[ind(i, j, dctSize)] += dctMat[ind(i, k, dctSize)] * int(data[ind(k, j, dctSize)]);
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
void dctR(double* data, char* res, double* dctMat, unsigned dctSize) {
	double t;
	double* mt = (double*)alloca(sizeof(double) * dctSize * dctSize);
	memset(mt, 0, sizeof(double) * dctSize * dctSize);
	for (int i = 0; i < dctSize; i++) {
		for (int j = 0; j < dctSize; j++) {
			for (int k = 0; k < dctSize; k++) {
				mt[ind(i, j, dctSize)] += dctMat[ind(k, i, dctSize)] * data[ind(k, j, dctSize)];
			}
		}
	}
	for (int i = 0; i < dctSize; i++) {
		for (int j = 0; j < dctSize; j++) {
			t = 0;
			for (int k = 0; k < dctSize; k++) {
				t += mt[ind(i, k, dctSize)] * dctMat[ind(k, j, dctSize)];
			}
			if (t > 127)t = 127;
			if (t < -128)t = -128;
			res[ind(i, j, dctSize)] = char(t + 0.5);

		}
	}

}
