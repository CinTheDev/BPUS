#include "vector.h"
#include <math.h>

Vector2::Vector2() {
	x = 0.;
	y = 0.;
}

Vector2::Vector2(double x, double y) {
	this->x = x;
	this->y = y;
}

double Vector2::length() {
	return sqrt(x * x + y * y);
}

Vector2 Vector2::normalized() {
	double X = x / length();
	double Y = y / length();
	return Vector2(X, Y);
}

void Vector2::normalize() {
	x /= length();
	y /= length();
}

// Vector3

Vector3::Vector3() {
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

double Vector3::length() {
	return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalized() {
	double X = x / length();
	double Y = y / length();
	double Z = z / length();
	return Vector3(X, Y, Z);
}

void Vector3::normalize() {
	x /= length();
	y /= length();
	z /= length();
}