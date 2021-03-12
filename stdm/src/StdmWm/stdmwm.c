#include "stdmwm.h"

void initWm(unsigned size) {
	dctSize = size;
	dctTemp = (double*) malloc(sizeof(double) * size * size);
	t1 = (char*) malloc(sizeof(char) * size * size);
	w = NULL;
}
/*wm only 64*64, im only 512*512 */
void addWm(const Image *im,const Image *wm, WmImage *res, unsigned K,double delta) {

	unsigned N = K * im->size / (wm->size * 8 * dctSize * dctSize);
	if (lastN != N || w == NULL) {
		lastN = N;
		w = (char*) malloc(sizeof(char) * N);
		memset(w, 1, sizeof(char) * N);
		for (unsigned i = 0; i < (N >> 1); i++) {
			w[i << 1] = -1;
		}
	}
	if (res->w == NULL) {
		res->w = (char*) malloc(sizeof(char) * N);
		memcpy(res->w, w, sizeof(char) * N);
	}
	res->K = K;
	res->N = N;
	res->dctSize = dctSize;
	res->start = START;
	char *dat = im->realDat;
	char *wmd = wm->realDat;
	char *ndat = res->im.realDat;
	unsigned bitcnt = 0, subcnt = 0;

	double a0, a1, xb, d0, d1,q1x, q0x;
	d0 = -delta / 4;
	d1 = delta / 4;

	FILE *f;
	f = fopen("add.txt", "w");

	for (unsigned i = 0; i < im->w; i += dctSize) {
		for (unsigned j = 0; j < im->h; j += dctSize) {
			// read out, coordinate of up left cornor is (i,j)
			for (int k = 0; k < dctSize * dctSize; k++) {
				t1[k] = dat[(i + k / dctSize) * im->w + j + k % dctSize] - 128;
			}
			// dct
			dct(t1, dctTemp);
			// get a
			xb = 0;
//			printf("get %d %d\n",i,j);

			for (unsigned k = 0; k < K; k++) {
				xb += w[k] == 1 ?
						dctTemp[zzMat[START + k]] : -dctTemp[zzMat[START + k]];
			}
			xb /= N;
			q1x = floor((xb - d1) / delta + 0.5) * delta + d1;
			q0x = floor((xb - d0) / delta + 0.5) * delta + d0;
			a1 = q1x - xb;
			a0 = q0x - xb;
			// add water mark
//			printf("add %d %d\n",i,j);
			for (int k = 0; k < K; k++) {
				dctTemp[zzMat[START + k]] +=
						(wmd[(bitcnt >> 3)] & (1 << (7 - (bitcnt & 0x7)))) ?
								a1 * w[subcnt] : a0 * w[subcnt];

//				fprintf(f, "%f\n", dctTemp[zzMat[START + k]]);

				subcnt++;
				// 如果嵌入总数到达了N位，就嵌入水印图像的下一位
				if (subcnt >= N) {
					bitcnt++;
					subcnt = 0;
				}
			}
			// Inverse dct
			dctR(dctTemp, t1);
			// write back
			for (int k = 0; k < dctSize * dctSize; k++) {
				ndat[(i + k / dctSize) * im->w + j + k % dctSize] = t1[k] + 128;
			}
		}
	}
	fclose(f);
}

void getWm(const WmImage *wi, Image *res,const double delta) {
	if (wi->N != lastN) {
		lastN = wi->N;
		free(w);
		w = (char*) malloc(sizeof(char) * lastN);
		memcpy(w, wi->w, sizeof(char) * lastN);
	}
	int wmSize = wi->K * wi->im.size / (8 * lastN * dctSize * dctSize);
	char *wmdat = res->realDat;
	char *dat = wi->im.realDat;
	double sum = 0;
	unsigned bitcnt = 0, subcnt = 0;
	memset(wmdat, 0, sizeof(char) * wmSize);

	double q0y/*,q1y*/, yb;
	double d0 = -delta / 4;

//	FILE *f = fopen("get.txt", "w");

	for (unsigned i = 0; i < wi->im.w; i += dctSize) {
		for (unsigned j = 0; j < wi->im.h; j += dctSize) {
			// read out
			for (int k = 0; k < dctSize * dctSize; k++) {
				t1[k] = dat[(i + k / dctSize) * wi->im.w + j + k % dctSize]
						- 128;
			}
			// dct
			dct(t1, dctTemp);
			//
			for (unsigned k = 0; k < wi->K; k++) {
				sum += dctTemp[zzMat[wi->start + k]] * wi->w[subcnt];
				subcnt++;
//				fprintf(f, "%f\n", dctTemp[zzMat[wi->start + k]]);
				if (subcnt == wi->N) {
					// get qb(y)
					yb = sum / wi->N;
					q0y = floor((yb - d0) / delta + 0.5) * delta + d0;

					if (fabs(q0y - yb) >= delta / 4)
						wmdat[bitcnt >> 3] |= (1 << (7 - (bitcnt & 0x7)));

					sum = 0;
					subcnt = 0;
					bitcnt++;
				}
			}
		}
	}
//	fclose(f);
}

void freeWm() {
	free(w);
	free(t1);
	free(dctTemp);
}
