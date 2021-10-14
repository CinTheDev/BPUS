#pragma once

#include "hvector.h"

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

#define global_variable static

template <class T>
inline void swap(T* p1, T* p2) {
	T temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

template <class T>
inline T clamp(T min, T val, T max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

template <class T>
inline T min_array(T* arr, int size) {
	T* temp = new T[size];
	for (int i = 0; i < size; i++) {
		temp[i] = *(arr + i);
	}

	for (int i = size - 1; i > 0; i--) {
		if (temp[i] < temp[i - 1]) {
			swap(&temp[i], &temp[i - 1]);
		}
	}

	T r = temp[0];
	delete temp;
	return r;
}

template <class T>
inline T max_array(T arr[], int size) {
	T* temp = new T[size];
	for (int i = 0; i < size; i++) {
		temp[i] = *(arr + i);
	}

	for (int i = size - 1; i > 0; i--) {
		if (temp[i] > temp[i - 1]) {
			swap(&temp[i], &temp[i - 1]);
		}
	}

	T r = temp[0];
	delete temp;
	return r;
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
