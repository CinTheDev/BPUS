#include "utils.cpp"
#include "Image.h"
#include "platform_common.cpp"

#pragma once
class Object
{
public:
	Vector2 position;
	Image* image;
	float size;

	Object();
	Object(Vector2 pos, Image* img, float s);
	~Object();

	bool operator==(const Object& o);

	virtual void init();
	virtual void update(Input* input, float dt);
};
