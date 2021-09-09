#include "utils.cpp"
#include "Image.h"
#include "platform_common.cpp"

#pragma once
enum ObjectType {
	
};

#pragma once
class Object
{
public:
	Vector2 position;
	Image* image;
	float size;
	void (*init)(Object* ths);
	void (*update)(Object* ths, Input* input, float dt);

	Object(void (*init)(Object* ths), void (*update)(Object* ths, Input* input, float dt));
	Object(Vector2 pos, Image* img, float s, void (*init)(Object* ths), void (*update)(Object* ths, Input* input, float dt));
	~Object();

	bool operator==(const Object& o) {
		if (*this == o) return true;
		return false;
	}
};