#include "utils.cpp"
#include "Image.h"

#pragma once
class Object
{
public:
	Vector2 position;
	Image* image;
	int size;

	Object();
	Object(Vector2 pos, Image* img, int s);
	~Object();

	bool operator==(const Object& o) {
		if (*this == o) return true;
		return false;
	}
};