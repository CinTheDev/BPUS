#pragma once

//#include "utils.cpp"
#include "Texture.h"
#include "platform_common.cpp"

class Object
{
public:
	Vector2 position;
	Vector2 size;
	Vector2 pivot;
	float rotation;

	Texture* image;
	int z;

	Object(Vector2 pos, Texture* img, Vector2 s, float r);
	Object(Vector2 pos, Texture* img, Vector2 s);
	Object(Vector2 pos, Texture* img);
	Object(Vector2 pos);
	Object();
	~Object();

	bool operator==(const Object& o);
	bool operator!=(const Object& o);

	virtual void init();
	virtual void update(Input* input, double dt);

	void centerPivot();
	Vector2 getPivAbs();
};
