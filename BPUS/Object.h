#pragma once

//#include "utils.cpp"
#include "Image.h"
#include "platform_common.cpp"

class Object
{
public:
	h_Vector2 position;
	h_Vector2 pivot;
	float rotation;
	float size;

	Image* image;
	int z;

	Object(h_Vector2 pos, Image* img, float s, float r);
	Object(h_Vector2 pos, Image* img, float s);
	Object(h_Vector2 pos, Image* img);
	Object(h_Vector2 pos);
	Object();
	~Object();

	bool operator==(const Object& o);
	bool operator!=(const Object& o);

	void centerPivot();
	h_Vector2 getPivAbs();

	virtual void init();
	virtual void update(Input* input, double dt);
};
