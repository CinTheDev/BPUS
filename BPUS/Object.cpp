#include "Object.h"

Object::Object(void (*init)(Object* ths), void (*update)(Object* ths, Input* input, float dt)) {
	position = Vector2(0, 0);
	image = new Image();
	size = 0;
	this->init = init;
	this->update = update;
}

Object::Object(Vector2 pos, Image* img, float s, void (*init)(Object* ths), void (*update)(Object* ths, Input* input, float dt)) {
	position = pos;
	image = img;
	size = s;
	this->init = init;
	this->update = update;
}

Object::~Object() {
	position.~Vector2();
	delete image;
}