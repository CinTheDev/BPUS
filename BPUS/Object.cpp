#include "Object.h"

Object::Object() {
	position = Vector2(0, 0);
	image = new Image();
	size = 1;
	z = 0;
}

Object::Object(Vector2 pos, Image* img, float s) {
	position = pos;
	image = img;
	size = s;
	z = 0;
}

Object::~Object() {
	position.~Vector2();
}

bool Object::operator==(const Object& o) {
	return this == &o;
}
bool Object::operator!=(const Object& o) {
	return this != &o;
}

void Object::init() {}

void Object::update(Input* input, double dt) {}
