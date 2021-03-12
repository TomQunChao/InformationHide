#include "Random.h"


/* initializes mt[NN] with a seed */
Mt19937::Mt19937(uint64_t seed)
{
	init_genrand64(seed);
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
Mt19937::Mt19937(uint64_t init_key[],
	uint64_t key_length)
{
	unsigned int i, j;
	uint64_t k;
	init_genrand64(UINT64_C(19650218));
	i = 1; j = 0;
	k = (NN > key_length ? NN : key_length);
	for (; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 62))* UINT64_C(3935559000370003845)))
			+ init_key[j] + j; /* non linear */
		i++; j++;
		if (i >= NN) { mt[0] = mt[NN - 1]; i = 1; }
		if (j >= key_length) j = 0;
	}
	for (k = NN - 1; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 62))* UINT64_C(2862933555777941757)))
			- i; /* non linear */
		i++;
		if (i >= NN) { mt[0] = mt[NN - 1]; i = 1; }
	}

	mt[0] = UINT64_C(1) << 63; /* MSB is 1; assuring non-zero initial array */
}
void Mt19937::init_genrand64(uint64_t seed)
{
	mt[0] = seed;
	for (mti = 1; mti < NN; mti++)
		mt[mti] = (UINT64_C(6364136223846793005) * (mt[mti - 1] ^ (mt[mti - 1] >> 62)) + mti);
}
/* generates a random number on [0, 2^64-1]-interval */
uint64_t Mt19937::extractLong(void)
{
	int i;
	uint64_t x;
	static uint64_t mag01[2] = { UINT64_C(0), MATRIX_A };

	if (mti >= NN) { /* generate NN words at one time */

		/* if init_genrand64() has not been called, */
		/* a default initial seed is used     */
		if (mti == NN + 1)
			init_genrand64(UINT64_C(5489));

		for (i = 0; i < NN - MM; i++) {
			x = (mt[i] & UM) | (mt[i + 1] & LM);
			mt[i] = mt[i + MM] ^ (x >> 1) ^ mag01[(int)(x & UINT64_C(1))];
		}
		for (; i < NN - 1; i++) {
			x = (mt[i] & UM) | (mt[i + 1] & LM);
			mt[i] = mt[i + (MM - NN)] ^ (x >> 1) ^ mag01[(int)(x & UINT64_C(1))];
		}
		x = (mt[NN - 1] & UM) | (mt[0] & LM);
		mt[NN - 1] = mt[MM - 1] ^ (x >> 1) ^ mag01[(int)(x & UINT64_C(1))];

		mti = 0;
	}

	x = mt[mti++];

	x ^= (x >> 29)& UINT64_C(0x5555555555555555);
	x ^= (x << 17) & UINT64_C(0x71D67FFFEDA60000);
	x ^= (x << 37) & UINT64_C(0xFFF7EEE000000000);
	x ^= (x >> 43);

	return x;
}

/* generates a random number on [0, 2^63-1]-interval */
int64_t Mt19937::extractLong63(void)
{
	return (int64_t)(extractLong() >> 1);
}

/* generates a random number on [0,1]-real-interval */
double Mt19937::extractDouble(void)
{
	return (extractLong() >> 11)* (1.0 / 9007199254740991.0);
}

/* generates a random number on [0,1)-real-interval */
double Mt19937::extractDouble2(void)
{
	return (extractLong() >> 11)* (1.0 / 9007199254740992.0);
}

/* generates a random number on (0,1)-real-interval */
double Mt19937::extractDouble3(void)
{
	return ((extractLong() >> 12) + 0.5)* (1.0 / 4503599627370496.0);
}

BoxMuller::BoxMuller(long s1,long s2) {
	this->m1 = new Mt19937(s1);
	this->m2 = new Mt19937(s2);
}
double BoxMuller::extract() {
	double theta = m2->extractDouble();
	while (theta == 0.0) {
		theta = m2->extractDouble();
	}
	theta = theta * PI * 2;
	double r = m1->extractDouble();
	while (r == 0) {
		r = m1->extractDouble();
	}
	r = sqrt(-2 * log(r));
	return r * cos(theta);
}
BoxMuller::~BoxMuller() {
	delete m1;
	delete m2;
}

ExpGen::ExpGen(long seed,double beta) {
	this->m = new Mt19937(seed);
	this->beta = beta;
}
double ExpGen::extract() {
	double temp = m->extractDouble();
	//防止生成1导致异常
	while (temp == 1) {
		temp = m->extractDouble();
	}
	return -1 * beta * log(1 - temp);
}
ExpGen::~ExpGen() {
	delete m;
}

GgdGen::GgdGen(double beta,long s1,long s2,long s3,long s4,long s5) {
	m1 = new Mt19937(s1);
	m2 = new Mt19937(s2);
	e1 = new ExpGen(s3,pow(1/beta,0.5));
	e2 = new ExpGen(s4, pow(1/beta, 0.5));
	e3 = new ExpGen(s5, 1/beta);
}
double GgdGen::extract05() {
	double y = e1->extract();
	double z = e2->extract();
	if (m1->extractDouble()>0.5) {
		return pow(y + z, 2);
	}
	else {
		return -pow(y + z, 2);
	}
}
double GgdGen::extract1() {
	if (m2->extractDouble()>0.5) {
		return e3->extract();
	}
	else {
		return -e3->extract();
	}
}
GgdGen::~GgdGen() {
	delete m1;
	delete m2;
	delete e1;
	delete e2;
	delete e3;
}

double est::getSum(const double* nums, unsigned len) {
	double sum = 0;
	for (unsigned i = 0; i < len; i++) {
		sum += nums[i];
	}
	return sum;
}
double est::getAbsSum(const double* nums, unsigned len) {
	double sum = 0;
	for (unsigned i = 0; i < len; i++) {
		sum += abs(nums[i]);
	}
	return sum;
}
double est::getSum(const double* nums, unsigned len, double p) {
	double sum = 0;
	for (unsigned i = 0; i < len; i++) {
		sum += pow(nums[i], p);
	}
	return sum;
}
est::NorEstRes est::NormalMoment(double* nums, unsigned len) {
	double mu = getSum(nums, len) / len;
	double delta2 = getSum(nums, len, 2) / len - mu * mu;
	return { mu,delta2 };
}
est::NorEstRes est::NorMaxLike(double* nums, unsigned len) {
	double mu = getSum(nums, len) / len;
	double* newArr = new double[len];
	for (unsigned i = 0; i < len; i++) {
		newArr[i] = nums[i] - mu;
	}
	double delta2 = getSum(newArr, len, 2) / len;
	delete[] newArr;
	return { mu,delta2 };
}
double est::exp(double* nums, unsigned len) {
	return getSum(nums, len) / len;
}

double est::ggd(double* nums, unsigned len) {
	double sigmax2 = getSum(nums, len, 2) / len,
		xavg = getAbsSum(nums, len) / len;
	//试值法
	int SLICE_NUM = 900;
	double min = 99999999,
		minc = 0;
	double slice = 0.05;
	double x = slice;

	for (int i = 0; i < SLICE_NUM; i++) {
		x = x + slice;
		double gamma2 = tgamma(2 / x),
			gamma3 = tgamma(3 / x),
			gamma1 = tgamma(1 / x);
		double dValue = abs(sigmax2 * gamma2 * gamma2 - gamma3 * gamma1 * xavg * xavg);

		if (dValue < min) {
			min = dValue;
			minc = x;
		}
	}
	return minc;
}
