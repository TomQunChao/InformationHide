#include "util.h"


double wmDiff(char *x, char *y, unsigned n){
	double res = 0;
	unsigned cnt = 0;
	for (unsigned i = 0; i < ((n) << 3); i++) {
		if (((x)[(i) >> 3] & (1 << (7 - ((i) & 0x7))))
				^ ((y)[(i) >> 3] & (1 << (7 - ((i) & 0x7)))))
			cnt++;
	}
	res = (double) (cnt) / ((n) << 3)/2;
	return res;
}
double mse(char *s, char *x, unsigned n) {
	double res = 0;
	for (unsigned i = 0; i < n; i++) {
		res += (s[i] - x[i]) * (s[i] - x[i]) / (double) (n);
	}
	return res;
}
