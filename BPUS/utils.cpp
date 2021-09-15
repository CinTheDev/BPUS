#include <math.h>
#include <string>
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

#pragma once
inline int
clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

#pragma once
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

#pragma once
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

#pragma once
inline u32
max2(u32 a, u32 b) {
	if (a > b) return a;
	return b;
}

#pragma once
inline u32
difference(u32 a, u32 b) {
	if (a > b) return a - b;
	return b - a;
}
