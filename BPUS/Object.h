#include "utils.cpp"
#include "Image.h"
#include "platform_common.cpp"

#pragma once
class Object
{
public:
	Vector2 position;
	float size;

	Image* image;
	int z;

	Object();
	Object(Vector2 pos, Image* img, float s);
	~Object();

	bool operator==(const Object& o);
	bool operator!=(const Object& o);

	virtual void init();
	virtual void update(Input* input, double dt);
};
