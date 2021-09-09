#pragma once
struct Vector2 {
	double x;
	double y;

	Vector2();
	Vector2(double x, double y);
	~Vector2();

	double length();
	Vector2 normalized();
	void normalize();
};

#pragma once
struct Vector3 {
	double x;
	double y;
	double z;

	Vector3();
	Vector3(double x, double y, double z);
	~Vector3();

	double length();
	Vector3 normalized();
	void normalize();
};