#include "Object.h"

Object::Object(h_Vector2 pos, Image* img, float s, float r) {
	position = pos;
	image = img;
	rotation = r;
	size = s;
	z = 0;
	centerPivot();
}

Object::Object(h_Vector2 pos, Image* img, float s) : Object(pos, img, s, 0) {
}

Object::Object(h_Vector2 pos, Image* img) : Object(pos, img, 1.f) {
}

Object::Object(h_Vector2 pos) : Object(pos, new Image()) {
}

Object::Object() : Object(h_Vector2(0, 0)) {
}

Object::~Object() {
	position.~h_Vector2();
}

bool Object::operator==(const Object& o) {
	return this == &o;
}
bool Object::operator!=(const Object& o) {
	return this != &o;
}

void Object::centerPivot() {
	pivot = h_Vector2(image->h * size / 2, image->w * size / 2);
}

h_Vector2 Object::getPivAbs() {
	return position + h_Vector2(image->h * size / 2, image->w * size / 2);
}

void Object::init() {}

void Object::update(Input* input, double dt) {}
