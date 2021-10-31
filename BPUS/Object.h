#pragma once

//#include "utils.cpp"
#include "Texture.h"

struct UpdateArguments {
	GLFWwindow* window;

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
	~Object();

	bool operator==(const Object& o);
	bool operator!=(const Object& o);

	virtual void init();
	virtual void update(UpdateArguments args);
};
