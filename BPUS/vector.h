#include <string>

#pragma once
struct Vector2 {
	double x;
	double y;

	Vector2();
	Vector2(double x, double y);
	~Vector2();

	Vector2 operator+(const Vector2& vec);
	Vector2 operator-(const Vector2& vec);
	Vector2 operator*(const Vector2& vec);
	Vector2 operator/(const Vector2& vec);
	void operator+=(const Vector2& vec);
	void operator-=(const Vector2& vec);
	void operator*=(const Vector2& vec);
	void operator/=(const Vector2& vec);
	bool operator==(const Vector2& vec);
	bool operator!=(const Vector2& vec);

	double len();
	double sqrlen();
	Vector2 normalized();
	void normalize();
};

#pragma once
struct Vector2Int {
	int x;
	int y;

	Vector2Int();
	Vector2Int(int x, int y);
	~Vector2Int();

	Vector2Int operator+(const Vector2Int& vec);
	Vector2Int operator-(const Vector2Int& vec);
	Vector2Int operator*(const Vector2Int& vec);
	Vector2Int operator/(const Vector2Int& vec);
	void operator+=(const Vector2Int& vec);
	void operator-=(const Vector2Int& vec);
	void operator*=(const Vector2Int& vec);
	void operator/=(const Vector2Int& vec);

	// These automatically round because I can't define "Vector2Int round();" in Vector2.
	/*Vector2Int operator+(const Vector2& vec);
	Vector2Int operator-(const Vector2& vec);
	Vector2Int operator*(const Vector2& vec);
	Vector2Int operator/(const Vector2& vec);*/
	bool operator==(const Vector2Int& vec);
	bool operator!=(const Vector2Int& vec);

	double len();
	double sqrlen();
	Vector2 todouble();
};

#pragma once
struct Vector3 {
	double x;
	double y;
	double z;

	Vector3();
	Vector3(double x, double y, double z);
	~Vector3();

	double len();
	double sqrlen();
	Vector3 normalized();
	void normalize();
};

#pragma once
struct Vector3Int {
	int x;
	int y;
	int z;

	Vector3Int();
	Vector3Int(int x, int y, int z);
	~Vector3Int();

	double len();
	double sqrlen();
	Vector3 todouble();
};
