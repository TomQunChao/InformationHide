#include "random.h"

/*Mersenne Twister*/

unsigned init_genrand64(uint64_t seed) {
	arrs[curId] = (uint64_t*) malloc(sizeof(uint64_t) * NN);
	arrs[curId][0] = seed;
	for (mti[curId] = 1; mti[curId] < NN; mti[curId]++)
		arrs[curId][mti[curId]] = (UINT64_C(6364136223846793005)
				* (arrs[curId][mti[curId] - 1] ^ (arrs[curId][mti[curId] - 1] >> 62))
				+ mti[curId]);
	curId++;
	return curId - 1;
}
/* generates a random number on [0, 2^64-1]-interval */
uint64_t extractLong(unsigned id) {
	int i;
	uint64_t x;
	static uint64_t mag01[2] = { UINT64_C(0), MATRIX_A };

	if (mti[id] >= NN) { /* generate NN words at one time */

		for (i = 0; i < NN - MM; i++) {
			x = (arrs[id][i] & UM) | (arrs[id][i + 1] & LM);
			arrs[id][i] = arrs[id][i + MM] ^ (x >> 1)
					^ mag01[(int) (x & UINT64_C(1))];
		}
		for (; i < NN - 1; i++) {
			x = (arrs[id][i] & UM) | (arrs[id][i + 1] & LM);
			arrs[id][i] = arrs[id][i + (MM - NN)] ^ (x >> 1)
					^ mag01[(int) (x & UINT64_C(1))];
		}
		x = (arrs[id][NN - 1] & UM) | (arrs[id][0] & LM);
		arrs[id][NN - 1] = arrs[id][MM - 1] ^ (x >> 1)
				^ mag01[(int) (x & UINT64_C(1))];

		mti[id] = 0;
	}

	x = arrs[id][mti[id]++];

	x ^= (x >> 29) & UINT64_C(0x5555555555555555);
	x ^= (x << 17) & UINT64_C(0x71D67FFFEDA60000);
	x ^= (x << 37) & UINT64_C(0xFFF7EEE000000000);
	x ^= (x >> 43);

	return x;
}

/* generates a random number on [0, 2^63-1]-interval */
int64_t extractLong63(unsigned id) {
	return (int64_t) (extractLong(id) >> 1);
}

/* generates a random number on [0,1]-real-interval */
double extractDouble(unsigned id) {
	return (extractLong(id) >> 11) * (1.0 / 9007199254740991.0);
}

/* generates a random number on [0,1)-real-interval */
double extractDouble2(unsigned id) {
	return (extractLong(id) >> 11) * (1.0 / 9007199254740992.0);
}

/* generates a random number on (0,1)-real-interval */
double extractDouble3(unsigned id) {
	return ((extractLong(id) >> 12) + 0.5) * (1.0 / 4503599627370496.0);
}
void freeMt(unsigned id) {
	free(arrs[id]);
	arrs[id] = NULL;
}

/*Mersenne Twister end*/

/*BoxMuller*/
void initBoxMuller(uint64_t s1, uint64_t s2) {
	bmId1 = init_genrand64(s1);
	bmId2 = init_genrand64(s2);
}
double extractGauss() {
	double theta = extractDouble3(bmId1);
	theta = theta * PI * 2;
	double r = extractDouble3(bmId2);
	r = sqrt(-2 * log(r));
	return r * cos(theta);
}
void freeBoxMuller() {
	freeMt(bmId1);
	freeMt(bmId2);
}
/*BoxMuller end*/
