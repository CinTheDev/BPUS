#pragma once

#include <string>

struct Vector2 {
	double x;
	double y;

	Vector2() { x = 0; y = 0; }
	Vector2(double x, double y) { this->x = x; this->y = y; }
	~Vector2() {};

	Vector2 operator+(const Vector2& vec) { return Vector2(x + vec.x, y + vec.y); }
	Vector2 operator-(const Vector2& vec) { return Vector2(x - vec.x, y - vec.y); }
	Vector2 operator*(const Vector2& vec) { return Vector2(x * vec.x, y * vec.y); }
	Vector2 operator*(const double& c) { return Vector2(x * c, y * c); }
	Vector2 operator/(const Vector2& vec) { return Vector2(x / vec.x, y / vec.y); }
	Vector2 operator/(const double& c) { return Vector2(x / c, y / c); }
	Vector2& operator+=(const Vector2& vec) { x += vec.x; y += vec.y; return *this;	}
	Vector2& operator-=(const Vector2& vec) { x -= vec.x; y -= vec.y; return *this; }
	Vector2& operator*=(const Vector2& vec) { x *= vec.x; y *= vec.y; return *this; }
	Vector2& operator*=(const double& c) { x *= c; y *= c; return *this; }
	Vector2& operator/=(const Vector2& vec) { x /= vec.x; y /= vec.y; return *this; }
	Vector2& operator/=(const double& c) { x /= c; y /= c; return *this; }
	bool operator==(const Vector2& vec) { return (this == &vec); }
	bool operator!=(const Vector2& vec) { return (this != &vec); }

	std::string str() { return "(x: " + std::to_string(x) + "; y: " + std::to_string(y) + ")"; }
	double len() { return sqrt(x * x + y * y); }
	double sqrlen() { return x * x + y * y; }

	Vector2 normalized() {
		double X = x / len();
		double Y = y / len();
		return Vector2(X, Y);
	}
	void normalize() {
		x /= len();
		y /= len();
	}

	void rotate(float angle) {
		double _x = x;
		x = _x * cos(angle) - y * sin(angle);
		y = _x * sin(angle) + y * cos(angle);
	}
};

struct Vector2Int {
	int x;
	int y;

	Vector2Int() { x = 0; y = 0; }
	Vector2Int(int x, int y) { this->x = x; this->y = y; }
	~Vector2Int() {};

	Vector2Int operator+(const Vector2Int& vec) { return Vector2Int(x + vec.x, y + vec.y); }
	Vector2Int operator-(const Vector2Int& vec) { return Vector2Int(x - vec.x, y - vec.y); }
	Vector2Int operator*(const Vector2Int& vec) { return Vector2Int(x * vec.x, y * vec.y); }
	Vector2Int operator*(const int& c) { return Vector2Int(x * c, y * c); }
	Vector2Int operator/(const Vector2Int& vec) { return Vector2Int(x / vec.x, y / vec.y); }
	Vector2Int operator/(const int& c) { return Vector2Int(x * c, y * c); }
	Vector2Int& operator+=(const Vector2Int& vec) { x += vec.x; y += vec.y; return *this; }
	Vector2Int& operator-=(const Vector2Int& vec) { x -= vec.x; y -= vec.y; return *this; }
	Vector2Int& operator*=(const Vector2Int& vec) { x *= vec.x; y *= vec.y; return *this; }
	Vector2Int& operator*=(const int& c) { x *= c; y *= c; return *this; }
	Vector2Int& operator/=(const Vector2Int& vec) { x /= vec.x; y /= vec.y; return *this; }
	Vector2Int& operator/=(const int& c) { x /= c; y /= c; return *this; }
	bool operator==(const Vector2Int& vec) { return (this == &vec); }
	bool operator!=(const Vector2Int& vec) { return (this != &vec); }

	std::string str() { return "(x: " + std::to_string(x) + "; y: " + std::to_string(y) + ")"; }

	double len() { return sqrt((double)x * (double)x + (double)y * (double)y); }
	double sqrlen() { return (double)x * (double)x + (double)y * (double)y; }

	Vector2 todouble() {
		return Vector2(x, y);
	}
};

struct Vector3 {
	double x;
	double y;
	double z;

	Vector3() : x(0), y(0), z(0) {};
	Vector3(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	~Vector3() {}

	double len() { return sqrt(x * x + y * y + z * z); }
	double sqrlen() { return x * x + y * y + z * z; }
	Vector3 normalized() {
		double X = x / len();
		double Y = y / len();
		double Z = z / len();
		return Vector3(X, Y, Z);
	}
	void normalize() {
		x /= len();
		y /= len();
		z /= len();
	}
};

struct Vector3Int {
	int x;
	int y;
	int z;

	Vector3Int() : x(0), y(0), z(0) {}
	Vector3Int(int x, int y, int z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	~Vector3Int() {}

	double len() { return sqrt((double)x * (double)x + (double)y * (double)y + (double)z * (double)z); }
	double sqrlen() { return (double)x * (double)x + (double)y * (double)y + (double)z * (double)z; }
	Vector3 todouble() { return Vector3(x, y, z); }
};
