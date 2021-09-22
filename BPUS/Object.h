#include "utils.cpp"
#include "Image.h"
#include "platform_common.cpp"

#pragma once
class Object
{
public:
	Vector2 position;
	Vector2 pivot;
	float rotation;
	float size;

	Image* image;
	int z;

	Object(Vector2 pos, Image* img, float s, float r);
	Object(Vector2 pos, Image* img, float s);
	Object(Vector2 pos, Image* img);
	Object(Vector2 pos);
	Object();
	~Object();

	bool operator==(const Object& o);
	bool operator!=(const Object& o);

	void centerPivot();

	virtual void init();
	virtual void update(Input* input, double dt);
};
