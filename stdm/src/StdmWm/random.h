#pragma once
#ifndef RANDMO_H
#define RANDOM_H

#include <inttypes.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SIZE 1024
#define NN 312
#define MM 156
#define MATRIX_A UINT64_C(0xB5026F5AA96619E9)
#define UM UINT64_C(0xFFFFFFFF80000000) /* Most significant 33 bits */
#define LM UINT64_C(0x7FFFFFFF) /* Least significant 31 bits */
#define PI 3.1415926535

static unsigned curId=0;
uint64_t *arrs[MAX_SIZE];
int mti[MAX_SIZE];
//uint64_t mt[NN];
/* mti==NN+1 means mt[NN] is not initialized */
unsigned init_genrand64(uint64_t seed);
unsigned initRand(uint64_t seed,uint64_t *arr);
/* generates a random number on [0, 2^64-1]-interval */
uint64_t extractLong(unsigned id);
/* generates a random number on [0, 2^63-1]-interval */
int64_t extractLong63(unsigned id);
/* generates a random number on [0,1]-real-interval */
double extractDouble(unsigned id);
/* generates a random number on [0,1)-real-interval */
double extractDouble2(unsigned id);
/* generates a random number on (0,1)-real-interval */
double extractDouble3(unsigned id);
void freeMt(unsigned id);

unsigned bmId1,bmId2;
void initBoxMuller(uint64_t seed1,uint64_t seed2);
double extractGauss();
void freeBoxMuller();

#endif
