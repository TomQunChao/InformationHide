#pragma once
#ifndef UTIL_H
#define UTIL_H
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PI 3.1415926535
#define ind(i,j,size) ((i)*(size)+j)
#define mul1n1(w,a) ((w)==1?(a):(-a));

double wmDiff(char *x, char *y, unsigned n);
double mse(char *s, char *x, unsigned n) ;

#endif
