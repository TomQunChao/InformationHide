#pragma once
#include <cmath>
#include <limits>
#include <inttypes.h>

#define NN 312
#define MM 156
#define MATRIX_A UINT64_C(0xB5026F5AA96619E9)
#define UM UINT64_C(0xFFFFFFFF80000000) /* Most significant 33 bits */
#define LM UINT64_C(0x7FFFFFFF) /* Least significant 31 bits */

typedef long int lint;

class Mt19937 {
private:
	/* The array for the state vector */
	uint64_t mt[NN];
	/* mti==NN+1 means mt[NN] is not initialized */
	int mti = NN + 1;
	void init_genrand64(uint64_t seed);

public:

	Mt19937(uint64_t seed);
	Mt19937(uint64_t key[], uint64_t key_len);
	/* generates a random number on [0, 2^64-1]-interval */
	uint64_t extractLong(void);


	/* generates a random number on [0, 2^63-1]-interval */
	int64_t extractLong63(void);

	/* generates a random number on [0,1]-real-interval */
	double extractDouble(void);

	/* generates a random number on [0,1)-real-interval */
	double extractDouble2(void);

	/* generates a random number on (0,1)-real-interval */
	double extractDouble3(void);

};

class BoxMuller {
private:
	Mt19937* m1, * m2;
	const double PI = 3.1415926535;
public:
	BoxMuller(long s1, long s2);
	double extract();
	~BoxMuller();
};

class ExpGen {
private:
	Mt19937* m;
	double beta;
public:
	ExpGen(long seed, double beta);
	double extract();
	~ExpGen();
};

class GgdGen {
private:
	Mt19937* m1, * m2;
	ExpGen* e1, * e2, * e3;
public:
	GgdGen(double beta, long s1, long s2, long s3, long s4, long s5);

	double extract05();
	double extract1();
	~GgdGen();
};

namespace est {
	struct NorEstRes {
		double mu;
		double delta2;
	};
	double getSum(const double* nums, unsigned len);
	double getAbsSum(const double* nums, unsigned len);
	double getSum(const double* nums, unsigned len, double p);
	NorEstRes NormalMoment(double* nums, unsigned len);
	NorEstRes NorMaxLike(double* nums, unsigned len);
	double exp(double* nums, unsigned len);
	
	double ggd(double* nums, unsigned len);
}
