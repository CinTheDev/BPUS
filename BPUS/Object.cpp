#include "Object.h"

Object::Object() {
	position = Vector2(0, 0);
	image = new Image();
	size = 1;
}

Object::Object(Vector2 pos, Image* img, float s) {
	position = pos;
	image = img;
	size = s;
}

Object::~Object() {
	position.~Vector2();
}

bool Object::operator==(const Object& o) {
	if (*this == o) return true;
	return false;
}

void Object::init() {}

void Object::update(Input* input, float dt) {}
