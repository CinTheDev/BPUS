#pragma once

#include <string>

struct h_Vector2 {
	double x;
	double y;

	h_Vector2() { x = 0; y = 0; }
	h_Vector2(double x, double y) { this->x = x; this->y = y; }
	~h_Vector2() {};

	h_Vector2 operator+(const h_Vector2& vec) { return h_Vector2(x + vec.x, y + vec.y); }
	h_Vector2 operator-(const h_Vector2& vec) { return h_Vector2(x - vec.x, y - vec.y); }
	h_Vector2 operator*(const h_Vector2& vec) { return h_Vector2(x * vec.x, y * vec.y); }
	h_Vector2 operator/(const h_Vector2& vec) { return h_Vector2(x / vec.x, y / vec.y); }
	h_Vector2& operator+=(const h_Vector2& vec) { x += vec.x; y += vec.y; return *this;	}
	h_Vector2& operator-=(const h_Vector2& vec) { x -= vec.x; y -= vec.y; return *this; }
	h_Vector2& operator*=(const h_Vector2& vec) { x *= vec.x; y *= vec.y; return *this; }
	h_Vector2& operator/=(const h_Vector2& vec) { x /= vec.x; y /= vec.y; return *this; }
	bool operator==(const h_Vector2& vec) { return (this == &vec); }
	bool operator!=(const h_Vector2& vec) { return (this != &vec); }

	std::string str() { return "(x: " + std::to_string(x) + "; y: " + std::to_string(y) + ")"; }
	double len() { return sqrt(x * x + y * y); }
	double sqrlen() { return x * x + y * y; }

	h_Vector2 normalized() {
		double X = x / len();
		double Y = y / len();
		return h_Vector2(X, Y);
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

struct h_Vector2Int {
	int x;
	int y;

	h_Vector2Int() { x = 0; y = 0; }
	h_Vector2Int(int x, int y) { this->x = x; this->y = y; }
	~h_Vector2Int() {};

	h_Vector2Int operator+(const h_Vector2Int& vec) { return h_Vector2Int(x + vec.x, y + vec.y); }
	h_Vector2Int operator-(const h_Vector2Int& vec) { return h_Vector2Int(x - vec.x, y - vec.y); }
	h_Vector2Int operator*(const h_Vector2Int& vec) { return h_Vector2Int(x * vec.x, y * vec.y); }
	h_Vector2Int operator/(const h_Vector2Int& vec) { return h_Vector2Int(x / vec.x, y / vec.y); }
	h_Vector2Int& operator+=(const h_Vector2Int& vec) { x += vec.x; y += vec.y; return *this; }
	h_Vector2Int& operator-=(const h_Vector2Int& vec) { x -= vec.x; y -= vec.y; return *this; }
	h_Vector2Int& operator*=(const h_Vector2Int& vec) { x *= vec.x; y *= vec.y; return *this; }
	h_Vector2Int& operator/=(const h_Vector2Int& vec) { x /= vec.x; y /= vec.y; return *this; }
	bool operator==(const h_Vector2Int& vec) { return (this == &vec); }
	bool operator!=(const h_Vector2Int& vec) { return (this != &vec); }

	std::string str() { return "(x: " + std::to_string(x) + "; y: " + std::to_string(y) + ")"; }
	double len() { return sqrt(x * x + y * y); }
	double sqrlen() { return x * x + y * y; }

	h_Vector2Int normalized() {
		double X = x / len();
		double Y = y / len();
		return h_Vector2Int(X, Y);
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

	h_Vector2 todouble() {
		return h_Vector2(x, y);
	}
};
