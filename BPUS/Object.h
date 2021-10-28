#pragma once

//#include "utils.cpp"
#include "Texture.h"
#include "platform_common.cpp"

struct UpdateArguments {
	GLFWwindow* window;
	Input* input;

	float deltaTime;
};

class Object
{
public:
	Vector2 position;
	Vector2 size;
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
	virtual void update(UpdateArguments args);

	//void centerPivot();
	//Vector2 getPivAbs();
};
