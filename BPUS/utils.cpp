#pragma once

#include "vector.h"

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

#define global_variable static
#define internal static

inline int
clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

inline int
min3(int a, int b, int c) {
	if (a < b) {
		if (a < c) return a;
	}
	else {
		if (b < c) return b;
	}
	return c;
}

inline int
max3(int a, int b, int c) {
	if (a > b) {
		if (a > c) return a;
	}
	else {
		if (b > c) return b;
	}
	return c;
}
