struct Vector2 {
	double x;
	double y;

	Vector2();
	Vector2(double x, double y);

	double length();
	Vector2 normalized();
	void normalize();
};

struct Vector3 {
	double x;
	double y;
	double z;

	Vector3();
	Vector3(double x, double y, double z);

	double length();
	Vector3 normalized();
	void normalize();
};