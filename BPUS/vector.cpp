#include "vector.h"
#include <math.h>

// Vector2

Vector2::Vector2() {
	x = 0.;
	y = 0.;
}

Vector2::Vector2(double x, double y) {
	this->x = x;
	this->y = y;
}

Vector2::~Vector2() {}

#pragma region operators
Vector2 Vector2::operator+(const Vector2& vec) {
	return Vector2(x + vec.x, y + vec.y);
}
Vector2 Vector2::operator-(const Vector2& vec) {
	return Vector2(x - vec.x, y - vec.y);
}
Vector2 Vector2::operator*(const Vector2& vec) {
	return Vector2(x * vec.x, y * vec.y);
}
Vector2 Vector2::operator/(const Vector2& vec) {
	return Vector2(x / vec.x, y / vec.y);
}
void Vector2::operator+=(const Vector2& vec) {
	x += vec.x;
	y += vec.y;
}
void Vector2::operator-=(const Vector2& vec) {
	x -= vec.x;
	y -= vec.y;
}
void Vector2::operator*=(const Vector2& vec) {
	x *= vec.x;
	y *= vec.y;
}
void Vector2::operator/=(const Vector2& vec) {
	x /= vec.x;
	y /= vec.y;
}
bool Vector2::operator==(const Vector2& vec) {
	if (this == &vec) return true;
	return false;
}
bool Vector2::operator!=(const Vector2& vec) {
	if (this != &vec) return true;
	return false;
}
#pragma endregion

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

// Vector2Int

Vector2Int::Vector2Int() {
	x = 0;
	y = 0;
}

Vector2Int::Vector2Int(int x, int y) {
	this->x = x;
	this->y = y;
}

Vector2Int::~Vector2Int() {}

Vector2Int Vector2Int::operator+(const Vector2Int& vec) {
	return Vector2Int(x + vec.x, y + vec.y);
}
Vector2Int Vector2Int::operator-(const Vector2Int& vec) {
	return Vector2Int(x - vec.x, y - vec.y);
}
Vector2Int Vector2Int::operator*(const Vector2Int& vec) {
	return Vector2Int(x * vec.x, y * vec.y);
}
Vector2Int Vector2Int::operator/(const Vector2Int& vec) {
	return Vector2Int(x / vec.x, y / vec.y);
}
void Vector2Int::operator+=(const Vector2Int& vec) {
	x += vec.x;
	y += vec.y;
}
void Vector2Int::operator-=(const Vector2Int& vec) {
	x -= vec.x;
	y -= vec.y;
}
void Vector2Int::operator*=(const Vector2Int& vec) {
	x *= vec.x;
	y *= vec.y;
}
void Vector2Int::operator/=(const Vector2Int& vec) {
	x /= vec.x;
	y /= vec.y;
}

/*Vector2Int Vector2Int::operator+(const Vector2& vec) {
	return Vector2Int(x + floor(vec.x), y + floor(vec.y));
}
Vector2Int Vector2Int::operator-(const Vector2& vec) {
	return Vector2Int(x - floor(vec.x), y - floor(vec.y));
}
Vector2Int Vector2Int::operator*(const Vector2& vec) {
	return Vector2Int(x * floor(vec.x), y * floor(vec.y));
}
Vector2Int Vector2Int::operator/(const Vector2& vec) {
	return Vector2Int(x / floor(vec.x), y / floor(vec.y));
}*/
bool Vector2Int::operator==(const Vector2Int& vec) {
	if (this == &vec) return true;
	return false;
}
bool Vector2Int::operator!=(const Vector2Int& vec) {
	if (this != &vec) return true;
	return false;
}

double Vector2Int::length() {
	return sqrt((double)x * (double)x + (double)y * (double)y);
}

Vector2 Vector2Int::todouble() {
	return Vector2(x, y);
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

Vector3::~Vector3() {}

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

// Vector3Int

Vector3Int::Vector3Int() {
	x = 0;
	y = 0;
	z = 0;
}

Vector3Int::Vector3Int(int x, int y, int z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3Int::~Vector3Int() {}

double Vector3Int::length() {
	return sqrt((double)x * (double)x + (double)y * (double)y + (double)z * (double)z);
}

Vector3 Vector3Int::todouble() {
	return Vector3(x, y, z);
}
