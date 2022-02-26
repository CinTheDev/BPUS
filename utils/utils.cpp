#pragma once

#include "hvector.h"

#define PI 3.14159265359

template <class T>
inline T* join_array(T* array1, int size1, T* array2, int size2) {
	T* result = new T[size1 + size2];
	for (int i = 0; i < size1; i++) {
		result[i] = array1[i];
	}
	for (int i = size1; i < size1 + size2; i++) {
		result[i] = array2[i - size1];
	}

	return result;
}

template <class T>
inline bool outside(T min, T val, T max) {
	return (val < min || val > max);
}

inline bool outside(Vector2Int p, Vector2Int bound1, Vector2Int bound2) {
	return (outside(bound1.x, p.x, bound2.x) || outside(bound1.y, p.y, bound2.y));
}

template <class T>
inline T clamp(T min, T val, T max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}
template <class T>
inline T sign(T val) {
	if (val == 0) return 0;

	return val / abs(val);
}

template <class T>
inline void swap(T* p1, T* p2) {
	T temp = *p1;
	*p1 = *p2;
	*p2 = temp;
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

inline Vector2Int
round_vector(Vector2 vector) {
	return Vector2Int((int)floor(vector.x), (int)floor(vector.y));
}
